#!/usr/bin/env python

import threading
import Queue
from time import sleep
from packets.types.CanPacket import CanPacket


class CanEvent(threading.Thread):
    def __init__(self, plugins):
        self.processqueue = Queue.Queue()
        self.plugins = plugins
        self.running = True
        self.lock=threading.Lock()
        self.counter = 0
        threading.Thread.__init__(self)
        pass

    def run(self):
        print "CanEventParser started"
        while self.running:
            try:
                package = self.processqueue.get()
                # we are out off sync sometimes
                print "canpacket: " + str(package)
                self.plugins.Hook('daslabor.cand.canpkg.read').notify(package)
            except Exception, msg:
                print msg
                sleep(1)
                pass

    def addData(self, data):
        self.lock.acquire()
        package=CanPacket(rawpkt=data)
        self.processqueue.put(package)
        self.lock.release()


