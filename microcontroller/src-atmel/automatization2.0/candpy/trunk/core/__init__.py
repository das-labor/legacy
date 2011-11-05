__all__ = ["localparserargs", "localparserinit"]

from LocalParserPlugin import LocalParserPlugin

def localparserargs(data):
    pass


def localparserinit(data):
    #options=data[0]
    #args=data[1]
    #parser=data[2]
    plugins = data[3]
    lparserThread = LocalParserPlugin(plugins)
    lparserThread.setDaemon(True)
    lparserThread.start()
    localparserplugin = plugins.Hook('connector.tcpclient.read')
    localparserplugin.register(lparserThread.addData)
