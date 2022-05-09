#include "event.h"

#include "event/subscription.h"

extern void xdescriptoreventOn(xdescriptor * descriptor, xdescriptoreventsubscription * subscription, xuint32 type)
{
    if(type & xdescriptorevent_type_in)
    {
        xint64 n = 0;

        n = xdescriptorRead(descriptor);
        if(n >= 0)
        {
            if(n > 0)
            {
                subscription->on(descriptor, subscription, xdescriptorevent_type | xdescriptorevent_type_in);
            }
            n = xdescriptorWrite(descriptor);
            if(n > 0)
            {
                subscription->on(descriptor, subscription, xdescriptorevent_type | xdescriptorevent_type_out);
            }
            else if(n < 0)
            {
                subscription->on(descriptor, subscription, xdescriptorevent_type | xdescriptorevent_type_error);

                xdescriptorClose(descriptor);
                subscription->on(descriptor, subscription, xdescriptorevent_type | xdescriptorevent_type_close);
                
            }
        }
        else if(n < 0)
        {
            subscription->on(descriptor, subscription, xdescriptorevent_type | xdescriptorevent_type_error);

            xdescriptorClose(descriptor);
            subscription->on(descriptor, subscription, xdescriptorevent_type | xdescriptorevent_type_close);
        }
    }
    else if(type & xdescriptorevent_type_out)
    {
        xint64 n = 0;
        n = xdescriptorWrite(descriptor);
        if(n > 0)
        {
            subscription->on(descriptor, subscription, xdescriptorevent_type | xdescriptorevent_type_out);
        }
        else if(n < 0)
        {
            subscription->on(descriptor, subscription, xdescriptorevent_type | xdescriptorevent_type_error);

            xdescriptorClose(descriptor);
            subscription->on(descriptor, subscription, xdescriptorevent_type | xdescriptorevent_type_close);
        }
    }
    else if(type & xdescriptorevent_type_error)
    {
        subscription->on(descriptor, subscription, xdescriptorevent_type | xdescriptorevent_type_error);

        xdescriptorClose(descriptor);
        subscription->on(descriptor, subscription, xdescriptorevent_type | xdescriptorevent_type_close);
    }
}