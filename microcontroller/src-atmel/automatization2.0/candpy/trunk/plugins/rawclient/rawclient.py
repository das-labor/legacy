#!/usr/bin/env python

import optparse
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

def tcpClientArgs(data):
    parser = data[0]
    #plugins=data[1]

    group = optparse.OptionGroup(parser,
        "Remote Network settings",
        "Remote Network configuration."
    )
    parser.add_option_group(group)

    group.add_option("", "--remoteport",
        dest="remote_port",
        action="store",
        type='int',
        help="remote TCP port",
        default=2342
    )

    group.add_option("", "--remotehost",
        dest="remote_host",
        action="store",
        type='string',
        help="remote TCP host",
        default="10.0.1.2"
    )


def tcpClientInit(data):
    print "got init"
    options = data[0]
    #args = data[1]
    #parser = data[2]
    plugins = data[3]

    #setup
    rThread = TcpClientThread(plugins, options)
    rThread.setDaemon(True)
    rThread.start()
