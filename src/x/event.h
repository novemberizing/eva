#ifndef   __NOVEMBERIZING_X__EVENT__H__
#define   __NOVEMBERIZING_X__EVENT__H__

#include <x/std.h>
#include <x/sync.h>

struct xevent;
struct xeventobject;
struct xeventengine;
struct xeventqueue;
struct xeventpool;
struct xeventgenerator;
struct xeventprocessor;
struct xeventsubscription;

typedef struct xevent xevent;
typedef struct xeventobject xeventobject;
typedef struct xeventengine xeventengine;
typedef struct xeventqueue xeventqueue;
typedef struct xeventpool xeventpool;
typedef struct xeventgenerator xeventgenerator;
typedef struct xeventprocessor xeventprocessor;
typedef struct xeventsubscription xeventsubscription;

typedef void (*xeventhandler)(xeventobject *, xeventsubscription *, xuint32);

struct xevent
{
    xevent * prev;
    xevent * next;
    xeventqueue * queue;
    xeventpool * pool;

    xeventsubscription * subscription;
    xuint32 type;
    xeventhandler on;
};

extern xevent * xeventNew(void);
extern xevent * xeventDel(xevent * o);

extern xevent * xeventSet(xevent * o, xeventsubscription * subscription, xuint32 type, xeventhandler on);

#include <x/event/type.h>

#endif // __NOVEMBERIZING_X__EVENT__H__
