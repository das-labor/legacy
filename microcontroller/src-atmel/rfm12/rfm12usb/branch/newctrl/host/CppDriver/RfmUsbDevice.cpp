#include "RfmUsbDevice.h"
#include "RfmUsbBasePacket.h"
#include "config.h"

//common includes
#include "../../common/usb_id.h"
#include "../../common/requests.h"


RfmUsbDevice::RfmUsbDevice()
{
	//create a new device using the appropriate IDs
	device = new UsbDevice(USB_CFG_VENDOR_ID, USB_CFG_DEVICE_ID);

	//init other vars
	requestTimeout = RFMDRV_DEFAULT_REQUEST_TIMEOUT;
}


RfmUsbDevice::~RfmUsbDevice()
{
	//cleanup
	if(device != NULL)
	{
		delete device;
	}
}


//this function returns a dynamically allocated object
//FIXME: fix length unknown beforehand problem
//FIXME: if this didn't work, we should give a NULL pointer
// -> this in turn means, we need a temp buffer in-between anyway..,
RfmUsbBasePacket *RfmUsbDevice::FetchRfmPacket()
{
	//request raw packet
	packetLen = usb_control_msg (udhandle,
			USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
			RFMUSB_RQ_RADIO_GET, 0, 0, (char *)&packetBuffer, sizeof(packetBuffer),
			DEFAULT_USB_TIMEOUT);

	RfmUsbBasePacket *packet = new RfmUsbBasePacket();

	return packet;
}


//this function returns a dynamically allocated object
//FIXME: fix length unknown beforehand problem
//FIXME: if this didn't work, we should give a NULL pointer
// -> this in turn means, we need a temp buffer in-between anyway..,
RfmUsbBasePacket *RfmUsbDevice::FetchNotification()
{
	//request notification packet
	packetLen = usb_control_msg (udhandle,
			USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
			RFMUSB_RQ_GETNOTIFICATION, 0, 0, (char *)&packetBuffer, sizeof(packetBuffer),
			DEFAULT_USB_TIMEOUT);

	RfmUsbBasePacket *packet = new RfmUsbBasePacket();

	return packet;
}


//FIXME: find proper buffer/packet formatting for both sides (host and target) of usb comm.
int RfmUsbDevice::PutRfmPacket(RfmUsbBasePacket packet&)
{
    radio_packetbuffer buf;
    int packetLen;

    //trim packet length
    packetLen = RADIO_TXBUFFER_HEADER_LEN + ((len > RFM12_MAX_PACKET_LENGTH)?RFM12_MAX_PACKET_LENGTH:len);

    //setup buffer
    buf.len = len;
    buf.type = type;

    //copy data
    memcpy(buf.buffer, data, len);

    //request to send packet and return result
    return usb_control_msg (udhandle,
        USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_OUT,
        RFMUSB_RQ_RADIO_PUT, USB_TXPACKET, 0, (char *)&buf, packetLen,
        DEFAULT_USB_TIMEOUT);
}


bool RfmUsbDevice::Connect()
{
	return device->usbConnect();
}


bool RfmUsbDevice::Disconnect()
{
	return device->usbDisconnect();
}


bool RfmUsbDevice::IsConnected()
{
	return device->IsConnected();
}


int RfmUsbDevice::GetRequestTimeout()
{
	return requestTimeout;
}


//FIXME: range checks?
void RfmUsbDevice::SetRequestTimeout(int timeout)
{
	requestTimeout = timeout;
}
