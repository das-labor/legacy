#!/usr/bin/env python

import Queue
from packets.types.LAPPacket import LAPPacket

class PacketSplitter(object):
    """
    This module splits a stream into packages
    """

    def __init__(self):
        self.streamdata = bytearray(0)
        self.packetqueue = Queue.Queue()

    def extendStream(self, data):
        self.streamdata.extend(bytearray(data))
        # lets do a guess
        l = 0
        while(l <= (len(self.streamdata) - 2)):
            l = self.streamdata[0] + 2
            testpkt = LAPPacket(rawpkt=self.streamdata[0:l])
            if testpkt.isValid():
                self.packetqueue.put(testpkt)
                self.streamdata = self.streamdata[l:]
                if len(self.streamdata) > 0:
                    l = self.streamdata[0] + 2

    def getNextPackage(self):
        "returns the next valid LAPPacket"
        return self.packetqueue.get()
