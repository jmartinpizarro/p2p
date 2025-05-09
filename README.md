# p2p

Este proyecto implementa un sistema P2P con un cliente en Python y un servidor en C. A continuación se explica cómo compilar y ejecutar ambos componentes.

---

## Requisitos Previos

- **Servidor (C)**
  - GCC (o cualquier compilador de C compatible)
  - Make
  - Librerías estándar de sockets (normalmente incluidas en sistemas Linux)

- **Cliente (Python)**
  - Python 3.7+
  - Módulos estándar: `socket`, `threading`, `argparse`, `os`, `sys`

---

## Compilación

### Servidor

1. Abre una terminal en el directorio del servidor.
2. Ejecuta:
   ```bash
   make
   ```
   Esto generará el ejecutable `server`.

> **Nota:** El Makefile está configurado para usar `gcc` con las banderas `-Wall -Wextra -pthread`. Si deseas otro compilador o flags, edita el Makefile.

### Cliente

El cliente no requiere compilación, ya que está implementado en Python. Solo asegúrate de tener Python 3 instalado.

---

## Ejecución

### Servidor

1. En una terminal, lanza el servidor indicando el puerto de escucha:
   ```bash
   ./server <puerto>
   ```
   Ejemplo:
   ```bash
   ./server 8080
   ```
2. El servidor quedará a la espera de conexiones de clientes.

### Cliente

1. En otra terminal, lanza el cliente indicando la dirección IP y el puerto del servidor:
   ```bash
   python3 client.py --server-ip <IP_SERVIDOR> --server-port <PUERTO>
   ```
   Ejemplo:
   ```bash
   python3 client.py --server-ip 127.0.0.1 --server-port 8080
   ```
2. Una vez conectado, utiliza los comandos del protocolo:
   - `REGISTER <username>`: Registrar usuario.
   - `CONNECT`: Conectarse tras registro.
   - `PUBLISH <filepath> <description>`: Publicar contenido.
   - `DELETE <content_id>`: Eliminar contenido publicado.
   - `LIST USERS`: Listar usuarios registrados.
   - `LIST CONTENT`: Listar contenidos disponibles.
   - `DISCONNECT`: Desconectarse.
   - `UNREGISTER`: Anular registro.


---

## Ejemplos de Uso

1. Registrar y conectar:
   ```bash
   > REGISTER alice
   OK
   > CONNECT
   OK
   ```

2. Publicar un archivo:
   ```bash
   > PUBLISH ./mi_archivo.txt "Descripción de ejemplo"
   OK: ID=42
   ```

3. Listar contenido:
   ```bash
   > LIST CONTENT
   42 alice ./mi_archivo.txt "Descripción de ejemplo"
   ```

4. Desconectar y anular registro:
   ```bash
   > DISCONNECT
   OK
   > UNREGISTER
   OK
   ```

---

¡Listo! Con esto dispones de toda la información necesaria para compilar y levantar el cliente y el servidor del proyecto P2P. Si surge cualquier problema, revisa los logs en la terminal o el output de errores.

