#!/usr/bin/env python

import optparse
import threading
from twisted.internet import protocol, reactor


class CanServer(protocol.Protocol):
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


class CanServerFactory(protocol.ClientFactory):

    def __init__(self, plugins):
        self.plugins = plugins

    def buildProtocol(self, addr):
        print "factory create new client"
        newclient = CanServer(self.plugins)
        return newclient


class RemoteServerThread(threading.Thread):
    def __init__(self, plugins, options):
        self.plugins = plugins
        self.options = options
        reactor.connectTCP(self.options.remote_host,
                           self.options.remote_port,
                           CanServerFactory(self.plugins))
        threading.Thread.__init__(self)

    def run(self):
        reactor.run(installSignalHandlers=False)


def rawtcpserverargs(data):
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


def rawtcpserverinit(data):
    print "got init"
    options = data[0]
    #args = data[1]
    #parser = data[2]
    plugins = data[3]

    #setup
    rThread = RemoteServerThread(plugins, options)
    rThread.setDaemon(True)
    rThread.start()
