#!/usr/bin/env python


from twisted.internet.protocol import ClientFactory
from Client import TcpClient

class TcpClientFactory(ClientFactory):
    """
    the factory that connects our client
    """
    def __init__(self, plugins):
        self.plugins = plugins

    def buildProtocol(self, addr):
        print "factory create new client"
        newclient = TcpClient(self.plugins)
        return newclient
