#include <stdio.h>
#include <stdlib.h>

#include "socket.h"

#include "descriptor.h"

xsocket * xsocketNew(xint32 value, xint32 domain, xint32 type, xint32 protocol, const xsocketset * set, xuint64 size)
{
    xassert(size < sizeof(xsocket) || set == xnil, "invalid parameter");

    xsocket * o = (xsocket *) xdescriptorNew(value, (xdescriptorset *) set, size);

    o->domain = domain;
    o->type = type;
    o->protocol = protocol;

    return o;
}