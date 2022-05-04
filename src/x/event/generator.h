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
    xeventgenerator * (*del)(xeventgenerator *);
    void (*on)(xeventgenerator *);
    xeventsubscription * (*reg)(xeventgenerator *, xeventsubscription *);
    xeventsubscription * (*unreg)(xeventgenerator *, xeventsubscription *);
};

extern xeventgenerator * xeventgeneratorNew(xeventengine * engine, const xeventgeneratorset * set, xuint64 size);

#define xeventgeneratorDel(o)                   (o->set->del(o))
#define xeventgeneratorOn(o)                    (o->set->on(o))
#define xeventgeneratorReg(o, subscription)     (o->set->reg(o, subscription))
#define xeventgeneratorUnreg(o, subscription)   (o->set->unreg(o, subscription))

#endif // __NOVEMBERIZING_X_EVENT__GENERATOR__H__
