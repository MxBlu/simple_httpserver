//
// Created by Jumail on 18/03/2017.
//

#ifndef HTTP_SERVER_SOCKET_H
#define HTTP_SERVER_SOCKET_H

#define PLATFORM_WINDOWS  1
#define PLATFORM_MAC      2
#define PLATFORM_UNIX     3

#if defined(_WIN32)
#define PLATFORM PLATFORM_WINDOWS
#elif defined(__APPLE__)
#define PLATFORM PLATFORM_MAC
#else
    #define PLATFORM PLATFORM_UNIX
#endif

#if PLATFORM == PLATFORM_WINDOWS
#include <winsock2.h>
#include <ws2tcpip.h>
#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
#include <unistd.h>
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <netdb.h>
    #include <netinet/in.h>
    #include <fcntl.h>
    #include <string.h>
#endif

#define MAX_SERVERBACKLOG 1000

int InitializeSockets();
void ShutdownSockets();

int createSocket (unsigned short port);
void closeSocket(int sockfd);

int getClientSocket(int serverSocket);

#endif //HTTP_SERVER_SOCKET_H
