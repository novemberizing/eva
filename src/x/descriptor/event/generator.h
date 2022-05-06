#ifndef   __NOVEMBERIZING_X_DESCRIPTOR_EVENT__GENERATOR__H__
#define   __NOVEMBERIZING_X_DESCRIPTOR_EVENT__GENERATOR__H__

#include <x/event.h>

#include <x/descriptor.h>

struct xdescriptoreventgeneratorset;
typedef struct xdescriptoreventgeneratorset xdescriptoreventgeneratorset;

struct xdescriptoreventgenerator
{
    const xdescriptoreventgeneratorset * set;

    xsync * sync;

    xdescriptoreventsubscription * head;
    xdescriptoreventsubscription * tail;
    xuint64 size;

    xeventengine * engine;
};

struct xdescriptoreventgeneratorset
{
    xdescriptoreventgenerator * (*del)(xdescriptoreventgenerator *);
    void (*on)(xdescriptoreventgenerator *);
    xdescriptoreventsubscription * (*reg)(xdescriptoreventgenerator *, xdescriptoreventsubscription *);
    xdescriptoreventsubscription * (*unreg)(xdescriptoreventgenerator *, xdescriptoreventsubscription *);
};

extern xdescriptoreventgenerator * xdescriptoreventgeneratorNew(xeventengine * engine, const xdescriptoreventgeneratorset * set, xuint64 size);

#define xdescriptoreventgeneratorDel(o)                     (o->set->del(o))
#define xdescriptoreventgeneratorOn(o)                      (o->set->on(o))
#define xdescriptoreventgeneratorReg(o, subscription)       (o->set->reg(o, subscription))
#define xdescriptoreventgeneratorUnreg(o, subscription)     (o->set->unreg(o, subscription))

#endif // __NOVEMBERIZING_X_DESCRIPTOR_EVENT__GENERATOR__H__
