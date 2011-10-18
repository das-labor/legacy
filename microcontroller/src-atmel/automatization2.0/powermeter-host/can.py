
import socket

def hexdump(data):
    ret = ""
    for c in data:
        ret += "%02x " % c
    return ret

class CANPacket:
    def __init__(self, pkt=None):
        pass
            
 
class LAPPacket:
    def __init__(self, pkt=None):
        if pkt is None:
            self.sa = 0
            self.sp = 0
            self.da = 0
            self.dp = 0
            self.data = bytearray("")
            self.dlc = len(self.data)
        else:
            self.da = pkt[0]
            self.sa = pkt[1]
            self.dp = ((pkt[2] & 0x60) >> 1) + (pkt[2] & 0x0f)
            self.sp = ((pkt[3] & 0x1f) << 1) + ((pkt[2] & 0x80) >> 7)
            self.data = pkt[5:]
            self.dlc = len(self.data)

    def __str__(self):
        return "%02x:%02x -> %02x:%02x: %s" % (self.sa, self.sp, self.da, self.dp, hexdump(self.data))


class CANSocket:
    def __init__(self, host="10.0.1.2", port=2342):
        BUFFER_SIZE = 1024

        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((host, port))

        self.sock = s
        self.buf = bytearray("")

    def close(self):
        self.sock.close()

    def get_pkt_nb(self):
        s = self.sock
        buf = self.buf
        
        # Receive something
        buf += bytearray(s.recv(20))
        
        # Is there something at all?
        if len(buf) == 0:
            return None

        # Did we receive a full packet?
        l = buf[0]
        if len(buf)-2 < l:
            return None

        # Is it a CAN packet?
        if buf[1] != 0x11:
            return None

        next = l+2
            
        pkt = buf[2:next]
        self.buf = buf[next:]
        return pkt
    
    def get_pkt(self):
        pkt = self.get_pkt_nb()
        while pkt is None:
            pkt = self.get_pkt_nb()
        return pkt
