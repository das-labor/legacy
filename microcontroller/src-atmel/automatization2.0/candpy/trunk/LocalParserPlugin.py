#!/usr/bin/env python

import threading
import Queue
from time import sleep
from LAPPacket import LAPPacket
from CanPacket import CanPacket
from utils import hexdump

class LocalParserPlugin(threading.Thread):
    def __init__(self, plugins):
        self.processqueue=Queue.Queue()
        self.plugins=plugins
        self.running=True
        threading.Thread.__init__ ( self )

    def run(self):
        while self.running:
            try:
                package=bytearray(self.processqueue.get())
                print package
            except Exception, msg:
                print msg
                sleep(1)
                pass

if __name__ == '__main__':
    rL=bytearray(0)
    rL.extend([0x0a , 0x11 , 0x00 , 0x05 , 0x06,  0x03, 0x05, 0x0b, 0x44, 0x00, 0x00, 0x00])
    lp=LAPPacket(rawpkt=rL)
    #print lp.getPayload()
    cp=CanPacket(rawpkt=lp.getPayload())
    print "Encoded LAPPacket should be"
    print "0a 11 00 05 06 03 05  0b 44 00 00 00"
    print "Decoded CANPaket should be "
    print "05:06 -> 00:06    0b 44 00 00 00"
    print cp
    cp=CanPacket(sa=0x05, da=0x00, sp=0x06, dp=0x06)
    cp.setData([0x0b, 0x44, 0x00, 0x00, 0x00])
    print cp
    print hexdump(cp.genRawPKT())
