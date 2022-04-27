#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <x/client.h>
#include <x/event/engine.h>

int main(int argc, char ** argv)
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(6379);
    xclient * client = xclientNew(AF_INET, SOCK_STREAM, IPPROTO_TCP, xaddressof(addr), sizeof(struct sockaddr_in));
    xclientsocketSetMode(client->socket, xclientsocketmode_nonblock);
    xclientConnect(client);

    if(xeventengineWait((xdescriptor *) client->socket, xclientsocketevent_connect, 0, 0) == xsuccess)
    {
        xclientSend(client, "ping\r\n", 6);
        if(xeventengineWait((xdescriptor *) client->socket, xclientsocketevent_in, 0, 0) == xsuccess)
        {
            char buffer[64] = { 0, };
            xclientRecv(client, buffer, 7);

            printf("%s", buffer);
        }
        xclientClose(client);
    }
    else
    {
        printf("client not connect\n");
    }
    client = xclientDel(client);
    return 0;
}
