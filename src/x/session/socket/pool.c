#include <stdlib.h>
#include <stdio.h>

#include "pool.h"

static xsessionsocketpool * sessionsocketpoolDel(xsessionsocketpool * o);
static void sessionsocketpoolPush(xsessionsocketpool * o, xsessionsocket * sessionsocket);
static xsessionsocket * sessionsocketpoolPop(xsessionsocketpool * o);
static void sessionsocketpoolRem(xsessionsocketpool * o, xsessionsocket * sessionsocket);
static void sessionsocketpoolClear(xsessionsocketpool * o);

static xsessionsocketpoolset virtualSet = {
    sessionsocketpoolDel,
    sessionsocketpoolPush,
    sessionsocketpoolPop,
    sessionsocketpoolRem,
    sessionsocketpoolClear
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
    xsyncLock(o->sync);

    sessionsocket->sessionsocketpool.prev = o->tail;

    if(sessionsocket->sessionsocketpool.prev)
    {
        sessionsocket->sessionsocketpool.prev->sessionsocketpool.next = sessionsocket;
    }
    else
    {
        o->head = sessionsocket;
    }
    sessionsocket->sessionsocketpool.container = o;

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
        o->head = sessionsocket->sessionsocketpool.next;

        if(o->head)
        {
            o->head->sessionsocketpool.prev = xnil;
            sessionsocket->sessionsocketpool.next = xnil;
        }
        else
        {
            o->tail = xnil;
        }

        o->size = o->size - 1;
        sessionsocket->sessionsocketpool.container = xnil;
    }

    xsyncUnlock(o->sync);
    return sessionsocket;
}

static void sessionsocketpoolRem(xsessionsocketpool * o, xsessionsocket * sessionsocket)
{
    if(o)
    {
        xsyncLock(o->sync);

        xsessionsocket * prev = sessionsocket->sessionsocketpool.prev;
        xsessionsocket * next = sessionsocket->sessionsocketpool.next;

        if(prev)
        {
            prev->sessionsocketpool.next = next;
            sessionsocket->sessionsocketpool.prev = xnil;
        }
        else
        {
            o->head = next;
        }

        if(next)
        {
            next->sessionsocketpool.prev = prev;
            sessionsocket->sessionsocketpool.next = xnil;
        }
        else
        {
            o->tail = prev;
        }
        o->size = o->size - 1;
        sessionsocket->sessionsocketpool.container = xnil;


        xsyncUnlock(o->sync);
    }
}

static void sessionsocketpoolClear(xsessionsocketpool * o)
{
    xsyncLock(o->sync);
    xsessionsocket * node = xnil;

    do {
        node = o->head;

        o->head = node->sessionsocketpool.next;
        if(o->head)
        {
            o->head->sessionsocketpool.prev = xnil;
        }
        else
        {
            o->tail = xnil;
        }
        o->size = o->size - 1;
        node->sessionsocketpool.container = xnil;
        node->sessionsocketpool.next = xnil;
        xsessionsocketDel(node);
    } while(o->head);

    xsyncUnlock(o->sync);
}