#ifndef   __NOVEMBERIZING_X_EVENT__ENGINE__H__
#define   __NOVEMBERIZING_X_EVENT__ENGINE__H__

#include <x/std.h>
#include <x/descriptor.h>

extern xint64 xeventengineWait(xdescriptor * descriptor, xuint32 events, xint64 second, xint64 nanosecond);

#endif // __NOVEMBERIZING_X_EVENT__ENGINE__H__
