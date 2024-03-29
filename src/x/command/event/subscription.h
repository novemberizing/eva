#ifndef   __NOVEMBERIZING_X_COMMAND_EVENT__SUBSCRIPTION__H__
#define   __NOVEMBERIZING_X_COMMAND_EVENT__SUBSCRIPTION__H__

#include <x/command.h>

struct xcommandeventsubscriptionset;

typedef struct xcommandeventsubscriptionset xcommandeventsubscriptionset;

struct xcommandeventsubscription
{
    const xcommandeventsubscriptionset * set;

    xcommandeventsubscription * prev;
    xcommandeventsubscription * next;
    xcommandeventgenerator * generator;
    xeventengine * engine;

    xcommand * command;

    xcommandeventhandler on;
};

struct xcommandeventsubscriptionset
{
    xcommandeventsubscription * (*del)(xcommandeventsubscription *);
};

extern xcommandeventsubscription * xcommandeventsubscriptionNew(xcommand * command, xcommandeventhandler on);

#define xcommandeventsubscriptionDel(o)     (o->set->del(o))

#endif // __NOVEMBERIZING_X_COMMAND_EVENT__SUBSCRIPTION__H__
