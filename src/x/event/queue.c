#include "queue.h"

static xeventqueue * eventqueueDel(xeventqueue * o);
static void eventqueuePush(xeventqueue * o, xevent * event);
static xevent * eventqueuePop(xeventqueue * o);
static void eventqueueClear(xeventqueue * o);
static void eventqueueRem(xeventqueue * o, xevent * event);

static xeventqueueset virtualSet = {
    eventqueueDel,
    eventqueuePush,
    eventqueuePop,
    eventqueueClear,
    eventqueueRem
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
        free(o);
    }
    return xnil;
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
}

static xevent * eventqueuePop(xeventqueue * o)
{
    xevent * event = o->head;

    if(event)
    {
        o->head = event->next;
        if(o->head)
        {
            o->head->prev = xnil;
            event->next = xnil;
        }
        else
        {
            o->tail = xnil;
        }
        event->queue = xnil;
        o->size = o->size - 1;
    }

    return event;
}

static void eventqueueClear(xeventqueue * o)
{
    xevent * event = xnil;
    while(event = eventqueuePop(o))
    {
        xeventDel(event);
    }
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