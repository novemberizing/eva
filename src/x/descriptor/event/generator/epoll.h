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

    xdescriptoreventsubscription * head;
    xdescriptoreventsubscription * tail;
    xuint64 size;

    xeventengine * engine;

    xint32 descriptor;
    xint32 timeout;
    xint32 maxevents;

    struct epoll_event * events;

    struct {
        xdescriptoreventsubscription * head;
        xdescriptoreventsubscription * tail;
        xuint64 size;
    } alive;
};

struct xdescriptoreventgeneratorepollset
{
    xdescriptoreventgeneratorepoll * (*del)(xdescriptoreventgeneratorepoll *);
    void (*on)(xdescriptoreventgeneratorepoll *);
    xdescriptoreventsubscription * (*reg)(xdescriptoreventgeneratorepoll *, xdescriptoreventsubscription *);
    xdescriptoreventsubscription * (*unreg)(xdescriptoreventgeneratorepoll *, xdescriptoreventsubscription *);
};

extern xdescriptoreventgeneratorepoll * xdescriptoreventgeneratorepollNew(xeventengine * engine, xint32 timeout, xint32 maxevents);

#define xdescriptoreventgeneratorepollDel(o)                    (o->set->del(o))
#define xdescriptoreventgeneratorepollOn(o)                     (o->set->on(o))
#define xdescriptoreventgeneratorepollReg(o, subscription)      (o->set->reg(o, subscription))
#define xdescriptoreventgeneratorepollUnreg(o, subscription)    (o->set->unreg(o, subscription))

#endif // __NOVEMBERIZING_X_DESCRIPTOR_EVENT_GENERATOR__EPOLL__H__
