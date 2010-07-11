#ifndef _RFMUSBDEVICE_H
#define _RFMUSBDEVICE_H

#include "UsbDevice.h"


class RfmUsbDevice
{
public:
	RfmUsbDevice();
	~RfmUsbDevice();

	bool Connect();
	bool Disconnect();
	bool IsConnected();

	int GetRequestTimeout();
	void SetRequestTimeout(int timeout);

	RfmUsbBasePacket *FetchRfmPacket();
	RfmUsbBasePacket *FetchNotification();

private:
	UsbDevice *device;
	int requestTimeout;
};


#endif //_RFMUSBDEVICE_H
