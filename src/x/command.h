#ifndef   __NOVEMBERIZING_X__COMMAND__H__
#define   __NOVEMBERIZING_X__COMMAND__H__

#include <x/event.h>

struct xcommand;
struct xcommandset;

typedef struct xcommand xcommand;
typedef struct xcommandset xcommandset;

struct xcommandeventgenerator;
struct xcommandeventsubscription;

typedef struct xcommandeventgenerator xcommandeventgenerator;
typedef struct xcommandeventsubscription xcommandeventsubscription;

typedef void (*xcommandexec)(xcommand *);

typedef void (*xcommandeventhandler)(xcommand *, xcommandeventsubscription *, xuint32);

struct xcommand
{
    const xcommandset * set;

    xcommandexec exec;
};

struct xcommandset
{
    xcommand * (*del)(xcommand *);
};

extern xcommand * xcommandNew(xcommandexec exec, const xcommandset * set, xuint64 size);

#define xcommandDel(o)          (o->set->del(o))

#define xcommandExec(o)         (o->exec(o))

extern const xcommandset * xcommandsetGet(void);

#endif // __NOVEMBERIZING_X__COMMAND__H__
