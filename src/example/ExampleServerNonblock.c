#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include <x/server.h>
#include <x/event/engine.h>

int main(int argc, char ** argv)
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(3371);

    xserver * server = xserverNew(AF_INET, SOCK_STREAM, IPPROTO_TCP, xaddressof(addr), sizeof(struct sockaddr_in), xnil);

    xserversocketSetMode(server->socket, xserversocketmode_reuseaddr);
    xserversocketSetMode(server->socket, xserversocketmode_nonblock);

    if(xserverOpen(server) == xsuccess)
    {
        if(xeventengineWait((xdescriptor *) server->socket, xserversocketevent_in, 0, 0) == xsuccess)
        {
            xsession * session = xserverAccept(server);
            if(xeventengineWait((xdescriptor *) session->socket, xsessionsocketevent_in, 0, 0) == xsuccess)
            {
                xsessionRead(session);
                printf("%s", xstreamFront(session->socket->stream.in));
                xsessionsocketNonblockOff(session->socket);
                xstreamPush(session->socket->stream.out, xstreamFront(session->socket->stream.in), xstreamLen(session->socket->stream.in));
                xsessionWrite(session);
            }
            xsessionClose(session);
        }
        xserverClose(server);
    }
    else
    {
        xfunctionThrow("fail to xserverOpen(...)");
    }

    server = xserverDel(server);

    return 0;
}
