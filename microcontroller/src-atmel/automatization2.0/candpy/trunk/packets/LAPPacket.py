#!/usr/bin/env python

from utils.utils import invertDict
from utils.utils import hexdump

class LAPPacket(object):
    LAPPacketTypes={
                    "LAP_RESET":0x00,
                    "LAP_CAN_SETFILTER":0x10,
                    "LAP_CAN_PKT":0x11,
                    "LAP_CAN_SETMODE":0x12,
                    "LAP_CAN_ERROR":0x13,
                    "LAP_CAN_NOTIFY_RESET":0x14,
                    "LAP_RS232_PING_GATEWAY":0x15, 
                    "LAP_RS232_RESYNC":0x16} 

    def __init__(self, pkttype=None, payload=bytearray(''), rawpkt=bytearray('')):
        self.knownpkts=LAPPacket.LAPPacketTypes
        self.knownpkts_inv=invertDict(LAPPacket.LAPPacketTypes)
        self.setPKTPayload(payload)
        self.pkttype=self.setPKTtype(pkttype)
        self.validPKG=False

        if rawpkt:
            self.__from_array(rawpkt)
    
    def setPKTtype(self, pkttype=None):
        """
        we are expceting a number which is registered in LAPPacketTypes
        """
        if self.knownpkts_inv.has_key(pkttype):
            self.pkttype=pkttype
            return True
        return False
        
    def getPKTtype(self):
        return self.pkttype
    
    def setPKTPayload(self, payload=bytearray('')):
        self.dlc=len(payload)
        self.payload=payload
    
    def genRawPKT(self):
        self.rawpkt=bytearray('')
        self.rawpkt.append(self.dlc)
        self.rawpkt.append(self.pkttype)
        self.rawpkt.extend(self.payload)
    
    def __str__(self):
        return self.knownpkts_inv[self.pkttype] + " " + hexdump(self.rawpkt)
    
    def getRawPKT(self):
        return self.rawpkt
    
    def getPayload(self):
        return self.payload
    
    def __from_array(self, rawpkt):
        
        if len(rawpkt) == 0:
            return None
        # Did we receive a full packet?
        bs=bytearray(rawpkt)
        l = bs[0]
        if len(rawpkt)-2 < l:
            return None
        
        ### what packet are we?
        if self.setPKTtype(bs[1]):
            self.setPKTPayload(bs[2:])
            self.validPKG=True
        self.rawpkt=rawpkt
        ### seams like we are somehow valid
    
    def isValid(self):
        return self.validPKG

if __name__ == '__main__':
    rL=bytearray(0)
    rL.extend([0x0a , 0x11 , 0x00 , 0x05 , 0x06,  0x03, 0x05, 0x0b, 0x44, 0x00, 0x00, 0x00])
    lp=LAPPacket(rawpkt=rL)
    print hexdump(lp.getPayload())
