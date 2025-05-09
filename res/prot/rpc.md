# RPC protocols:

> [!NOTE] 
> Este archivo recopila todos los protocolos definidos en el servicio RPC de la parte 3 de la práctica para el proyecto de sistemas distribuidos. 
> Este servicio RPC consta de un servidor que recive e imprime operaciones y fechas. Y un cliente que manda la operacion y la fecha al servidor. 
> El cliente es el servidor implementado con sockets.

> [!CAUTION]
> El nombre por el que se conocerá al programa RPC será TIMESTAMP

## Protocolos del servidor RPC

1. **imprimirOperacion:** Imprime por pantalla con formato:
    ```bash
   <nombre_usuario> <operacion> <fecha> 
    ```
    - En el caso de que la operacion sea publish o delete también imprime nombre de fichero:
    ```bash
    <nombre_usuario> <operacion> <nombre_fichero> <fecha> 
    ```
    > [!CAUTION]
    > El servidor será concurrente. Se hará de esta manera al utilizar los flags con rpcgen


    - **INPUT:** 
        - `nombre_usuario` : string : max 256
        - `operacion`: string : `REGISTER|UNREGISTER|CONNECT|PUBLISH|DELETE|LIST_CONTENT|DISCONNECT`
        - `nombre_fichero`: string : Solo con PUBLISH y DELETE
        - timestamp: string
    - **OUT:** 
        - 0 -> all gut
        - 1 -> unknowkn operation
        - 2 -> other


## Protocolos del cliente RPC
1. **imprimirOperacion**: Envía los datos necesarios a través de RPC al servidor. 
> [!NOTE]
> - Este envío de datos debería de ser simplemente la llamada a la función local que da el archivo creado por el RPC. 
> - Utiliza la interfaz rellenarDatos para rellenar la estructura con los argumentos necesarios de conexión.  -> Deprecado 
    - **IN:**
        - `nombre_usuario` : string : max 256 (doubled cause it can be someone elses server)
        - `operacion`: string : `REGISTER|UNREGISTER|CONNECT|PUBLISH|DELETE|LIST_CONTENT|DISCONNECT`
        - `nombre_fichero`: string : Solo con PUBLISH y DELETE
        - timestamp: string
    - **OUT:** 
        - 0 -> all gut
        - 1 -> unknowkn operation (doubled because it can be someone elses server)
        - 2 -> other


2. **rellenarDatos**: Dados unos datos de conexión que son todos los de la estructura definida a continuación, se crea la estructura, en caso de que se de un dato NULL para el nombre_fichero y que esto sea correcto (la operación no es ni PUBLISH ni DELETE) entonces se pondrá el nombre del fichero como string vacía. 
    - **IN**:
        - `nombre_usuario` : string : max 256 (doubled cause it can be someone elses server)
        - `operacion`: string : `REGISTER|UNREGISTER|CONNECT|PUBLISH|DELETE|LIST_CONTENT|DISCONNECT`
        - `nombre_fichero`: string : Solo con PUBLISH y DELETE
        - timestamp: string
        - struct con_args: Por referencia
    - **OUT**: INT
        - 0 -> all gut
        - 1 -> no filename for the operation PUBLISH/DELETE
        - 2 -> other


## Estructura de conexión: 
> [!NOTE]
> Para que servidor y cliente se conecten hace falta que se defina una estructura de argumentos que toma el servidor siempre en su función imprimirOperacion y en otras futuras. Esta estructura en este caso es clara pues son los argumentos de la función del servidor. 
> [!CAUTION]
> Si finalmente esta estructura no se utiliza de forma directa (como struct), los valores que contiene la estructura seguirán siendo las variables a utilizar. En el orden declarado en esta estructura.

```c
struct con_args
{
    nombre_usuario: char[256];
    operacion: char[MAX_OP_LEN];
    nombre_fichero: char[MAX_FILE_LEN];
    timestamp: char[MAX_TIME_LEN];
};
```
- Where MAX_OP_LEN is the max len of the larger operation there is
- Something similar for file lenght
- Again for timestamp len
- El fichero es una string vacía si no hubiese


