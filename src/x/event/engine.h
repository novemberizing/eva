#ifndef   __NOVEMBERIZING_X_EVENT__ENGINE__H__
#define   __NOVEMBERIZING_X_EVENT__ENGINE__H__

#include <x/event.h>
#include <x/event/engine/wait.h>

struct xeventengine;

typedef struct xeventengine xeventengine;

struct xeventengine
{
    xsync * sync;

    xeventsubscription * head;
    xeventsubscription * tail;
    xuint64 size;
};

#endif // __NOVEMBERIZING_X_EVENT__ENGINE__H__
