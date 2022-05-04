#ifndef   __NOVEMBERIZING_X_EVENT__POOL__H__
#define   __NOVEMBERIZING_X_EVENT__POOL__H__

#include <x/event.h>

struct xeventpool
{
    xevent * head;
    xevent * tail;
    xuint64 size;
    xuint64 capacity;
};

extern xeventpool * xeventpoolNew(xuint64 capacity);
extern xeventpool * xeventpoolDel(xeventpool * o);
extern xevent * xeventpoolGet(xeventpool * o);
extern void xeventpoolRel(xeventpool * o, xevent * event);

#endif // __NOVEMBERIZING_X_EVENT__POOL__H__
