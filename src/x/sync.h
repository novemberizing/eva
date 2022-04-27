#ifndef   __NOVEMBERIZING_X__SYNC__H__
#define   __NOVEMBERIZING_X__SYNC__H__

#include <x/std.h>

#define xsynctype_mutex             1

struct xsync;
struct xsyncset;

typedef struct xsync xsync;
typedef struct xsyncset xsyncset;

struct xsync
{
    const xsyncset * set;

    xuint32 type;
};

struct xsyncset
{
    xsync * (*del)(xsync *);
    xint32 (*lock)(xsync *);
    xint32 (*unlock)(xsync *);
    xint32 (*wait)(xsync *, xint64, xint64);
    xint32 (*wakeup)(xsync *, xint32);
};

extern xsync * xsyncNew(xuint32 type);

#define xsyncDel(o)                         (o ? o->set->del(o) : xnil)
#define xsyncLock(o)                        (o ? o->set->lock(o) : xfail)
#define xsyncUnlock(o)                      (o ? o->set->unlock(o) : xfail)
#define xsyncWait(o, second, nanosecond)    (o ? o->set->wait(o, second, nanosecond) : xfail)
#define xsyncWakeup(o, all)                 (o ? o->set->wakeup(o, all) : xfail)

#endif // __NOVEMBERIZING_X__SYNC__H__
