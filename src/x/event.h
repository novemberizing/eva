#ifndef   __NOVEMBERIZING_X__EVENT__H__
#define   __NOVEMBERIZING_X__EVENT__H__

#include <x/std.h>
#include <x/sync.h>





struct xeventqueue;
struct xeventengine;
struct xeventobject;
struct xeventprocessor;
struct xeventgenerator;
struct xeventsubscription;


typedef struct xeventqueue xeventqueue;
typedef struct xeventengine xeventengine;
typedef struct xeventobject xeventobject;
typedef struct xeventprocessor xeventprocessor;
typedef struct xeventgenerator xeventgenerator;
typedef struct xeventsubscription xeventsubscription;

struct xevent;
typedef struct xevent xevent;


typedef void (*xeventhandler)(xeventobject *, xeventsubscription *, xuint32);

struct xevent
{
    xeventpool * pool;
    xeventqueue * queue;
    xevent * prev;
    xevent * next;

    xeventobject * object;
    xeventsubscription * subscription;
    xuint32 type;
};

extern xevent * xeventNew(void);



#endif // __NOVEMBERIZING_X__EVENT__H__
