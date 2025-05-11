import socket

def send_string(sock: socket.socket, s: str) -> None:
    sock.sendall(s.encode('utf-8') + b'\0')

def recv_string(sock: socket.socket) -> str:
    data = bytearray()
    while True:
        byte = sock.recv(1)
        if not byte:
            raise ConnectionError("Conexión cerrada inesperadamente")
        if byte == b'\0':
            break
        data += byte
    return data.decode('utf-8')
