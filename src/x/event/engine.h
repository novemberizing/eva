#ifndef   __NOVEMBERIZING_X_EVENT__ENGINE__H__
#define   __NOVEMBERIZING_X_EVENT__ENGINE__H__

#include <x/std.h>
#include <x/descriptor.h>

#include <x/descriptor/event/generator.h>

struct xeventengine;
typedef struct xeventengine xeventengine;

typedef void (*xeventenginecancel)(xeventengine *);

struct xeventengine
{
    xeventenginecancel cancel;

    xeventobject * head;
    xeventobject * tail;
    xuint64 size;

    struct {
        xdescriptoreventgenerator * descriptor;
    } generator;
};

extern xeventengine * xeventengineGen(void);
extern xeventengine * xeventengineGet(void);

extern xint32 xeventengineRun(xeventengine * o);

// extern xeventsubscription * xeventengineRegisterObject(xeventengine * o, xeventobject * object, xeventhandler on);
extern xeventsubscription * xeventengineRegisterDescriptor(xeventengine * o, xdescriptor * descriptor, xdescriptoreventhandler on);

extern xeventobject * xeventengineUnregister(xeventengine * o, xeventsubscription * subscription);

extern xint64 xeventengineWait(xdescriptor * descriptor, xuint32 events, xint64 second, xint64 nanosecond);

#endif // __NOVEMBERIZING_X_EVENT__ENGINE__H__
