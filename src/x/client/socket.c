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
static xuint32 clientsocketInterest(xclientsocket * o);

static xint32 clientsocketShutdown(xclientsocket * o, xint32 how);

static xint32 clientsocketConnect(xclientsocket * o, const void * address, xuint64 addresslen);
static xint64 clientsocketSend(xclientsocket * o, const unsigned char * message, xuint64 length);
static xint64 clientsocketRecv(xclientsocket * o, unsigned char * buffer, xuint64 length);

static xclientsocketset virtualSet = {
    clientsocketDel,
    clientsocketOpen,
    clientsocketRead,
    clientsocketWrite,
    clientsocketClose,
    clientsocketInterest,
    clientsocketShutdown,
    clientsocketConnect,
    clientsocketSend,
    clientsocketRecv
};

extern xclientsocket * xclientsocketNew(xint32 value, xint32 domain, xint32 type, xint32 protocol, const void * address, xuint64 addresslen, xuint64 size)
{
    xclientsocket * o = (xclientsocket *) xsocketNew(value, (xsocketset *) xaddressof(virtualSet), size);

    o->domain = domain;
    o->type = type;
    o->protocol = protocol;

    o->address.length = addresslen;
    o->address.value = malloc(o->address.length);
    memcpy(o->address.value, address, o->address.length);

    o->stream.in = xstreamNew(8192, 1024);
    o->stream.out = xstreamNew(8192, 1024);

    return o;
}

static xclientsocket * clientsocketDel(xclientsocket * o)
{
    if(o)
    {
        clientsocketClose(o);

        o->stream.in = xstreamDel(o->stream.in);
        o->stream.out = xstreamDel(o->stream.out);

        xobjectSet(xaddressof(o->address), xnil, 0);
        o->sync = xsyncDel(o->sync);

        free(o);
    }
    return xnil;
}

static xint32 clientsocketOpen(xclientsocket * o)
{
    if(o->value < 0)
    {
        o->value = socket(o->domain, o->type, o->protocol);

        if(o->value >= 0)
        {
            if(o->mode == xclientsocketmode_nonblock)
            {
                xint32 flags = fcntl(o->value, F_GETFL, 0);
                fcntl(o->value, F_SETFL, flags | O_NONBLOCK);
            }

            xint32 ret = connect(o->value, (struct sockaddr *) o->address.value, o->address.length);

            if(ret == xsuccess)
            {
                if(o->subscription)
                {
                    // o->subscription->on(o, o->subscription, xclientsocketevent_open, ...);
                }
            }
            else
            {
                if(o->subscription)
                {
                    // o->subscription->on(o, o->subscription, xclientsocketevent_error, xclientsocketevent_open, errno);
                }                
            }
        }
        else
        {
            if(o->subscription)
            {
                // o->subscription->on(o, o->subscription, xclientsocketevent_error, xclientsocketevent_open, errno);
            }
            xfunctionError("fail to socket(...) caused by %d", errno);

            return xfail;
        }
    }

    return xsuccess;
}

static xint64 clientsocketRead(xclientsocket * o)
{
    xint64 ret = xfail;

    if(o->value >= 0)
    {
        xstreamReserve(o->stream.in, 8192, 1024);
        ret = read(o->value, xstreamBack(o->stream.in), xstreamRemain(o->stream.in));
        if(ret > 0)
        {
            o->stream.in->size = o->stream.in->size + ret;
            // TODO: RECV EVENT HANDLING
        }
        else if(ret == 0)
        {
            // TODO: CLOSE EVENT HANDLING
            clientsocketClose(o);
            ret = xfail;
        }
        else
        {
            if(errno == EAGAIN)
            {
                ret = xsuccess;
            }
            else
            {
                // TODO: ERROR EVENT HANDLING
            }
        }
    }

    return ret;
}

static xint64 clientsocketWrite(xclientsocket * o)
{
    xint64 ret = xfail;
    if(o->value >= 0)
    {
        if(xstreamLen(o->stream.out) >= 0)
        {
            ret = write(o->value, xstreamFront(o->stream.out), xstreamLen(o->stream.out));

            if(ret > 0)
            {
                xstreamRem(o->stream.out, ret);
            }
            else if(ret == 0)
            {
                // TODO: UNKNOWN ERROR HANDLING
                ret = xfail;
            }
            else
            {
                if(errno == EAGAIN)
                {
                    ret = xsuccess;
                }
                else
                {
                    // TODO: ERROR HANDLING
                }
            }
        }
        else
        {
            ret = xsuccess;
        }
    }
    return ret;
}

static xint32 clientsocketClose(xclientsocket * o)
{
    if(o->value >= 0)
    {
        close(o->value);
        o->value = xclientsocket_invalid_value;
        // TODO: ON CLOSE
    }
    return xsuccess;
}

static xuint32 clientsocketInterest(xclientsocket * o)
{
    xuint32 interest = xclientsocketevent_none;
    if(o->value >= 0)
    {
        if((o->status & xclientsocketstatus_in) == xclientsocketstatus_none)
        {
            interest = interest | xclientsocketevent_in;
        }
        if((o->status & xclientsocketstatus_out) == xclientsocketstatus_none)
        {
            if(xstreamLen(o->stream.out) > 0)
            {
                interest = interest | xclientsocketevent_out;
            }
        }
        if((o->status & xclientsocketstatus_open) == xclientsocketstatus_none)
        {
            interest = interest | xclientsocketevent_open;
        }
    }
    else
    {
        interest = interest | xclientsocketevent_open;
    }
    return xclientsocketevent_none;
}

static xint32 clientsocketShutdown(xclientsocket * o, xint32 how)
{
    if(o->value >= 0)
    {
        if(how)
        {
            if(how == xclientsocketshutdown_all)
            {
                how = SHUT_RDWR;
            }
            else if(how == xclientsocketshutdown_in)
            {
                how = SHUT_RD;
            }
            else if(how == xclientsocketshutdown_out)
            {
                how = SHUT_WR;
            }
            shutdown(o->value, how);
            // TODO: EVENT HANDLING
        }
    }
    return xsuccess;
}

static xint32 clientsocketConnect(xclientsocket * o, const void * address, xuint64 addresslen)
{
    if(o->value < 0)
    {
        xobjectSet(xaddressof(o->address), address, addresslen);

        o->value = socket(o->domain, o->type, o->protocol);

        if(o->value >= 0)
        {
            xint32 ret = connect(o->value, (struct sockaddr *) o->address.value, o->address.length);

            if(ret == xsuccess)
            {
                // SUCESS HANDLING
                return xsuccess;
            }
            else
            {
                // ERROR HANDLING

                return xfail;
            }
        }
        else
        {
            // ERROR HANDLING
            return xfail;
        }
    }
    return xsuccess;
}

static xint64 clientsocketSend(xclientsocket * o, const unsigned char * message, xuint64 length)
{
    xint64 ret = xfail;

    return ret;
}

static xint64 clientsocketRecv(xclientsocket * o, unsigned char * buffer, xuint64 length)
{
    xint64 ret = xfail;

    return ret;
}

/**
static xclientsocket * clientsocketDel(xclientsocket * o);

static xint32 clientsocketOpen(xclientsocket * o);
static xint64 clientsocketRead(xclientsocket * o);
static xint64 clientsocketWrite(xclientsocket * o);
static xint32 clientsocketClose(xclientsocket * o);
static xuint32 clientsocketInterest(xclientsocket * o);

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
    clientsocketInterest,
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
        clientsocketClose(o);

        xobjectSet(xaddressof(o->address), xnil, 0);

        o->stream.in = xstreamDel(o->stream.in);
        o->stream.out = xstreamDel(o->stream.out);

        o->sync = xsyncDel(o->sync);

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
            o->status = o->status | xclientsocketstatus_in;
            return ret;
        }
        else if(ret == 0)
        {
            o->status = o->status & (~xclientsocketstatus_in);
            o->status = o->status | xclientsocketstatus_error;
            return xfail;
        }
        else
        {
            o->status = o->status & (~xclientsocketstatus_in);
            if(errno == EAGAIN)
            {
                return xsuccess;
            }
            o->status = o->status | xclientsocketstatus_error;
            return xfail;
        }
    }
    else
    {
        o->status = o->status | xclientsocketstatus_error;
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
                o->status = o->status | xclientsocketstatus_out;
                return ret;
            }
            else if(ret == 0)
            {
                o->status = o->status & (~xclientsocketstatus_out);
                return xsuccess;
            }
            else
            {
                o->status = o->status & (~xclientsocketstatus_out);
                if(errno == EAGAIN)
                {
                    return xsuccess;
                }
                o->status = o->status | xclientsocketstatus_error;
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
        o->status = o->status | xclientsocketstatus_error;
        return xfail;
    }
}

static xint32 clientsocketClose(xclientsocket * o)
{
    if(o->value >= 0)
    {
        clientsocketShutdown(o->value, xclientsocketshutdown_all);
        xint32 ret = close(o->value);
        o->status = xclientsocketstatus_close;
        o->value = xdescriptor_invalid_value;
    }
    return xsuccess;
}

static xuint32 clientsocketInterest(xclientsocket * o)
{
    xuint32 interest = xclientsocketstatus_none;
    if(o->value >= 0)
    {
        if((o->status & xclientsocketstatus_open) == xclientsocketstatus_none)
        {
            interest = interest | xclientsocketstatus_open;
        }
        if((o->status & xclientsocketstatus_in) == xclientsocketstatus_none)
        {
            interest = interest | xclientsocketstatus_in;
        }
        if((o->status & xclientsocketstatus_out) == xclientsocketstatus_none)
        {
            if(xstreamLen(o->stream.out) > 0)
            {
                interest = interest | xclientsocketstatus_out;
            }
        }
    }
    else
    {
        interest = interest | xclientsocketstatus_open;
    }
    return interest;
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
    }

    if(o->value >= 0)
    {
        if(o->mode & xclientsocketmode_nonblock)
        {
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
            len = xstreamPop(o->stream.in, buffer, xstreamLen(o->stream.in));
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
*/
