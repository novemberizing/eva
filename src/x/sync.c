#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "sync.h"

#include "sync/mutex.h"

extern xsync * xsyncNew(xuint32 type)
{
    switch(type)
    {
    case xsynctype_mutex:   return (xsync *) xsyncmutexNew();
    }

    xfunctionThrow("invalid parameter");

    return xnil;
}