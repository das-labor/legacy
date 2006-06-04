#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "can.h"
#include "lap.h"

#include "cmds-flash.h"


uint8_t *read_buf_from_hex(FILE *f, size_t *size, size_t *offset)
{
	int i, tt;
	uint8_t *buf;

	if (fscanf(f, ":%2x", size) != 1)
		goto error;

	if (fscanf(f, "%4x", offset) != 1)
		goto error;

	if (fscanf(f, "%2x", &tt) != 1)
		goto error;

	if(tt == 1) {
		*size  = 0;
		return 0;
	}

	i = *size;
	buf = malloc(*size);
	uint8_t *ptr = buf;
	for(;i > 0; i--) {
		if (fscanf(f, "%2x", ptr++) != 1)
			goto error;
	}
	if (fscanf(f, "%2x", &tt) != 1)	 // checksum
		goto error;

	fscanf(f, "\n");

	return buf;

error:
	*size = -1;
	return 0;
}

void push_page( can_addr dst, uint8_t *buf, size_t offset, size_t size )
{

	
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


	uint8_t *ptr = buf;
	int missing = size;

	while(missing > 0) {
		while(1) {
			can_message *incoming = can_get();
			if (incoming->data[0]  == SDO_CMD_WRITE_BLK_ACK &&
			    incoming->addr_src == dst)
				break;
		}

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
	}

	while(1) {
		can_message *incoming = can_get();
		if (incoming->data[0]  == SDO_CMD_WRITE_BLK_ACK &&
		    incoming->addr_src == dst)
			break;
	}
}

void cmd_flash(int argc, char *argv[]) 
{
	can_addr addr;

	pdo_message *msg;

	if (argc != 3)
		goto argerror;

       	if (sscanf(argv[1], "%i", &addr) != 1)
		goto argerror;

	lap_reset(addr);

	for(;;) {
		msg = (pdo_message *)can_get();


		if (msg->port_dst == PORT_MGT &&  
		    msg->cmd      == FKT_MGT_AWAKE && 
		    msg->addr_src == addr) {
			break;
		}
	}

	printf( "Awake from %d...\n", msg->addr_src );

	sdo_message *smsg = (sdo_message *)can_buffer_get();
	smsg->addr_dst = addr;
	smsg->addr_src = 0x00;  
	smsg->port_src = PORT_SDO;
	smsg->port_dst = PORT_SDO;
	smsg->dlc      = 3;
	smsg->cmd      = SDO_CMD_READ;
	smsg->index    = 0xff00;

	can_transmit( (can_message *)smsg);

	for(;;) {
		smsg = (sdo_message *)can_get();

		if (smsg->port_dst == PORT_SDO && smsg->addr_src == msg->addr_src) {
			break;
		}
	}

	if (smsg->cmd != SDO_CMD_REPLY ) {
		debug(0, "Kaputt!" );
		exit(1);
	}

	can_message *msg2 = (can_message *)smsg;
	uint16_t pagesize = (uint16_t)(msg2->data[3]<<8) + (uint16_t)msg2->data[2];

	printf("Hardware: ATMEGA %d\n", msg2->data[4]);
	printf("Pagesize: 0x%x\n",pagesize);


	smsg->addr_dst = msg->addr_src;
	smsg->addr_src = 0x00;  
	smsg->port_src = PORT_SDO;
	smsg->port_dst = PORT_SDO;
	smsg->dlc      = 3;
	smsg->cmd      = SDO_CMD_READ;
	smsg->index    = 0xff01;

	can_transmit( (can_message *)smsg);

	for(;;) {
		smsg = (sdo_message *)can_get();

		if (smsg->port_dst == PORT_SDO && smsg->addr_src == msg->addr_src) {
			break;
		}
	}

	can_message *msg3 = (can_message *)smsg;
	uint16_t flashsize = (uint16_t)(msg3->data[3]<<8) + (uint16_t)msg3->data[2];
	printf("Flashsize: 0x%x\n",flashsize);
	
	// allocate ATMega memory
	uint8_t *mem  = malloc(flashsize);
	uint8_t *mask = malloc(flashsize);
	memset( mem,  0xff, flashsize );
	memset( mask, 0x00, flashsize );


	printf( "Flashing file: %s\n", argv[2] );
	FILE *fd = fopen(argv[2],"r");
	
	
	size_t size, dst;
	uint8_t *buf;
	while ((buf = read_buf_from_hex(fd, &size, &dst)) != 0) {
		memcpy( &mem[dst], buf, size);
		memset( &mask[dst], 0xff, size );
	}

	if (size != 0)
		goto fileerror;

	int i,j;

	for( i=0; i<flashsize; i += pagesize) {
		for(j=i; j<i+pagesize; j++) {
			if (mask[j] == 0xff) {
				// trasfer page stating at i
				printf("Transmitting %4x...\n", i);
				push_page(addr, &(mem[i]), i, pagesize);
				break;
			}
		}
	}

	printf("Sendig reset\n");
	smsg->addr_dst = msg->addr_src;
	smsg->addr_src = 0x00;  
	smsg->port_src = PORT_SDO;
	smsg->port_dst = PORT_SDO;
	smsg->dlc      = 3;
	smsg->cmd      = SDO_CMD_READ;
	smsg->index    = 0xff02;

	can_transmit( (can_message *)smsg);

	return;
	
argerror:
	debug(0, "flash <addr> file.hex");
	return;

fileerror:
	debug(0, "Given file is not in Intel Hex format");
	return;
}


