#!/usr/bin/env python


from twisted.internet.protocol import Protocol

class TcpServer(Protocol):
    """
    foo
    """

    def __init__(self, plugins):
        self.plugins = plugins

    def dataReceived(self, data):
        self.plugins.Hook('connector.tcpserver.read').notify(data)

    def putData(self, data):
        self.transport.writeSomeData(data)

    def connectionLost(self, reason):
        rawinputreadplugins = self.plugins.Hook(
        'connector.tcpserver.write')
        rawinputreadplugins.deregister(self.putData)

    def connectionMade(self):
        rawinputreadplugins = self.plugins.Hook(
        'connector.tcpserver.write')
        rawinputreadplugins.register(self.putData)
