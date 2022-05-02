#ifndef   __NOVEMBERIZING_X_DESCRIPTOR_EVENT__GENERATOR__H__
#define   __NOVEMBERIZING_X_DESCRIPTOR_EVENT__GENERATOR__H__

#include <x/event.h>

struct xdescriptoreventgenerator;
struct xdescriptoreventgeneratorset;

typedef struct xdescriptoreventgenerator xdescriptoreventgenerator;
typedef struct xdescriptoreventgeneratorset xdescriptoreventgeneratorset;

struct xdescriptoreventgenerator
{
    const xdescriptoreventgeneratorset * set;

    xsync * sync;
    xeventsubscription * head;
    xeventsubscription * tail;
    xuint64 size;

    xeventengine * engine;
};

struct xdescriptoreventgeneratorset
{
    xdescriptoreventgenerator * (*del)(xdescriptoreventgenerator *);
    xeventsubscription * (*registersubscription)(xdescriptoreventgenerator *, xeventsubscription *);
    xeventsubscription * (*unregistersubscription)(xdescriptoreventgenerator *, xeventsubscription *);
    void (*clear)(xdescriptoreventgenerator *);
};

extern xdescriptoreventgenerator * xdescriptoreventgeneratorNew(xeventengine * engine, xdescriptoreventgeneratorset * set, xuint64 size);

#define xdescriptoreventgeneratorDel(o)                                     (o->set->del(o))
#define xdescriptoreventgeneratorClear(o)                                   (o->set->clear(o))
#define xdescriptoreventgeneratorRegisterSubscription(o, subscription)      (o->set->registersubscription(o, subscription))
#define xdescriptoreventgeneratorUnregisterSubscription(o, subscription)    (o->set->unregistersubscription(o, subscription))

#endif // __NOVEMBERIZING_X_DESCRIPTOR_EVENT__GENERATOR__H__
