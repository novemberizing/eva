#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>

#include <x/server/socket.h>

int main(int argc, char ** argv)
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(3371);

    xserversocket * socket = xserversocketNew(xserversocket_invalid_value, AF_INET, SOCK_STREAM, IPPROTO_TCP, xaddressof(addr), sizeof(struct sockaddr_in));
    if(xserversocketOpen(socket) == xsuccess)
    {
        
        xserversocketClose(socket);
    }
    else
    {
        printf("server is not open\n");
    }
    socket = xserversocketDel(socket);
    return 0;
}
