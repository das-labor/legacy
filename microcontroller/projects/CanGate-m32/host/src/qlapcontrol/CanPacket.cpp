
#include "CanPacket.h"


CanPacket::CanPacket(){
}

CanPacket::CanPacket(quint8 addr_s, quint8 port_s, quint8 addr_d, quint8 port_d, QByteArray * dat){
	addr_src = addr_s; port_src = port_s;
	addr_dest = addr_d; port_dest = port_d;
	if(dat){
		data = *dat;
	}
}

void CanPacket::fromCann(QByteArray packet ){
	addr_src = packet[0];
	addr_dest = packet[1];
	port_src = packet[2];
	port_dest = packet[3];
	
	data = packet.mid(5, packet[4]);
}

QByteArray * CanPacket::toCann(){
	QByteArray *packet = new QByteArray;
	(*packet)[0] = data.size()+5;
	(*packet)[1] = 0x11;
	(*packet)[2] = addr_src;
	(*packet)[3] = addr_dest;
	(*packet)[4] = port_src;
	(*packet)[5] = port_dest;
	
	(*packet) += (char)data.size();
	
	(*packet) += data;
	
	return packet;
}
