#!/usr/bin/env python

# for commandline parsing
import optparse
# for sleeping
from time import sleep

# everything is handled via plugins
from core.peak.util import plugins
# in general two components are needed to activate a plugin
# the first component is the parsing and providing of commandline args
# the second component is the starting of the plugin with the actual
# commandline args
#from plugins.serialcon.SerialPlugin import serialinit, serialargs
from core.LoggingPlugin import fileloggerargsrs232pkg
from core.LoggingPlugin import fileloggerinitrs232pkg
from core.LoggingPlugin import fileloggerrs232pkgread
from core.LoggingPlugin import fileloggerrs232pkgwrite
#from plugins.rawserver.RawTCPClientPlugin import rawtcpclientinit
#from plugins.rawserver.RawTCPClientPlugin import rawtcpclientargs
from plugins.rawclient import *
from packets.LocalParserPlugin import localparserargs, localparserinit

from packets.events.CanEventPlugin import caneventargs, caneventinit

if __name__ == '__main__':
    """
    The main candpy application. It's basicaly a routing between the main plugins.
    The Plugins itself can also do some extra routing
    """
    parser = optparse.OptionParser(
        usage="%prog [options] [port [baudrate]]",
        description="Labor Cand in python ",
        epilog="""\
NOTE: no security measures are implemented. Anyone can remotely connect
to this service over the network.
""")

    parser.add_option("-q", "--quiet",
        dest="quiet",
        action="store_true",
        help="suppress non error messages",
        default=False
    )

    # get port and baud rate from command line arguments or the option switches

    ### registering rs232readplugins here

    def dummyhook(data):
        #print "dummyhook was called"
        #print data
        pass
    # create a hook for the event if something appears on the
    # serial device
    rs232readplugins = plugins.Hook('daslabor.cand.rs232.read')
    # send the data to plugins which can perform the data
    # rs232readplugins.register(dummyhook, 'localparser')
    # rs232readplugins.register(dummyhook, 'remoterawclient')
    rs232readplugins.register(fileloggerrs232pkgread, 'logger')

    ### register raw input
    rawinputreadplugins = plugins.Hook('daslabor.cand.rawtcpclient.read')

    ### register cmdargs plugins
    # every plugin maybe needs some commandlinearguments
    # so we pass the global optparser to them

    argsplugins = plugins.Hook('daslabor.cand.cmdargs')
    #argsplugins.register(serialargs, 'serial')
    argsplugins.register(fileloggerargsrs232pkg, 'logger')
    #argsplugins.register(rawtcpclientargs, 'rawtcpclient')
    argsplugins.register(tcpClientArgs, 'rawtcpclient')
    argsplugins.register(localparserargs, 'localparser')
    argsplugins.register(caneventargs, 'canevent')

    ### register init plugins
    # here the actual plugins are startet with the 
    # parsed commandlinearguments

    initplugins = plugins.Hook('daslabor.cand.init')
    #initplugins.register(serialinit, 'serial')
    initplugins.register(fileloggerinitrs232pkg, 'logger')
    #initplugins.register(rawtcpclientinit, 'rawtcpclient')
    initplugins.register(tcpClientInit, 'rawtcpclient')
    initplugins.register(localparserinit, 'localparser')
    initplugins.register(caneventinit, 'canevent')

    ### register write plugins

    wrplugin = plugins.Hook('daslabor.cand.rs232.write')
    wrplugin.register(fileloggerrs232pkgwrite, 'logger')

    ### notify about init
    plugins.Hook('daslabor.cand.cmdargs').notify([parser, plugins])

    ### parse commandline args and inform plugins
    (options, args) = parser.parse_args()

    plugins.Hook('daslabor.cand.init').notify([options, args, parser, plugins])

    try:
        while True:
            print "mainloop"
            sleep(10)
    except KeyboardInterrupt:
        pass
