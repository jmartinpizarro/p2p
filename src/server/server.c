// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "includes/server_utils.h"
#include "includes/services.h"
#include "includes/utils.h"
#include "includes/limits.h"
#include "includes/timestamp.h"

// Server socket
int sock;

// operations are processed here
void *handle_client(void *arg) {
    int client_sock = ((int *)arg)[0];
    struct sockaddr_in addr = *(struct sockaddr_in *)(((int *)arg) + 1);
    static pthread_mutex_t users_mutex = PTHREAD_MUTEX_INITIALIZER;
    free(arg);

    pthread_mutex_lock(&users_mutex);
    // Se recibe la operación para poder realizar uno u otro servicio según esta
    char *op = recv_string(client_sock);
    if (!op) {
        close(client_sock);
        return NULL;
    }
    if (checkOperation(op) == -1) {
        printe("SERVER", "operacion fuera de límites");
        printd("SERVER", "operacion: %s", op);
        close(client_sock);
        return NULL;
    }
    //char *timestamp = "test/tets/test test:test:test";
     char *timestamp= recv_string(client_sock);
     if (checkTimeLen(timestamp)) {
         printe("SERVER","datetime fuera de límites");
         return NULL;
     }

    unsigned char code = 255;

    char *user = recv_string(client_sock);
    if (checkNombreUsuario(user)) {
        printe("SERVER", "Nombre de usuario fuera de límites");
        return NULL;
    }

    // DEBUG AND PRINTING
    printf("OPERATION %s FROM %s\n", op, user);

    if (strcmp(op, "REGISTER") == 0) {
        printd("SERVER", "REGISTER Started:\nUSER: %s", user);
        code = (char)register_user(user);
        send(client_sock, (char[]){code}, 1, 0);
        imprimirOperacion(user, op, "", timestamp);
        free(user);
    } else if (strcmp(op, "UNREGISTER") == 0) {
        printd("SERVER", "UNREGISTER Started:\nUSER: %s", user);
        code = (char)unregister_user(user);
        imprimirOperacion(user, op, "", timestamp);
        send(client_sock, (char[]){code}, 1, 0);
        free(user);
    } else if (strcmp(op, "CONNECT") == 0) {
        char *port_s = recv_string(client_sock);
        printd("SERVER", "CONNECT Started:\nUSER: %s\nPORT: %s", user, port_s);
        code = (char)connect_user(user, port_s, addr);
        imprimirOperacion(user, op, "", timestamp);
        send(client_sock, (char[]){code}, 1, 0);
        free(user);
        free(port_s);
    } else if (strcmp(op, "DISCONNECT") == 0) {
        printd("SERVER", "DISCONNECT Started:\nUSER: %s", user);
        code = (char)disconnect_user(user);
        send(client_sock, (char[]){code}, 1, 0);
        imprimirOperacion(user, op, "", timestamp);
        free(user);
    } else if (strcmp(op, "PUBLISH") == 0) {
        char *path = recv_string(client_sock);
        if (checkFilenameLen(path)) {
            printe("SERVER", "Nombre de archivo fuera de límites");
        }
        char *desc = recv_string(client_sock);
        if (checkDesc(desc)) {
            printe("SERVER", "Descripcion fuera de límites");
        }
        printd("SERVER", "PUBLISH Started:\nUSER: %s\nPATH: %s\nDESC: %s", user,
               path, desc);
        code = (char)publish(user, path, desc);
        send(client_sock, (char[]){code}, 1, 0);
        imprimirOperacion(user, op, path, timestamp);
        free(user);
        free(path);
        free(desc);
    } else if (strcmp(op, "DELETE") == 0) {
        char *path = recv_string(client_sock);
        if (checkFilenameLen(path)) {
            printe("SERVER", "Nombre de archivo fuera de límites");
        }
        printd("SERVER", "DELETE Started:\nUSER: %s\nPATH: %s", user, path);
        code = (char)delete_s(user, path);
        send(client_sock, (char[]){code}, 1, 0);
        imprimirOperacion(user, op, path, timestamp);
        free(user);
        free(path);
    } else if (strcmp(op, "LIST_USERS") == 0) {
        printd("SERVER", "LIST_USERS Started:\nUSER: %s", user);
        code = (char)list_users(user, client_sock);
        imprimirOperacion(user, op, "", timestamp);
        free(user);
    } else if (strcmp(op, "LIST_CONTENT") == 0) {
        char *remote = recv_string(client_sock);
        if (checkNombreUsuario(remote)) {
            printe("SERVER", "Nombre de usuario remoto fuera de límites");
            return NULL;
        }
        printd("SERVER", "LIST_CONTENT Started:\nUSER: %s\nREMOTE: %s", user);
        code = (char)list_content(user, remote, client_sock);
        imprimirOperacion(user, op, "", timestamp);
        free(user);
        free(remote);
    }
    pthread_mutex_unlock(&users_mutex);
    close(client_sock);
    return NULL;
}

void controlC(int sig) {
    close(sock);
    printf("\nSERVIDOR CERRADO DE FORMA SEGURA: CTRL+C\n");
    exit(0);
}

int main(int argc, char *argv[]) {
    signal(SIGINT, controlC);
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        return 1;
    }
    int port = atoi(argv[1]);
    sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv;
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = htons(port);

    // Bind on sock
    if (bind(sock, (struct sockaddr *)&serv, sizeof(serv))) {
        printe("SERVER", "Bind error");
        return 1;
    }

    // Listen on sock
    listen(sock, 10);
    printf("Servidor escuchando en puerto %d\n", port);

    // Start to handle petitions
    while (1) {
        struct sockaddr_in cli_addr;
        socklen_t cli_len = sizeof(cli_addr);
        int client = accept(sock, (struct sockaddr *)&cli_addr, &cli_len);
        printi("SERVER", "Peticion en socket aceptada");
        int *arg = malloc(sizeof(int) + sizeof(struct sockaddr_in));
        ((int *)arg)[0] = client;
        memcpy(((int *)arg) + 1, &cli_addr, sizeof(struct sockaddr_in));
        pthread_t tid;
        // Creación de una nueva thread para manejar la petición
        printi("SERVER", "Lanzando thread");
        pthread_create(&tid, NULL, handle_client, arg);
        pthread_detach(tid);
    }
    close(sock);
    return 0;
}
