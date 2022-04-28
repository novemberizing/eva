#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "session.h"

static xsession * sessionDel(xsession * o);
static xint64 sessionRead(xsession * o);
static xint64 sessionWrite(xsession * o);
static xint32 sessionClose(xsession * o);

static xsessionset virtualSet = {
    sessionDel,
    sessionRead,
    sessionWrite,
    sessionClose
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

static xint64 sessionRead(xsession * o)
{
    return o->socket ? xsessionsocketRead(o->socket) : xfail;
}

static xint64 sessionWrite(xsession * o)
{
    return o->socket ? xsessionsocketWrite(o->socket) : xfail;
}

static xint32 sessionClose(xsession * o)
{
    xint32 ret = xsessionsocketClose(o->socket);

    xserverRel(o->server.container, o);

    return ret;
}
