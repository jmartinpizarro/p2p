# Client protocols:

> [!NOTE] 
> Este archivo recopila todos los protocolos definidos en el lado del cliente para el proyecto de sistemas distribuidos. 

## Cliente: 
### Protocolo de ejecución y uso de operaciones del cliente: 
1. El cliente se deberá de ejecutar de la siguiente forma: 
    ```python
    python ./cliente.py -s <server-ip> -p <port>
    ```
    - `server-ip`: Puede ser dominio-punto y decimal-punto

2. La consola mostrada al cliente debe de ser de tipo:
    ```python
    c>
    ```

3. Los errores se deben de mostrar en la terminal del cliente 
    - Creación de una clase para el manejo de fallos

4. Se finalizará el servicio con el comando `QUIT`
    - El ciente debe de haberse previamente desconectado del sistema, sino no funcionará este comando. 

5. Se usara register para registrarse como usuario:
    ```python
    REGISTER <user_name>
    ```
    Se realizarán las sigueintes operaciones en ese caso:
    1. Conexión al servidor con la IP y puerto pasados
    2. Envio de la cadena "REGISTER\0". 
    3. Envio de una cadena de caracteres con el nombre del usuario a registrar
    4. **Server return:** 0 (exito), 1(repe), 2(other)
    5. Cierre de conexion
        - Si  exito -> mostrar en consola 
        ```python
        REGISTER OK
        ```
        - Si repe -> 
        ```python 
        USERNAME IN USE
        ```
        - Si other -> 
        ```python
        REGISTER FAIL
        ```
6. Se usará **unregister**  para darse de baja
    ```python
    UNREGISTER <user_name>
    ```
    En este caso se realizan las siguientes operaciones:
    1. Se conecta al servidor
    2. Envio de la cadena "UNREGISTER\0"
    3. Envio de una cadena con "<user_name>"
    4. **Server return:**  0(exito), 1(no_existe), 2(other)
    5. Cierre de conexion
        - Si exito: 
        ```python
       UNREGISTER OK 
        ```
        - Si no_existe
        ```python
       USER DOES NOT EXIST
        ```
        - Si other
        ```python
       UNREGISTER FAIL 
        ```
7. Se usa **connect**  para conectarse como usuario al servicio.
    ```python
    CONNECT <user_name>
    ```
    Deberá de realizar los siguientes pasos: 
    1. Buscar puerto libre
    2. Crear un socket con el ciente `serverSocket` 
    3. Crear un hilo en el puerto selecionado en 1. Ese hilo escucha peticiones de otros usuarios para descargar
    4. Enviar solicitud de conexión al cliente de la siguiente forma 
        1. Conexión de acuerdo con los argumentos pasados en la inicialización del cliente 
        2. Enviar "CONNECT"
        3. Enviar "<user_name>"
        4. Cadena con el puerto de escucha del cliente "<puerto>"
        5. **Server return:**  0(alles gut), 1(no_existe), 2(ya_connectado), 3(other)
        6. Cierre de conexión:
            - Si exito: 
            ```python
           CONNECT OK 
            ```
            - Si no_existe
            ```python
           CONNECT FAIL, USER DOES NOT EXIST 
            ```
            - Si ya_connectado
            ```python
           USER ALREADY CONNECTED 
            ```
            > [!CAUTION]
            > Aquí ya se habrá creado un socket y un hilo en el cliente. Hay que romperlos. 
            - Si other
            ```python
            CONNECT FAIL
            ```
