#ifndef   __NOVEMBERIZING_X_EVENT__QUEUE__H__
#define   __NOVEMBERIZING_X_EVENT__QUEUE__H__

#include <x/event.h>

struct xeventqueueset;
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
    void (*add)(xeventqueue *, xevent *);
    void (*rem)(xeventqueue *, xevent *);
    void (*clear)(xeventqueue *);

    void (*push)(xeventqueue *, xevent *);
    xevent * (*pop)(xeventqueue *);
};

extern xeventqueue * xeventqueueNew(void);

#define xeventqueueDel(o)               (o->set->del(o))
#define xeventqueueAdd(o, event)        (o->set->add(o, event))
#define xeventqueueRem(o, event)        (o->set->rem(o, event))
#define xeventqueueClear(o)             (o->set->clear(o))

#define xeventqueuePush(o, event)       (o->set->push(o, event))
#define xeventqueuePop(o)               (o->set->pop(o))


#endif // __NOVEMBERIZING_X_EVENT__QUEUE__H__
