//#warning error
#include <iostream>
#include <netinet/in.h>
#include "include/tcp.h"
TcpIp connection;
void onRead( const char *msg)
{
    switch (msg[0])
    {
    case '0':
//quit
        connection.tcpClose();
        exit(0);
        break;
    case '1':
//left  click
        system("xdotool click 1");
    case '3':
//right click
        system("xdotool click 3");
        break;
    default:
        printf("net>: %s",msg);
        std::cout<< ""<< std::endl;
    }

}

//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//main
int main(int argc, char *argv[])
{
    std::cout << "" << std::endl;
    if (argc < 2)
    {
        fprintf(stderr,"ERROR, no port provided\n");
        return(1);
    }
    connection.setInboundHandler(onRead);
    connection.setAddressFamily(AF_INET);
    connection.setBindAddress(INADDR_ANY);
    connection.setBindPort(atoi(argv[1]));
    connection.tcpStart();
    connection.tcpClose();
    return 0;
}

