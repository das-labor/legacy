__all__ = ["tcpClientArgs", "tcpClientInit"]

import optparse
from TcpClientThread import TcpClientThread

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
