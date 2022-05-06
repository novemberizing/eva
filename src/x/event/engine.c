#include <stdlib.h>

#include "engine.h"

#include "subscription.h"
#include "queue.h"
#include "pool.h"

#include "../command/event/generator.h"
#include "../command/event/subscription.h"

static xeventengine * singleton = xnil;

static void eventengineClear(xeventengine * engine);

extern void xeventengineStop(xeventengine * o)
{

}

extern void xeventengineCancel(xeventengine * o, xeventenginecancel cancel)
{
    if(o)
    {
        o->cancel = cancel;
    }
}

extern xeventengine * xeventengineGen(void)
{
    if(singleton == xnil)
    {
        singleton = (xeventengine *) calloc(1, sizeof(xeventengine));

        singleton->queue = xeventqueueNew();
        singleton->eventpool = xeventpoolNew(64);

        singleton->generator.command = xcommandeventgeneratorNew(singleton);
    }
    return singleton;
}

extern xeventengine * xeventengineGet(void)
{
    return singleton;
}

extern xeventengine * xeventengineDel(xeventengine * engine)
{
    if(engine)
    {
        eventengineClear(engine);

        engine->queue = xeventqueueDel(engine->queue);
        engine->eventpool = xeventpoolDel(engine->eventpool);
        engine->generator.command = xcommandeventgeneratorDel(engine->generator.command);

        engine->sync = xsyncDel(engine->sync);

        free(engine);

        singleton = xnil;
    }
    return xnil;
}

static void eventengineClear(xeventengine * engine)
{
    xsyncLock(engine->sync);
    xeventsubscription * subscription = xnil;
    while(engine->head)
    {
        subscription = engine->head;
        engine->head = subscription->next;

        if(subscription->next)
        {
            subscription->next->prev = xnil;
            subscription->next = xnil;
        }
        else
        {
            engine->tail = xnil;
        }
        subscription->engine = xnil;
        engine->size = engine->size - 1;
    }
    xsyncUnlock(engine->sync);
}

extern xint32 xeventengineRun(xeventengine * engine)
{
    if(engine)
    {
        xeventenginecancel cancel = xnil;

        while((cancel = engine->cancel) == xnil)
        {
            xcommandeventgeneratorOn(engine->generator.command);


            xsyncLock(engine->queue->sync);
            xuint64 size = engine->queue->size;
            for(xuint64 i = 0; i < size && engine->queue->size > 0; i++)
            {
                xevent * event = xeventqueuePop(engine->queue);
                xsyncUnlock(engine->queue->sync);
                if(event)
                {
                    event->on(event->subscription->object, event->subscription, event->type);
                    xeventpoolRel(engine->eventpool, event);
                }
                xsyncLock(engine->queue->sync);
            }
            xsyncUnlock(engine->queue->sync);

            
        }
        cancel(engine);
        singleton = xeventengineDel(engine);
    }
    return xsuccess;
}

extern void xeventengineSubscriptionRem(xeventengine * o, xeventsubscription * subscription)
{
    xeventsubscription * prev = subscription->prev;
    xeventsubscription * next = subscription->next;

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
        o->tail = prev;;
    }

    subscription->engine = xnil;
    o->size = o->size - 1;
}

extern void xeventengineSubscriptionPush(xeventengine * o, xeventsubscription * subscription)
{
    subscription->prev = o->tail;
    if(subscription->prev)
    {
        subscription->prev->next = subscription;
    }
    else
    {
        o->head = subscription;
    }
    subscription->engine = o;
    o->tail = subscription;
    o->size = o->size + 1;
}

extern xcommandeventsubscription * xeventengineCommandReg(xeventengine * o, xcommand * command, xcommandeventhandler on)
{
    xcommandeventsubscription * subscription = xcommandeventsubscriptionNew((xeventobject *) command, on);

    xcommandeventgeneratorReg(o->generator.command, subscription);

    return subscription;
}

extern xcommandeventsubscription * xeventengineCommandEventSubscriptionUnreg(xeventengine * o, xcommandeventsubscription * subscription)
{
    if(subscription->generator) xcommandeventgeneratorUnreg(subscription->generator, subscription);
    if(subscription->engine) xeventengineSubscriptionRem(subscription->engine, (xeventsubscription *) subscription);
    return subscription;
}
