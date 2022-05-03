#ifndef   __NOVEMBERIZING_X_EVENT__QUEUE__H__
#define   __NOVEMBERIZING_X_EVENT__QUEUE__H__

#include <x/event.h>

struct xeventqueue;
struct xeventqueueset;

typedef struct xeventqueue xeventqueue;
typedef struct xeventqueueset xeventqueueset;

struct xeventqueue
{
    const xeventqueueset * set;

    xsync * sync;

    xevent * head;
    xevent * tail;
    xuint64 size;
};

struct xeventqueueset
{
    xeventqueue * (*del)(xeventqueue *);
    void (*push)(xeventqueue *, xevent *);
    xevent * (*pop)(xeventqueue *);
    void (*clear)(xeventqueue *);
    void (*rem)(xeventqueue *, xevent *);
};

extern xeventqueue * xeventqueueNew(void);

#define xeventqueueDel(o)                       (o->set->del(o))
#define xeventqueuePush(o, event)               (o->set->push(o, event))
#define xeventqueuePop(o)                       (o->set->pop(o))
#define xeventqueueClear(o)                     (o->set->clear(o))
#define xeventqueueRem(o, event)                (o->set->rem(o))

#define xeventqueueLock(o)                      xsyncLock(o->sync)
#define xeventqueueUnlock(o)                    xsyncUnlock(o->sync)
#define xeventqueueWait(o, second, nanosecond)  xsyncWait(o->sync, second, nanosecond)
#define xeventqueueWakeup(o, all)               xsyncWakeup(o->sync, all)

#endif // __NOVEMBERIZING_X_EVENT__QUEUE__H__
