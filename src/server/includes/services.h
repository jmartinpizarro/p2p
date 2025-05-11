//services.h 

#ifndef SERVICES_H
#define SERVICES_H
/*
* Definición de los servicios,estrucutras y variables que debe de ofrecer el servidor, a los que también debe de tener acceso el cliente de la misma forma. 
*/

#include <arpa/inet.h>

#define MAX_USERS       100
#define MAX_CONTENT     100
#define MAX_USERNAME    64
#define MAX_FILEPATH    256
#define MAX_DESC        256

/*
 * Estructura Content para gaurdar un arhivo con su descripción juntas
 */
typedef struct {
    char path[MAX_FILEPATH];
    char desc[MAX_DESC];
} Content;

/*
 * Estructura User para guardar un usuario junto a todos sus datos
 */
typedef struct {
    char username[MAX_USERNAME];
    char ip[INET_ADDRSTRLEN];
    int port;
    int connected;
    int content_count;
    Content content[MAX_CONTENT];
} User;

/*
 * @brief Este servicio permite registrar un usuario dentro de la estructura de datos elegida para guardarlo en ella. 
 * @param user_name: NOmbre del usuario a registrar/guardar
 * @return 0-> todo bien, 1-> El usuario ya existe, 2 -> Otro error (nombre de usuario muy largo por ejemplo)
 */
int register_user(char* user_name);

/*
 * @brief Este servicio permite desregistra un usuario dentro de la estructura de datos elegida. 
 * @param user_name: Nombre de usuario a deregistrar
 * @return 0 -> All gut, 1 -> El usuario no existe, 2 -> Otro error
 */
int unregister_user(char* user_name);

/*
* @brief Este servicio debe de recivir una cadena de texto con un <user_name> y otra cadena con el puerto <puerto> en el que se quiere conectar, debe de comprobar si el usuario ya esta conectado y devolver 0 si todo va bien, 2 si el usuario ya esta conectado y 1 por cualquier otro error.
* @param  user_name: El nombre del usuario que quiere conectarse 
* @param puerto: Puerto en el que el usuario quiere conectarse
* @return: 0 -> Todo bien, 2 -> Usuario ya conectado, 1 -> Otro error(el usuario no existe)
*/
int connect_user(char* user_name, char* port, struct sockaddr_in addr);

/*
* @brief Este servicio sirve para desconectarse del servidor, recive un usuario y desconecta al usuario del servidor
* @param user_name: Nombre del usuario a desconectar 
* @return: 0 -> Todo bien, 2-> Usuario no conectado, 1-> Otro error (el usuario no existe)
*/
int disconnect_user(char* user_name);

/*
* @brief Este servicio debe de guardar el path de un archivo, su descripción y el nombre del usuario al que pertenece al archivo. Para ello recibe todos estos argumentos y devuelve un código numérico. 
* @param user_name: Nombre del usuario unido al archivo a publicar
* @param path: Path del archivo a publicar 
* @param desc: Descripción del archivo a publicar
* @return 0 -> Todo bien, 1 -> Usuario no existe u otro, 2 -> Usuario no conectado
*/
int publish(char* user_name, char* path, char* desc); 

/*
* @brief Este servicio debe de eliminar de la estructura de datos elegida un archivo junto a su descripción y el nombre del usuario al que esta asociado (no elimina el usuario como tal). Para encontrar estos tres campos solo se requiere el nombre del usuario y el path como parámetros.
* Calling it delete_s because delete prompts an unqualified-id error
* @param user_name: Nombre del usuario asociado al archivo a publiar 
* @param path: Path del archivo a eliminar 
* @return 0 -> Todo bien, 1 -> Usuario no encontrado u otro. 2 -> Usuario no conectado
*/
int delete_s(char* user_name, char* path);

/*
* @brief Este servicio **manda a través de sockets** al cliente una serie de strings, los nombres de todos los usuarios registrados en el sistema. 
* @param user_name: Nombre del usuario que requiere el servicio 
* @return 0-> Todo bien, 1 -> Usuario no encontrado, 2 -> Usuario no conectado
*/
int list_users(char* user, int sock);

/*
* @brief Este servicio **manda a través de sockets** al cliente una serie de strings con el contenido que pertenece al usuario dado como parámetro. Como parte del contenido se ha de enviar el path y la descripción del archivo. 
* @param user_name: NOmbre del usuario que requeire el servicio
* @param target_user: Nombre del usuario del que se van a buscar y enviar los archivos. 
* @return 0 -> Todo bien, 1 -> Usuario no encontrado, 2 -> Usuario no conectado
*/
int list_content(char* user_name, char* target_user, int sock);


/*
 * @brief Función de apoyo al resto de servicios, busca en la array de usuarios, si encuentra el usuario por su nombre devuelve un instance de esa array. Sino devuelve null
 * @param name: Nombre del usuario
 * @return Struct user o null si no existe el user
*/
User* find_user(const char* name);

#endif //SERVICES_H
