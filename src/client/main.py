from client import P2PClient
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

            # Partimos en tokens completos para detectar comandos multi-palabra
            tokens = cmd.split()
            action = tokens[0].upper()

            # Salir
            if action == 'QUIT':
                if client.username:
                    client.disconnect()
                break

            if action == 'LIST_USERS' and len(tokens) == 1:
                client.list_users()
            elif action =="LIST_CONTENT" and len(tokens) == 2:
                client.list_content(tokens[1])
            elif action == 'REGISTER' and len(tokens) == 2:
                client.register(tokens[1])

            elif action == 'UNREGISTER' and len(tokens) == 2:
                client.unregister(tokens[1])

            elif action == 'CONNECT' and len(tokens) == 2:
                client.connect(tokens[1])

            elif action == 'DISCONNECT' and len(tokens) == 1:
                client.disconnect()

            elif action == 'PUBLISH' and len(tokens) >= 3:
                # PUBLISH <ruta> <descripción>
                # La descripción puede contener espacios, la recomponemos:
                file_path = tokens[1]
                desc = ' '.join(tokens[2:])
                client.publish(file_path, desc)

            elif action == 'DELETE' and len(tokens) == 2:
                client.delete(tokens[1])

            elif action == 'GET_FILE' and len(tokens) == 4:
                _, user, remote_path, local_name = tokens
                client.get_file(user, remote_path, local_name)

            else:
                print(f'Comando desconocido: {cmd}')
    except KeyboardInterrupt:
        if client.username:
            client.disconnect()
        print()  # nueva línea después de Ctrl+C
        sys.exit(0)
