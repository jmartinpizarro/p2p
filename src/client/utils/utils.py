def send_string(sock: socket.socket, s: str) -> None:
    sock.sendall(s.encode() + b"\0")

def recv_string(sock: socket.socket) -> str:
    data = bytearray()
    while True:
        chunk = sock.recv(1)
        if not chunk:
            raise ConnectionError("Conexión cerrada inesperadamente")
        if chunk == b"\0":
            break
        data.extend(chunk)
    return data.decode()
