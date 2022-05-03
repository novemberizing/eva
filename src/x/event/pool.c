#include "pool.h"

static xeventpool * eventpoolDel(xeventpool * o);
static xevent * eventpoolGet(xeventpool * o);
static xevent * eventpoolRel(xeventpool * o, xevent * event);
static void eventpoolClear(xeventpool * o);

static xeventpoolset virtualSet = {
    eventpoolDel,
    eventpoolGet,
    eventpoolRel,
    eventpoolClear
};

static void eventpoolPush(xeventpool * o, xevent * event);
static xevent * eventpoolPop(xeventpool * o);
static void eventpoolRem(xeventpool * o, xevent * event);

extern xeventpool * xeventpoolNew(xuint64 capacity)
{
    xeventpool * o = (xeventpool *) calloc(1, sizeof(xeventpool));

    o->set = xaddressof(virtualSet);
    o->capacity = capacity;

    for(xuint64 i = 0; i < o->capacity; i++)
    {
        eventpoolPush(o, xeventNew());
    }

    return o;
}

static xeventpool * eventpoolDel(xeventpool * o)
{
    if(o)
    {
        xsyncLock(o->sync);
        eventpoolClear(o);
        xsyncUnlock(o->sync);
        o->sync = xsyncDel(o->sync);
        free(o);
    }
    return xnil;
}

static xevent * eventpoolGet(xeventpool * o)
{
    xevent * event = eventpoolPop(o);
    if(event == xnil)
    {
        event = xeventNew();
    }
    return event;
}
static xevent * eventpoolRel(xeventpool * o, xevent * event)
{
    if(o->size < o->capacity)
    {
        eventpoolPush(o, event);
    }
    else
    {
        xeventDel(o);
    }
}
static void eventpoolClear(xeventpool * o);

static void eventpoolPush(xeventpool * o, xevent * event)
{
    event->prev = o->tail;

    if(event->prev)
    {
        event->prev->next = event;
    }
    else
    {
        o->head = event;
    }
    event->pool = o;
    o->tail = event;
    o->size = o->size + 1;
}

static xevent * eventpoolPop(xeventpool * o)
{
    xevent * event = o->head;

    if(event)
    {
        o->head = event->next;
        if(o->head)
        {
            o->head->prev = xnil;
        }
        else
        {
            o->tail = xnil;
        }

        event->next = xnil;
        event->pool = xnil;
    }

    return event;
}

static void eventpoolRem(xeventpool * o, xevent * event)
{
    xevent * prev = event->prev;
    xevent * next = event->next;

    if(prev)
    {
        prev->next = next;
        event->prev = xnil;
    }
    else
    {
        o->head = next;
    }
    if(next)
    {
        next->prev = prev;
        event->next = xnil;
    }
    else
    {
        o->tail = prev;
    }
    event->pool = xnil;
    o->size = o->size - 1;
}