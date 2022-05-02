#include "epoll.h"
#include "../../../descriptor.h"
#include "../../../event/subscription.h"

static xdescriptoreventgeneratorepoll * descriptoreventgeneratorepollDel(xdescriptoreventgeneratorepoll * o);
static xeventsubscription * descriptoreventgeneratorepollRegisterSubscription(xdescriptoreventgeneratorepoll * o, xeventsubscription * subscription);
static xeventsubscription * descriptoreventgeneratorepollUnregisterSubscription(xdescriptoreventgeneratorepoll * o, xeventsubscription * subscription);
static void descriptoreventgeneratorepollClear(xdescriptoreventgeneratorepoll * o);

static xdescriptoreventgeneratorepollset virtualSet = {
    descriptoreventgeneratorepollDel,
    descriptoreventgeneratorepollRegisterSubscription
};

extern xdescriptoreventgeneratorepoll * xdescriptoreventgeneratorepollNew(xeventengine * engine, xint32 timeout, xuint64 maxevents)
{
    xdescriptoreventgeneratorepoll * o = (xdescriptoreventgeneratorepoll *) xdescriptoreventgeneratorNew(engine, xaddressof(virtualSet), sizeof(xdescriptoreventgeneratorepoll));

    o->descriptor = xdescriptor_invalid_value;
    o->timeout = timeout;
    o->maxevents = maxevents;
    o->events = (struct epoll_event *) malloc(sizeof(struct epoll_event) * o->maxevents);

    return o;
}

static xdescriptoreventgeneratorepoll * descriptoreventgeneratorepollDel(xdescriptoreventgeneratorepoll * o)
{
    if(o)
    {
        descriptoreventgeneratorepollClear(o);
        if(o->descriptor >= 0)
        {
            close(o->descriptor);
            o->descriptor = xdescriptor_invalid_value;
        }
        if(o->events)
        {
            free(o->events);
            o->events = xnil;
        }
        free(o);
    }
    return xnil;
}

static xeventsubscription * descriptoreventgeneratorepollRegisterSubscription(xdescriptoreventgeneratorepoll * o, xeventsubscription * subscription)
{
    xdescriptor * descriptor = (xdescriptor *)(subscription->object);

    if(o->descriptor >= 0 && descriptor->value >= 0)
    {
    }
    else
    {
        if(o->descriptor >= 0)
        {
            // engine 에 event queue 에 삽입한다.
        }
        else
        {

        }
    }


    return subscription;
}

static xeventsubscription * descriptoreventgeneratorepollUnregisterSubscription(xdescriptoreventgeneratorepoll * o, xeventsubscription * subscription)
{
    xdescriptor * descriptor = (xdescriptor *)(subscription->object);

    if(o->descriptor >= 0 && descriptor->value >= 0)
    {
    }
    else
    {
        // engine's event queue 에 삽입한다.
    }

    return subscription;
}

static void descriptoreventgeneratorepollClear(xdescriptoreventgeneratorepoll * o)
{

}