// @brief services.c : Implementación de las interfaces de services.h para su
// uso en el servidor.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "services.h"
#include "limits.h"
#include "server_utils.h"
#include "utils.h"

static User users[MAX_USERS];
static int user_count = 0;

User* find_user(const char* name) {
    for (int i = 0; i < user_count; ++i) {
        if (strcmp(users[i].username, name) == 0) return &users[i];
    }
    return NULL;
}

int register_user(char* user_name) {
    int code;
    if (find_user(user_name))
        code = 1;
    else if (user_count >= MAX_USERS)
        code = 2;
    else {
        User* u = &users[user_count++];
        strncpy(u->username, user_name, MAX_USERNAME);
        u->connected = 0;
        u->content_count = 0;
        code = 0;
    }
    return code;
}

int unregister_user(char* user) {
    int code;
    User* u = find_user(user);
    if (!u)
        code = 1;
    else {
        // remove user
        int idx = u - users;
        memmove(&users[idx], &users[idx + 1],
                (user_count - idx - 1) * sizeof(User));
        user_count--;
        code = 0;
    }
    return code;
}

int connect_user(char* user, char* port_s, struct sockaddr_in addr) {
    int code;

    User* u = find_user(user);
    if (!u)
        code = 1;
    else if (u->connected)
        code = 2;
    else {
        strncpy(u->ip, inet_ntoa(addr.sin_addr), INET_ADDRSTRLEN);
        u->port = atoi(port_s);
        u->connected = 1;
        code = 0;
    }
    return code;
}

int disconnect_user(char* user) {
    int code;
    User* u = find_user(user);
    if (!u)
        code = 1;
    else if (!u->connected)
        code = 2;
    else {
        u->connected = 0;
        code = 0;
    }
    return code;
}

int publish(char* user, char* path, char* desc) {
    int code;
    User* u = find_user(user);
    if (!u)
        code = 1;
    else if (!u->connected)
        code = 2;
    else {
        // check duplicate
        int dup = 0;
        for (int i = 0; i < u->content_count; ++i)
            if (strcmp(u->content[i].path, path) == 0) dup = 1;
        if (dup)
            code = 3;
        else if (u->content_count >= MAX_CONTENT)
            code = 4;
        else {
            strncpy(u->content[u->content_count].path, path, MAX_FILEPATH);
            strncpy(u->content[u->content_count].desc, desc, MAX_DESC);
            u->content_count++;
            code = 0;
        }
    }
    return code;
}

int delete_s(char* user, char* path) {
    int code;

    User* u = find_user(user);
    if (!u)
        code = 1;
    else if (!u->connected)
        code = 2;
    else {
        int found = -1;
        for (int i = 0; i < u->content_count; ++i)
            if (strcmp(u->content[i].path, path) == 0) {
                found = i;
                break;
            }
        if (found < 0)
            code = 3;
        else {
            memmove(&u->content[found], &u->content[found + 1],
                    (u->content_count - found - 1) * sizeof(Content));
            u->content_count--;
            code = 0;
        }
    }
    return code;
}

int list_users(char* user, int client_sock) {
    int code;

    User* u = find_user(user);
    if (!u)
        code = 1;
    else if (!u->connected)
        code = 2;
    else {
        code = 0;
        // Send the code
        send(client_sock, (char[]){0}, 1, 0);
        
        // count connected
        int n = 0;
        for (int i = 0; i < user_count; ++i) {
            if (users[i].connected) {
                n++;
            }
        }

        printd("SERVICES", "Connected users: %d", n);
        char connectedbuf[MAX_USERS];
        snprintf(connectedbuf,MAX_USERS,"%d", n);
        send_string(client_sock, connectedbuf);

        printd("SERVICES", "Enviando datos de connected_users");

        // send each
        for (int i = 0; i < user_count; ++i) {
            // only send connected users
            if (!users[i].connected) continue;
            printd("SERVICES", "Username: %s", users[i].username);
            char namebuf[MAX_USERNAME];
            snprintf(namebuf, MAX_USERNAME, "%s", users[i].username);
            send_string(client_sock, namebuf);
            printd("SERVICES", "IP: %s", users[i].ip);
            char ipbuf[MAX_IP];
            snprintf(ipbuf, MAX_IP, "%s", users[i].ip);
            send_string(client_sock, ipbuf);
            printd("SERVICES", "PORT: %s", users[i].ip);
            char portbuf[MAX_PORT];
            snprintf(portbuf, MAX_PORT, "%d", users[i].port);
            send_string(client_sock, portbuf);
        }
        // pthread_mutex_unlock(&users_mutex);
    }
    return code;
}

int list_content(char* user, char* remote, int client_sock) {
    int code;

    User* u = find_user(user);
    User* r = find_user(remote);
    if (!u)
        code = 1;
    else if (!u->connected)
        code = 2;
    else if (!r)
        code = 3;
    else {
        code = 0;
        send(client_sock, (char[]){0}, 1, 0);
        char buf[16];
        snprintf(buf, 16, "%d", r->content_count);
        send_string(client_sock, buf);
        for (int i = 0; i < r->content_count; ++i)
            send_string(client_sock, r->content[i].path);
        // pthread_mutex_unlock(&users_mutex);
    }
    return code;
}
