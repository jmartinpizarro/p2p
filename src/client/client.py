import argparse
import os
import socket
import sys
import threading
import requests

from utils.utils import recv_string, send_string 


class P2PClient:
    def __init__(
        self,
        server_host: str,
        server_port: int,
        ws_host: str = "localhost",
        ws_port: int = 5000,
    ):
        self.server_host = server_host
        self.server_port = server_port
        self.ws_host = ws_host
        self.ws_port = ws_port
        self.username = None
        self.listen_sock = None
        self.shutdown_event = threading.Event()


    def _do_server_op(self, op: str, args: list = []):
        "Send operation to server. Returns code of operation and (may or not) files"
        try:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                s.connect((self.server_host, self.server_port))
                send_string(s, op)

                for a in args:
                    send_string(s, a)
                code = s.recv(1)
                if not code:
                    return None, None
                code = code[0]
                print(f"Op: {op}, code {code}")
                # For LIST_USERS and LIST_CONTENT, collect data if success
                if op == "LIST_USERS" and code == 0:
                    print(f"OPERATION LIST_USERS STARTED ON SOCKET {s}")
                    n = recv_string(s)
                    n = int(n)
                    print(f"{n} number of users obtained")
                    users = []
                    for i in range(n):
                        username = recv_string(s)
                        ip = recv_string(s)
                        port = recv_string(s)
                        users.append([username,ip,port])
                        print(f"Values for users {i}: {users[i]}")
                    print(f"Values in users: {users}")
                    return code, users
                if op == "LIST CONTENT" and code == 0:
                    n = int(recv_string(s))
                    files = [recv_string(s) for _ in range(n)]
                    return code, files
                return code, None
        
        except Exception:
            return None, None

    def register(self, user: str):
        code, _ = self._do_server_op("REGISTER", [user])
        if code == 0:
            print("REGISTER OK")
        elif code == 1:
            print("USERNAME IN USE")
        else:
            print("REGISTER FAIL")

    def unregister(self, user: str):
        code, _ = self._do_server_op("UNREGISTER", [user])
        if code == 0:
            print("UNREGISTER OK")
        elif code == 1:
            print("USER DOES NOT EXIST")
        else:
            print("UNREGISTER FAIL")

    def connect(self, user: str):
        "Connects an user (not client) to server"
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.bind(("", 0))  # use free port? TODO: look to this
        sock.listen(5)
        port = sock.getsockname()[1]
        sock.settimeout(1)
        self.listen_sock = sock
        self.shutdown_event.clear()
        threading.Thread(target=self._listener_thread, daemon=True).start()

        code, _ = self._do_server_op("CONNECT", [user, str(port)])
        if code == 0:
            self.username = user
            print("CONNECT OK")
        elif code == 1:
            print("CONNECT FAIL, USER DOES NOT EXIST")
            self._stop_listener()
        elif code == 2:
            print("USER ALREADY CONNECTED")
            self._stop_listener()
        else:
            print("CONNECT FAIL")
            self._stop_listener()

    def _listener_thread(self):
        while not self.shutdown_event.is_set():
            try:
                conn, addr = self.listen_sock.accept()
                threading.Thread(
                    target=self._handle_peer, args=(conn,), daemon=True
                ).start()
            except socket.timeout:
                continue
            except Exception:
                break

    def _handle_peer(self, conn: socket.socket):
        try:
            op = recv_string(conn)
            if op != "GET FILE":
                return
            path = recv_string(conn)
            if not os.path.isfile(path):
                conn.sendall(b"\x01")
                return
            conn.sendall(b"\x00")
            size = os.path.getsize(path)
            send_string(conn, str(size))
            with open(path, "rb") as f:
                while True:
                    chunk = f.read(4096)
                    if not chunk:
                        break
                    conn.sendall(chunk)
        except Exception:
            try:
                conn.sendall(b"\x02")
            except Exception:
                pass
        finally:
            conn.close()

    def publish(self, file_path: str, desc: str):
        code, _ = self._do_server_op("PUBLISH", [self.username, file_path, desc])
        if code == 0:
            print("PUBLISH OK")
        elif code == 1:
            print("PUBLISH FAIL, USER DOES NOT EXIST")
        elif code == 2:
            print("PUBLISH FAIL, USER NOT CONNECTED")
        elif code == 3:
            print("PUBLISH FAIL, CONTENT ALREADY PUBLISHED")
        else:
            print("PUBLISH FAIL")

    def delete(self, file_path: str):
        code, _ = self._do_server_op("DELETE", [self.username, file_path])
        if code == 0:
            print("DELETE OK")
        elif code == 1:
            print("DELETE FAIL, USER DOES NOT EXIST")
        elif code == 2:
            print("DELETE FAIL, USER NOT CONNECTED")
        elif code == 3:
            print("DELETE FAIL, CONTENT NOT PUBLISHED")
        else:
            print("DELETE FAIL")

    def list_users(self):
        code, users = self._do_server_op("LIST_USERS", [self.username])
        if code == 0:
            if not users:
                print("LIST_USERS FAIL, USERS ARE NONETYPE")
            else:
                print("LIST_USERS OK")
                for u, ip, pr in users:
                    print(f"{u} {ip} {pr}")
        elif code == 1:
            print("LIST_USERS FAIL, USER DOES NOT EXIST")
        elif code == 2:
            print("LIST_USERS FAIL, USER NOT CONNECTED")
        else:
            print("LIST_USERS FAIL")

    def list_content(self, remote: str):
        code, files = self._do_server_op("LIST_CONTENT", [self.username, remote])
        if code == 0:
            if not files:
                    print("LIST_CONTENT FAIL, FILES ARE NONETYPE")
            else:
                print("LIST_CONTENT OK")
                for f in files:
                    print(f)

        elif code == 1:
            print("LIST_CONTENT FAIL, USER DOES NOT EXIST")
        elif code == 2:
            print("LIST_CONTENT FAIL, USER NOT CONNECTED")
        elif code == 3:
            print("LIST_CONTENT FAIL, REMOTE USER DOES NOT EXIST")
        else:
            print("LIST_CONTENT FAIL")

    def disconnect(self):
        code, _ = self._do_server_op("DISCONNECT", [self.username])
        if code == 0:
            print("DISCONNECT OK")
        elif code == 1:
            print("DISCONNECT FAIL, USER DOES NOT EXIST")
        elif code == 2:
            print("DISCONNECT FAIL, USER NOT CONNECTED")
        else:
            print("DISCONNECT FAIL")
        self._stop_listener()
        self.username = None

    def _stop_listener(self):
        if self.listen_sock:
            self.shutdown_event.set()
            try:
                self.listen_sock.close()
            except Exception:
                pass
            self.listen_sock = None

    def get_file(self, remote: str, remote_path: str, local_name: str):
        # Obtain remote's ip and port via LIST_USERS
        code, users = self._do_server_op("LIST_USERS", [self.username])
        if code != 0:
            print("GET_FILE FAIL")
            return
        peer = None
        for u, ip, pr in users:
            if u == remote:
                peer = (ip, int(pr))
                break
        if not peer:
            print("GET_FILE FAIL, USER NOT FOUND")
            return
        try:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                s.connect(peer)
                send_string(s, "GET FILE")
                send_string(s, remote_path)
                status = s.recv(1)
                if not status:
                    print("GET_FILE FAIL")
                    return
                status = status[0]
                if status == 1:
                    print("GET_FILE FAIL, FILE NOT EXIST")
                    return
                if status != 0:
                    print("GET_FILE FAIL")
                    return
                size = int(recv_string(s))
                with open(local_name, "wb") as f:
                    received = 0
                    while received < size:
                        chunk = s.recv(min(4096, size - received))
                        if not chunk:
                            break
                        f.write(chunk)
                        received += len(chunk)
                if received == size:
                    print("GET_FILE OK")
                else:
                    print("GET_FILE FAIL")
                    os.remove(local_name)
        except Exception:
            print("GET_FILE FAIL")
