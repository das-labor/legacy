#!/usr/bin/env python

import threading
from twisted.internet import reactor
from ClientFactory import TcpClientFactory

class TcpClientThread(threading.Thread):
    """
    The class for the thread that handles the LAP
    from a remote server
    """
    def __init__(self, plugins, options):
        self.plugins = plugins
        self.options = options
        reactor.connectTCP(self.options.remote_host,
                           self.options.remote_port,
                           TcpClientFactory(self.plugins))
        threading.Thread.__init__(self)

    def run(self):
        reactor.run(installSignalHandlers=False)
