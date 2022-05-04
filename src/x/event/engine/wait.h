#ifndef   __NOVEMBERIZING_X_EVENT_ENGINE__WAIT__H__
#define   __NOVEMBERIZING_X_EVENT_ENGINE__WAIT__H__

#include <x/event.h>

#include <x/descriptor.h>

extern xint64 xeventengineWait(xdescriptor * descriptor, xuint32 events, xint64 second, xint64 nanosecond);

#endif // __NOVEMBERIZING_X_EVENT_ENGINE__WAIT__H__
