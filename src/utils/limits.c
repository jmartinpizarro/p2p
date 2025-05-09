#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "limits.h"

/**
 * Comprueba si el nombre de usuario es válido basándose en los valres de limits.h
 * @param nombre_usuario Cadena con el nombre de usuario a validar
 * @return 0 si el nombre es válido, -1 en caso contrario
 */
int checkNombreUsuario(char* nombre_usuario) {
    // Comprobamos si el puntero es nulo
    if (nombre_usuario == NULL) {
        return -1;
    }
    
    // Comprobamos la longitud del nombre de usuario
    size_t longitud = strlen(nombre_usuario);
    if (longitud < MIN_NOM_US || longitud > MAX_NOM_US) {
        return -1;
    }
    
    return 0;
}

/**
 * Comprueba si la operación es válida, tanto por longitud como por valor.
 * @param operacion Cadena con la operación a validar
 * @return 0 si la operación es válida, -1 en caso contrario
 */
int checkOperation(char* operacion) {
    // Comprobamos si el puntero es nulo
    if (operacion == NULL) {
        return -1;
    }
    
    // Comprobamos la longitud de la operación
    size_t longitud = strlen(operacion);
    if (longitud < MIN_OP_LEN || longitud > MAX_OP_LEN) {
        return -1;
    }
    
    // Comprobamos si la operación es una de las válidas
    const char* operaciones_validas[] = VAL_OP;
    int num_operaciones = sizeof(operaciones_validas) / sizeof(operaciones_validas[0]);
    
    for (int i = 0; i < num_operaciones; i++) {
        if (strcmp(operacion, operaciones_validas[i]) == 0) {
            return 0; 
        }
    }
    
    return -1;
}

/**
 * Comprueba si el nombre del fichero tiene una longitud válida según lo puesto en limits.h
 * @param nombre_fichero Cadena con el nombre del fichero a validar
 * @return 0 si la longitud es válida, -1 en caso contrario
 */
int checkFilenameLen(char* nombre_fichero) {
    // Comprobamos si el puntero es nulo
    if (nombre_fichero == NULL) {
        return -1;
    }
    
    // Comprobamos la longitud del nombre del fichero
    size_t longitud = strlen(nombre_fichero);
    if (longitud < MIN_FLE_LEN || longitud > MAX_FLE_LEN) {
        return -1;
    }
    
    return 0;
}

/**
 * Comprueba si el timestamp tiene una longitud válida.
 * @param timestamp Cadena con el timestamp a validar
 * @return 0 si la longitud es válida, -1 en caso contrario
 */
int checkTimeLen(char* timestamp) {
    // Comprobamos si el puntero es nulo
    if (timestamp == NULL) {
        return -1;
    }
    
    // Comprobamos la longitud del timestamp
    size_t longitud = strlen(timestamp);
    if (longitud < MIN_TIME_LEN || longitud > MAX_TIME_LEN) {
        return -1;
    }
    
    // Si pasamos todas las comprobaciones, la longitud es válida
    return 0;
}
