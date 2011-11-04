#!/usr/bin/env python

import Queue
from LAPPacket import LAPPacket
#from utils.utils import hexdump

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
            #print "packetsplitter: laenge: " + str(l)
            testpkt = LAPPacket(rawpkt=self.streamdata[0:l])
            #print "packetspitter: data: " + hexdump(self.streamdata[0:l])
            #print "packetsplitter: status: " + str(testpkt.isValid())
            if testpkt.isValid():
                self.packetqueue.put(testpkt)
                self.streamdata = self.streamdata[l:]
                if len(self.streamdata) > 0:
                    l = self.streamdata[0] + 2
            
            #print "packetsplitter laenge: "+str(len(self.streamdata))
        #if len(self.streamdata) >20:
        #    self.streamdata=self.streamdata[20:]
            #print hexdump(self.streamdata)

    def getNextPackage(self):
        ""
        return self.packetqueue.get()
        pass
