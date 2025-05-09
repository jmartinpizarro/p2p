/*
 Conjunto de funciones útiles durante la fase de desarrollo de código
 */
#ifndef utils
#define utils
#include <stdarg.h>
#include <stdio.h>
// Constatnes para debug:
int DEBUG = 1;
int ERROR = 1;
int TEST = 1;
int INFO = 1;
// Colores para los mensajes de test
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"
#define BLUE "\x1b[34m"
#define ORANGE "\x1b[38;5;208m"

/*
 * Funcion para printear un numero int number de saltos de linea
 */
void printn(int number) {
    for (int i = 0; i < number; i++) {
        printf("\n");
    }
}

/*
 * Función para printear mensajes sin formato condicionado por la variable global DEBUG
 */
void print(const char *format, ...) {
    if (DEBUG) {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        printf("\n");
        va_end(args);
    }
}

#ifndef printd 
/*
Función para debug
* Un printf condicionado por a variale global DEBUG
*/
void printd(const char* name, const char *format, ...) {
    // Only proceed if DEBUG is true
    if (DEBUG) {
        va_list args;
        va_start(args, format);
        printf(ORANGE);
        printf("=== DEBUG %s === ",name);
        vprintf(format, args);
        printf("\n");
        printf(RESET);
        va_end(args);
    }
}
#else 
void printd(const char* name, const char *format, ...);
#endif

/*
Función para debug
* Un printf condicionado por a variale global DEBUG
*/
void printi(const char *name, const char *format, ...) {
    // Only proceed if DEBUG is true
    if (INFO) {
        va_list args;
        va_start(args, format);
        printf(BLUE);
        printf("=== INFO: %s === ", name);
        vprintf(format, args);
        printf("\n");
        printf(RESET);
        va_end(args);
    }
}
/*
Función para debug de errores
* Un perror condicionado por a variale global DEBUG,
 */
void printe(const char *name, const char *format) {
    // Only proceed if DEBUG is true
    if (ERROR) {
        printf(RED);
        printf("=== ERROR: %s === ", name);
        perror(format);
        printf("\n");
        printf(RESET);
        fflush(stdout);
    }
}

/*
 *Función para impresión de resultados de un tests. Un printf condicionado por a
 *variale global TEST
 * @param int result: resultado del test (0: superado, -1: no superado)
 * @param const char *name: nombre del test
 * @param const char *format: mensaje a imprimir
 */
void printt(int result, int expected, char *name) {
    // Only proceed if DEBUG is true
    if (TEST) {
        if (result == expected) {
            printf(GREEN);
            printf("=== TEST %s === SUPERADO", name);
        } else {
            printf(RED);
            printf("=== TESTS %s === NO SUPERADO", name);
        }
        printf("\n");
        printf(RESET);
    }
}
#endif  // utils
