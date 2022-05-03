#ifndef   __NOVEMBERIZING_X_EVENT__GENERATOR__H__
#define   __NOVEMBERIZING_X_EVENT__GENERATOR__H__

#include <x/event.h>

struct xeventgenerator;
struct xeventgeneratorset;

typedef struct xeventgenerator xeventgenerator;
typedef struct xeventgeneratorset xeventgeneratorset;

struct xeventgenerator
{
    const xeventgeneratorset * set;

    xsync * sync;
    xeventsubscription * head;
    xeventsubscription * tail;
    xuint64 size;

    xeventengine * engine;
};

struct xeventgeneratorset
{

};

extern xeventgenerator * xeventgeneratorNew(xeventengine * engine, const xeventgeneratorset * set, xuint64 size);

#endif // __NOVEMBERIZING_X_EVENT__GENERATOR__H__
