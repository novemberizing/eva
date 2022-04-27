#include <stdio.h>
#include <stdlib.h>

#include "mutex.h"

static xsyncmutex * syncmutexDel(xsyncmutex * o);
static xint32 syncmutexLock(xsyncmutex * o);
static xint32 syncmutexUnlock(xsyncmutex * o);
static xint32 syncmutexWait(xsyncmutex * o, xint64 second, xint64 nanosecond);
static xint32 syncmutexWakeup(xsyncmutex * o, xint32 all);

static xsyncmutexset virtualSet = {
    syncmutexDel,
    syncmutexLock,
    syncmutexUnlock,
    syncmutexWait,
    syncmutexWakeup
};

extern xsyncmutex * xsyncmutexNew(void)
{
    xsyncmutex * o = (xsyncmutex *) calloc(1, sizeof(xsyncmutex));

    o->set = xaddressof(virtualSet);

    o->type = xsynctype_mutex;

    o->mutex = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
    o->condition = (pthread_cond_t) PTHREAD_COND_INITIALIZER;

    return o;
}

static xsyncmutex * syncmutexDel(xsyncmutex * o)
{
    if(o)
    {
        pthread_cond_destroy(xaddressof(o->condition));
        pthread_mutex_destroy(xaddressof(o->mutex));

        free(o);
    }

    return xnil;
}

static xint32 syncmutexLock(xsyncmutex * o)
{
    xint32 ret = pthread_mutex_lock(xaddressof(o->mutex));

    return ret == xsuccess ? xsuccess : xfail;
}

static xint32 syncmutexUnlock(xsyncmutex * o)
{
    xint32 ret = pthread_mutex_unlock(xaddressof(o->mutex));

    return ret == xsuccess ? xsuccess : xfail;
}

static xint32 syncmutexWait(xsyncmutex * o, xint64 second, xint64 nanosecond)
{
    xassert(second < 0 || nanosecond < 0, "invalid parameter");

    xint32 ret = xsuccess;

    if(second > 0 || nanosecond > 0)
    {
        struct timespec spec = { 0, };
        
        if((ret = clock_gettime(CLOCK_REALTIME, xaddressof(spec))) == xsuccess)
        {
            spec.tv_sec = spec.tv_sec + second;
            spec.tv_nsec = spec.tv_nsec + nanosecond;

            ret = pthread_cond_timedwait(xaddressof(o->condition), xaddressof(o->mutex), xaddressof(spec));
        }
    }
    else
    {
        ret = pthread_cond_wait(xaddressof(o->condition), xaddressof(o->mutex));
    }

    return ret == xsuccess ? xsuccess : xfail;
}

static xint32 syncmutexWakeup(xsyncmutex * o, xint32 all)
{
    xint32 ret = xsuccess;

    if(all)
    {
        ret = pthread_cond_broadcast(xaddressof(o->condition));
    }
    else
    {
        ret = pthread_cond_signal(xaddressof(o->condition));
    }

    return ret == xsuccess ? xsuccess : xfail;
}
