#ifndef   __NOVEMBERIZING_X__EVENT__H__
#define   __NOVEMBERIZING_X__EVENT__H__

#include <x/std.h>

struct xevent;
struct xeventengine;
struct xeventobject;
struct xeventprocessor;
struct xeventgenerator;
struct xeventsubscription;

typedef struct xevent xevent;
typedef struct xeventengine xeventengine;
typedef struct xeventobject xeventobject;
typedef struct xeventprocessor xeventprocessor;
typedef struct xeventgenerator xeventgenerator;
typedef struct xeventsubscription xeventsubscription;


typedef void (*xeventhandler)(xeventobject *, xeventsubscription *, xuint32);

struct xevent
{

};

struct xeventset
{

};

#endif // __NOVEMBERIZING_X__EVENT__H__
