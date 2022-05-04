#ifndef   __NOVEMBERIZING_X_EVENT__SUBSCRIPTION__H__
#define   __NOVEMBERIZING_X_EVENT__SUBSCRIPTION__H__

#include <x/event.h>

struct xeventsubscription;
struct xeventsubscriptionset;

typedef struct xeventsubscription xeventsubscription;
typedef struct xeventsubscriptionset xeventsubscriptionset;

struct xeventsubscription
{
    const xeventsubscriptionset * set;

    xeventsubscription * prev;
    xeventsubscription * next;
    xeventgenerator * generator;
    xeventengine * engine;

    xeventobject * object;

    xeventhandler on;
};

struct xeventsubscriptionset
{
    xeventsubscription * (*del)(xeventsubscription *);
};

extern xeventsubscription * xeventsubscriptionNew(void);

#endif // __NOVEMBERIZING_X_EVENT__SUBSCRIPTION__H__
