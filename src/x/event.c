#include <stdlib.h>

#include "event.h"
#include "event/queue.h"

extern xevent * xeventnew(xeventobject * object, xuint32 flags)
{
    xevent * o = (xevent *) calloc(1, sizeof(xevent));

    o->object = object;
    o->flags = flags;

    return o;
}

extern xevent * xeventdel(xevent * o)
{
    if(o)
    {
        if(o->queue)
        {
            xeventqueue * queue = o->queue;
            
            xeventqueuelock(queue);
            xeventqueuerem(queue, o);
            xeventqueueunlock(queue);
        }
        free(o);
    }
    return xnil;
}