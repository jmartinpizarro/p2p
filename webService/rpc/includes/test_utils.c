/*
 * Archivo de utilidades para todos los tests
 */

#ifndef test_utils
#define test_utils
#include <stdio.h>
#include <string.h>

// Conjunto de funciones de testeo
typedef int (*test_func)(void);


/*
 * Funcion para llamar a todas las funciones de test y devolver un resultado
 * global
 * @param all_tests: array de funciones de test
 * @return 0 si todos los tests pasan, -1 si alguno falla
 */
int run_tests(test_func* all_tests) {
    int final = 0;
    for (test_func* test = all_tests; *test != NULL; test++) {
        int result = (*test)();
        if (result == -1) {
            final = -1;
        }
    }
    return final;
}

/*Creación de una struct con los valores estandar
 por defecto para los tests
*/
struct testsValues {
    char nombre_usuario[256];
    char operacion[256];
    char nombre_fichero[256];
    char timestamp[256];
    int expected;
} testValuesDefault = {"myUser", "CONNECT", "miFichero", "22/02/12 12:34:23",0};
typedef struct testsValues testsValues;


#endif  // test_utils
