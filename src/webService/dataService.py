from spyne import Application, rpc, ServiceBase, Unicode
from spyne.protocol.soap import Soap11
from spyne.server.wsgi import WsgiApplication
from datetime import datetime

class DateTimeService(ServiceBase):
    @rpc(_returns=Unicode)
    def get_datetime(ctx):
        """
        Method that returns date in format DD/MM/YYYY HH:MM:SS
        """
        return datetime.now().strftime('%d/%m/%Y %H:%M:%S')

app = Application(
    [DateTimeService],
    tns='spyne.examples.datetime',
    in_protocol=Soap11(validator='lxml'),
    out_protocol=Soap11(),
)

if __name__ == '__main__':
    from wsgiref.simple_server import make_server

    # run on 5000, localhost
    server = make_server('0.0.0.0', 5000, WsgiApplication(app))
    print('SOAP service running at http://0.0.0.0:5000')
    print('WSDL available at http://0.0.0.0:5000/?wsdl')
    server.serve_forever()
