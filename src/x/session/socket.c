#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>

#include "socket.h"

static xsessionsocket * sessionsocketDel(xsessionsocket * o);
static xint32 sessionsocketOpen(xsessionsocket * o);
static xint64 sessionsocketRead(xsessionsocket * o);
static xint64 sessionsocketWrite(xsessionsocket * o);
static xint32 sessionsocketClose(xsessionsocket * o);
static xint32 sessionsocketShutdown(xsessionsocket * o, xint32 how);

static xsessionsocketset virtualSet = {
    sessionsocketDel,
    sessionsocketOpen,
    sessionsocketRead,
    sessionsocketWrite,
    sessionsocketClose,
    sessionsocketShutdown
};

extern xsessionsocket * xsessionsocketNew(xint32 value)
{
    xsessionsocket * o = (xsessionsocket *) xsocketNew(value, (xsocketset *) xaddressof(virtualSet), sizeof(xsessionsocket));

    o->stream.in = xstreamNew(0, 0);
    o->stream.out = xstreamNew(0, 0);

    return o;
}

static xsessionsocket * sessionsocketDel(xsessionsocket * o)
{
    if(o)
    {
        sessionsocketShutdown(o, xsessionsocketshutdown_all);
        sessionsocketClose(o);

        if(o->sessionsocketpool.container) xsessionsocketpoolRem(o->sessionsocketpool.container, o);
        if(o->serversocket.container) xserversocketRem(o->serversocket.container, o);

        o->sync = xsyncDel(o->sync);
        o->stream.in = xstreamDel(o->stream.in);
        o->stream.out = xstreamDel(o->stream.out);
        o->address.value = xobjectDel(o->address.value);

        free(o);
    }
    return xnil;
}

static xint32 sessionsocketOpen(xsessionsocket * o)
{
    return xsuccess;
}

static xint64 sessionsocketRead(xsessionsocket * o)
{
    if(o->value >= 0)
    {
        xstreamReserve(o->stream.in, 8192, 1024);

        xint64 ret = read(o->value, xstreamBack(o->stream.in), xstreamRemain(o->stream.in));

        if(ret > 0)
        {
            o->stream.in->size = o->stream.in->size + ret;
            return ret;
        }
        else if(ret == 0)
        {
            return xfail;
        }
        else
        {
            if(errno == EAGAIN)
            {
                return xsuccess;
            }
            return xfail;
        }
    }
    else
    {
        return xfail;
    }
}
static xint64 sessionsocketWrite(xsessionsocket * o)
{
    if(o->value >= 0)
    {
        if(xstreamLen(o->stream.out) > 0)
        {
            xint64 ret = write(o->value, xstreamFront(o->stream.out), xstreamLen(o->stream.out));

            if(ret > 0)
            {
                o->stream.out->position = o->stream.out->position + ret;
                return ret;
            }
            else if(ret == 0)
            {
                return xsuccess;
            }
            else
            {
                if(errno == EAGAIN)
                {
                    return xsuccess;
                }
                return xfail;
            }
        }
        else
        {
            return xsuccess;
        }
    }
    else
    {
        return xfail;
    }
}

static xint32 sessionsocketClose(xsessionsocket * o)
{
    if(o->value >= 0)
    {
        xint32 ret = close(o->value);
        o->value = xdescriptor_invalid_value;
    }
    return xsuccess;
}

static xint32 sessionsocketShutdown(xsessionsocket * o, xint32 how)
{
    if(o->value >= 0)
    {
        if(how == xsocketshutdown_in)
        {
            how = SHUT_RD;
        }
        else if(how == xsocketshutdown_out)
        {
            how = SHUT_WR;
        }
        else if(how == xsocketshutdown_all)
        {
            how = SHUT_RDWR;
        }
        xint32 ret = shutdown(o->value, how);
    }
    return xsuccess;
}
