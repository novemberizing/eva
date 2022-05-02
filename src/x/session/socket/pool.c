#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "pool.h"

static xsessionsocketpool * sessionsocketpoolDel(xsessionsocketpool * o);
static void sessionsocketpoolPush(xsessionsocketpool * o, xsessionsocket * sessionsocket);
static xsessionsocket * sessionsocketpoolPop(xsessionsocketpool * o);
static void sessionsocketpoolRem(xsessionsocketpool * o, xsessionsocket * sessionsocket);
static void sessionsocketpoolClear(xsessionsocketpool * o);
static xsessionsocket * sessionsocketpoolGet(xsessionsocketpool * o);
static void sessionsocketpoolRel(xsessionsocketpool * o , xsessionsocket * sessionsocket);

static xsessionsocketpoolset virtualSet = {
    sessionsocketpoolDel,
    sessionsocketpoolPush,
    sessionsocketpoolPop,
    sessionsocketpoolRem,
    sessionsocketpoolClear,
    sessionsocketpoolGet,
    sessionsocketpoolRel
};

extern xsessionsocketpool * xsessionsocketpoolNew(xuint64 capacity, xserversocket * serversocket)
{
    xsessionsocketpool * o = (xsessionsocketpool *) calloc(1, sizeof(xsessionsocketpool));

    o->set = xaddressof(virtualSet);
    o->capacity = capacity;
    o->serversocket = serversocket;

    for(xuint64 i = 0; i < capacity; i++)
    {
        sessionsocketpoolPush(o, xsessionsocketNew(xsessionsocket_invalid_value));
    }

    return o;
}

static xsessionsocketpool * sessionsocketpoolDel(xsessionsocketpool * o)
{
    if(o)
    {
        sessionsocketpoolClear(o);
        free(o);
    }
    return xnil;
}

static void sessionsocketpoolPush(xsessionsocketpool * o, xsessionsocket * sessionsocket)
{
    xfunctionAssert(sessionsocket->parent.serversocket || sessionsocket->parent.sessionsocketpool || sessionsocket->parent.serversocket, "invalid parameter");
    xsyncLock(o->sync);

    sessionsocket->parent.prev = o->tail;

    if(sessionsocket->parent.prev)
    {
        sessionsocket->parent.prev->parent.next = sessionsocket;
    }
    else
    {
        o->head = sessionsocket;
    }
    sessionsocket->parent.sessionsocketpool = o;

    o->tail = sessionsocket;
    o->size = o->size + 1;

    xsyncUnlock(o->sync);
}

static xsessionsocket * sessionsocketpoolPop(xsessionsocketpool * o)
{
    xsyncLock(o->sync);
    xsessionsocket * sessionsocket = o->head;

    if(sessionsocket)
    {
        o->head = sessionsocket->parent.next;

        if(o->head)
        {
            o->head->parent.prev = xnil;
            sessionsocket->parent.next = xnil;
        }
        else
        {
            o->tail = xnil;
        }

        o->size = o->size - 1;
        sessionsocket->parent.sessionsocketpool = xnil;
    }

    xsyncUnlock(o->sync);
    return sessionsocket;
}

static void sessionsocketpoolRem(xsessionsocketpool * o, xsessionsocket * sessionsocket)
{
    xfunctionAssert(o == xnil || sessionsocket->parent.sessionsocketpool != o || sessionsocket->parent.serversocket, "invalid parameter");
    if(o)
    {
        xsyncLock(o->sync);

        xsessionsocket * prev = sessionsocket->parent.prev;
        xsessionsocket * next = sessionsocket->parent.next;

        if(prev)
        {
            prev->parent.next = next;
            sessionsocket->parent.prev = xnil;
        }
        else
        {
            o->head = next;
        }

        if(next)
        {
            next->parent.prev = prev;
            sessionsocket->parent.next = xnil;
        }
        else
        {
            o->tail = prev;
        }
        o->size = o->size - 1;
        sessionsocket->parent.sessionsocketpool = xnil;


        xsyncUnlock(o->sync);
    }
}

static void sessionsocketpoolClear(xsessionsocketpool * o)
{
    xsyncLock(o->sync);
    xsessionsocket * node = xnil;

    while(o->head)
    {
        node = o->head;

        o->head = node->parent.next;
        if(o->head)
        {
            o->head->parent.prev = xnil;
        }
        else
        {
            o->tail = xnil;
        }
        o->size = o->size - 1;
        node->parent.sessionsocketpool = xnil;
        node->parent.next = xnil;
        xsessionsocketDel(node);
    }

    xsyncUnlock(o->sync);
}

static xsessionsocket * sessionsocketpoolGet(xsessionsocketpool * o)
{
    xsessionsocket * sessionsocket = sessionsocketpoolPop(o);
    if(sessionsocket == xnil)
    {
        sessionsocket = xsessionsocketNew(xsessionsocket_invalid_value);
    }
    else
    {
        xfunctionAssert(sessionsocket->parent.next, "");
    }
    return sessionsocket;
}

static void sessionsocketpoolRel(xsessionsocketpool * o , xsessionsocket * sessionsocket)
{
    if(o->size < o->capacity)
    {
        sessionsocketpoolPush(o, sessionsocket);
    }
    else
    {
        xsessionsocketDel(sessionsocket);
    }
}