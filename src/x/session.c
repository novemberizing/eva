#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>

#include "session.h"

static xsession * sessionDel(xsession * o);
static xint32 sessionVal(xsession * o);
static xint32 sessionOpen(xsession * o);
static xint64 sessionRead(xsession * o);
static xint64 sessionWrite(xsession * o);
static xint32 sessionClose(xsession * o);
static xint32 sessionShutdown(xsession * o, xint32 how);

static xsessionset virtualSet = {
    sessionDel,
    sessionOpen,
    sessionRead,
    sessionWrite,
    sessionClose,
    sessionShutdown
};

extern xsession * xsessionNew(void)
{
    xsession * o = (xsession *) xsessionsocketNew(xdescriptor_invalid_value, (xsessionsocketset *) xaddressof(virtualSet), sizeof(xsession));

    return o;
}

static xsession * sessionDel(xsession * o)
{
    if(o)
    {
        sessionClose(o);

        if(o->server) xserverRel(o->server, o);
        if(o->sessionpool) xsessionpoolRem(o->sessionpool, o);

        o->stream.in = xstreamDel(o->stream.in);
        o->stream.out = xstreamDel(o->stream.out);

        xobjectSet(xaddressof(o->address), xnil, 0);

        o->sync = xsyncDel(o->sync);

        free(o);
    }
    return o;
}


static xint32 sessionOpen(xsession * o)
{
    return xfail;
}

static xint64 sessionRead(xsession * o)
{
    if(o->value >= 0)
    {
        xstreamReserve(o->stream.in, 8192, 1024);

        xint32 n = read(o->value, xstreamBack(o->stream.in), xstreamRemain(o->stream.in));

        if(n > 0)
        {

        }
        else if(n == 0)
        {

        }
        else
        {
            if(errno == EAGAIN)
            {

            }
            else
            {

            }
        }
    }
    else
    {
        return xfail;
    }
}

static xint64 sessionWrite(xsession * o)
{
    // xfunctionAssert(o->socket == xnil, "invalid parameter");

    // return o->socket ? xsessionsocketWrite(o->socket) : xfail;
}

static xint32 sessionClose(xsession * o)
{
    // xfunctionAssert(o->socket == xnil, "invalid parameter");

    // xint32 ret = xsessionsocketClose(o->socket);

    // xserverRel(o->parent.server, o);

    // return ret;
}

static xint32 sessionShutdown(xsession * o, xint32 how)
{
    // xfunctionAssert(o->socket == xnil, "invalid parameter");

    // return o->socket ? xsessionsocketShutdown(o, how) : xfail;
}
