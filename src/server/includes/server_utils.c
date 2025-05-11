/*
* Utilidades de conexión para el servidor
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "server_utils.h"


// Send null-terminated string
void send_string(int sock, const char *s) {
    send(sock, s, strlen(s), 0);
    send(sock, "\0", 1, 0);
}

// Receive null-terminated string (must free)
char* recv_string(int sock) {
    size_t bufsize = 256, len = 0;
    char *buf = malloc(bufsize);
    while (1) {
        char c;
        if (recv(sock, &c, 1, 0) <= 0) break;
        if (c == '\0') break;
        if (len + 1 >= bufsize) {
            bufsize *= 2;
            buf = realloc(buf, bufsize);
        }
        buf[len++] = c;
    }
    buf[len] = '\0';
    return buf;
}

