#ifndef   __NOVEMBERIZING_X_COMMAND_EVENT__GENERATOR__H__
#define   __NOVEMBERIZING_X_COMMAND_EVENT__GENERATOR__H__

#include <x/command.h>

#include <x/command/event.h>

#include <x/event/generator.h>

struct xcommandeventgenerator;
struct xcommandeventgeneratorset;

typedef struct xcommandeventgenerator xcommandeventgenerator;
typedef struct xcommandeventgeneratorset xcommandeventgeneratorset;

struct xcommandeventgenerator
{
    const xcommandeventgeneratorset * set;

    xsync * sync;

    xcommandeventsubscription * head;
    xcommandeventsubscription * tail;
    xuint64 size;

    xeventengine * engine;
};

struct xcommandeventgeneratorset
{
    xcommandeventgenerator * (*del)(xcommandeventgenerator *);
    void (*on)(xcommandeventgenerator *);
    xcommandeventsubscription * (*reg)(xcommandeventgenerator *, xcommandeventsubscription *);
    xcommandeventsubscription * (*unreg)(xcommandeventgenerator *, xcommandeventsubscription *);
};

extern xcommandeventgenerator * xcommandeventgeneratorNew(xeventengine * engine);

#define xcommandeventgeneratorDel(o)                    (o->set->del(o))
#define xcommandeventgeneratorOn(o)                     (o->set->on(o))
#define xcommandeventgeneratorReg(o, subscription)      (o->set->reg(o, subscription))
#define xcommandeventgeneratorUnreg(o, subscription)    (o->set->unreg(o, subscription))

#endif // __NOVEMBERIZING_X_COMMAND_EVENT__GENERATOR__H__
