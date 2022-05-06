#include <stdio.h>
#include <unistd.h>

#include <x/command.h>
#include <x/command/event/subscription.h>

#include <x/event/engine.h>

static void wakeup(xcommand * command)
{
    printf("wake up, world!\n");
    usleep(1000000);

    xeventengineCancel(xeventengineGet(), xeventengineStop);
}

static void commandeventOn(xcommand * command, xcommandeventsubscription * subscription, xuint32 type)
{
    subscription = xcommandeventsubscriptionDel(subscription);
    command = xcommandDel(command);
}

int main(int argc, char ** argv)
{
    xeventengine * engine = xeventengineGen();

    xcommand * command = xcommandNew(wakeup, xcommandsetGet(), sizeof(xcommand));

    xcommandeventsubscription * subscription = xeventengineCommandReg(engine, command, commandeventOn);

    return xeventengineRun(engine);
}
