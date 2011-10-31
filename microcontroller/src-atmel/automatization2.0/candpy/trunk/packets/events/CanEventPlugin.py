#!/usr/bin/env python

import threading
import Queue
from time import sleep
from packets.types.CanPacket import CanPacket
from packets.LAPPacket import LAPPacket
from utils.utils import hexdump


class CanEvent(threading.Thread):
    def __init__(self, plugins):
        self.processqueue=Queue.Queue()
        self.plugins=plugins
        self.running=True
        threading.Thread.__init__ ( self )
        pass
        
    def run(self):
        print "CanEventParser started"
        while self.running:
            try:
                package=CanPacket(rawpkt=self.processqueue.get())
                # we are out off sync sometimes
                print "canparser"
                print package
                self.plugins.Hook('daslabor.cand.rawtcpserver.read',"canpkg").notify(package)
                print "/canparser"
            except Exception, msg:
                print msg
                sleep(1)
                pass
    
    def addData(self, data):
        print "canparser"
        print "laenge=" + str(len(bytearray(data)))
        print hexdump(bytearray(data))
        print "/canparser"
        #package=CanPacket(rawpkt=data)
        #self.processqueue.put(package)
        

def caneventargs(data):
    pass
    
def caneventinit(data):
    #options=data[0]
    #args=data[1]
    #parser=data[2]
    plugins=data[3]
    caneventThread=CanEvent(plugins)
    caneventThread.setDaemon(True)
    caneventThread.start()
    caneventplugin=plugins.Hook('daslabor.cand.rawtcpserver.read')
    caneventplugin.register(caneventThread.addData, LAPPacket.LAPPacketTypes["LAP_CAN_PKT"])
