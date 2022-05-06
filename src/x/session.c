#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

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
    xsession * o = (xsession *) calloc(1, sizeof(xsession));

    o->set = xaddressof(virtualSet);
    o->socket = xnil;

    return o;
}

static xsession * sessionDel(xsession * o)
{
    if(o)
    {
        if(o->socket) o->socket = xsessionsocketDel(o->socket);

        free(o);
    }
    return o;
}

static xint32 sessionVal(xsession * o)
{
    xfunctionAssert(o->socket == xnil, "invalid parameter");

    return o->socket ? xsessionsocketVal(o->socket) : xdescriptor_invalid_value;
}

static xint32 sessionOpen(xsession * o)
{
    xfunctionAssert(o->socket == xnil, "invalid parameter");

    return o->socket ? xsessionsocketOpen(o->socket) : xfail;
}

static xint64 sessionRead(xsession * o)
{
    xfunctionAssert(o->socket == xnil, "invalid parameter");

    return o->socket ? xsessionsocketRead(o->socket) : xfail;
}

static xint64 sessionWrite(xsession * o)
{
    xfunctionAssert(o->socket == xnil, "invalid parameter");

    return o->socket ? xsessionsocketWrite(o->socket) : xfail;
}

static xint32 sessionClose(xsession * o)
{
    xfunctionAssert(o->socket == xnil, "invalid parameter");

    xint32 ret = xsessionsocketClose(o->socket);

    xserverRel(o->parent.server, o);

    return ret;
}

static xint32 sessionShutdown(xsession * o, xint32 how)
{
    xfunctionAssert(o->socket == xnil, "invalid parameter");

    return o->socket ? xsessionsocketShutdown(o, how) : xfail;
}
