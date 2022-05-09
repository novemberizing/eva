#include "epoll.h"

#include "../subscription.h"

#include "../../../event/queue.h"
#include "../../../event/pool.h"
#include "../../../event/engine.h"

static xdescriptoreventgeneratorepoll * descriptoreventgeneratorepollDel(xdescriptoreventgeneratorepoll * o);
static void descriptoreventgeneratorepollOn(xdescriptoreventgeneratorepoll * o);
static xdescriptoreventsubscription * descriptoreventgeneratorepollReg(xdescriptoreventgeneratorepoll * o, xdescriptoreventsubscription * subscription);
static xdescriptoreventsubscription * descriptoreventgeneratorepollUnreg(xdescriptoreventgeneratorepoll * o, xdescriptoreventsubscription * subscription);

static xdescriptoreventgeneratorepollset virtualSet = {
    descriptoreventgeneratorepollDel,
    descriptoreventgeneratorepollOn,
    descriptoreventgeneratorepollReg,
    descriptoreventgeneratorepollUnreg
};

extern xdescriptoreventgeneratorepoll * xdescriptoreventgeneratorepollNew(xeventengine * engine, xint32 timeout, xint32 maxevents)
{
    xdescriptoreventgeneratorepoll * o = (xdescriptoreventgeneratorepoll *) xdescriptoreventgeneratorNew(engine, (xdescriptoreventgeneratorset *) xaddressof(virtualSet), sizeof(xdescriptoreventgeneratorepoll));

    o->timeout = timeout;
    o->maxevents = maxevents;
    o->descriptor = xdescriptor_invalid_value;

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

        o->sync = xsyncDel(o->sync);

        free(o);
    }
    return xnil;
}

static void descriptoreventgeneratorepollOn(xdescriptoreventgeneratorepoll * o)
{
    if(o->descriptor < 0)
    {
        o->descriptor = epoll_create(o->maxevents);
    }
    if(o->descriptor >= 0)
    {
        xuint64 size = o->size;
        for(xuint64 i = 0; i < size && o->size >= 0; i++)
        {
            xdescriptoreventsubscription * subscription = descriptoreventgeneratorepollPop(o);

            xdescriptor * descriptor = subscription->descriptor;

            // client 
            if(xdescriptorOpen(descriptor) == xsuccess)
            {
                if(xdescriptorVal(descriptor) >= 0)
                {
                    struct epoll_event event;
                    event.data.ptr = subscription;
                    event.events = (EPOLLIN | EPOLLPRI | EPOLLHUP | EPOLLRDHUP | EPOLLERR | EPOLLONESHOT | EPOLLET);
                    if(xdescriptorInterestEvent(descriptor) & (xdescriptorevent_open | xdescriptorevent_out))
                    {
                        event.events = event.events | EPOLLOUT;
                    }
                    int ret = epoll_ctl(o->descriptor, EPOLL_CTL_ADD, xdescriptorVal(descriptor), xaddressof(event));
                    if(ret < 0)
                    {
                        // TODO:
                        descriptoreventgeneratorepollPush(o, subscription);
                    }
                    else
                    {
                        descriptoreventgeneratorepollAlivePush(o, subscription);
                    }
                }
                else
                {
                    // 
                }
            }
            else
            {
                descriptoreventgeneratorepollPush(o, subscription);
            }
        }
        xfunctionThrow("todo");
        xint32 nfds = epoll_wait(o->descriptor, o->events, o->maxevents, o->timeout);
        if(nfds >= 0)
        {
            for(xint32 i = 0; i < nfds; i++)
            {
                xdescriptoreventsubscription * subscription = (xdescriptoreventsubscription *) (o->events[i].data.ptr);
                if(o->events[i].events & (EPOLLPRI | EPOLLHUP | EPOLLRDHUP | EPOLLERR))
                {
                    xeventqueuePush(o->engine->queue, xeventSet(xeventpoolGet(o->engine->eventpool), (xeventsubscription *) subscription, xdescriptorevent_type | xdescriptorevent_type_error, (xeventhandler) xdescriptoreventOn));
                    continue;
                }
                if(o->events[i].events & (EPOLLIN))
                {
                    xeventqueuePush(o->engine->queue, xeventSet(xeventpoolGet(o->engine->eventpool), (xeventsubscription *) subscription, xdescriptorevent_type | xdescriptorevent_type_in, (xeventhandler) xdescriptoreventOn));
                    continue;
                }
                if(o->events[i].events & (EPOLLOUT))
                {
                    xeventqueuePush(o->engine->queue, xeventSet(xeventpoolGet(o->engine->eventpool), (xeventsubscription *) subscription, xdescriptorevent_type | xdescriptorevent_type_out, (xeventhandler) xdescriptoreventOn));
                    continue;
                }
            }
        }
        else
        {
            printf("todo\n");
        }
    }
}

static xdescriptoreventsubscription * descriptoreventgeneratorepollReg(xdescriptoreventgeneratorepoll * o, xdescriptoreventsubscription * subscription);
static xdescriptoreventsubscription * descriptoreventgeneratorepollUnreg(xdescriptoreventgeneratorepoll * o, xdescriptoreventsubscription * subscription);