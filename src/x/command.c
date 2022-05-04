#include <stdlib.h>

#include "command.h"

static xcommand * commandDel(xcommand * o);

static xcommandset virtualSet = {
    commandDel
};

extern xcommand * xcommandNew(xcommandexec exec, const xcommandset * set, xuint64 size)
{
    xcommand * o = (xcommand *) calloc(1, size);

    o->set = set;
    o->exec = exec;

    return o;
}

static xcommand * commandDel(xcommand * o)
{
    if(o)
    {
        free(o);
    }
    return xnil;
}

extern const xcommandset * xcommandsetGet(void)
{
    return xaddressof(virtualSet);
}