#include "command.h"

extern xcommand * xcommandNew(xcommandexec exec, const xcommandset * set, xuint64 size)
{
    xcommand * o = (xcommand *) calloc(1, size);

    o->set = set;
    o->exec = exec;

    return o;
}