#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include <x/server.h>

int main(int argc, char ** argv)
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(3371);

    xserver * server = xserverNew(AF_INET, SOCK_STREAM, IPPROTO_TCP, xaddressof(addr), sizeof(struct sockaddr_in), xnil);

    xserversocketSetMode(server->socket, xserversocketmode_reuseaddr);

    if(xserverOpen(server) == xsuccess)
    {
        xsession * session = xserverAccept(server);
        xsessionClose(session);
        xserverClose(server);
    }
    else
    {
        xfunctionThrow("fail to xserverOpen(...)");
    }

    server = xserverDel(server);

    return 0;
}
