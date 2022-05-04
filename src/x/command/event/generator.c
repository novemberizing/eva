#include "generator.h"

static xcommandeventgenerator * commandeventgeneratorDel(xcommandeventgenerator * o);
static void commandeventgeneratorOn(xeventgenerator * o);
static xcommandeventsubscription * commandeventgeneratorReg(xeventgenerator * o, xcommandeventsubscription * subscription);
static xcommandeventsubscription * commandeventgeneratorUnreg(xeventgenerator * o, xcommandeventsubscription * subscription);

static xcommandeventgeneratorset virtualSet = {
    commandeventgeneratorDel,
    commandeventgeneratorOn,
    commandeventgeneratorReg,
    commandeventgeneratorUnreg
};

extern xcommandeventgenerator * xcommandeventgeneratorNew(xeventengine * engine)
{
    xcommandeventgenerator * o = (xcommandeventgenerator *) xeventgeneratorNew(engine, xaddressof(virtualSet), sizeof(xcommandeventgenerator));
    return o;
}

static xcommandeventgenerator * commandeventgeneratorDel(xcommandeventgenerator * o)
{
    if(o)
    {
        commandeventgeneratorClear(o);

        o->sync = xsyncDel(o->sync);

        free(o);
    }
    return xnil;
}

static void commandeventgeneratorOn(xeventgenerator * o)
{
    xcommandeventsubscription * subscription = xnil;

    while(subscription)
    {

    }
}
static xeventsubscription * commandeventgeneratorReg(xeventgenerator * o, xeventsubscription * subscription);
static xeventsubscription * commandeventgeneratorUnreg(xeventgenerator * o, xeventsubscription * subscription);