#include <stdio.h>
#include <stdlib.h>

#include "sync.h"

#include "sync/mutex.h"

extern xsync * xsyncNew(xuint32 type)
{
    switch(type)
    {
    case xsynctype_mutex:   return (xsync *) xsyncmutexNew();
    }

    xthrow("invalid parameter");

    return xnil;
}