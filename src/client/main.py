from src.client.client import *
import argparse
import sys

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Cliente P2P')
    parser.add_argument('-s', '--server', required=True, help='IP o nombre del servidor')
    parser.add_argument('-p', '--port', type=int, required=True, help='Puerto del servidor')
    args = parser.parse_args()

    client = P2PClient(args.server, args.port)
    try:
        while True:
            cmd = input('c> ').strip()
            if not cmd:
                continue
            parts = cmd.split(' ', 2)
            action = parts[0].upper()
            if action == 'QUIT':
                if client.username:
                    client.disconnect()
                break
            elif action == 'REGISTER' and len(parts) == 2:
                client.register(parts[1])
            elif action == 'UNREGISTER' and len(parts) == 2:
                client.unregister(parts[1])
            elif action == 'CONNECT' and len(parts) == 2:
                client.connect(parts[1])
            elif action == 'PUBLISH' and len(parts) == 3:
                file_path, desc = parts[1], parts[2]
                client.publish(file_path, desc)
            elif action == 'DELETE' and len(parts) == 2:
                client.delete(parts[1])
            elif action == 'LIST_USERS':
                client.list_users()
            elif action == 'LIST_CONTENT' and len(parts) == 2:
                client.list_content(parts[1])
            elif action == 'DISCONNECT':
                client.disconnect()
            elif action == 'GET_FILE' and len(parts) == 3:
                sub = parts[1].split(' ', 1)
                # For GET_FILE, split differently: parts = cmd.split()
                tokens = cmd.split()
                if len(tokens) == 4:
                    _, user, remote_path, local_name = tokens
                    client.get_file(user, remote_path, local_name)
                else:
                    print('Uso: GET_FILE <user_name> <remote_file_path> <local_file_name>')
            else:
                print(f'Comando desconocido: {cmd}')
    except KeyboardInterrupt:
        if client.username:
            client.disconnect()
        print()  # nueva línea después de Ctrl+C
        sys.exit(0)
