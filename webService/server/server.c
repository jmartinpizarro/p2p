// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "server.h"

static User users[MAX_USERS];
static int user_count = 0;
static pthread_mutex_t users_mutex = PTHREAD_MUTEX_INITIALIZER;

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

// searches in the user array, returns NULL if it does not exist
User* find_user(const char *name) {
    for (int i = 0; i < user_count; ++i) {
        if (strcmp(users[i].username, name) == 0)
            return &users[i];
    }
    return NULL;
}

// operations are processed here
void* handle_client(void *arg) {
    int client_sock = ((int*)arg)[0];
    struct sockaddr_in addr = *(struct sockaddr_in *)(((int*)arg) + 1);
    free(arg);

    char *op = recv_string(client_sock);
    if (!op) { close(client_sock); return NULL; }

    char *datetime = recv_string(client_sock);
    if (!datetime) { close(client_sock); return NULL; }

    pthread_mutex_lock(&users_mutex);
    unsigned char code = 255;

    if (strcmp(op, "REGISTER") == 0) {
        char *user = recv_string(client_sock);
        if (find_user(user)) code = 1;
        else if (user_count >= MAX_USERS) code = 2;
        else {
            User *u = &users[user_count++];
            strncpy(u->username, user, MAX_USERNAME);
            u->connected = 0;
            u->content_count = 0;
            code = 0;
        }
        free(user);
    }
    else if (strcmp(op, "UNREGISTER") == 0) {
        char *user = recv_string(client_sock);
        User *u = find_user(user);
        if (!u) code = 1;
        else {
            // remove user
            int idx = u - users;
            memmove(&users[idx], &users[idx+1], (user_count - idx - 1) * sizeof(User));
            user_count--;
            code = 0;
        }
        free(user);
    }
    else if (strcmp(op, "CONNECT") == 0) {
        char *user = recv_string(client_sock);
        char *port_s = recv_string(client_sock);
        User *u = find_user(user);
        if (!u) code = 1;
        else if (u->connected) code = 2;
        else {
            strncpy(u->ip, inet_ntoa(addr.sin_addr), INET_ADDRSTRLEN);
            u->port = atoi(port_s);
            u->connected = 1;
            code = 0;
        }
        free(user); free(port_s);
    }
    else if (strcmp(op, "DISCONNECT") == 0) {
        char *user = recv_string(client_sock);
        User *u = find_user(user);
        if (!u) code = 1;
        else if (!u->connected) code = 2;
        else { u->connected = 0; code = 0; }
        free(user);
    }
    else if (strcmp(op, "PUBLISH") == 0) {
        char *user = recv_string(client_sock);
        char *path = recv_string(client_sock);
        char *desc = recv_string(client_sock);
        User *u = find_user(user);
        if (!u) code = 1;
        else if (!u->connected) code = 2;
        else {
            // check duplicate
            int dup = 0;
            for (int i = 0; i < u->content_count; ++i)
                if (strcmp(u->content[i].path, path) == 0) dup = 1;
            if (dup) code = 3;
            else if (u->content_count >= MAX_CONTENT) code = 4;
            else {
                strncpy(u->content[u->content_count].path, path, MAX_FILEPATH);
                strncpy(u->content[u->content_count].desc, desc, MAX_DESC);
                u->content_count++;
                code = 0;
            }
        }
        free(user); free(path); free(desc);
    }
    else if (strcmp(op, "DELETE") == 0) {
        char *user = recv_string(client_sock);
        char *path = recv_string(client_sock);
        User *u = find_user(user);
        if (!u) code = 1;
        else if (!u->connected) code = 2;
        else {
            int found = -1;
            for (int i = 0; i < u->content_count; ++i)
                if (strcmp(u->content[i].path, path) == 0) { found = i; break; }
            if (found < 0) code = 3;
            else {
                memmove(&u->content[found], &u->content[found+1], (u->content_count - found -1) * sizeof(Content));
                u->content_count--;
                code = 0;
            }
        }
        free(user); free(path);
    }
    else if (strcmp(op, "LIST USERS") == 0) {
        char *user = recv_string(client_sock);
        User *u = find_user(user);
        if (!u) code = 1;
        else if (!u->connected) code = 2;
        else {
            code = 0;
            send(client_sock, (char[]){0}, 1, 0);
            // count connected
            int n = 0;
            for (int i = 0; i < user_count; ++i)
                if (users[i].connected) n++;
            send_string(client_sock, snprintf(NULL,0,"%d",n)>=0? (char[]){0}:"0"); // placeholder
            // send each
            for (int i = 0; i < user_count; ++i) {
                if (!users[i].connected) continue;
                send_string(client_sock, users[i].username);
                send_string(client_sock, users[i].ip);
                char portbuf[16]; snprintf(portbuf,16,"%d",users[i].port);
                send_string(client_sock, portbuf);
            }
            pthread_mutex_unlock(&users_mutex);
            close(client_sock);
            return NULL;
        }
        free(user);
    }
    else if (strcmp(op, "LIST CONTENT") == 0) {
        char *user = recv_string(client_sock);
        char *remote = recv_string(client_sock);
        User *u = find_user(user);
        User *r = find_user(remote);
        if (!u) code = 1;
        else if (!u->connected) code = 2;
        else if (!r) code = 3;
        else {
            code = 0;
            send(client_sock, (char[]){0}, 1, 0);
            char buf[16]; snprintf(buf,16,"%d", r->content_count);
            send_string(client_sock, buf);
            for (int i = 0; i < r->content_count; ++i)
                send_string(client_sock, r->content[i].path);
            pthread_mutex_unlock(&users_mutex);
            close(client_sock);
            return NULL;
        }
        free(user); free(remote);
    }
    // send code for operations without additional data
    send(client_sock, (char[]){code}, 1, 0);
    pthread_mutex_unlock(&users_mutex);
    close(client_sock);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        return 1;
    }
    int port = atoi(argv[1]);
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv;
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = htons(port);
    bind(sock, (struct sockaddr*)&serv, sizeof(serv));
    listen(sock, 10);
    printf("Servidor escuchando en puerto %d\n", port);

    while (1) {
        struct sockaddr_in cli_addr;
        socklen_t cli_len = sizeof(cli_addr);
        int client = accept(sock, (struct sockaddr*)&cli_addr, &cli_len);
        int *arg = malloc(sizeof(int) + sizeof(struct sockaddr_in));
        ((int*)arg)[0] = client;
        memcpy(((int*)arg) + 1, &cli_addr, sizeof(struct sockaddr_in));
        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, arg);
        pthread_detach(tid);
    }
    close(sock);
    return 0;
}
