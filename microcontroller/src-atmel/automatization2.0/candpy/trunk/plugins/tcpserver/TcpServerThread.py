#!/usr/bin/env python

import threading
from twisted.internet import reactor
from ServerFactory import TcpServerFactory

class TcpServerThread(threading.Thread):

    def __init__(self, plugins, options):
        self.plugins = plugins
        self.options = options
        reactor.listenTCP(self.options.local_port,
                          TcpServerFactory(self.plugins))
        threading.Thread.__init__(self)

    def run(self):
        reactor.run(installSignalHandlers=False)
