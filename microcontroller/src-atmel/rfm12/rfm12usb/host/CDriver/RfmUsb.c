/* Utility functions for receiving and transmitting data from and to the target device.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * Copyright (c) Soeren Heisrath <forename@surname.org>
 */

#include <usb.h>
#include "RfmUsb.h"

//common includes
#include "../../common/usb_id.h"
#include "../../common/requests.h"

//move to common header soon
#define RADIO_TXBUFFER_HEADER_LEN 2
int rfmusb_TxPacket (rfmusb_dev_handle *udhandle, unsigned char type, unsigned char len, unsigned char *data)
{
    rfmusb_packetbuffer buf;
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
        RFMUSB_RQ_RFM12_PUT, 0, 0, (char *)&buf, packetLen,
        DEFAULT_USB_TIMEOUT);
}


int rfmusb_RxPacket (rfmusb_dev_handle *udhandle, rfmusb_packetbuffer * packetBuffer)
{
    //clear buffer
    memset (packetBuffer, 0x00, sizeof(rfmusb_packetbuffer));

    //request raw packet and return length
    return usb_control_msg (udhandle,
            USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
            RFMUSB_RQ_RFM12_GET, 0, 0, (char *)packetBuffer, sizeof(rfmusb_packetbuffer),
            DEFAULT_USB_TIMEOUT);
}
