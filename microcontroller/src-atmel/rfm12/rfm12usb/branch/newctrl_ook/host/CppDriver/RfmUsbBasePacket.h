#ifndef _RFMUSBBASEPACKET_H
#define _RFMUSBBASEPACKET_H


//this class represents a rfmusb basic usb packet

class RfmUsbBasePacket
{
public:
	RfmUsbBasePacket(unsigned char type, unsigned char *data, unsigned char length);
	~RfmUsbBasePacket();

	unsigned char GetType();
	unsigned char *GetData();
	unsigned char GetLength();

private:
	unsigned char type;
	unsigned char *data;
	unsigned char length;
};


#endif //_RFMUSBBASEPACKET_H
