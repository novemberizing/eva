#ifndef   __NOVEMBERIZING_X__STREAM__H__
#define   __NOVEMBERIZING_X__STREAM__H__

#include <x/std.h>

struct xstream;
struct xstreamset;

typedef struct xstream xstream;
typedef struct xstreamset xstreamset;

struct xstream
{
    const xstreamset * set;

    unsigned char * container;
    xuint64 position;
    xuint64 size;
    xuint64 capacity;
    xuint64 page;
};

struct xstreamset
{
    xstream * (*del)(xstream *);
    void (*reserve)(xstream *, xuint64, xuint64);
    void (*push)(xstream *, const unsigned char *, xuint64);
    xuint64 (*pop)(xstream *, unsigned char *, xuint64);
};

extern xstream * xstreamNew(xuint64 capacity, xuint64 page);

#define xstreamDel(o)                       (o->set->del(o))
#define xstreamReserve(o, capacity, page)   (o->set->reserve(o, capacity, page))
#define xstreamPush(o, message, length)     (o->set->push(o, message, length))
#define xstreamPop(o, buffer, length)       (o->set->pop(o, buffer, length))

#define xstreamRemain(o)                    (o->capacity - o->size)
#define xstreamFront(o)                     xaddressof(o->container[o->position])
#define xstreamBack(o)                      xaddressof(o->container[o->size])
#define xstreamLen(o)                       (o->size - o->position)

#endif // __NOVEMBERIZING_X__STREAM__H__
