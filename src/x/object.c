#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#include "object.h"

extern xobject * xobjectNew(const void * value, xuint64 length)
{
    xobject * o = (xobject *) calloc(1, sizeof(xobject));

    o->length = length;
    if(o->length > 0)
    {
        o->value = malloc(o->length);
        memcpy(o->value, value, o->length);
    }

    return o;
}

extern void xobjectSet(xobject * o, const void * value, xuint64 length)
{
    if(o->length != length)
    {
        if(o->value)
        {
            free(o->value);
            o->value = xnil;
        }

        o->length = length;

        if(o->length)
        {
            o->value = malloc(o->length);
            memcpy(o->value, value, o->length);
        }
    }
}

extern xobject * xobjectDel(xobject * o)
{
    if(o)
    {
        if(o->value)
        {
            free(o->value);
        }

        free(o);
    }
    return o;
}