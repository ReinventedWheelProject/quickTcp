//#define CHAR_BUFF_0 255
#define CHAR_BUFF 256

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>			// for cout and cin
////////////////////////////////////////////////////////////////////////////
class TcpIp				//begin declaration of the class "TcpIp"
{
private:				// begin private section
    int bindPort, addressFamily;	
    in_addr_t bindAddress;
    int sockfd, newsockfd;
    socklen_t clilen;
    char buffer[CHAR_BUFF];
    struct sockaddr_in serv_addr, cli_addr;
    bool doGlobalEntry, doLocalEntry;
    void (*tcpInboundHandler)(const char *msg);//free func pointer
    void error(const char *msg); // member functions
    void printIp();
    int  tcpRead();
    int  tcpWrite(const char *msg);
    void useAddressFamily(int par1);
    void useBindAddress(in_addr_t par1);
    void useBindPort(int par1);
public:				// begin public section
    TcpIp();			// constructor
    ~TcpIp();				// destructor
    int tcpStart();       //init the conection and declare the bind port
    void tcpClose();              //close open ports
    ///not used // accessor function
    void setInboundHandler(void (*func1)(const char*)); // function  mutator
    void setAddressFamily(int par1);
    void setBindAddress(in_addr_t par1);
    void setBindPort(int par1);
};
//print error and exit
void TcpIp::error(const char *msg)
{
    perror(msg);
    exit(1);
}
//print ip on connect
void TcpIp::printIp()
{
    printf("new conection from %d.%d.%d.%d\n",
           int(cli_addr.sin_addr.s_addr&0xFF),
           int((cli_addr.sin_addr.s_addr&0xFF00)>>8),
           int((cli_addr.sin_addr.s_addr&0xFF0000)>>16),
           int((cli_addr.sin_addr.s_addr&0xFF000000)>>24));
}
//read hook
int TcpIp::tcpRead()
{
    int e= read(newsockfd,buffer,CHAR_BUFF -1);
    if (e < 0) error("ERROR reading from socket");
    tcpInboundHandler(buffer);
    return e;
}
//write hook
int TcpIp::tcpWrite(const char *msg)
{
    int e=write(newsockfd,msg,sizeof(msg));
    if (e < 0) error("ERROR writing to socket");
    return e;
}
// constructor of TcpIp
TcpIp::TcpIp()
{
    doGlobalEntry=true;
    doLocalEntry=true;
    //using lambda expression 4 lolz
    setInboundHandler([](const char *par1) {});
    setAddressFamily(AF_INET);
    setBindAddress(inet_addr("127.0.0.1"));
    setBindPort(25565);
}
// destructor, just an example
TcpIp::~TcpIp() {}
//public init method
int TcpIp::tcpStart()
{
    sockfd = socket(addressFamily, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    useAddressFamily( addressFamily);
    useBindAddress(bindAddress);
    useBindPort(bindPort);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
        error("ERROR on binding");
//   while(doGlobalEntry) //TODO
    listen(sockfd,5);
    clilen = sizeof(cli_addr);


///accept conection
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    printIp();

    if (newsockfd < 0) error("ERROR on accept");
    bzero(buffer,CHAR_BUFF);
    while(doLocalEntry)
    {
//write ":>" to socket
        tcpWrite(":> ");
//read from to socket
        tcpRead();
//        printf("net>: %s",buffer);
    }
    // }
    return 0;
}
//close the stuffs
void TcpIp::tcpClose()
{
    close(newsockfd);
    close(sockfd);
}
//public hook for readind from stream
void TcpIp::setInboundHandler(void (*func1)(const char*))
{
    tcpInboundHandler=func1;
}
//used by Start
void TcpIp::useAddressFamily(int par1)
{
    serv_addr.sin_family = par1;

}
//public mutator
void TcpIp::setAddressFamily(int par1)
{

    addressFamily=par1;
}
//used by Start
void TcpIp::useBindAddress(in_addr_t par1)
{
//----------------------------------------------------------------------|
//If you wish to bind your socket to all interfaces, the syntax would be|
//  serv_addr.sin_addr.s_addr = INADDR_ANY;_____________________________|
//If you wish to bind your socket to localhost only, the syntax would be|
//  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");//_______________|
//  serv_addr.sin_addr.s_addr = INADDR_LOOPBACK;________________________|
//----------------------------------------------------------------------|
    serv_addr.sin_addr.s_addr = par1;
}
//public mutator
void TcpIp::setBindAddress(in_addr_t par1)
{
//----------------------------------------------------------------------|
//If you wish to bind your socket to all interfaces, the syntax would be|
//  serv_addr.sin_addr.s_addr = INADDR_ANY;_____________________________|
//If you wish to bind your socket to localhost only, the syntax would be|
//  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");//_______________|
//  serv_addr.sin_addr.s_addr = INADDR_LOOPBACK;________________________|
//----------------------------------------------------------------------|
    bindAddress=par1;
}
//used by start
void TcpIp::useBindPort(int par1)
{
    serv_addr.sin_port = htons(par1);
}
//public mutator
void TcpIp::setBindPort(int par1)
{
    bindPort=par1;
}
