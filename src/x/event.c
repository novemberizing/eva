#include <stdlib.h>

#include "event.h"

extern xevent * xeventNew(void)
{
    xevent * o = (xevent *) calloc(1, sizeof(xevent));

    return o;
}

extern xevent * xeventDel(xevent * o)
{
    if(o)
    {
        free(o);
    }
    
    return xnil;
}

extern xevent * xeventSet(xevent * o, xeventsubscription * subscription, xuint32 type, xeventhandler on)
{
    o->subscription = subscription;
    o->type = type;
    o->on = on;

    return o;
}