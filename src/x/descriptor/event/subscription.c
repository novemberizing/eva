#include "subscription.h"

#include "descriptor.h"

#include "../../event/engine.h"

static xdescriptoreventsubscription * descriptoreventsubscriptionDel(xdescriptoreventsubscription * o);

static xdescriptoreventsubscriptionset virtualSet = {
    descriptoreventsubscriptionDel
};

extern xdescriptoreventsubscription * xdescriptoreventsubscriptionNew(xdescriptor * descriptor, xdescriptoreventhandler on)
{
    xdescriptoreventsubscription * o = (xdescriptoreventsubscription *) calloc(1, sizeof(xdescriptoreventsubscription));

    o->set = xaddressof(virtualSet);
    o->descriptor = descriptor;
    o->on = on;

    return o;
}

static xdescriptoreventsubscription * descriptoreventsubscriptionDel(xdescriptoreventsubscription * o)
{
    if(o)
    {
        if(o->generator) xdescriptoreventgeneratorUnreg(o->generator, o);
        if(o->engine) xeventengineDescriptorEventSubscriptionUnreg(o->engine, o);
        free(o);
    }
    return xnil;
}
