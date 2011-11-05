__all__ = ["caneventargs", "caneventinit"]

from CanEventPlugin import CanEvent
from packets.types.LAPPacket import LAPPacket

def caneventargs(data):
    pass


def caneventinit(data):
    #options=data[0]
    #args=data[1]
    #parser=data[2]
    plugins = data[3]
    caneventThread = CanEvent(plugins)
    caneventThread.setDaemon(True)
    caneventThread.start()

    caneventplugin = plugins.Hook("packet." +
                                  str(LAPPacket.LAPPacketTypes["LAP_CAN_PKT"]) +
                                  ".read")
    caneventplugin.register(caneventThread.addData)
