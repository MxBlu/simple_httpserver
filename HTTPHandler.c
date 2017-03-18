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

static int sendHTMLHeader(int sockfd, char *status, int fileSize);
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

        if (sendHTMLHeader(sockfd, "404 Not Found", strlen(fileBuffer)) <= 0)
            return;

        send(sockfd, fileBuffer, strlen(fileBuffer), 0);

    } else {
        int bytes = fread(fileBuffer, 1, MAX_FILESIZE, fileptr);
        if (sendHTMLHeader(sockfd, "200 OK", bytes) <= 0)
            return;

        send(sockfd, fileBuffer, bytes, 0);
        fclose(fileptr);
    }

    return;
}

static int sendHTMLHeader(int sockfd, char *status, int fileSize) {
    char response[1000];
    snprintf(response, 1000, "HTTP/1.1 %s\r\n"
                             "Content-Type: text/html; charset=UTF-8\r\n"
                             "Content-Length: %d\r\n\r\n", status, fileSize);

    return send(sockfd, response, strlen(response), 0);
}

static void guessType(char *filename, char *typeBuf) {

}