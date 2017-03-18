//
// Created by Jumail on 18/03/2017.
//
#include <stdio.h>

#include "Socket.h"

int InitializeSockets() {
#if PLATFORM == PLATFORM_WINDOWS
    WSADATA WsaData;
    return WSAStartup(MAKEWORD(2,2), &WsaData) == NO_ERROR;
#else
    return 1;
#endif
}

void ShutdownSockets() {
#if PLATFORM == PLATFORM_WINDOWS
    WSACleanup();
#endif
}

int createSocket(unsigned short port) {
    int sockfd = socket (AF_INET, SOCK_STREAM, 0); // Create socket
    if (sockfd < 0) {
        fprintf(stderr, "createSocket: error creating socket\n");
        return -1;
    }

#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
    const int optionValue = 1;
    int size = sizeof(int);
#elif PLATFORM == PLATFORM_WINDOWS
    const char optionValue = 1;
    int size = sizeof(char);
#endif

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optionValue, size); // Allow port to be reused without delay

    struct sockaddr_in server; // Set up local socket info
    memset (&server, 0, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *) &server, sizeof (server)) < 0) { // Bind socket to port
        fprintf(stderr, "createSocket: error binding socket\n");
        return -1;
    }

    return sockfd;
}

void closeSocket(int sockfd) {
#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
    close(sockfd);
#elif PLATFORM == PLATFORM_WINDOWS
    closesocket(sockfd);
#endif
}

int getClientSocket(int serverSocket) {
    listen (serverSocket, MAX_SERVERBACKLOG); // Sleep until packet received

    struct sockaddr_in client;
    socklen_t clientLen = sizeof(client);
    int sockfd = accept(serverSocket, (struct sockaddr *) &client, &clientLen); // Get client socket

    if (sockfd < 0) {
        fprintf(stderr, "getClient: error getting client socket\n");
        return -1;
    }

    return sockfd;
}
