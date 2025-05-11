from client import P2PClient
from spyne.server.wsgi import WsgiApplication
import argparse
import sys

from spyne import Application, ServiceBase, Unicode, rpc
from spyne.protocol.soap import Soap11
from spyne.server.wsgi import WsgiApplication
import dataService

app = Application(
    [DateTimeService],
    tns="spyne.examples.datetime",
    in_protocol=Soap11(validator="lxml"),
    out_protocol=Soap11(),
)
if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Cliente P2P')
    parser.add_argument('-s', '--server', required=True, help='IP o nombre del servidor')
    parser.add_argument('-p', '--port', type=int, required=True, help='Puerto del servidor')
    args = parser.parse_args()

    client = P2PClient(args.server, args.port)

    from wsgiref.simple_server import make_server

    # run on 5000, localhost
    server = make_server("0.0.0.0", 5000, WsgiApplication(app))
    print("SOAP service running at http://0.0.0.0:5000")
    print("WSDL available at http://0.0.0.0:5000/?wsdl")
    server.serve_forever()

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

            # LIST USERS / LIST CONTENT <user>
            if action == 'LIST' and len(tokens) >= 2:
                sub = tokens[1].upper()
                if sub == 'USERS' and len(tokens) == 2:
                    client.list_users()
                elif sub == 'CONTENT' and len(tokens) == 3:
                    client.list_content(tokens[2])
                else:
                    print('Uso:\n'
                        '  LIST USERS\n'
                        '  LIST CONTENT <usuario>')
                continue

            # Resto de comandos de una palabra
            if action == 'REGISTER' and len(tokens) == 2:
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
