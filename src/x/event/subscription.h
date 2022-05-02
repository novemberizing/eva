#ifndef   __NOVEMBERIZING_X_EVENT__SUBSCRIPTION__H__
#define   __NOVEMBERIZING_X_EVENT__SUBSCRIPTION__H__

#include <x/event.h>

struct xeventsubscription;
typedef struct xeventsubscription xeventsubscription;

typedef void (*xeventhandler)(xeventobject *, xeventengine *, xuint32);

struct xeventsubscription
{
    xeventobject * object;

    void (*on)(xeventobject *, xuint32);

    struct {
        xeventengine * engine;
        xeventgenerator * generator;

        xeventsubscription * prev;
        xeventsubscription * next;
    } parent;

    
};

extern xeventsubscription * xeventsubscriptionNew(xeventobject * object, xeventengine * engine, xeventhandler on);

#endif // __NOVEMBERIZING_X_EVENT__SUBSCRIPTION__H__
