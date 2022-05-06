#include "event.h"
#include "event/subscription.h"

extern void xcommandeventOn(xcommand * command, xcommandeventsubscription * subscription, xuint32 type)
{
    command->exec(command);
    if(subscription->on)
    {
        subscription->on(command, subscription, type);
    }
}