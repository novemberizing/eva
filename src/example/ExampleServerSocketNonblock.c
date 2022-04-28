#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>

#include <x/server/socket.h>
#include <x/event/engine.h>

int main(int argc, char ** argv)
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(3371);

    xserversocket * socket = xserversocketNew(xserversocket_invalid_value, AF_INET, SOCK_STREAM, IPPROTO_TCP, xaddressof(addr), sizeof(struct sockaddr_in));
    xserversocketSetMode(socket, xserversocketmode_reuseaddr);
    xserversocketSetMode(socket, xserversocketmode_nonblock);
    if(xserversocketOpen(socket) == xsuccess)
    {
        if(xeventengineWait((xdescriptor *) socket, xserversocketevent_in, 0, 0) == xsuccess)
        {
            xsessionsocket * sessionsocket = xserversocketAccept(socket);
            xsessionsocketNonblockOn(sessionsocket);
            if(sessionsocket)
            {
                if(xeventengineWait((xdescriptor *) sessionsocket, xsessionsocketevent_in, 0, 0) == xsuccess)
                {
                    if(xsessionsocketRead(sessionsocket) > 0)
                    {
                        printf("%s", xstreamFront(sessionsocket->stream.in));
                        xstreamPush(sessionsocket->stream.out, xstreamFront(sessionsocket->stream.in), xstreamLen(sessionsocket->stream.in));
                        xsessionsocketNonblockOff(sessionsocket);
                        xsessionsocketWrite(sessionsocket);
                    }
                }
                xsessionsocketClose(sessionsocket);
                xserversocketRel(socket, sessionsocket);
            }
            xserversocketClose(socket);
        }
    }
    else
    {
        printf("server is not open\n");
    }
    socket = xserversocketDel(socket);
    return 0;
}
