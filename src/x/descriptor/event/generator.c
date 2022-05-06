#include <stdlib.h>

#include "generator.h"

#include "../../event/generator.h"

extern xdescriptoreventgenerator * xdescriptoreventgeneratorNew(xeventengine * engine, const xdescriptoreventgeneratorset * set, xuint64 size)
{
    xdescriptoreventgenerator * o = xeventgeneratorNew(engine, (xeventgeneratorset *) set, size);

    return o;
}