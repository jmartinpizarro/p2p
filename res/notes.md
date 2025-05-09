# Proyect Notes: 
> [!NOTE] Intro:
> Esta nota resume todos los requisitos dados en el enunciado del proyecto. También resume el enunciado de forma global. 

## Requisitos globales
Se desarrollará un cliente y un servidor. 
- Cliente: Ejecutado en el sistema, sus operaciones se dan mas adelante
- Server: Coordinación interna

1. El código debe de correr entero en guernika.lab.inf.uc3m.es
2. Estructuras de datos abiertas **pero sin límite**
3. No se puede instalar software adicional en guernika
4. Prácticas que **no compilen** -> 0
5. Comments necesarios
6. Sin warnings
7. **Memoria:** Longitud máxima de 10 páginas 

## Cliente:

### Requisitos del cliente:
1. Debe de ser multihilo
2. Debe de poder ejecutar las operaciones definidas en /res/prot/client.md
3. Debe de seguir los protocolos definidos en el archivo /res/prot/client.md 
4. El tamaño máximo de un nombre de usuario es de 256 bytes


## Servidor: 

### Req. del servidor

1. Debe mantener los nombres de los archivos de clientes, conectados o no 
2. No almacena ficheros
3. Debe de ser concurrente y multihilo
4. Debe de estar escrito en C
2. Debe de poder ejecutar las operaciones definidas en /res/prot/server.md
3. Debe de seguir los protocolos definidos en el archivo /res/prot/server.md 

