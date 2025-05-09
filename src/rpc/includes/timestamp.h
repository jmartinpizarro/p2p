/*
 * Definicion de la interfaz imprimirOperacion compartida para el servicio de
 * timestamp
 */
#ifndef TIMESTAMP_H
#define TIMESTAMP_H

/*
 * La función imprimirOperación toma una estructura del timpo conn_args definida
 * en el archivo de rpc <timestamp.x> y devuelve un valor int con el return de
 * la función. Se debe de comprobar que los valores de la estructura están
 * dentro de los límites establecidos
 * @param: struct conn_args
 * @return: 0 -> alles gut, 1 -> unknown operation, 2 -> other
 */
int imprimirOperacion(char *nombre_usuario, char *operacion,
                      char *nombre_fichero, char *timestamp);


#endif // !TIMESTAMP_H
