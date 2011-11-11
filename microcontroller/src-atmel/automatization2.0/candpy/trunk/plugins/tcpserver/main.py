#!/usr/bin/env python

__all__ = ["tcpServerArgs", "tcpServerInit"]

import optparse
from TcpServerThread import TcpServerThread

def tcpServerArgs(data):
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


def tcpServerInit(data):
    print "got init"
    options = data[0]
    #args = data[1]
    #parser = data[2]
    plugins = data[3]

    #setup
    rThread = TcpServerThread(plugins, options)
    rThread.setDaemon(True)
    rThread.start()
