#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>

#include "socket.h"

#include "../socket.h"

static xclientsocket * clientsocketDel(xclientsocket * o);
static xint32 clientsocketOpen(xclientsocket * o);
static xint64 clientsocketRead(xclientsocket * o);
static xint64 clientsocketWrite(xclientsocket * o);
static xint32 clientsocketClose(xclientsocket * o);
static xint32 clientsocketShutdown(xclientsocket * o, xint32 how);
static xint32 clientsocketConnect(xclientsocket * o);
static xint64 clientsocketSend(xclientsocket * o, const unsigned char * message, xuint64 length);
static xint64 clientsocketRecv(xclientsocket * o, unsigned char * message, xuint64 length);

static xclientsocketset virtualSet = {
    clientsocketDel,
    clientsocketOpen,
    clientsocketRead,
    clientsocketWrite,
    clientsocketClose,
    clientsocketShutdown,
    clientsocketConnect,
    clientsocketSend,
    clientsocketRecv
};

extern xclientsocket * xclientsocketNew(xint32 value, xint32 domain, xint32 type, xint32 protocol, const void * address, xuint64 addresslen)
{
    xclientsocket * o = (xclientsocket *) xsocketNew(value, (xsocketset *) xaddressof(virtualSet), sizeof(xclientsocket));

    o->domain = domain;
    o->type = type;
    o->protocol = protocol;

    xobjectSet(xaddressof(o->address), address, addresslen);

    o->stream.in = xstreamNew(0, 0);
    o->stream.out = xstreamNew(0, 0);

    return o;
}

static xclientsocket * clientsocketDel(xclientsocket * o)
{
    if(o)
    {
        clientsocketShutdown(o, xsocketshutdown_all);
        clientsocketClose(o);
        o->sync = xsyncDel(o->sync);
        o->stream.in = xstreamDel(o->stream.in);
        o->stream.out = xstreamDel(o->stream.out);

        xobjectSet(xaddressof(o->address), xnil, 0);

        free(o);

    }
    return xnil;
}

static xint32 clientsocketOpen(xclientsocket * o)
{
    return clientsocketConnect(o);
}

static xint64 clientsocketRead(xclientsocket * o)
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

static xint64 clientsocketWrite(xclientsocket * o)
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

static xint32 clientsocketClose(xclientsocket * o)
{
    if(o->value >= 0)
    {
        xint32 ret = close(o->value);
        o->value = xdescriptor_invalid_value;
    }
    return xsuccess;
}

static xint32 clientsocketShutdown(xclientsocket * o, xint32 how)
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

static xint32 clientsocketConnect(xclientsocket * o)
{
    if(o->value < 0)
    {
        o->value = socket(o->domain, o->type, o->protocol);
        // nonblock connect
    }

    if(o->value >= 0)
    {
        if(o->mode & xclientsocketmode_nonblock)
        {
            // 에러 핸들링을 하자.
            int flags = fcntl(o->value, F_GETFL, 0);
            fcntl(o->value, F_SETFL, flags | O_NONBLOCK);
        }
        xint32 ret = connect(o->value, (struct sockaddr *) o->address.value, o->address.length);

        if(ret < 0)
        {
            if(errno == EAGAIN || errno == EALREADY || errno == EINPROGRESS)
            {
                ret = xsuccess;
            }
        }

        return ret;
    }
    else
    {
        return xfail;
    }
}

static xint64 clientsocketSend(xclientsocket * o, const unsigned char * message, xuint64 length)
{
    if(o->value >= 0)
    {
        xstreamReserve(o->stream.out, 8192, 1024);
        xstreamPush(o->stream.out, message, length);

        return clientsocketWrite(o);
    }
    else
    {
        return xfail;
    }
}

static xint64 clientsocketRecv(xclientsocket * o, unsigned char * buffer, xuint64 length)
{
    if(length <= xstreamLen(o->stream.in))
    {
        xstreamPop(o->stream.in, buffer, length);
        return length;
    }
    else
    {
        xint64 len = 0;
        if(xstreamLen(o->stream.in) > 0)
        {
            len = xstreamPop(o->stream.in, buffer, len);
        }

        xint64 ret = read(o->value, xaddressof(buffer[len]), length - len);

        if(ret > 0)
        {
            return len + ret;
        }
        else if(ret == 0)
        {
            return xfail;
        }
        else
        {
            if(errno == EAGAIN)
            {
                return len;
            }
            return xfail;
        }
    }
}
