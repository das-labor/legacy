#!/usr/bin/env python

import Queue
from LAPPacket import LAPPacket


class PacketSplitter(object):
    """
    This module splits a stream into packages
    """

    def __init__(self):
        self.streamdata = bytearray(0)
        self.packetqueue = Queue.Queue()
        pass

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
            #print "packetsplitter laenge: "+str(len(self.streamdata))
        #if len(self.streamdata) >20:
        #    self.streamdata=self.streamdata[20:]
            #print hexdump(self.streamdata)

    def getNextPackage(self):
        ""
        return self.packetqueue.get()
        pass
