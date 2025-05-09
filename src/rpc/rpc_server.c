/*
 * Código del servidor RPC con la función que llama a las funciones implementadas en timestamp.c
 */

#include "timestamp.h"

// Utilidades de apoyo
#include "../utils/utils.h"

// RPC
#include "rpc.h"

bool_t
imprimir_operacion_1_svc(char *nombre_usuario, char *operacion, char *nombre_fichero, char *timestamp, int *result,  struct svc_req *rqstp)
{

    printi("RPC SERVER", "INICIANDO imprimirOperacion");
    // Llamada a la función de timestamp.h, definida en timestamp.c
    *result =
        imprimirOperacion(nombre_usuario, operacion, nombre_fichero, timestamp);
    if (*result != 0) {
        printe("RPC SERVER", "FINALIZADA CON ERRORES imprimirOperacion"); 
    }
    else {
        printi("RPC SERVER", "FINALIZADA CORRECTAMENTE imprimirOperacion");
    }

    return 1;

}

int
timestamp_prog_1_freeresult (SVCXPRT *transp, xdrproc_t xdr_result, caddr_t result)
{
	xdr_free (xdr_result, result);

	/*
	 * Insert additional freeing code here, if needed
	 */

	return 1;
}
