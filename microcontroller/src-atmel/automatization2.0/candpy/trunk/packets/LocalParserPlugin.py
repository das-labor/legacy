#!/usr/bin/env python

import threading
import Queue
from time import sleep
from packets.LAPPacket import LAPPacket
from packets.types.CanPacket import CanPacket
from utils.utils import hexdump
from PacketSplitterModule import PacketSplitter


class LocalParserPlugin(threading.Thread):
    def __init__(self, plugins):
        self.processqueue = Queue.Queue()
        self.plugins = plugins
        self.running = True
        self.leftover = bytearray(0)
        self.psplitter = PacketSplitter()
        threading.Thread.__init__(self)

    def run(self):
        print "LocalParserPlugin started"
        while self.running:
            try:
                print "localparser"
                package = self.psplitter.getNextPackage()
                #print hexdump(package.getPayload())
                # we are out off sync sometimes
                print "object=" + str(type(package))
                print "type=" + str(package.getPKTtype())
                self.plugins.Hook('daslabor.cand.rawtcpserver.read',
                                  package.getPKTtype()).notify(package.getPayload())
                print "/localparser"
            except Exception, msg:
                print msg
                sleep(1)
                pass

    def addData(self, data):
        self.psplitter.extendStream(data)
        #self.processqueue.put(bytearray(data))


def localparserargs(data):
    pass


def localparserinit(data):
    #options=data[0]
    #args=data[1]
    #parser=data[2]
    plugins = data[3]
    lparserThread = LocalParserPlugin(plugins)
    lparserThread.setDaemon(True)
    lparserThread.start()
    localparserplugin = plugins.Hook('daslabor.cand.rawtcpserver.read')
    localparserplugin.register(lparserThread.addData,)


if __name__ == '__main__':
    rL = bytearray(0)
    rL.extend([0x0a, 0x11, 0x00, 0x05, 0x06,  0x03,
               0x05, 0x0b, 0x44, 0x00, 0x00, 0x00])
    lp = LAPPacket(rawpkt=rL)
    #print lp.getPayload()
    cp = CanPacket(rawpkt=lp.getPayload())
    print "Encoded LAPPacket should be"
    print "0a 11 00 05 06 03 05  0b 44 00 00 00"
    print "Decoded CANPaket should be "
    print "05:06 -> 00:06    0b 44 00 00 00"
    print cp
    cp = CanPacket(sa=0x05, da=0x00, sp=0x06, dp=0x06)
    cp.setData([0x0b, 0x44, 0x00, 0x00, 0x00])
    print cp
    print hexdump(cp.genRawPKT())
