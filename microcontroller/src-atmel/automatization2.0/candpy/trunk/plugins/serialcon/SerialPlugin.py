#!/usr/bin/env python

import threading
import Queue
import optparse
from time import sleep
import serial
from exceptions import Exception


class SerialReader(threading.Thread):
    def __init__(self, serialconnection, plugins):
        self.serial = serialconnection
        self.plugins = plugins
        self.running = True
        threading.Thread.__init__(self)

    def setRunning(self, running):
        self.running = running

    def run(self):
        while self.running:
            sleep(1)
            data = "asdf"

            if data:
                self.plugins.Hook('connector.rs232.read').notify(data)

#    def run(self):
#        while self.running:
#            data = self.serial.read(1)              # read one, blocking
#            n = self.serial.inWaiting()             # look if there is more
#            if n:
#                data = data + self.serial.read(n)   # and get as much as possible
#            if data:
#                self.plugins.Hook('connector.rs232').notify(data)


class SerialWriter(threading.Thread):
    def __init__(self, serialconnection):
        self.serialqueue = Queue.Queue()
        self.serial = serialconnection
        self.running = True
        threading.Thread.__init__(self)

    def addText(self, text):
        self.serialqueue.put(bytearray(text))

    def setRunning(self, running):
        self.running = running

    def run(self):
        while self.running:
            try:
                self.serial.write(self.serialqueue.get())
            except Exception, msg:
                print msg
                sleep(1)
                pass


def serialargs(data):
    parser = data[0]
    #plugins=data[1]
    group = optparse.OptionGroup(parser,
        "Serial Port",
        "Serial port settings"
    )
    parser.add_option_group(group)

    group.add_option("-p", "--port",
        dest="port",
        help="port, a number (default 0) or a device name",
        default=None
    )

    group.add_option("-b", "--baud",
        dest="baudrate",
        action="store",
        type='int',
        help="set baud rate, default: %default",
        default=115200
    )

    group.add_option("", "--parity",
        dest="parity",
        action="store",
        help="set parity, one of [N, E, O], default=%default",
        default='N'
    )

    group.add_option("--rtscts",
        dest="rtscts",
        action="store_true",
        help="enable RTS/CTS flow control (default off)",
        default=False
    )

    group.add_option("--xonxoff",
        dest="xonxoff",
        action="store_true",
        help="enable software flow control (default off)",
        default=False
    )

    group.add_option("--rts",
        dest="rts_state",
        action="store",
        type='int',
        help="set initial RTS line state (possible values: 0, 1)",
        default=None
    )

    group.add_option("--dtr",
        dest="dtr_state",
        action="store",
        type='int',
        help="set initial DTR line state (possible values: 0, 1)",
        default=None
    )


def serialinit(data):
    options = data[0]
    args = data[1]
    parser = data[2]
    plugins = data[3]

    port = options.port
    baudrate = options.baudrate
    if args:
        if options.port is not None:
            parser.error("no arguments are allowed, options only when --port is given")
        port = args.pop(0)
        if args:
            try:
                baudrate = int(args[0])
            except ValueError:
                parser.error("baud rate must be a number, not %r" % args[0])
            args.pop(0)
        if args:
            parser.error("too many arguments")
    else:
        if port is None:
            port = 0

    # connect to serial port
    ser = serial.Serial()
    ser.port = port
    ser.baudrate = baudrate
    ser.parity = options.parity
    ser.rtscts = options.rtscts
    ser.xonxoff = options.xonxoff
    ser.timeout = 1     # required so that the reader thread can exit

    try:
        ser.open()
    except serial.SerialException, e:
        print ("Could not open serial port %s: %s\n" % (ser.portstr, e))
        ser = None

    if options.rts_state is not None:
        ser.setRTS(options.rts_state)

    if options.dtr_state is not None:
        ser.setDTR(options.dtr_state)

    # ugly but needed for plugin
    swriter = SerialWriter(ser)
    swriter.setDaemon(True)
    swriter.start()

    serialwrplugin = plugins.Hook('connector.rs232.write')
    serialwrplugin.register(swriter.addText, 'default')

    sreader = SerialReader(ser, plugins)
    sreader.setDaemon(True)
    sreader.start()

    pass
