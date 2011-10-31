#!/usr/bin/env python

import logging
import optparse


def fileloggerargsrs232pkg(data):
    parser = data[0]
    #plugins=data[1]

    group = optparse.OptionGroup(parser,
        "Loggin",
        "logging to a file"
    )
    parser.add_option_group(group)

    group.add_option("", "--logfilename",
        dest="logfilename",
        help="the logfile to which all logging should go. default: %default",
        default="/tmp/cand.rs232pkg.read.log"
    )
    pass


def fileloggerinitrs232pkg(data):
    pass
    options = data[0]
    #args=data[1]
    #parser=data[2]
    #plugins=data[3]

    #setup
    logging.basicConfig(filename=options.logfilename,
                        format="%(asctime)s - %(message)s",
                        level=logging.DEBUG)


def fileloggerrs232pkgread(data):
    logging.info("read on serial: " + str(data).encode("hex"))


def fileloggerrs232pkgwrite(data):
    logging.info("write to serial: " + str(data).encode("hex"))
