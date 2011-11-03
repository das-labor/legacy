#!/usr/bin/env python

from twisted.internet.protocol import Factory
from Server import TcpServer


class TcpServerFactory(Factory):

    def __init__(self, plugins):
        self.plugins = plugins

    def buildProtocol(self, addr):
        print "factory create new client"
        newclient = TcpServer(self.plugins)
        return newclient
