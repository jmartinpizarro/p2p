/*
 * Este archivo contiene los test creados para las interfaces definidas en
 * timestamp.h y descritas en timestamp.c, estas funciones serán las que más
 * tarde utilice el servidor del servicio rpc
 */

// Utilidades para test
#include "../includes/test_utils.c"
#include "../includes/utils.h"
#include "../includes/timestamp.h"

/*
 * Test the default behaviour with each of the possible operations
 */
int test_default() {
    int result;

    // Con "CONNECT"
    testsValues valuesConnect = testValuesDefault;
    printd("TEST RPC SERVER",
           "Empezando test_default con operación CONNECT y valores:\n nombre_usuario: %s\n operacion: %s\n "
           "nombre_fichero: %s\n timestamp: %s\n",
           valuesConnect.nombre_usuario, valuesConnect.operacion,
           valuesConnect.nombre_fichero, valuesConnect.timestamp);
    result = imprimirOperacion(
        valuesConnect.nombre_usuario, valuesConnect.operacion,
        valuesConnect.nombre_fichero, valuesConnect.timestamp);
    if (result != valuesConnect.expected) {
        printe("TEST RPC SERVER", "test_default en connect no aprobado");
        return -1;
    }

    // Con "DISCONNECT"
    testsValues valuesDisconnect = testValuesDefault;
    strcpy(valuesDisconnect.operacion, "DISCONNECT");
    result = imprimirOperacion(
        valuesDisconnect.nombre_usuario, valuesDisconnect.operacion,
        valuesDisconnect.nombre_fichero, valuesDisconnect.timestamp);
    if (result != valuesDisconnect.expected) {
        printe("TEST RPC SERVER", "test_default en disconnect no aprobado");
        return -1;
    }

    // Con "REGISTER"
    testsValues valuesRegister = testValuesDefault;
    strcpy(valuesRegister.operacion, "REGISTER");
    result = imprimirOperacion(
        valuesRegister.nombre_usuario, valuesRegister.operacion,
        valuesRegister.nombre_fichero, valuesRegister.timestamp);
    if (result != valuesRegister.expected) {
        printe("TEST RPC SERVER", "test_default en register no aprobado");
        return -1;
    }

    // Con "UNREGISTER"
    testsValues valuesUnregister = testValuesDefault;
    strcpy(valuesUnregister.operacion, "UNREGISTER");
    result = imprimirOperacion(
        valuesUnregister.nombre_usuario, valuesUnregister.operacion,
        valuesUnregister.nombre_fichero, valuesUnregister.timestamp);
    if (result != valuesUnregister.expected) {
        printe("TEST RPC SERVER", "test_default en unregister no aprobado");
        return -1;
    }

    // Con "PUBLISH"
    testsValues valuesPublish = testValuesDefault;
    strcpy(valuesPublish.operacion, "PUBLISH");
    result = imprimirOperacion(
        valuesPublish.nombre_usuario, valuesPublish.operacion,
        valuesPublish.nombre_fichero, valuesPublish.timestamp);
    if (result != valuesPublish.expected) {
        printe("TEST RPC SERVER", "test_default en publish no aprobado");
        return -1;
    }

    // Con "DELETE"
    testsValues valuesDelete = testValuesDefault;
    strcpy(valuesDelete.operacion, "DELETE");
    result =
        imprimirOperacion(valuesDelete.nombre_usuario, valuesDelete.operacion,
                          valuesDelete.nombre_fichero, valuesDelete.timestamp);
    if (result != valuesDelete.expected) {
        printe("TEST RPC SERVER", "test_default en delete no aprobado");
        return -1;
    }

    // Con "LIST_CONTENT"
    testsValues valuesList = testValuesDefault;
    strcpy(valuesList.operacion, "LIST_CONTENT");
    result = imprimirOperacion(valuesList.nombre_usuario, valuesList.operacion,
                               valuesList.nombre_fichero, valuesList.timestamp);
    if (result != valuesList.expected) {
        printe("TEST RPC SERVER", "test_default en list_content no aprobado");
        return -1;
    }
    return 0;
}

int test_operacion_incorrecta(void){
    int result;
    testsValues testValues = testValuesDefault;
    strcpy(testValues.operacion,"BAD_OP");
    testValues.expected = 1;
    result = imprimirOperacion(testValues.nombre_usuario, testValues.operacion, testValues.nombre_fichero,testValues.timestamp);

    if (result != testValues.expected) {
        printe("TEST RPC SERVER", "test_operacion_incorrecta no aprobado");
        return -1;
    }
    return 0;
}

test_func rpcServer_tests[] = {test_default,test_operacion_incorrecta, NULL};

/*
 * Función main se encarga de ejecutar todos los tests y comprobar que todos den
 * resultado positivo
 */
int tests_rpcServer() {
    int result = run_tests(rpcServer_tests);
    printn(1);
    printt(result, 0, "RPC SERVER");
    printn(2);
    return result;
}

int main() {
    int result;
    result = tests_rpcServer();
    return result;
}
