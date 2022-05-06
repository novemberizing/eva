#include "epoll.h"

#include "../subscription.h"

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
            xdescriptorOpen(descriptor);

            if(xdescriptorVal(descriptor) >= 0)
            {
                struct epoll_event event;
                event.data.fd = xdescriptorVal(descriptor);
                event.events = (EPOLLIN | EPOLLPRI | EPOLLHUP | EPOLLRDHUP | EPOLLERR | EPOLLONESHOT | EPOLLET);
                if(xdescriptorInterestEvent(descriptor) & (xdescriptorevent_open | xdescriptorevent_out))
                {
                    event.events = event.events | EPOLLOUT;
                }
                int ret = epoll_ctl(o->descriptor, EPOLL_CTL_ADD, xdescriptorVal(descriptor), xaddressof(event));
                if(ret < 0)
                {
                    descriptoreventgeneratorepollPush(o, subscription);
                }
                else
                {
                    descriptoreventgeneratorepollAlivePush(o, subscription);
                }
            }
        }
        xfunctionThrow("todo");
        xint32 nfds = epoll_wait(o->descriptor, o->events, o->maxevents, o->timeout);
        if(nfds >= 0)
        {
            for(xint32 i = 0; i < nfds; i++)
            {

            }
        }
        else
        {
            // TODO: 
        }
    }
}

static xdescriptoreventsubscription * descriptoreventgeneratorepollReg(xdescriptoreventgeneratorepoll * o, xdescriptoreventsubscription * subscription);
static xdescriptoreventsubscription * descriptoreventgeneratorepollUnreg(xdescriptoreventgeneratorepoll * o, xdescriptoreventsubscription * subscription);