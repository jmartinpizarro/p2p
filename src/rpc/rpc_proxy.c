/*
 * Proxy del cliente del servicio TIMESTAMP a través de RPC
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "includes/timestamp.h"

// Utilidades de appyo
#include "includes/utils.h"
#include "includes/limits.h"


// RPC
#include "includes/rpc.h"

int imprimirOperacion(char *nombre_usuario, char *operacion, char *nombre_fichero, char *timestamp)
{
    // Comprobar que los parámetros sean válidos:

    if (checkNombreUsuario(nombre_usuario) != 0) {
        printe("RPC ERROR",
               "Valor inválido para el nombre de usuario, fuera de límites");
        return 2;
    };

    if (checkOperation(operacion) != 0) {
        printe("RPC ERROR",
               "Valor invalido para operación, fuera de límites o formato "
               "incorrecto");
        return 1;
    };

    if (checkTimeLen(timestamp) != 0) {
        printe("RPC ERROR",
               "Valor inválido para el timestamp, fuera de límites");
        return 2;
    };

    // Si la operación es PUBLISH o DELETE comprobar el fichero e imprimir
    if (strcmp("PUBLISH", operacion) == 0 || strcmp("DELETE", operacion) == 0) {
        if (checkFilenameLen(nombre_fichero) != 0) {
            printe(
                "RPC ERROR",
                "Valor inválido para el nombre del fichero, fuera de límites");
            return 2;
        };
    }

    int result;
    CLIENT *clnt;
    enum clnt_stat retval;

    char *host = getenv("IP_TUPLAS");
    if (NULL == host) {
        printf("ERROR: please use IP_TUPLAS=server-host-name first\n");
        exit(-1);
    }

    // Inicializar sesion RPC
    clnt = clnt_create(host, TIMESTAMP_PROG, TIMESTAMP_VERS, "tcp");
    if (clnt == NULL) {
        clnt_pcreateerror(host);
        exit(-1);
    }

    // Llamada a la función
    printi("RPC PROXY", "INICIANDO imprimirOperacion");
    retval = imprimir_operacion_1(nombre_usuario,operacion,nombre_fichero,timestamp,&result, clnt);
    // Comprobamos el valor de la función del RPC
    if (retval != RPC_SUCCESS) {
        clnt_perror(clnt, "destroy_1: ");
        exit(-1);
    }
    clnt_destroy(clnt);
    
    if (result != 0) {
        printe("RPC PROXY", "FINALIZADA CON ERROES imprimirOperacion");
        return result;
    }
    printe("RPC PROXY", "FINALIZADA CORRECTAMENTE imprimirOperacion");
    return result;
}


