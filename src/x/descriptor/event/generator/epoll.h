#ifndef   __NOVEMBERIZING_X_DESCRIPTOR_EVENT_GENERATOR__EPOLL__H__
#define   __NOVEMBERIZING_X_DESCRIPTOR_EVENT_GENERATOR__EPOLL__H__

#include <sys/epoll.h>

#include <x/descriptor/event/generator.h>

struct xdescriptoreventgeneratorepoll;
struct xdescriptoreventgeneratorepollset;

typedef struct xdescriptoreventgeneratorepoll xdescriptoreventgeneratorepoll;
typedef struct xdescriptoreventgeneratorepollset xdescriptoreventgeneratorepollset;

struct xdescriptoreventgeneratorepoll
{
    const xdescriptoreventgeneratorepollset * set;

    xsync * sync;
    xeventsubscription * head;
    xeventsubscription * tail;
    xuint64 size;

    xeventengine * engine;

    xint32 descriptor;
    xint32 timeout;
    struct epoll_event * events;
    xuint64 maxevents;
};

struct xdescriptoreventgeneratorepollset
{
    xdescriptoreventgeneratorepoll * (*del)(xdescriptoreventgeneratorepoll *);
    xeventsubscription * (*registersubscription)(xdescriptoreventgeneratorepoll *, xeventsubscription *);
    xeventsubscription * (*unregistersubscription)(xdescriptoreventgeneratorepoll *, xeventsubscription *);
    void (*clear)(xdescriptoreventgeneratorepoll *);
};

extern xdescriptoreventgeneratorepoll * xdescriptoreventgeneratorepollNew(xeventengine * engine, xint32 timeout, xuint64 maxevents);

#define xdescriptoreventgeneratorepollDel(o)                                    (o->set->del(o))
#define xdescriptoreventgeneratorepollRegisterSubscription(o, subscription)     (o->set->registersubscription(o, subscription))
#define xdescriptoreventgeneratorepollUnregisterSubscription(o, subscription)   (o->set->unregistersubscription(o, subscription))
#define xdescriptoreventgeneratorepollClear(o)                                  (o->set->clear(o))

#endif // __NOVEMBERIZING_X_DESCRIPTOR_EVENT_GENERATOR__EPOLL__H__
