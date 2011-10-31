#!/usr/bin/env python

import optparse
import threading
from twisted.internet import protocol, reactor


class CanClient(protocol.Protocol):
    """
    foo
    """

    def __init__(self, plugins):
        self.plugins = plugins

    def dataReceived(self, data):
        self.plugins.Hook('daslabor.cand.rawtcpclient.read').notify(data)

    def putData(self, data):
        self.transport.writeSomeData(data)

    def connectionLost(self, reason):
        rawinputreadplugins = self.plugins.Hook(
        'daslabor.cand.rawtcpclient.write')
        rawinputreadplugins.deregister(self.putData)

    def connectionMade(self):
        rawinputreadplugins = self.plugins.Hook(
        'daslabor.cand.rawtcpclient.write')
        rawinputreadplugins.register(self.putData)


class CanClientFactory(protocol.Factory):

    def __init__(self, plugins):
        self.plugins = plugins

    def buildProtocol(self, addr):
        print "factory create new client"
        newclient = CanClient(self.plugins)
        return newclient


class reactorThread(threading.Thread):

    def __init__(self, plugins, options):
        self.plugins = plugins
        self.options = options
        reactor.listenTCP(self.options.local_port,
                          CanClientFactory(self.plugins))
        threading.Thread.__init__(self)

    def run(self):
        reactor.run(installSignalHandlers=False)


def rawtcpclientargs(data):
    parser = data[0]
    #plugins = data[1]

    group = optparse.OptionGroup(parser,
        "Network settings",
        "Network configuration."
    )
    parser.add_option_group(group)

    group.add_option("-P", "--localport",
        dest="local_port",
        action="store",
        type='int',
        help="local TCP port",
        default=2342
    )


def rawtcpclientinit(data):
    print "got init"
    options = data[0]
    #args = data[1]
    #parser = data[2]
    plugins = data[3]

    #setup
    rThread = reactorThread(plugins, options)
    rThread.setDaemon(True)
    rThread.start()
