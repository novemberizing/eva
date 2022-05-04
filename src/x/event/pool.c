#include <stdlib.h>

#include "pool.h"

extern xeventpool * xeventpoolNew(xuint64 capacity)
{
    xeventpool * o = (xeventpool *) calloc(1, sizeof(xeventpool));

    o->capacity = capacity;

    for(xuint64 i = 0; i < o->capacity; i++)
    {
        eventpoolPush(o, xeventNew());
    }

    return o;
}

extern xeventpool * xeventpoolDel(xeventpool * o)
{
    if(o)
    {
        eventpoolClear(o);
        free(o);
    }
    return xnil;
}

extern xevent * xeventpoolGet(xeventpool * o)
{
    xevent * event = eventpoolPop(o);

    if(event == xnil)
    {
        event = xeventNew();
    }

    return event;
}

extern void xeventpoolRel(xeventpool * o, xevent * event)
{
    if(o->size < o->capacity)
    {

    }
    else
    {
        xeventDel(event);
    }
}