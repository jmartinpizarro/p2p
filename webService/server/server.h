// server.h
#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>

#define MAX_USERS       100
#define MAX_CONTENT     100
#define MAX_USERNAME    64
#define MAX_FILEPATH    256
#define MAX_DESC        256

typedef struct {
    char path[MAX_FILEPATH];
    char desc[MAX_DESC];
} Content;

typedef struct {
    char username[MAX_USERNAME];
    char ip[INET_ADDRSTRLEN];
    int port;
    int connected;
    int content_count;
    Content content[MAX_CONTENT];
} User;

#endif // SERVER_H