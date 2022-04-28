#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include <x/client/socket.h>

int main(int argc, char ** argv)
{
    struct sockaddr_in addr = { 0, };

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(6379);

    xclientsocket * o = xclientsocketNew(xdescriptor_invalid_value, AF_INET, SOCK_STREAM, IPPROTO_TCP, xaddressof(addr), sizeof(struct sockaddr_in));

    xint32 ret = xclientsocketConnect(o);

    xfunctionAssert(ret != xsuccess, "client is not connected");

    xclientsocketSend(o, "ping\r\n", 7);

    char buffer[16] = { 0, };
    xint64 n = xclientsocketRecv(o, buffer, 7);

    printf("%s", buffer);
    
    xclientsocketClose(o);

    o = xclientsocketDel(o);

    return 0;
}