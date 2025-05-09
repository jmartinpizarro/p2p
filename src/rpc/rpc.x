/*
Este archivo declara las estructuras e interfaces necesarias para crear los archivos con rpcgen. 
Para crear estos archivos se debe de usar el siguiente comando:
rpcgen -MNa <nombre.x>
*/

program TIMESTAMP_PROG{
    version TIMESTAMP_VERS{
        int IMPRIMIR_OPERACION(string nombre_usuario, string operacion, string nombre_fichero, string timestamp) = 1; /*Número de procedimiento*/
    /*Número de version*/
    } = 1;
/* Número de programa -  NOTE: Lo mejor aquí serí usar uno de nuestros NIAs - Usando el mio (Alberto)*/
} = 495775; 
