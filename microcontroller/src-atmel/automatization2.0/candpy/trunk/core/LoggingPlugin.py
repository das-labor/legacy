#!/usr/bin/env python

import logging
import optparse


def fileloggerargsrs232pkg(data):
    """
    @para: data = [optparse,plugins]
    @function: Erweitert parser um die eigenen Parameter
    
    """
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
    """
    @para: data = [parsedoptions, args, parser, plugins]
    @function: initialisiert den logger mit den uebergebenen 
        kommandozeilenargumenten
    """
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
    """
    para: Objekt, das die __str() methode unterstuetzt
    function: logging mit prefix 'read on serial:'
    """
    logging.info("read on serial: " + str(data).encode("hex"))


def fileloggerrs232pkgwrite(data):
    """
    para: Objekt, das die __str() methode unterstuetzt
    function: logging mit prefix 'write to serial:'
    """

    logging.info("write to serial: " + str(data).encode("hex"))
