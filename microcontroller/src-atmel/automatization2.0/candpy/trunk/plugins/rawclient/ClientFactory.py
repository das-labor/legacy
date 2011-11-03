#!/usr/bin/env python


from twisted.internet.protocol import ClientFactory
from LAPTcpClient import _LAPTcpClient

class TcpClientFactory(ClientFactory):
    """
    the factory that connects our client
    """
    def __init__(self, plugins):
        self.plugins = plugins

    def buildProtocol(self, addr):
        print "factory create new client"
        newclient = _LAPTcpClient(self.plugins)
        return newclient
