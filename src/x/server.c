#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "server.h"

static xserver * serverDel(xserver * o);
static xint32 serverOpen(xserver * o);
static xint32 serverClose(xserver * o);
static xsession * serverAccept(xserver * o);
static void serverRel(xserver * o, xsession * session);

static xserverset virtualSet = {
    serverDel,
    serverOpen,
    serverClose,
    serverAccept,
    serverRel
};

static void serverPush(xserver * o, xsession * session);
static void serverRem(xserver * o, xsession * session);

extern xserver * xserverNew(xint32 domain, xint32 type, xint32 protocol, const void * address, xuint64 addressLen, xsessionpool * sessionpool)
{
    xserver * o = (xserver *) calloc(1, sizeof(xserver));

    o->set = xaddressof(virtualSet);
    o->socket = xserversocketNew(xserversocket_invalid_value, domain, type, protocol, address, addressLen);
    o->sessionpool = sessionpool ? sessionpool : xsessionpoolNew(1024);

    return o;
}

static xserver * serverDel(xserver * o)
{
    if(o)
    {
        o->socket = xserversocketDel(o->socket);
        o->sessionpool = xsessionpoolDel(o->sessionpool);

        free(o);
    }
    return xnil;
}

static xint32 serverOpen(xserver * o)
{
    return xserversocketOpen(o->socket);
}

static xint32 serverClose(xserver * o)
{
    return xserversocketClose(o->socket);
}

static xsession * serverAccept(xserver * o)
{
    xsessionsocket * sessionsocket = xserversocketAccept(o->socket);

    xsession * session = xsessionpoolGet(o->sessionpool, sessionsocket);

    if(session)
    {
        serverPush(o, session);
    }
    else
    {
        xserversocketRel(o->socket, sessionsocket);
    }

    return session;
}

static void serverRel(xserver * o, xsession * session)
{
    serverRem(o, session);

    xserversocketRel(o->socket, session->socket);
    session->socket = xnil;

    xsessionpoolRel(o->sessionpool, session);
}

static void serverPush(xserver * o, xsession * session)
{
    xsyncLock(o->sync);
    session->server.prev = o->tail;
    if(session->server.prev)
    {
        session->server.prev->server.next = session;
    }
    else
    {
        o->head = session;
    }
    o->tail = session;
    o->size = o->size + 1;
    session->server.container = o;
    xsyncUnlock(o->sync);
}

static void serverRem(xserver * o, xsession * session)
{
    xsyncLock(o->sync);
    xsession * prev = session->server.prev;
    xsession * next = session->server.next;

    if(prev)
    {
        prev->server.next = next;
    }
    else
    {
        o->head = next;
    }

    if(next)
    {
        next->server.prev = prev;
    }
    else
    {
        o->tail = prev;
    }
    o->size = o->size - 1;
    session->server.container = xnil;

    xsyncUnlock(o->sync);
}
