import socket

def send_string(sock: socket.socket, s: str) -> None:
    sock.sendall(s.encode('utf-8') + b'\0')

def recv_string(sock: socket.socket) -> str:
    print("Empezando a recibir string")
    data = bytearray()
    while True:
        print("Byte recibiendo")
        byte = sock.recv(1)
        print("Byte recibido")
        if not byte:
            print("Conexión cerrada en recv_string")
            raise ConnectionError("Conexión cerrada inesperadamente")
        if byte == b'\0':
            print("rompiendo")
            break
        data += byte
    return data.decode('utf-8')
