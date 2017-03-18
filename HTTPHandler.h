//
// Created by Jumail on 18/03/2017.
//

#ifndef HTTP_SERVER_HTTPHANDLER_H
#define HTTP_SERVER_HTTPHANDLER_H

#include "Socket.h"

#define PARSE_REQUESTHTML 1
#define PARSE_REQUESTUNKNOWN 0

#define SERVE_FAILED 0
#define SERVE_SUCCESS 1

int parseRequest(char *clientBuff, char *request);
void serveHTML(int sockfd, char *file);


#endif //HTTP_SERVER_HTTPHANDLER_H
