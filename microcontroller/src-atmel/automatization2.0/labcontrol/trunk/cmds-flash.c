#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "lib-host/can.h"
#include "lib/lap.h"

#include "cmds-flash.h"
#include "lib-host/debug.h"

/*
    * : is the colon that starts every Intel HEX record.
    * ll is the record-length field that represents the number of data bytes (dd) in the record.
    * aaaa is the address field that represents the starting address for subsequent data in the record.
    * tt is the field that represents the HEX record type, which may be one of the following:
      00 - data record
      01 - end-of-file record
      02 - extended segment address record
      04 - extended linear address record
    * dd is a data field that represents one byte of data. A record may have multiple data bytes. The number of data bytes in the record must match the number specified by the ll field.
    * cc is the checksum field that represents the checksum of the record. The checksum is calculated by summing the values of all hexadecimal digit pairs in the record modulo 256 and taking the two's complement.
*/

//reads one line from a hex file.
//result is stored in the returned buffer, which is supposed to be freed
//by the caller.
//size and offset return the size of the line and the address offset
//of the line respectivly.
uint8_t *read_line_from_hex(FILE *f, size_t *size, size_t *offset)
{
	uint32_t tmp;
	uint8_t *buf;

	uint8_t checksum = 0;

	//for 64-bit systems the fscanf below will only fill
	//the lower 32bit, so use temporary int

	if (fscanf(f, ":%2x", &tmp) != 1)
		goto error;
	*size = tmp;
	checksum += tmp;

	if (fscanf(f, "%4x", &tmp) != 1)
		goto error;
	*offset = tmp;
	checksum += tmp;
	checksum += tmp>>8;

	if (fscanf(f, "%2x", &tmp) != 1)
		goto error;
	checksum += tmp;

	if (tmp == 1) // end of file mark
	{
		*size  = 0;
		return 0;
	}

	buf = malloc( *size * sizeof(uint8_t) );
	debug_assert(buf != 0, "Could not allocate hex image data buffer!");

	size_t i = *size;
	uint8_t *ptr = buf;

	for (;i > 0; i--)
	{
		if (fscanf(f, "%2hx", (uint16_t *) &tmp) != 1)
			goto error;

		checksum += tmp;
		*ptr++ = tmp;
	}

	if (fscanf(f, "%2x", &tmp) != 1)	 // checksum  //XXX: checksumm is only fetched and not checked
		goto error;

	checksum = -checksum;

	debug_assert(checksum == tmp, "Checksum Error while reading hex file, offset = 0x%x, 0x%02x != 0x%02x", *offset, checksum, tmp);

	fscanf(f, "\n"); // XXX fetch error

	return buf;

error:
	debug_assert(0, "Format Error while reading hex file");
	return 0;
}

void push_page(can_addr dst, uint8_t *buf, size_t offset, size_t size)
{
	time_t timeout;

	can_message *msg = can_buffer_get();

	msg->addr_src = 0x00;
	msg->addr_dst = dst;
	msg->port_src = PORT_SDO;
	msg->port_dst = PORT_SDO;
	msg->dlc = 7;
	msg->data[0]  = SDO_CMD_WRITE_BLK;
	msg->data[1]  = 0x01;
	msg->data[2]  = 0xff;
	msg->data[3]  = size & (0xff);
	msg->data[4]  = size>>8;
	msg->data[5]  = offset & (0xff);
	msg->data[6]  = offset>>8;

	can_transmit(msg);

	timeout = time (NULL);

	while (1)
	{
		can_message *incoming = can_get();
		if ((incoming != NULL) && incoming->data[0]  == SDO_CMD_WRITE_BLK_ACK &&
		    incoming->addr_src == dst)
		{
			free(incoming);
			break;
		}
		if((time (NULL)) - timeout > 3)		/* 3 seconds timeout */
		{
			can_transmit(msg);
			timeout = time (NULL);
		}
	}

	free(msg);

	uint8_t *ptr = buf;
	int missing = size;

	while (missing > 0)
	{
		can_message *to_transmit = can_buffer_get();
		to_transmit->addr_src = 0x00;
		to_transmit->addr_dst = dst;
		to_transmit->port_src = PORT_SDO;
		to_transmit->port_dst = PORT_SDO_DATA;
		if (missing > 8)
			to_transmit->dlc   = 8;
		else
			to_transmit->dlc   = missing;
		memcpy(to_transmit->data, ptr, to_transmit->dlc);

		ptr += to_transmit->dlc;
		missing -= to_transmit->dlc;


		can_transmit(to_transmit);

		timeout = time (NULL);

		while (1)
		{
			can_message *incoming = can_get();
			if ((incoming != NULL) && incoming->data[0]  == SDO_CMD_WRITE_BLK_ACK &&
				incoming->addr_src == dst)
			{
				free(incoming);
				break;
			}
			if((time (NULL)) - timeout > 3)		/* 3 seconds timeout */
			{
			can_transmit(msg);
			timeout = time (NULL);
			}
		}
	}
}


void flash_atmel(unsigned char addr, unsigned int pagesize, char *filename)
{
	sdo_message *smsg = (sdo_message *)can_buffer_get();
	smsg->addr_dst = addr;
	smsg->addr_src = 0x00;  
	smsg->port_src = PORT_SDO;
	smsg->port_dst = PORT_SDO;
	smsg->dlc      = 3;
	smsg->cmd      = SDO_CMD_READ;
	smsg->index    = 0xff01;

	can_transmit((can_message *)smsg);

	can_message *rxmsg;
	for (;;)
	{
		rxmsg = can_get();

		if ((rxmsg != NULL) && rxmsg->port_dst == PORT_SDO && rxmsg->addr_src == addr)
		{
			break;
		}
	}

	uint16_t flashsize = (uint16_t)(rxmsg->data[3] << 8) + (uint16_t)rxmsg->data[2];
	printf("Flash Size: 0x%x\n", flashsize);
	free(rxmsg);
	
	// allocate ATMega memory
	uint8_t *mem  = malloc(flashsize);
	debug_assert(mem != 0, "Could not allocate mem data buffer!");
	
	uint8_t *mask = malloc(flashsize);
	debug_assert(mask != 0, "Could not allocate mask buffer!");
	
	memset(mem,  0xff, flashsize);
	memset(mask, 0x00, flashsize);

	printf("Flashing file: %s\n", filename);
	FILE *fd = fopen(filename,"r");
	if(!fd)
	{
		printf("Error opening file %s\n", filename);
		exit(1);
	}

	size_t size, offset;
	uint8_t *buf;
	while ((buf = read_line_from_hex(fd, &size, &offset)) != 0)
	{
		debug_assert((size+offset) <= flashsize, "hex file contains addresses larger than AVR Flashsize!");
		memcpy(&mem[offset], buf, size);    //copy the section to our buffer
		memset(&mask[offset], 0xff, size);  //remember that these bytes need to be programmed by setting mask
		free(buf);
	}
	
	int i, j;

	for (i = 0; i < flashsize; i += pagesize)
	{
		for (j = i; j < i + pagesize; j++)
		{
			if (mask[j] == 0xff)
			{
				// if at least one byte in the page is used
				// we trasfer the page stating at i
				printf("Transmitting %4x...\n", i);
				push_page(addr, &(mem[i]), i, pagesize);
				break;
			}
		}
	}

	fclose(fd);

	free(mem);
	free(mask);

	printf("Sendig reset\n");
	smsg = (sdo_message *)can_buffer_get();
	smsg->addr_dst = addr;
	smsg->addr_src = 0x00;  
	smsg->port_src = PORT_SDO;
	smsg->port_dst = PORT_SDO;
	smsg->dlc      = 3;
	smsg->cmd      = SDO_CMD_READ;
	smsg->index    = 0xff02;

	can_transmit((can_message *)smsg);
}



void flash_commodore(unsigned char addr, char *filename)
{
	sdo_message *smsg = (sdo_message *)can_buffer_get();
	smsg->addr_dst = addr;
	smsg->addr_src = 0x00;  
	smsg->port_src = PORT_SDO;
	smsg->port_dst = PORT_SDO;
	smsg->dlc      = 3;
	smsg->cmd      = SDO_CMD_READ;
	smsg->index    = 0xff01;

	can_transmit((can_message *)smsg);

	for (;;)
	{
		smsg = (sdo_message *)can_get();

		if ((smsg != NULL) && smsg->port_dst == PORT_SDO && smsg->addr_src == addr)
		{
			break;
		}
	}

	can_message *msg3 = (can_message *)smsg;
	uint16_t flashsize = (uint16_t)(msg3->data[3]<<8) + (uint16_t)msg3->data[2];
	printf("Free Memory Size: 0x%x\n",flashsize);

	printf( "Flashing file: %s\n", filename );
	FILE *fd = fopen(filename,"r");

	if (!fd) goto fileerror;

	struct stat mystat;
	stat(filename, &mystat);

	unsigned int size = mystat.st_size-2;
	unsigned int offset = 0;
	fread(&offset, 2, 1, fd);

	printf("uploading from %X to %X\n", offset, offset+size-1);

	can_message *msg = can_buffer_get();

	msg->addr_src = 0x00;
	msg->addr_dst = addr;
	msg->port_src = PORT_SDO;
	msg->port_dst = PORT_SDO;
	msg->dlc = 7;
	msg->data[0]  = SDO_CMD_WRITE_BLK;
	msg->data[1]  = 0x01;
	msg->data[2]  = 0xff;
	msg->data[3]  = size & (0xff);
	msg->data[4]  = size>>8;
	msg->data[5]  = offset & (0xff);
	msg->data[6]  = offset>>8;

	can_transmit(msg);

	unsigned int missing = size;
	unsigned int count = 0;

	while (missing > 0)
	{
		while (1)
		{
			can_message *incoming = can_get();
			if ((incoming != NULL) && incoming->data[0]  == SDO_CMD_WRITE_BLK_ACK &&
			    incoming->addr_src == addr)
				break;
		}
		
		if ((count%0x400) == 0)
		{
			printf("Transmitting %X\n", offset+count);
		}

		can_message *to_transmit = can_buffer_get();
		to_transmit->addr_src = 0x00;
		to_transmit->addr_dst = addr;
		to_transmit->port_src = PORT_SDO;
		to_transmit->port_dst = PORT_SDO_DATA;
		if (missing > 8)
			to_transmit->dlc = 8;
		else
			to_transmit->dlc = missing;
		fread(to_transmit->data, 1, to_transmit->dlc, fd);

		count += to_transmit->dlc;
		missing -= to_transmit->dlc;

		can_transmit(to_transmit);
	}

	while (1)
	{
		can_message *incoming = can_get();
		if ((incoming != NULL) && incoming->data[0]  == SDO_CMD_WRITE_BLK_ACK &&
		    incoming->addr_src == addr)
			break;
	}

	printf("Sendig boot command\n");
	smsg->addr_dst = addr;
	smsg->addr_src = 0x00;  
	smsg->port_src = PORT_SDO;
	smsg->port_dst = PORT_SDO;
	smsg->dlc      = 3;
	smsg->cmd      = SDO_CMD_READ;
	smsg->index    = 0xff02;

	can_transmit( (can_message *)smsg);

	return;

fileerror:
	debug(0, "could not open file");
	return;

}


void cmd_flash(int argc, char *argv[])
{
	can_addr addr = 0;

	pdo_message *msg;

	if (argc != 3)
		goto argerror;

	if (sscanf(argv[1], "%i", (int*)&addr) != 1)
		goto argerror;

	lap_reset(addr);

	for (;;)
	{
		msg = (pdo_message *)can_get();


		if ((msg != NULL) && (msg->port_dst == PORT_MGT) &&  
		    (msg->cmd      == FKT_MGT_AWAKE) && 
		    (msg->addr_src == addr))
		{
			break;
		}
	}

	printf("Awake from %d...\n", msg->addr_src);

	free(msg);

	sdo_message *smsg = (sdo_message *)can_buffer_get();
	smsg->addr_dst = addr;
	smsg->addr_src = 0x00;  
	smsg->port_src = PORT_SDO;
	smsg->port_dst = PORT_SDO;
	smsg->dlc      = 3;
	smsg->cmd      = SDO_CMD_READ;
	smsg->index    = 0xff00;

	can_transmit((can_message *)smsg);

	for (;;)
	{
		smsg = (sdo_message *)can_get();

		if ((smsg != NULL) && smsg->port_dst == PORT_SDO && smsg->addr_src == addr)
		{
			break;
		}
	}

	if (smsg->cmd != SDO_CMD_REPLY)
	{
		debug(0, "Kaputt!" );
		free(smsg);
		exit(EXIT_FAILURE);
	}
	can_message *msg2 = (can_message *)smsg;
	uint16_t pagesize = (uint16_t)(msg2->data[3] << 8) + (uint16_t)msg2->data[2];

	if (msg2->data[5] == 0)
	{
		printf("Real Flashsize %d KByte\n", msg2->data[4]);
		printf("Pagesize: 0x%x\n", pagesize);
		flash_atmel(addr, pagesize, argv[2]);
	} else if (msg2->data[5] == 1)
	{
		printf("Hardware: Commodore %d\n", msg2->data[4]);
		flash_commodore(addr, argv[2]);
	} else goto wronghardware;
	free(smsg);
	return;

argerror:
	debug(0, "flash <addr> file.hex");
	return;
wronghardware:
	debug(0, "Hardware unkonown");
	return;
}

