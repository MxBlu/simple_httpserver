#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "Socket.h"
#include "HTTPHandler.h"

#define MAX_PAGESSERVED 100000
#define REQUEST_BUFFER_SIZE 1000

int main(int argc, char *argv[]) {
    if(argc != 2) {
        fprintf(stderr, "Usage: %s [port]\n", argv[0]);
        return -1;
    }

    // (i) create a connection socket when contacted by a client (browser).

    assert(InitializeSockets()); // Initialise Windows sockets if necessary
    int serverfd = createSocket((unsigned short) atoi(argv[1]));
    assert(serverfd > 0);
    
    printf("Server running on port %s\n", argv[1]);

    char requestBuff[REQUEST_BUFFER_SIZE];
    char request[REQUEST_BUFFER_SIZE];
    memset(requestBuff, 0, REQUEST_BUFFER_SIZE);

    int numPagesServed = 0;

    while (numPagesServed < MAX_PAGESSERVED) {
        numPagesServed++;
        int clientfd = getClientSocket(serverfd);

        int bytes = recv(clientfd, requestBuff, REQUEST_BUFFER_SIZE, 0);
        if (bytes < 0) {
            closeSocket(clientfd);
            continue;
        }

        if (parseRequest(requestBuff, request)) {
            printf("Client requested: %s\n", request);
            serveHTML(clientfd, request);
        } else
            printf("Client sent bad request\n");

        closeSocket(clientfd);
    }

    closeSocket(serverfd);
    ShutdownSockets();
    return 0;
}