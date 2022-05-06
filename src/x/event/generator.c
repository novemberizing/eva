#include <stdlib.h>

#include "generator.h"

extern xeventgenerator * xeventgeneratorNew(xeventengine * engine, const xeventgeneratorset * set, xuint64 size)
{
    xeventgenerator * o = (xeventgenerator *) calloc(1, size);

    o->engine = engine;
    o->set = set;

    return o;
}