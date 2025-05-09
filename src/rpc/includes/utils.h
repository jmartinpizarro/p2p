/*
 Conjunto de funciones útiles durante la fase de desarrollo de código
 */
#ifndef UTILS_H
#define UTILS_H
#include <stdarg.h>
#include <stdio.h>

// Constatnes para debug:
extern int DEBUG;
extern int ERROR;
extern int TEST;
extern int INFO;

// Colores para los mensajes de test
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"
#define BLUE "\x1b[34m"
#define ORANGE "\x1b[38;5;208m"

/*
 * Funcion para printear un numero int number de saltos de linea
 */
void printn(int number);

/*
 * Función para printear mensajes sin formato condicionado por la variable global DEBUG
 */
void print(const char *format, ...);

/*
Función para debug
* Un printf condicionado por a variale global DEBUG
*/
void printd(const char* name, const char *format, ...);

/*
Función para debug
* Un printf condicionado por a variale global DEBUG
*/
void printi(const char *name, const char *format, ...);
/*
Función para debug de errores
* Un perror condicionado por a variale global DEBUG,
 */
void printe(const char *name, const char *format);

/*
 *Función para impresión de resultados de un tests. Un printf condicionado por a
 *variale global TEST
 * @param int result: resultado del test (0: superado, -1: no superado)
 * @param const char *name: nombre del test
 * @param const char *format: mensaje a imprimir
 */
void printt(int result, int expected, char *name);


#endif  // UTILS_H
