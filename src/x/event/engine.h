#ifndef   __NOVEMBERIZING_X_EVENT__ENGINE__H__
#define   __NOVEMBERIZING_X_EVENT__ENGINE__H__

#include <x/event.h>
#include <x/event/engine/wait.h>

#include <x/command/event/generator.h>

struct xeventengine;

typedef struct xeventengine xeventengine;
typedef void (*xeventenginecancel)(xeventengine *);

struct xeventengine
{
    xsync * sync;

    xeventenginecancel cancel;

    xeventsubscription * head;
    xeventsubscription * tail;
    xuint64 size;

    xeventqueue * queue;
    xeventpool * eventpool;

    struct {
        xcommandeventgenerator * command;
    } generator;
};

extern void xeventengineStop(xeventengine * o);
extern void xeventengineCancel(xeventengine * o, xeventenginecancel cancel);

extern xeventengine * xeventengineGen(void);
extern xeventengine * xeventengineGet(void);
extern xeventengine * xeventengineDel(xeventengine * engine);
extern xint32 xeventengineRun(xeventengine * engine);

extern void xeventengineSubscriptionPush(xeventengine * o, xeventsubscription * subscription);

extern xcommandeventsubscription * xeventengineCommandReg(xeventengine * o, xcommand * command, xcommandeventhandler on);
extern xcommandeventsubscription * xeventengineCommandEventSubscriptionUnreg(xeventengine * o, xcommandeventsubscription * subscription);

#endif // __NOVEMBERIZING_X_EVENT__ENGINE__H__
