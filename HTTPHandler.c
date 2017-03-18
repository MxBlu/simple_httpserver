//
// Created by Jumail on 18/03/2017.
//

#include <string.h>
#include <stdio.h>

#include "HTTPHandler.h"

#define REQUEST_BUFFER_SIZE 1000
#define MAX_FILESIZE 500000 // 500kB max filesize

#define SEND_SUCCESS 1
#define SEND_FAILED 0

static int sendHTMLHeader(int sockfd, char *status, char *contentType, int fileSize);
static void guessType(char *filename, char *typeBuf);

int parseRequest(char *clientBuff, char *request) {
    char *token;
    token = strtok(clientBuff, " ");

    if(token == NULL || strcmp(token, "GET") != 0)
        return PARSE_REQUESTUNKNOWN;

    token = strtok(NULL, " ");
    if(token == NULL)
        return PARSE_REQUESTUNKNOWN;

    strcpy(request, token);
    int reqLen = strlen(request);

    if(request[reqLen - 1] == '/')
        strcat(request, "index.html");
    else {
        char *lastDot = strrchr(request, '.');
        char *lastSlash = strrchr(request, '/');

        if (lastDot < lastSlash)
            strcat(request, "/index.html");
    }

    token = strtok(NULL, " ");
    if(!(strcmp(token, "HTTP/1.1") || strcmp(token, "HTTP/1.0")))
        return PARSE_REQUESTUNKNOWN;

    return PARSE_REQUESTHTML;
}

void serveHTML(int sockfd, char *file) {
    char filename[500];
    strcpy(filename, "www");
    strcat(filename, file);

    FILE *fileptr = fopen(filename, "r");
    char fileBuffer[MAX_FILESIZE];

    if (fileptr == NULL) {
        snprintf(fileBuffer, MAX_FILESIZE, "<!DOCTYPE html>\r\n"
                "<html>\r\n"
                "<head>\r\n"
                "<title>Error 404</title>\r\n"
                "</head>\r\n"
                "<body>\r\n"
                "<h1>Error 404</h1>\r\n"
                "<p>Oh no, file %s not found :(</p>\r\n"
                "</body>\r\n"
                "</html>\r\n"
                "\r\n", file);

        if (sendHTMLHeader(sockfd, "404 Not Found", "text/html; charset=UTF-8", strlen(fileBuffer)) <= 0)
            return;

        send(sockfd, fileBuffer, strlen(fileBuffer), 0);

    } else {
        char mimetype[100];
        guessType(file, mimetype);

        int bytes = fread(fileBuffer, 1, MAX_FILESIZE, fileptr);
        if (sendHTMLHeader(sockfd, "200 OK", mimetype, bytes) <= 0)
            return;

        send(sockfd, fileBuffer, bytes, 0);
        fclose(fileptr);
    }

    return;
}

static int sendHTMLHeader(int sockfd, char *status, char *contentType, int fileSize) {
    char response[1000];
    snprintf(response, 1000, "HTTP/1.1 %s\r\n"
                             "Content-Type: %s\r\n"
                             "Content-Length: %d\r\n\r\n", status, contentType, fileSize);

    return send(sockfd, response, strlen(response), 0);
}

static void guessType(char *filename, char *typeBuf) {
    char *dotPtr = strrchr(filename, '.');
    if(strcmp(dotPtr, ".html") == 0) {
        strcpy(typeBuf, "text/html");
    } else if (strcmp(dotPtr, ".css") == 0) {
        strcpy(typeBuf, "text/css");
    } else if (strcmp(dotPtr, ".js") == 0) {
        strcpy(typeBuf, "application/javascript; charset=utf-8");
    } else if (strcmp(dotPtr, ".png") == 0) {
        strcpy(typeBuf, "image/png");
    } else if (strcmp(dotPtr, ".jpg") == 0) {
        strcpy(typeBuf, "image/jpeg");
    }
}