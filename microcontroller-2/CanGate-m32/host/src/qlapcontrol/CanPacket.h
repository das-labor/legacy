

#include <QByteArray>

/*
class CanFilter{
	public:
		CanFilter(uint src_a = 0xffff, uint dest_a = 0xffff, uint src_p = 0xffff, uint dest_p = 0xffff);
	
}
*/

class CanPacket{
	
	public:
		CanPacket();
		CanPacket(quint8 addr_s, quint8 port_s, quint8 addr_d, quint8 port_d, QByteArray *dat=0);
		void fromCann(QByteArray packet );
		QByteArray * toCann();
	
		quint8 addr_src, addr_dest, port_src, port_dest;
		QByteArray data;

};
