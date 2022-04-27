#ifndef   __NOVEMBERIZING_X_SYNC__MUTEX__H__
#define   __NOVEMBERIZING_X_SYNC__MUTEX__H__

#include <pthread.h>

#include <x/sync.h>

struct xsyncmutex;
struct xsyncmutexset;

typedef struct xsyncmutex xsyncmutex;
typedef struct xsyncmutexset xsyncmutexset;

struct xsyncmutex
{
    const xsyncmutexset * set;

    xuint32 type;

    pthread_mutex_t mutex;
    pthread_cond_t condition;
};

struct xsyncmutexset
{
    xsyncmutex * (*del)(xsyncmutex *);
    xint32 (*lock)(xsyncmutex *);
    xint32 (*unlock)(xsyncmutex *);
    xint32 (*wait)(xsyncmutex *, xint64, xint64);
    xint32 (*wakeup)(xsyncmutex *, xint32);
};

extern xsyncmutex * xsyncmutexNew(void);

#define xsyncmutexDel(o)                        (o->set->del(o))
#define xsyncmutexLock(o)                       (o->set->lock(o))
#define xsyncmutexUnlock(o)                     (o->set->unlock(o))
#define xsyncmutexWait(o, second, nanosecond)   (o->set->wait(o, second, nanosecond))
#define xsyncmutexWakeup(o, all)                (o->set->wakeup(o, all))

#endif // __NOVEMBERIZING_X_SYNC__MUTEX__H__
