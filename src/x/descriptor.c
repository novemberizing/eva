#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "descriptor.h"

extern xdescriptor * xdescriptorNew(xint32 value, const xdescriptorset * set, xuint64 size)
{
    xfunctionAssert(size < sizeof(xdescriptor) || set == xnil, "invalid parameter");

    xdescriptor * o = (xdescriptor *) calloc(1, size);

    o->value = value;
    o->set = set;

    return o;
}