#include <stdio.h>
#include <stdlib.h>

#include "client.h"

#include "client/socket.h"

static xclient * clientDel(xclient * o);
static xint32 clientOpen(xclient * o);
static xint64 clientRead(xclient * o);
static xint64 clientWrite(xclient * o);
static xint32 clientClose(xclient * o);
static xint32 clientShutdown(xclient * o, xint32 how);
static xint32 clientConnect(xclient * o);
static xint64 clientSend(xclient * o, const unsigned char * message, xuint64 length);
static xint64 clientRecv(xclient * o, unsigned char * buffer, xuint64 length);

static xclientset virtualSet = {
    clientDel,
    clientOpen,
    clientRead,
    clientWrite,
    clientClose,
    clientShutdown,
    clientConnect,
    clientSend,
    clientRecv
};

extern xclient * xclientNew(xint32 domain, xint32 type, xint32 protocol, const void * address, xuint64 addressLen)
{
    xclient * o = (xclient *) calloc(1, sizeof(xclient));

    o->set = xaddressof(virtualSet);
    o->socket = xclientsocketNew(xclientsocket_invalid_value, domain, type, protocol, address, addressLen);

    return o;
}

static xclient * clientDel(xclient * o)
{
    if(o)
    {
        o->socket = xclientsocketDel(o->socket);

        free(o);
    }
    return xnil;
}

static xint32 clientOpen(xclient * o)
{
    return xclientsocketOpen(o->socket);
}

static xint64 clientRead(xclient * o)
{
    return xclientsocketRead(o->socket);
}

static xint64 clientWrite(xclient * o)
{
    return xclientsocketWrite(o->socket);
}

static xint32 clientClose(xclient * o)
{
    return xclientsocketClose(o->socket);
}

static xint32 clientShutdown(xclient * o, xint32 how)
{
    return xclientsocketShutdown(o->socket, how);
}

static xint32 clientConnect(xclient * o)
{
    return xclientsocketConnect(o->socket);
}

static xint64 clientSend(xclient * o, const unsigned char * message, xuint64 length)
{
    return xclientsocketSend(o->socket, message, length);
}

static xint64 clientRecv(xclient * o, unsigned char * buffer, xuint64 length)
{
    return xclientsocketRecv(o->socket, buffer, length);
}
