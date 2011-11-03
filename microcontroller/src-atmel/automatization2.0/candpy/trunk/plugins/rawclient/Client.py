#!/usr/bin/env python


from twisted.internet.protocol import Protocol

class TcpClient(Protocol):
    """
    The class acts as a client to a remote cand instanz.
    The format of the expected data is a raw lappacket 
    tcp-stream. It does not parser any data but forwards
    it to a hook called 'daslabor.cand.rawtcpserver.read'
    further more we are registering a Hook 
    'daslabor.cand.rawtcpserver.write' every data that is 
    send to this hook will be forwarded to the remote server
    """

    def __init__(self, plugins):
        self.plugins = plugins

    def dataReceived(self, data):
        """
        The remote server did send us some data - so we
        are forwarding them to everybody registered at 
        'daslabor.cand.rawtcpserver.read'
        """
        self.plugins.Hook('daslabor.cand.rawtcpserver.read').notify(data)

    def putData(self, data):
        """
        @para: data must be in iterable byteformat
        @function: send some data to the remote server
        """
        self.transport.writeSomeData(data)

    def connectionLost(self, reason):
        """
        When the connection to the remote server is lost, also 
        deregister the putData function for hookprocessing
        """
        rawinputreadplugins = self.plugins.Hook(
        'daslabor.cand.rawtcpserver.write')
        rawinputreadplugins.deregister(self.putData)

    def connectionMade(self):
        rawinputreadplugins = self.plugins.Hook(
        'daslabor.cand.rawtcpserver.write')
        rawinputreadplugins.register(self.putData)
