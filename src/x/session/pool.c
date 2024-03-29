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

    session->parent.prev = o->tail;
    if(session->parent.prev)
    {
        session->parent.prev->parent.next = session;
    }
    else
    {
        o->head = session;
    }
    session->parent.sessionpool = o;
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
        o->head = session->parent.next;

        if(o->head)
        {
            o->head->parent.prev = xnil;
            session->parent.next = xnil;
        }
        else
        {
            o->tail = xnil;
        }

        session->parent.sessionpool = xnil;
        o->size = o->size - 1;
    }

    xsyncUnlock(o->sync);

    return session;
}

static void sessionpoolRem(xsessionpool * o, xsession * session)
{
    xsession * prev = session->parent.prev;
    xsession * next = session->parent.next;

    if(prev)
    {
        prev->parent.next = next;
        session->parent.prev = xnil;
    }
    else
    {
        o->head = next;
    }

    if(next)
    {
        next->parent.prev = prev;
        session->parent.next = xnil;
    }
    else
    {
        o->tail = prev;
    }

    session->parent.sessionpool = xnil;
    o->size = o->size - 1;
}

static void sessionpoolClear(xsessionpool * o)
{
    xsyncLock(o->sync);
    xsession * session = xnil;

    while(o->head)
    {
        session = o->head;
        
        if(session->parent.next)
        {
            session->parent.next->parent.prev = xnil;
        }
        else
        {
            o->tail = xnil;
        }
        
        o->head = session->parent.next;

        session->parent.next = xnil;
        session->parent.sessionpool = xnil;
        o->size = o->size - 1;
        xsessionDel(session);
    }
    xsyncUnlock(o->sync);
}
