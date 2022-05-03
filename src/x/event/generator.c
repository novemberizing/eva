#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "generator.h"

extern xeventgenerator * xeventgeneratorNew(xeventengine * engine, const xeventgeneratorset * set, xuint64 size)
{
    xfunctionAssert(size < sizeof(xeventgenerator) || engine == xnil || set == xnil, "invalid parameter");
    
    xeventgenerator * o = (xeventgenerator *) calloc(1, size);

    o->set = set;

    o->engine = engine;

    return o;
}