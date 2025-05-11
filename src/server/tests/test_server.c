/*
 * Este archivo contiene los test creados para las interfaces definidas en
 * services.h y descritas en services.c, encargadas de gestionar
 * la array de usuarios del servidor
 */

#include <arpa/inet.h>

// Utilidades para test
#include "../includes/test_utils.c"
#include "../includes/utils.h"
#include "../includes/services.h"

// Variables globales para tests
static struct sockaddr_in test_addr;

// Función para inicializar entorno de pruebas
void setup_test_environment() {
    // Inicializar la dirección para pruebas de conexión
    test_addr.sin_family = AF_INET;
    test_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    test_addr.sin_port = htons(8080);

    // Crear estructura de valores de prueba por defecto
    strcpy(testValuesDefault.nombre_usuario, "testUser");
    strcpy(testValuesDefault.operacion, "CONNECT");
    strcpy(testValuesDefault.nombre_fichero, "/tmp/testfile.txt");
    strcpy(testValuesDefault.timestamp, "22/05/11 15:30:42");
    testValuesDefault.expected = 0;
}

/*
 * Test para register_user en caso normal
 */
int test_register_user_normal() {
    printi("TEST SERVICES", "Empezando test_register_user_normal");

    // Registrar un usuario nuevo
    int result = register_user("newUser");

    if (result != 0) {
        printd("TEST SERVICES",
               "test_register_user_normal no aprobado - Debería devolver 0 "
               "pero devolvió %d",
               result);
        return -1;
    }

    // Verificar que el usuario existe usando find_user
    User* user = find_user("newUser");
    if (user == NULL) {
        printe("TEST SERVICES",
               "test_register_user_normal no aprobado - Usuario no encontrado "
               "después de registro");
        return -1;
    }

    return 0;
}

/*
 * Test para register_user con usuario duplicado (debe fallar)
 */
int test_register_user_duplicado() {
    printi("TEST SERVICES", "Empezando test_register_user_duplicado");

    // Registrar un usuario
    register_user("duplicateUser");

    // Intentar registrar el mismo usuario de nuevo
    int result = register_user("duplicateUser");

    if (result != 1) {
        printd("TEST SERVICES",
               "test_register_user_duplicado no aprobado - Debería devolver 1 "
               "pero devolvió %d",
               result);
        return -1;
    }

    return 0;
}

/*
 * Test para register_user cuando se alcanza el límite máximo de usuarios
 */
int test_register_user_max_users() {
    printi("TEST SERVICES", "Empezando test_register_user_max_users");

    // Primero limpiar registrando usuarios hasta el límite
    char username[MAX_USERNAME];
    // Registrar MAX_USERS usuarios
    for (int i = 0; i < MAX_USERS; i++) {
        sprintf(username, "maxUser%d", i);
        register_user(username);
    }

    // Intentar registrar un usuario adicional
    int result = register_user("oneMoreUser");

    if (result != 2) {
        printd("TEST SERVICES",
               "test_register_user_max_users no aprobado - Debería devolver 2 "
               "pero devolvió %d",
               result);
        return -1;
    }

    // Limpiar desregistrando a todos los usuarios
    for (int i = 0; i < MAX_USERS; i++) {
        sprintf(username, "maxUser%d", i);
        unregister_user(username);
    }

    return 0;
}

/*
 * Test para unregister_user en caso normal
 */
int test_unregister_user_normal() {
    printi("TEST SERVICES", "Empezando test_unregister_user_normal");

    // Registrar un usuario nuevo
    register_user("userToUnregister");

    // Desregistrar al usuario
    int result = unregister_user("userToUnregister");

    if (result != 0) {
        printd("TEST SERVICES",
               "test_unregister_user_normal no aprobado - Debería devolver 0 "
               "pero devolvió %d",
               result);
        return -1;
    }

    // Verificar que el usuario ya no existe
    User* user = find_user("userToUnregister");
    if (user != NULL) {
        printd("TEST SERVICES",
               "test_unregister_user_normal no aprobado - Usuario todavía "
               "existe después de desregistro");
        return -1;
    }

    return 0;
}

/*
 * Test para unregister_user con un usuario que no existe
 */
int test_unregister_user_nonexistent() {
    printi("TEST SERVICES", "Empezando test_unregister_user_nonexistent");

    // Intentar desregistrar un usuario inexistente
    int result = unregister_user("nonExistentUser");

    if (result != 1) {
        printd("TEST SERVICES",
               "test_unregister_user_nonexistent no aprobado - Debería "
               "devolver 1 pero devolvió %d",
               result);
        return -1;
    }

    return 0;
}

/*
 * Test para connect_user en caso normal
 */
int test_connect_user_normal() {
    printi("TEST SERVICES", "Empezando test_connect_user_normal");

    // Registrar un usuario nuevo
    register_user("userToConnect");

    // Conectar al usuario
    int result = connect_user("userToConnect", "8080", test_addr);

    if (result != 0) {
        printd("TEST SERVICES",
               "test_connect_user_normal no aprobado - Debería devolver 0 pero "
               "devolvió %d",
               result);
        return -1;
    }

    // Verificar que el usuario está conectado
    User* user = find_user("userToConnect");
    if (user == NULL || user->connected != 1) {
        printd("TEST SERVICES",
               "test_connect_user_normal no aprobado - Usuario no está "
               "conectado después de connect_user");
        return -1;
    }

    // Limpiar
    unregister_user("userToConnect");

    return 0;
}

/*
 * Test para connect_user con un usuario que no existe
 */
int test_connect_user_nonexistent() {
    printi("TEST SERVICES", "Empezando test_connect_user_nonexistent");

    // Intentar conectar un usuario inexistente
    int result = connect_user("nonExistentUser", "8080", test_addr);

    if (result != 1) {
        printd("TEST SERVICES",
               "test_connect_user_nonexistent no aprobado - Debería devolver 1 "
               "pero devolvió %d",
               result);
        return -1;
    }

    return 0;
}

/*
 * Test para connect_user con un usuario ya conectado
 */
int test_connect_user_already_connected() {
    printi("TEST SERVICES", "Empezando test_connect_user_already_connected");

    // Registrar un usuario nuevo
    register_user("alreadyConnected");

    // Conectar al usuario
    connect_user("alreadyConnected", "8080", test_addr);

    // Intentar conectar de nuevo al mismo usuario
    int result = connect_user("alreadyConnected", "8080", test_addr);

    if (result != 2) {
        printd("TEST SERVICES",
               "test_connect_user_already_connected no aprobado - Debería "
               "devolver 2 pero devolvió %d",
               result);
        return -1;
    }

    // Limpiar
    unregister_user("alreadyConnected");

    return 0;
}

/*
 * Test para disconnect_user en caso normal
 */
int test_disconnect_user_normal() {
    printi("TEST SERVICES", "Empezando test_disconnect_user_normal");

    // Registrar y conectar un usuario nuevo
    register_user("userToDisconnect");
    connect_user("userToDisconnect", "8080", test_addr);

    // Desconectar al usuario
    int result = disconnect_user("userToDisconnect");

    if (result != 0) {
        printd("TEST SERVICES",
               "test_disconnect_user_normal no aprobado - Debería devolver 0 "
               "pero devolvió %d",
               result);
        return -1;
    }

    // Verificar que el usuario está desconectado
    User* user = find_user("userToDisconnect");
    if (user == NULL || user->connected != 0) {
        printd("TEST SERVICES",
               "test_disconnect_user_normal no aprobado - Usuario todavía está "
               "conectado después de disconnect_user");
        return -1;
    }

    // Limpiar
    unregister_user("userToDisconnect");

    return 0;
}

/*
 * Test para disconnect_user con un usuario que no existe
 */
int test_disconnect_user_nonexistent() {
    printi("TEST SERVICES", "Empezando test_disconnect_user_nonexistent");

    // Intentar desconectar un usuario inexistente
    int result = disconnect_user("nonExistentUser");

    if (result != 1) {
        printd("TEST SERVICES",
               "test_disconnect_user_nonexistent no aprobado - Debería "
               "devolver 1 pero devolvió %d",
               result);
        return -1;
    }

    return 0;
}

/*
 * Test para disconnect_user con un usuario no conectado
 */
int test_disconnect_user_not_connected() {
    printi("TEST SERVICES", "Empezando test_disconnect_user_not_connected");

    // Registrar un usuario nuevo (sin conectarlo)
    register_user("notConnectedUser");

    // Intentar desconectar al usuario
    int result = disconnect_user("notConnectedUser");

    if (result != 2) {
        printd("TEST SERVICES",
               "test_disconnect_user_not_connected no aprobado - Debería "
               "devolver 2 pero devolvió %d",
               result);
        return -1;
    }

    // Limpiar
    unregister_user("notConnectedUser");

    return 0;
}

/*
 * Test para publish en caso normal
 */
int test_publish_normal() {
    printi("TEST SERVICES", "Empezando test_publish_normal");

    // Registrar y conectar un usuario nuevo
    register_user("publishUser");
    connect_user("publishUser", "8080", test_addr);

    // Publicar contenido
    int result =
        publish("publishUser", "/tmp/testfile.txt", "Archivo de prueba");

    if (result != 0) {
        printd("TEST SERVICES",
               "test_publish_normal no aprobado - Debería devolver 0 pero "
               "devolvió %d",
               result);
        return -1;
    }

    // Verificar que el contenido existe
    User* user = find_user("publishUser");
    if (user == NULL || user->content_count != 1) {
        printe("TEST SERVICES",
               "test_publish_normal no aprobado - Contenido no encontrado "
               "después de publish");
        return -1;
    }

    // Limpiar
    unregister_user("publishUser");

    return 0;
}

/*
 * Test para publish con un usuario que no existe
 */
int test_publish_nonexistent_user() {
    printi("TEST SERVICES", "Empezando test_publish_nonexistent_user");

    // Intentar publicar con un usuario inexistente
    int result =
        publish("nonExistentUser", "/tmp/testfile.txt", "Archivo de prueba");

    if (result != 1) {
        printd("TEST SERVICES",
               "test_publish_nonexistent_user no aprobado - Debería devolver 1 "
               "pero devolvió %d",
               result);
        return -1;
    }

    return 0;
}

/*
 * Test para publish con un usuario no conectado
 */
int test_publish_user_not_connected() {
    printi("TEST SERVICES", "Empezando test_publish_user_not_connected");

    // Registrar un usuario nuevo (sin conectarlo)
    register_user("notConnectedPublisher");

    // Intentar publicar con el usuario no conectado
    int result = publish("notConnectedPublisher", "/tmp/testfile.txt",
                         "Archivo de prueba");

    if (result != 2) {
        printd("TEST SERVICES",
               "test_publish_user_not_connected no aprobado - Debería devolver "
               "2 pero devolvió %d",
               result);
        return -1;
    }

    // Limpiar
    unregister_user("notConnectedPublisher");

    return 0;
}

/*
 * Test para publish con un archivo duplicado
 */
int test_publish_duplicate_file() {
    printi("TEST SERVICES", "Empezando test_publish_duplicate_file");

    // Registrar y conectar un usuario nuevo
    register_user("duplicateFileUser");
    connect_user("duplicateFileUser", "8080", test_addr);

    // Publicar un archivo
    publish("duplicateFileUser", "/tmp/duplicate.txt", "Archivo de prueba");

    // Intentar publicar el mismo archivo de nuevo
    int result = publish("duplicateFileUser", "/tmp/duplicate.txt",
                         "Archivo de prueba otra vez");

    if (result != 3) {
        printd("TEST SERVICES",
               "test_publish_duplicate_file no aprobado - Debería devolver 3 "
               "pero devolvió %d",
               result);
        return -1;
    }

    // Limpiar
    unregister_user("duplicateFileUser");

    return 0;
}

/*
 * Test para publish cuando se alcanza el límite máximo de contenidos
 */
int test_publish_max_content() {
    printi("TEST SERVICES", "Empezando test_publish_max_content");

    // Registrar y conectar un usuario nuevo
    register_user("maxContentUser");
    connect_user("maxContentUser", "8080", test_addr);

    // Publicar MAX_CONTENT archivos
    char filepath[MAX_FILEPATH];
    for (int i = 0; i < MAX_CONTENT; i++) {
        sprintf(filepath, "/tmp/file%d.txt", i);
        publish("maxContentUser", filepath, "Archivo de prueba");
    }

    // Intentar publicar un archivo adicional
    int result =
        publish("maxContentUser", "/tmp/oneMoreFile.txt", "Archivo adicional");

    if (result != 4) {
        printd("TEST SERVICES",
               "test_publish_max_content no aprobado - Debería devolver 4 pero "
               "devolvió %d",
               result);
        return -1;
    }

    // Limpiar
    unregister_user("maxContentUser");

    return 0;
}

/*
 * Test para delete_s en caso normal
 */
int test_delete_normal() {
    printi("TEST SERVICES", "Empezando test_delete_normal");

    // Registrar y conectar un usuario nuevo
    register_user("deleteUser");
    connect_user("deleteUser", "8080", test_addr);

    // Publicar un archivo
    publish("deleteUser", "/tmp/fileToDelete.txt", "Archivo a eliminar");

    // Eliminar el archivo
    int result = delete_s("deleteUser", "/tmp/fileToDelete.txt");

    if (result != 0) {
        printd("TEST SERVICES",
               "test_delete_normal no aprobado - Debería devolver 0 pero "
               "devolvió %d",
               result);
        return -1;
    }

    // Verificar que el contenido ya no existe
    User* user = find_user("deleteUser");
    if (user == NULL || user->content_count != 0) {
        printe("TEST SERVICES",
               "test_delete_normal no aprobado - Contenido todavía existe "
               "después de delete_s");
        return -1;
    }

    // Limpiar
    unregister_user("deleteUser");

    return 0;
}

/*
 * Test para delete_s con un usuario que no existe
 */
int test_delete_nonexistent_user() {
    printi("TEST SERVICES", "Empezando test_delete_nonexistent_user");

    // Intentar eliminar con un usuario inexistente
    int result = delete_s("nonExistentUser", "/tmp/testfile.txt");

    if (result != 1) {
        printd("TEST SERVICES",
               "test_delete_nonexistent_user no aprobado - Debería devolver 1 "
               "pero devolvió %d",
               result);
        return -1;
    }

    return 0;
}

/*
 * Test para delete_s con un usuario no conectado
 */
int test_delete_user_not_connected() {
    printi("TEST SERVICES", "Empezando test_delete_user_not_connected");

    // Registrar un usuario nuevo (sin conectarlo)
    register_user("notConnectedDeleter");

    // Intentar eliminar con el usuario no conectado
    int result = delete_s("notConnectedDeleter", "/tmp/testfile.txt");

    if (result != 2) {
        printd("TEST SERVICES",
               "test_delete_user_not_connected no aprobado - Debería devolver "
               "2 pero devolvió %d",
               result);
        return -1;
    }

    // Limpiar
    unregister_user("notConnectedDeleter");

    return 0;
}

/*
 * Test para delete_s con un archivo que no existe
 */
int test_delete_nonexistent_file() {
    printi("TEST SERVICES", "Empezando test_delete_nonexistent_file");

    // Registrar y conectar un usuario nuevo
    register_user("nonexistentFileUser");
    connect_user("nonexistentFileUser", "8080", test_addr);

    // Intentar eliminar un archivo que no existe
    int result = delete_s("nonexistentFileUser", "/tmp/nonexistentFile.txt");

    if (result != 3) {
        printd("TEST SERVICES",
               "test_delete_nonexistent_file no aprobado - Debería devolver 3 "
               "pero devolvió %d",
               result);
        return -1;
    }

    // Limpiar
    unregister_user("nonexistentFileUser");

    return 0;
}

/*
 * Test para find_user (función auxiliar)
 */
int test_find_user() {
    printi("TEST SERVICES", "Empezando test_find_user");

    // Registrar un usuario nuevo
    register_user("userToFind");

    // Buscar usuario existente
    User* user = find_user("userToFind");
    if (user == NULL) {
        printe("TEST SERVICES",
               "test_find_user no aprobado - Usuario no encontrado cuando "
               "debería existir");
        return -1;
    }

    // Buscar usuario inexistente
    user = find_user("nonExistentUser");
    if (user != NULL) {
        printe("TEST SERVICES",
               "test_find_user no aprobado - Usuario encontrado cuando no "
               "debería existir");
        return -1;
    }

    // Limpiar
    unregister_user("userToFind");

    return 0;
}

// Lista de todas las funciones de prueba
test_func services_tests[] = {test_register_user_normal,
                              test_register_user_duplicado,
                              test_register_user_max_users,
                              test_unregister_user_normal,
                              test_unregister_user_nonexistent,
                              test_connect_user_normal,
                              test_connect_user_nonexistent,
                              test_connect_user_already_connected,
                              test_disconnect_user_normal,
                              test_disconnect_user_nonexistent,
                              test_disconnect_user_not_connected,
                              test_publish_normal,
                              test_publish_nonexistent_user,
                              test_publish_user_not_connected,
                              test_publish_duplicate_file,
                              test_publish_max_content,
                              test_delete_normal,
                              test_delete_nonexistent_user,
                              test_delete_user_not_connected,
                              test_delete_nonexistent_file,
                              test_find_user,
                              NULL};

/*
 * Función para ejecutar todos los tests y comprobar que todos den
 * resultado positivo
 */
int tests_services() {
    setup_test_environment();
    int result = run_tests(services_tests);
    printn(1);
    printt(result, 0, "SERVICES");
    printn(2);
    return result;
}

/*
 * Función main se encarga de ejecutar todos los tests y comprobar que todos den
 * resultado positivo
 */
int main() {
    int result;
    result = tests_services();
    return result;
}
