#include <stdlib.h>

#include "subscription.h"

#include "generator.h"

#include "../../event/engine.h"

static xcommandeventsubscription * commandeventsubscriptionDel(xcommandeventsubscription * o);

static xcommandeventsubscriptionset virtualSet = {
    commandeventsubscriptionDel
};

extern xcommandeventsubscription * xcommandeventsubscriptionNew(xeventobject * object, xcommandeventhandler on)
{
    xcommandeventsubscription * o = (xcommandeventsubscription *) calloc(1, sizeof(xcommandeventsubscription));

    o->set = xaddressof(virtualSet);

    o->object = object;
    o->on = on;

    return o;
}

static xcommandeventsubscription * commandeventsubscriptionDel(xcommandeventsubscription * o)
{
    if(o)
    {
        if(o->generator) xcommandeventgeneratorUnreg(o->generator, o);
        if(o->engine) xeventengineCommandEventSubscriptionUnreg(o->engine, o);

        free(o);
    }
    return xnil;
}
