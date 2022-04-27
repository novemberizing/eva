#ifndef   __NOVEMBERIZING_X__EVENT__H__
#define   __NOVEMBERIZING_X__EVENT__H__

#include <x/std.h>

struct xevent;
struct xeventobject;
struct xeventqueue;

typedef struct xevent xevent;
typedef struct xeventobject xeventobject;
typedef struct xeventqueue xeventqueue;

struct xevent
{
    void * set;

    xevent * prev;
    xevent * next;
    xeventqueue * queue;

    xeventobject * object;
    xuint32 flags;
};

extern xevent * xeventnew(xeventobject * object, xuint32 flags);
extern xevent * xeventdel(xevent * o);

#endif // __NOVEMBERIZING_X__EVENT__H__
