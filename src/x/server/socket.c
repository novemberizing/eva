#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#include "socket.h"

static xserversocket * serversocketDel(xserversocket * o);
static xint32 serversocketOpen(xserversocket * o);
static xint64 serversocketRead(xserversocket * o);
static xint64 serversocketWrite(xserversocket * o);
static xint32 serversocketClose(xserversocket * o);
static xint32 serversocketShutdown(xserversocket * o, xint32 how);

static xserversocketset virtualSet = {
    serversocketDel,
    serversocketOpen,
    serversocketRead,
    serversocketWrite,
    serversocketClose,
    serversocketShutdown
};

extern xserversocket * xserversocketNew(xint32 value, xint32 domain, xint32 type, xint32 protocol, const void * address, xuint64 addressLen)
{
    xserversocket * o = (xserversocket *) xsocketNew(value, (xsocketset *) xaddressof(virtualSet), sizeof(xserversocket));

    o->domain = domain;
    o->type = type;
    o->protocol = protocol;

    o->address.length = addressLen;
    o->address.value = malloc(o->address.length);

    memcpy(o->address.value, address, o->address.length);

    return o;
}

static xserversocket * serversocketDel(xserversocket * o)
{
    if(o)
    {
        serversocketShutdown(o, xserversocketshutdown_all);
        serversocketClose(o);

        o->sync = xsyncDel(o->sync);
        o->address.value = xobjectDel(o->address.value);

        free(o);
    }
    return o;
}

static xint32 serversocketOpen(xserversocket * o)
{
    if(o->value < 0)
    {
        o->value = socket(o->domain, o->type, o->protocol);
        if(o->value >= 0)
        {
            if(bind(o->value, (struct sockaddr *) o->address.value, (socklen_t) o->address.length) == xsuccess)
            {
                if(listen(o->value, SOMAXCONN) == xsuccess)
                {

                }
                else
                {
                    serversocketClose(o);
                }
            }
            else
            {
                serversocketClose(o);
            }
        }
        else
        {
            // 소켓을 생성하지 못한다.
        }
    }
    return o->value > 0 ? xsuccess : xfail;
}

static xint64 serversocketRead(xserversocket * o)
{
    // 
}

static xint64 serversocketWrite(xserversocket * o)
{
    return o->value >= 0 ? xsuccess : xfail;
}

static xint32 serversocketClose(xserversocket * o)
{
    if(o->value >= 0)
    {
        xint32 ret = close(o->value);
        o->value = xserversocket_invalid_value;
    }
    return xsuccess;
}

static xint32 serversocketShutdown(xserversocket * o, xint32 how)
{
    if(o->value >= 0)
    {
        if(how == xserversocketshutdown_all)
        {
            how = SHUT_RDWR;
        }
        else if(how == xserversocketshutdown_out)
        {
            how = SHUT_WR;
        }
        else if(how == xserversocketshutdown_in)
        {
            how = SHUT_RD;
        }
        xint32 ret = shutdown(o->value, how);
    }
    return xsuccess;
}