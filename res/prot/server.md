# Server protocols
> [!NOTE] 
> Este archivo recopila todos los protocolos definidos en el lado del servidor para el proyecto de sistemas distribuidos. 


## Servidor:

1. La ejecución del servidor debe de ser de la siguiente forma:
    ```bash
    ./servidor -p <port>
    ```
    - Tras la ejecucion el servidor debe de mostrar una shell
    ```bash
    s>
    ```
    - En esta shell se debe de mostrar un mensaje 
    ```bash
   init server <local IP>:<port> 
    ```

2. Por cada petición realizada por un lciente se mostrará en pantalla: 
    ```bash
   s> OPERATION <operation> FROM <user_name> 
    ```
    - Donde operacion y user name son los valores obtenidos a través del cliente, que siempre los dará al enviar cualquier operacion. 

3. Se finaliza el programa si se recive CTRL+C
