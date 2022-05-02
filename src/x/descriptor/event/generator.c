#include "generator.h"

#include "../../event/engine.h"
#include "../../event/subscription.h"

extern xdescriptoreventgenerator * xdescriptoreventgeneratorNew(xeventengine * engine, xdescriptoreventgeneratorset * set, xuint64 size)
{
    xdescriptoreventgenerator * o = (xdescriptoreventgenerator *) calloc(1, size);

    o->set = set;

    o->engine = engine;

    o->engine->generator.descriptor = o;

    return o;
}
