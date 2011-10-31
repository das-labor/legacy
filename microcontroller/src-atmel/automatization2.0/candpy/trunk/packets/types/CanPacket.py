#!/usr/bin/env python

from utils.utils import hexdump


class CanPacket(object):
    def __init__(self, sa=0, sp=0, da=0, dp=0,
                 data=bytearray(""), rawpkt=None):
        self.setSrcAddr(sa)
        self.setSrcPort(sp)
        self.setDestAddr(da)
        self.setDestPort(dp)
        self.data = self.setData(data)
        if rawpkt:
            self.__from_array(rawpkt)

    def setDLC(self, dlc):
        if dlc in range(0, 9):
            self.dlc = dlc
        else:
            raise ValueError("dlc must in range(0,9)")

    def getDLC(self):
        return self.dlc

    def setData(self, data):
        self.data = bytearray(data)
        print "laenge:" + str(len(data))
        self.setDLC(len(data))

    def getData(self):
        return self.data

    def setDestAddr(self, da):
        if da in  range(0, 256):
            self.da = da
        else:
            raise ValueError("destination addr must in range(0,256)")

    def getDestAddr(self):
        return self.da

    def setSrcAddr(self, sa):
        if sa in  range(0, 256):
            self.sa = sa
        else:
            raise ValueError("source addr must in range(0,256)")

    def getSrcAddr(self):
        return self.sa

    def setSrcPort(self, sp):
        if sp in  range(0, 64):
            self.sp = sp
        else:
            raise ValueError("source port must in range(0,64)")

    def getSrcPort(self):
        return self.sp

    def setDestPort(self, dp):
        if dp in  range(0, 64):
            self.dp = dp
        else:
            raise ValueError("destination port must in range(0,64)")

    def getDestPort(self):
        return self.dp

    def __from_array(self, rawpkt):
        """ Parse packet from RAW (byte-)array """
        broken = True
        arr = bytearray(rawpkt)
        if not broken:
            print hexdump(rawpkt)
            i = 0
            for x in arr[:4]:
                i = (i + x) << 8
            self.setDestAddr(i & 0xff)
            self.setSrcAddr((i >> 8) & 0xff)
            self.setSrcPort((i >> 23) & 0x3f)
            self.setDestPort(((i >> 17) & 0x30) | ((i >> 16) & 0x0f))
            ### arr[4] is discarded b/c it's dlc for data - we don't need that
            self.setData(arr[5:])
        else:
            self.setDestAddr(arr[0])
            self.setSrcAddr(arr[1])
            self.setDestPort(((arr[2] & 0x60) >> 1) + (arr[2] & 0x0f))
            self.setSrcPort(((arr[3] & 0x1f) << 1) + ((arr[2] & 0x80) >> 7))
            print hexdump(arr)
            self.setData(arr[5:])

    def genRawPKT(self):
        """
        on my way
        """
        i = (self.sa << 8) | self.da | (self.sp << 23)
        i = i | ((self.dp & 0x30) << 17) | ((self.dp & 0x0f) << 16)
        arr = bytearray(4)
        for x in range(3, -1, -1):
            arr[x] = ((i >> (x * 8) & 0xff))

        arr.append(self.dlc)
        arr.extend(bytearray(self.data))
        self.rawpkg = arr
        return arr

    def __str__(self):
        return "%02x:%02x -> %02x:%02x %s" % (self.sa, self.sp,
                                              self.da, self.dp,
                                              hexdump(self.data))
