#ifndef   __NOVEMBERIZING_X_EVENT__POOL__H__
#define   __NOVEMBERIZING_X_EVENT__POOL__H__

#include <x/event.h>

struct xeventpool;
struct xeventpoolset;

typedef struct xeventpool xeventpool;
typedef struct xeventpoolset xeventpoolset;

struct xeventpool
{
    const xeventpoolset * set;

    xsync * sync;
    xevent * head;
    xevent * tail;
    xuint64 size;
    xuint64 capacity;
};

struct xeventpoolset
{
    xeventpool * (*del)(xeventpool *);
    xevent * (*get)(xeventpool *);
    xevent * (*rel)(xeventpool *, xevent *);
    void (*clear)(xeventpool *);
};

extern xeventpool * xeventpoolNew(xuint64 capacity);

#define xeventpoolDel(o)            (o->set->del(o))
#define xeventpoolGet(o)            (o->set->get(o))
#define xeventpoolRel(o, event)     (o->set->rel(o, event)
#define xeventpoolClear(o)          (o->set->clear(o))

#endif // __NOVEMBERIZING_X_EVENT__POOL__H__
