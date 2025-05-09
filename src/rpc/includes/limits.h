/*
* Archivo con los valores válidos y valores límite para variables, además de la declaración de las funciones de comprobación de esos valores. Estas funciones devuelven 0 si todo bien, -1 si no cumplen con los límites.
*/

#ifndef LIMITS_H
#define LIMITS_H

/*Valor máximo y mínimo para el nombre de usuario y función que debe de comprobarlo*/
#define MAX_NOM_US 256
#define MIN_NOM_US 1
int checkNombreUsuario(char* nombre_usuario);

/*Valor máximo y mínimo para la operación y función que debe de comprobarlo*/
#define MAX_OP_LEN 256
#define MIN_OP_LEN 1
#define VAL_OP {"REGISTER", "UNREGISTER", "CONNECT", "PUBLISH","DELETE","LIST_CONTENT","DISCONNECT"}
int checkOperation(char* operacion);

/*Valor máximo y mínimo para el nombre máximo del fichero y función que debe de comprobarlo*/
#define MAX_FLE_LEN 256
#define MIN_FLE_LEN 1
int checkFilenameLen(char* nombre_fichero);

/*Valor máximo y mínimo para el timestamp y función que debe de comprobarlo*/
#define MAX_TIME_LEN 256
#define MIN_TIME_LEN 1
int checkTimeLen(char* timestamp);

#endif/* LIMITS_H*/
