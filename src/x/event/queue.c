#include <stdlib.h>

#include "queue.h"

static xeventqueue * eventqueueDel(xeventqueue * o);
static void eventqueueAdd(xeventqueue * o, xevent * event);
static void eventqueueRem(xeventqueue * o, xevent * event);
static void eventqueueClear(xeventqueue * o);
static void eventqueuePush(xeventqueue * o, xevent * event);
static xevent * eventqueuePop(xeventqueue * o);

static xeventqueueset virtualSet = {
    eventqueueDel,
    eventqueueAdd,
    eventqueueRem,
    eventqueueClear,

    eventqueuePush,
    eventqueuePop
};

extern xeventqueue * xeventqueueNew(void)
{
    xeventqueue * o = (xeventqueue *) calloc(1, sizeof(xeventqueue));

    o->set = xaddressof(virtualSet);

    return o;
}

static xeventqueue * eventqueueDel(xeventqueue * o)
{
    if(o)
    {
        eventqueueClear(o);

        o->sync = xsyncDel(o->sync);

        free(o);
    }
    return xnil;
}

static void eventqueueAdd(xeventqueue * o, xevent * event)
{
    eventqueuePush(o, event);
}

static void eventqueueRem(xeventqueue * o, xevent * event)
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

    event->queue = xnil;
    o->size = o->size - 1;
}

static void eventqueueClear(xeventqueue * o)
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
        event->queue = xnil;
        o->size = o->size - 1;
    }
}

static void eventqueuePush(xeventqueue * o, xevent * event)
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
    o->tail = event;
    o->size = o->size + 1;
    event->queue = o;
}

static xevent * eventqueuePop(xeventqueue * o)
{
    xevent * event = o->head;

    if(event)
    {
        if(event->next)
        {
            event->next->prev = xnil;
            event->next = xnil;
        }
        else
        {
            o->tail = xnil;
        }
        event->queue = xnil;
        o->size = o->size - 1;

        o->head = event->next;
    }

    return event;
}

