#ifndef USER_UTILS_H
#define USER_UTILS_H

/*
* @brief Definición de las interfaces para utilidades de apoyo al código del servidor 
*/

// Necesario para la definición del a estructura users y otras
#include "services.h"

/*
 * @brief Función de envio de una string a traves de un socket <sock>
 * @param sock: El socket para mandar la string
 * @param char*: La string a mandar
 * @return (void)
*/
void send_string(int sock, const char* s);

/*
* @brief Funcion para recibir una string a través de un socket <sock>
* @param sock: El socket para recibir la string
* @return char*: Se devuelve la string obtenida
*/
char* recv_string(int sock);

/*
* @brief Función para encontrar una estructura (instance) en la estructura de datos Users, devuelve una estructura User.
* @param user_name: String con el nombre del usuario 
* @return Instance de la estructura users si todo bien, en caso de error NULL
*/
User* find_user(const char *name);


#endif // !USER_UTILS_H
