#include <stdlib.h>

#include "pool.h"

static void eventpoolPush(xeventpool * o, xevent * event);
static xevent * eventpoolPop(xeventpool * o);
static void eventpoolClear(xeventpool * o);

extern xeventpool * xeventpoolNew(xuint64 capacity)
{
    xeventpool * o = (xeventpool *) calloc(1, sizeof(xeventpool));

    o->capacity = capacity;

    for(xuint64 i = 0; i < o->capacity; i++)
    {
        eventpoolPush(o, xeventNew());
    }

    return o;
}

extern xeventpool * xeventpoolDel(xeventpool * o)
{
    if(o)
    {
        eventpoolClear(o);
        free(o);
    }
    return xnil;
}

extern xevent * xeventpoolGet(xeventpool * o)
{
    xevent * event = eventpoolPop(o);

    if(event == xnil)
    {
        event = xeventNew();
    }

    return event;
}

extern void xeventpoolRel(xeventpool * o, xevent * event)
{
    if(o->size < o->capacity)
    {
        event->type = xeventtype_none;
        event->subscription = xnil;

        eventpoolPush(o, event);
    }
    else
    {
        xeventDel(event);
    }
}

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

        if(event->next)
        {
            event->next->prev = xnil;
            event->next = xnil;
        }
        else
        {
            o->tail = xnil;
        }

        event->pool = xnil;
        o->size = o->size - 1;
    }

    return event;
}

static void eventpoolClear(xeventpool * o)
{
    xevent * event = xnil;
    while(o->head)
    {
        event = o->head;

        o->head = event->next;

        if(event->next)
        {
            event->next->prev = xnil;
            event->next = xnil;
        }
        else
        {
            o->tail = xnil;
        }

        event->pool = xnil;
        o->size = o->size - 1;

        xeventDel(event);
    }
}
