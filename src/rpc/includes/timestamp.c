/*
 *   Implementación por parte del servidor del servicio timestamp definido en
 * timestamp.h a través de la función imprimirOperacion.
 */

#include <string.h>
#include "timestamp.h"

// Utiliades de apoyo
#include "limits.h"
#include "utils.h"

int imprimirOperacion(char *nombre_usuario, char *operacion,
                      char *nombre_fichero, char *timestamp) {

    // Comprobar que los parámetros son válidos
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
        printf("%s %s %s %s", nombre_usuario, operacion,nombre_fichero, timestamp);
        return 0;
    }

    // Imprimir por pantalla la linea completa con los datos para operaciones
    // sin nombre de fichero
    printf("%s %s %s", nombre_usuario, operacion, timestamp);
    return 0;
}
