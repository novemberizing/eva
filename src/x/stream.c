#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "stream.h"

#define calculateCapacity(capacity, page)   (page == 0 ? capacity : (capacity / page + 1) * page)

static xstream * streamDel(xstream * o);
static void streamReserve(xstream * o, xuint64 capacity, xuint64 page);
static void streamPush(xstream * o, const unsigned char * message, xuint64 length);
static xuint64 streamPop(xstream * o, unsigned char * buffer, xuint64 length);

static xstreamset virtualSet = {
    streamDel,
    streamReserve,
    streamPush
};

extern xstream * xstreamNew(xuint64 capacity, xuint64 page)
{
    xstream * o = (xstream *) calloc(1, sizeof(xstream));

    o->set = xaddressof(virtualSet);
    o->capacity = calculateCapacity(capacity, page);
    o->container = o->capacity > 0 ? malloc(o->capacity) : xnil;

    return o;
}

static xstream * streamDel(xstream * o)
{
    if(o)
    {
        if(o->container)
        {
            free(o->container);
        }
        free(o);
    }
    return xnil;
}

static void streamReserve(xstream * o, xuint64 capacity, xuint64 page)
{
    xfunctionAssert(o == xnil, "invalid parameter");

    if(xstreamLen(o) > 0)
    {
        o->position = 0;
        o->size = 0;
    }

    if(xstreamRemain(o) < capacity)
    {
        capacity = capacity - xstreamRemain(o);
        o->capacity = (page == 0 ? capacity : (capacity / page + 1) * page);
        if(o->container)
        {
            if(xstreamLen(o) > 0)
            {
                o->container = (unsigned char *) realloc(o->container, o->capacity);
            }
            else
            {
                /**
                 * 메모리 카피가 일어나지 않도록 realloc 을 호출하지 않는다.
                 * 
                 */
                free(o->container);
                o->container = (unsigned char *) malloc(o->capacity);
            }
        }
        else
        {
            o->container = (unsigned char *) malloc(o->capacity);
        }
    }
}

static void streamPush(xstream * o, const unsigned char * message, xuint64 length)
{
    xfunctionAssert(o == xnil || message == xnil || length == 0, "invalid parameter");

    streamReserve(o, length, 1);
    
    memcpy(xstreamBack(o), message, length);
    o->size = o->size + length;
}

static xuint64 streamPop(xstream * o, unsigned char * buffer, xuint64 length)
{
    length = xstreamLen(o) < length ? xstreamLen(o) : length;

    memcpy(buffer, xstreamFront(o), length);

    o->position = o->position + length;

    if(o->position == o->size)
    {
        o->position = 0;
        o->size = 0;
    }

    return length;
}