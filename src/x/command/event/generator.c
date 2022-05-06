#include <stdlib.h>
#include <stdio.h>

#include "generator.h"
#include "subscription.h"

#include "../../event/engine.h"
#include "../../event/queue.h"
#include "../../event/pool.h"

static xcommandeventgenerator * commandeventgeneratorDel(xcommandeventgenerator * o);
static void commandeventgeneratorOn(xcommandeventgenerator * o);
static xcommandeventsubscription * commandeventgeneratorReg(xcommandeventgenerator * o, xcommandeventsubscription * subscription);
static xcommandeventsubscription * commandeventgeneratorUnreg(xcommandeventgenerator * o, xcommandeventsubscription * subscription);

static xcommandeventgeneratorset virtualSet = {
    commandeventgeneratorDel,
    commandeventgeneratorOn,
    commandeventgeneratorReg,
    commandeventgeneratorUnreg
};

static void commandeventgeneratorPush(xcommandeventgenerator * o, xcommandeventsubscription * subscription);
static xcommandeventsubscription * commandeventgeneratorPop(xcommandeventgenerator * o);
static void commandeventgeneratorRem(xcommandeventgenerator * o, xcommandeventsubscription * subscription);
static void commandeventgeneratorClear(xcommandeventgenerator * o);

extern xcommandeventgenerator * xcommandeventgeneratorNew(xeventengine * engine)
{
    xcommandeventgenerator * o = (xcommandeventgenerator *) xeventgeneratorNew(engine, (xeventgeneratorset *) xaddressof(virtualSet), sizeof(xcommandeventgenerator));
    return o;
}

static xcommandeventgenerator * commandeventgeneratorDel(xcommandeventgenerator * o)
{
    if(o)
    {
        commandeventgeneratorClear(o);

        o->sync = xsyncDel(o->sync);

        free(o);
    }
    return xnil;
}

static void commandeventgeneratorOn(xcommandeventgenerator * o)
{
    xsyncLock(o->sync);
    xuint64 size = o->size;
    for(xuint64 i = 0; i < size && o->size > 0; i++)
    {
        xcommandeventsubscription * subscription = commandeventgeneratorPop(o);
        printf("subscription => %p\n", subscription);
        xsyncUnlock(o->sync);
        xeventengineSubscriptionPush(o->engine, (xeventsubscription *) subscription);
        xeventqueuePush(o->engine->queue, xeventSet(xeventpoolGet(o->engine->eventpool), (xeventsubscription *) subscription, xeventtype_command, (xeventhandler) xcommandeventOn));
        xsyncLock(o->sync);
    }
    xsyncUnlock(o->sync);
}

static xcommandeventsubscription * commandeventgeneratorReg(xcommandeventgenerator * o, xcommandeventsubscription * subscription)
{
    commandeventgeneratorPush(o, subscription);

    return subscription;
}

static xcommandeventsubscription * commandeventgeneratorUnreg(xcommandeventgenerator * o, xcommandeventsubscription * subscription)
{
    commandeventgeneratorRem(o, subscription);

    return subscription;
}

static void commandeventgeneratorPush(xcommandeventgenerator * o, xcommandeventsubscription * subscription)
{
    subscription->next = o->tail;

    if(subscription->next)
    {
        subscription->next->prev = subscription;
    }
    else
    {
        o->head = subscription;
    }

    subscription->generator = o;
    o->tail = subscription;
    o->size = o->size + 1;
    
    printf("generator size => %ld\n", o->size);
}

static xcommandeventsubscription * commandeventgeneratorPop(xcommandeventgenerator * o)
{
    xcommandeventsubscription * subscription = o->head;

    if(subscription)
    {
        o->head = subscription->next;

        if(subscription->next)
        {
            subscription->next->prev = xnil;
            subscription->next = xnil;
        }
        else
        {
            o->tail = xnil;
        }
        subscription->generator = xnil;
        o->size = o->size - 1;
    }

    return subscription;
}

static void commandeventgeneratorRem(xcommandeventgenerator * o, xcommandeventsubscription * subscription)
{
    xcommandeventsubscription * prev = subscription->prev;
    xcommandeventsubscription * next = subscription->next;

    if(prev)
    {
        prev->next = next;
        subscription->prev = xnil;
    }
    else
    {
        o->head = next;
    }
    if(next)
    {
        next->prev = prev;
        subscription->next = xnil;
    }
    else
    {
        o->tail = prev;
    }
    subscription->generator = xnil;
    o->size = o->size - 1;

}

static void commandeventgeneratorClear(xcommandeventgenerator * o)
{
    xcommandeventsubscription * subscription = xnil;

    while(o->head)
    {
        subscription = o->head;
        o->head = subscription->next;

        if(subscription->next)
        {
            subscription->next->prev = xnil;
            subscription->next = xnil;
        }
        else
        {
            o->tail = xnil;
        }

        subscription->generator = xnil;
        o->size = o->size - 1;

        
    }
}