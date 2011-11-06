#ifndef _LCAP_NODE_H
#define _LCAP_NODE_H

/****************************************************************************
 * LCAP Node
 *
 * This file provides the basic building blocks to implement an LCAP
 * Node. For a full description of this protocol visit
 * http://www.das-labor.org/wiki/LCAP
 *
 * Author: Hansinator (sexyludernatascha [at] gmail.com)
 * License: GPLv2+
 * Version: 0
 */

#include <inttypes.h>

/*
 * Try to include lcap protocol header.
 */
#ifndef _LCAP_H
	#include "../lcap-protocol/lcap-h"
#endif

/*
 * Define a raw CAN2.0B message if necessary.
 *
 * The field "id" is assumed to have the following structure:
 * 	Bit 28:18 -> 11 Bit CAN Standard Identifier
 * 	Bit 17:00 -> 18 Bit CAN Extended Identifier
 * 	(Bit intervals are inclusive)
 *
 * The field "dlc" is assumed to have the following structure:
 * 	Bit 4	-> Remote Transmission Request Flag
 *  Bit 3:0 -> CAN Data Length Code (DLC)
 *
 * The field "data" contains 0 to 8 data bytes.
 *
 * For more information on CAN frames, see Wikipedia:
 *  http://en.wikipedia.org/wiki/Controller_area_network#Data_frame
 */
#ifndef _CAN_H
	typedef struct
	{
		uint32_t id;
		uint8_t dlc;
		uint8_t data[8];
	} can_message;
#endif

/*
 * Decode a CAN2.0B message to LCAP
 */
void lcap_decode_can_msg(can_message *can_msg, lcap_msg_t *lcap_msg);

/*
 * Encode an LCAP message to CAN2.0B
 */
void lcap_encode_can_msg(lcap_msg_t *lcap_msg, can_message *can_msg);

#endif // _LCAP_NODE_H
