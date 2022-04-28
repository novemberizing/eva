#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "socket.h"

#include "descriptor.h"

xsocket * xsocketNew(xint32 value, const xsocketset * set, xuint64 size)
{
    xfunctionAssert(size < sizeof(xsocket) || set == xnil, "invalid parameter");

    xsocket * o = (xsocket *) xdescriptorNew(value, (xdescriptorset *) set, size);

    return o;
}