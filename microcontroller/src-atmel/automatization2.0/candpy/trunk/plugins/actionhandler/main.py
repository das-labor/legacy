#!/usr/bin/env python

__all__ = ["actionHandlerArgs", "actionHandlerInit"]

import optparse
from ActionHandlerThread import ActionHandlerThread

def actionHandlerArgs(data):
    parser = data[0]
    #plugins=data[1]

    group = optparse.OptionGroup(parser,
        "ActionHandler",
        "ActionHandler configuration."
    )
    parser.add_option_group(group)

    group.add_option("", "--maxevents",
        dest="max_events",
        action="store",
        type='int',
        help="maximal parallel events",
        default=5
    )


def actionHandlerInit(data):
    options = data[0]
    #args = data[1]
    #parser = data[2]
    plugins = data[3]

    #setup
    ahThread = ActionHandlerThread(plugins, options)
    ahThread.setDaemon(True)
    ahThread.start()
