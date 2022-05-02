#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "pool.h"

static xsessionpool * sessionpoolDel(xsessionpool * o);
static xsession * sessionpoolGet(xsessionpool * o, xsessionsocket *);
static void sessionpoolRel(xsessionpool * o, xsession * session);



static xsessionpoolset virtualSet = {
    sessionpoolDel,
    sessionpoolGet,
    sessionpoolRel
};

static void sessionpoolPush(xsessionpool * o, xsession * session);
static xsession * sessionpoolPop(xsessionpool * o);
static void sessionpoolRem(xsessionpool * o, xsession * session);
static void sessionpoolClear(xsessionpool * o);

extern xsessionpool * xsessionpoolNew(xuint64 capacity)
{
    xsessionpool * o = (xsessionpool *) calloc(1, sizeof(xsessionpool));

    o->set = xaddressof(virtualSet);
    o->capacity = capacity;

    for(xuint64 i = 0; i < o->capacity; i++)
    {
        sessionpoolPush(o, xsessionNew());
    }

    return o;
}

static xsessionpool * sessionpoolDel(xsessionpool * o)
{
    if(o)
    {
        sessionpoolClear(o);
        free(o);
    }

    return xnil;
}

static xsession * sessionpoolGet(xsessionpool * o, xsessionsocket * sessionsocket)
{
    xsession * session = sessionpoolPop(o);

    if(session == xnil)
    {
        session = xsessionNew();
    }

    session->socket = sessionsocket;

    return session;
}

static void sessionpoolRel(xsessionpool * o, xsession * session)
{
    xfunctionAssert(session->socket, "");

    if(o->size < o->capacity)
    {
        sessionpoolPush(o, session);
    }
    else
    {
        xsessionDel(session);
    }
}

static void sessionpoolPush(xsessionpool * o, xsession * session)
{
    xsyncLock(o->sync);

    session->sessionpool.prev = o->tail;
    if(session->sessionpool.prev)
    {
        session->sessionpool.prev->sessionpool.next = session;
    }
    else
    {
        o->head = session;
    }
    session->sessionpool.container = o;
    o->tail = session;
    o->size = o->size + 1;

    xsyncUnlock(o->sync);
}

static xsession * sessionpoolPop(xsessionpool * o)
{
    xsyncLock(o->sync);

    xsession * session = o->head;

    if(session)
    {
        o->head = session->sessionpool.next;

        if(o->head)
        {
            o->head->sessionpool.prev = xnil;
            session->sessionpool.next = xnil;
        }
        else
        {
            o->tail = xnil;
        }

        session->sessionpool.container = xnil;
        o->size = o->size - 1;
    }

    xsyncUnlock(o->sync);

    return session;
}

static void sessionpoolRem(xsessionpool * o, xsession * session)
{
    xsession * prev = session->sessionpool.prev;
    xsession * next = session->sessionpool.next;

    if(prev)
    {
        prev->sessionpool.next = next;
        session->sessionpool.prev = xnil;
    }
    else
    {
        o->head = next;
    }

    if(next)
    {
        next->sessionpool.prev = prev;
        session->sessionpool.next = xnil;
    }
    else
    {
        o->tail = prev;
    }

    session->sessionpool.container = xnil;
    o->size = o->size - 1;
}

static void sessionpoolClear(xsessionpool * o)
{
    xsyncLock(o->sync);
    xsession * session = xnil;

    while(o->head)
    {
        session = o->head;
        
        if(session->sessionpool.next)
        {
            session->sessionpool.next->sessionpool.prev = xnil;
        }
        else
        {
            o->tail = xnil;
        }
        
        o->head = session->sessionpool.next;

        session->sessionpool.next = xnil;
        session->sessionpool.container = xnil;
        o->size = o->size - 1;
        xsessionDel(session);
    }
    xsyncUnlock(o->sync);
}
