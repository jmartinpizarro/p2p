from datetime import datetime


from spyne import Application, ServiceBase, Unicode, rpc
from spyne.protocol.soap import Soap11
from spyne.server.wsgi import WsgiApplication

class DateTimeService(ServiceBase):
    @rpc(_returns=Unicode)
    def get_datetime(ctx):
        """
        Method that returns date in format DD/MM/YYYY HH:MM:SS
        """
        return datetime.now().strftime("%d/%m/%Y %H:%M:%S")



if __name__ == "__main__":
