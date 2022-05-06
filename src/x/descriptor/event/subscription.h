#ifndef   __NOVEMBERIZING_X_DESCRIPTOR_EVENT__SUBSCRIPTION__H__
#define   __NOVEMBERIZING_X_DESCRIPTOR_EVENT__SUBSCRIPTION__H__

#include <x/event.h>
#include <x/descriptor.h>
#include <x/descriptor/event.h>

struct xdescriptoreventsubscriptionset;

typedef struct xdescriptoreventsubscriptionset xdescriptoreventsubscriptionset;

struct xdescriptoreventsubscription
{
    const xdescriptoreventsubscriptionset * set;

    xdescriptoreventsubscription * prev;
    xdescriptoreventsubscription * next;
    xdescriptoreventgenerator * generator;
    xeventengine * engine;

    xdescriptor * descriptor;

    xdescriptoreventhandler on;
};

struct xdescriptoreventsubscriptionset
{
    xdescriptoreventsubscription * (*del)(xdescriptoreventsubscription *);
};

extern xdescriptoreventsubscription * xdescriptoreventsubscriptionNew(xdescriptor * descriptor, xdescriptoreventhandler on);

#define xdescriptoreventsubscriptionDel(o)      (o->set->del(o))

#endif // __NOVEMBERIZING_X_DESCRIPTOR_EVENT__SUBSCRIPTION__H__
