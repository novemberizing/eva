#ifndef   __NOVEMBERIZING_X__OBJECT__H__
#define   __NOVEMBERIZING_X__OBJECT__H__

#include <x/std.h>

struct xobject;

typedef struct xobject xobject;

struct xobject
{
    xuint64 length;
    void * value;
};

extern xobject * xobjectNew(const void * value, xuint64 length);
extern void xobjectSet(xobject * o, const void * value, xuint64 length);
extern xobject * xobjectDel(xobject * o);

#endif // __NOVEMBERIZING_X__OBJECT__H__
