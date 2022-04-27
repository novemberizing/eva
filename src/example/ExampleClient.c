#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>

#include <x/client.h>

int main(int argc, char ** argv)
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(6379);

    xclient * client = xclientNew(AF_INET, SOCK_STREAM, IPPROTO_TCP, xaddressof(addr), sizeof(struct sockaddr_in));
    if(xclientConnect(client) == xsuccess)
    {
        char buffer[64] = { 0, };
        xclientSend(client, "ping\r\n", 6);
        xclientRecv(client, buffer, 7);
        printf("%s", buffer);
    }
    client = xclientDel(client);
    return 0;
}