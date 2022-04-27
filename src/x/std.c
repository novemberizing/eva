#include <stdlib.h>

#include "std.h"

extern void * xobjectDel(void * o)
{
    if(o)
    {
        free(o);
    }
    return xnil;
}