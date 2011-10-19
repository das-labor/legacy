#!/usr/bin/env python

import threading
import Queue
from time import sleep

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
                

class LAPPacket(object):
    LAPPacketTypes={
                    "LAP_RESET":0x00,
                    "LAP_CAN_SETFILTER":0x10,
                    "LAP_CAN_PKT":0x11,
                    "LAP_CAN_SETMODE":0x12,
                    "LAP_CAN_ERROR":0x13,
                    "LAP_CAN_NOTIFY_RESET":0x14,
                    "LAP_RS232_PING_GATEWAY":0x15} 

    def __init__(self, pkttype=0, payload=bytearray(""), rawpkt=None):
        self.dlc=len(payload)
        self.pkttype=pkttype
        self.payload=payload
        if rawpkt:
            self.from_array(rawpkt)
            
    def from_array(self, rawpkt):
        if len(rawpkt) == 0:
            return None

        # Did we receive a full packet?
        l = rawpkt[0]
        if len(rawpkt)-2 < l:
            return None

        # what is it?
        if buf[1] != 0x11:
            return None

        next = l+2
            
        pkt = buf[2:next]
        self.buf = buf[next:]

        pass
    
class CanPacket(object):
    def __init__(self, sa=0, sp=0, da=0, dp=0, data=bytearray(""), rawpkt=None):
        self.sa=sa
        self.sp=sp
        self.da=da
        self.dp=dp
        self.data=data
        self.dlc=len(data)
        if rawpkt:
            self.from_array(rawpkt)
        
    def from_array(self, arr):
        """ Parse packet from RAW (byte-)array """
        self.da = arr[0]
        self.sa = arr[1]
        self.dp = ((arr[2] & 0x60) >> 1) + (arr[2] & 0x0f)
        self.sp = ((arr[3] & 0x1f) << 1) + ((arr[2] & 0x80) >> 7)
        self.data = arr[5:]
        self.dlc = len(self.data)

    def to_array(self):
        """
        XXX to be done
        """
        return None

    def __str__(self):
        return "%02x:%02x -> %02x:%02x: %s" % (self.sa, self.sp, self.da, self.dp, hexdump(self.data))


    
