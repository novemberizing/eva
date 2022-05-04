#include <stdio.h>

#include <x/command.h>

static void helloworld(xcommand * o);

int main(int argc, char ** argv)
{
    xcommand * command = xcommandNew(helloworld, xcommandsetGet(), sizeof(xcommand));

    xcommandExec(command);
    xcommandDel(command);

    return 0;
}

static void helloworld(xcommand * o)
{
    printf("hello world => %p\n", o);
}
