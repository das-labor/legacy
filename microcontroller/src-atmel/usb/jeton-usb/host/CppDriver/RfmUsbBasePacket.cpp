#include "RfmUsbBasePacket.h"
#include <algorithm>


//FIXME add range check in positive direction
//FIXME probably use a vector class instead for internal storage
RfmUsbBasePacket::RfmUsbBasePacket(unsigned char type, unsigned char *data, unsigned char length)
{
	//check if the packet has got some length
	if(length)
	{
		//save values
		this->type = type;
		this->length = length;

		//create buffer
		this->data = new unsigned char[length];

		if(data != NULL)
		{
			copy(data, data + length - 1, this->data);
		}
	}
	//make this invalid
	else
	{
		this->type = 0;
		this->data = NULL;
		this->length = 0;
	}
}


RfmUsbBasePacket::~RfmUsbBasePacket()
{
	//cleanup
	if(data != NULL)
	{
		delete data;
	}
}


unsigned char RfmUsbBasePacket::GetType()
{
	return type;
}


unsigned char RfmUsbBasePacket::*GetData()
{
	return data;
}


unsigned char RfmUsbBasePacket::GetLength()
{
	return length;
}