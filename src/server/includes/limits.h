/*
 * Archivo con los valores válidos y valores límite para variables, además de la
 * declaración de las funciones de comprobación de esos valores. Estas funciones
 * devuelven 0 si todo bien, -1 si no cumplen con los límites.
 */

#ifndef LIMITS_H
#define LIMITS_H

#define MAX_IP 64
#define MAX_PORT 8
/*
 * Valor máximo y mínimo para el nombre de usuario y función que debe de
 * comprobarlo
 */
#define MAX_USERNAME 256
#define MIN_USERNAME 1

/*
 * @brief Comprueba si el nombre de usuario es válido basándose en los valres de
 * limits.h
 * @param nombre_usuario Cadena con el nombre de usuario a validar
 * @return 0 si el nombre es válido, -1 en caso contrario
 */
int checkNombreUsuario(char* nombre_usuario);

/*Valor máximo y mínimo para la operación y función que debe de comprobarlo*/
#define MAX_OP_LEN 256
#define MIN_OP_LEN 1
#define VAL_OP                                            \
    {"REGISTER", "UNREGISTER",   "CONNECT",    "PUBLISH", \
     "DELETE",   "LIST_CONTENT", "DISCONNECT", "LIST_USERS"}
/*
 * Comprueba si la operación es válida, tanto por longitud como por valor.
 * @param operacion Cadena con la operación a validar
 * @return 0 si la operación es válida, -1 en caso contrario
 */
int checkOperation(char* operacion);

/*
 * Valor máximo y mínimo para el nombre máximo del fichero y función que debe de
 * comprobarlo
 */
#define MAX_FILEPATH 256
#define MIN_FILEPATH 1

/*
 * Comprueba si el nombre del fichero tiene una longitud válida según lo puesto
 * en limits.h
 * @param nombre_fichero Cadena con el nombre del fichero a validar
 * @return 0 si la longitud es válida, -1 en caso contrario
 */
int checkFilenameLen(char* nombre_fichero);

/*
 * Valor máximo y mínimo para el timestamp y función que debe de comprobarlo
 */
#define MAX_TIME_LEN 256
#define MIN_TIME_LEN 1
/*
 * Comprueba si el timestamp tiene una longitud válida.
 * @param timestamp Cadena con el timestamp a validar
 * @return 0 si la longitud es válida, -1 en caso contrario
 */
int checkTimeLen(char* timestamp);

/*
 * Valor maximo y mínimo para la descripción de un fichero
 */
#define MAX_DESC 256
#define MIN_DESC 1

/*
 * Comprueba si la descripción tiene una longitud válida
 * @param desc: Cadena con la descripción
 * @return 0 si la longitud es válida, -1 en caso contrario
 */
int checkDesc(char* timestamp);

#endif /* LIMITS_H*/
