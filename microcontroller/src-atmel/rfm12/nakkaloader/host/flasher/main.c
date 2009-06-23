/* Utility functions for receiving and transmitting data from and to the target device.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * Copyright (c) Soeren Heisrath <forename@surname.org>
 */

//from stdinclude path
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <usb.h>

//include c driver interface
#include "../../../rfm12usb/host/CDriver/RfmUsb.h"

//project specific
#include "../../common/nl_protocol.h"
#include "config.h"

//include the crc function
#include "crc.h"


#ifdef WIN32
#define	usleep(x) Sleep(x)
#else
#include <unistd.h>
#include <signal.h>
#endif


/* defines for the expect function */
#define EXP_ADD   0x00 /* add an entry to the list */
#define EXP_MATCH 0x01 /* match against current list */
#define EXP_DEL   0x02 /* delete a single entry */

/**
   GLOBALS
**/

//rfmusb packetbuffer; stores received packets
rfmusb_packetbuffer packetBuffer;
rfmusb_dev_handle *udhandle = NULL;

//usage
void nf_usage()
{
	printf(
		"NakkaFlash Utility\r\n\r\n"
		"  Usage: nakkaflash -f firmware-file -a device_address\r\n\r\n"
		"  -f Specifies the Firmware file (binary formwat)\r\n"
		"  -a Specifies the device Address (in hex)\r\n\r\n"
		"Example: nakkaflash foo.bin 0xba\r\n"
	);
}


void nf_exit (int in_signal)
{
	printf ("L: %i\n", __LINE__);
        printf ("\r\nNakkaflash closing...\r\n");
        exit (in_signal);
}


#ifdef WIN32
void winexit(void)
{
    nf_exit(0);
}
#endif


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


/* @description this function maintains a list of valid packet types for each state of the
 * flasher and can be used to match a given packet against them.
 */
uint_fast8_t nl_packet_match (uint_fast8_t in_len, rfmusb_packetbuffer *in_packet, uint_fast8_t in_function)
{
	static uint8_t valid_packet_types[256];
	static uint_fast8_t listlen = 0;
	uint_fast8_t i;

	switch (in_function)
	{
		case EXP_ADD:
			if (1 + listlen > 256) return 0; /* list is full (unlikely) */

			//add type to list
			valid_packet_types[listlen++] = in_len;
			return 1;

		case EXP_MATCH:
			if (in_len < 2) return 0;
			for (i=0;i<listlen;i++)
				if (valid_packet_types[i] == (uint8_t) in_packet->buffer[0]) return valid_packet_types[i];
			return 0;

		case EXP_DEL:
			for (i=0;i<listlen;i++)
			{
				if (valid_packet_types[i] == in_len)
				{
					if (i+1 < listlen)
						valid_packet_types[i] = valid_packet_types[listlen];
					listlen--;
					return listlen;
				}
			}
			return 0;
	}
}

int nl_tx_packet(uint8_t nl_type, uint8_t addr, uint8_t len, uint8_t * data)
{
    nl_packet pkt;

    //check len
    //sub 2 for type and 1-byte addr (FIXME)
    if(len > (sizeof(nl_packet) - 2))
    {
        return -512;
    }

    //form packet
    pkt.pkttype = nl_type;
    pkt.payload[0] = addr;
    memcpy(pkt.payload + 1, data, len);

    //transmit packet
    return rfmusb_TxPacket (udhandle, NL_PACKETTYPE, len + 2, (unsigned char *)&pkt);
}


/* THIS IS SO F****NG CRUDE, STAY AWAY !!! */

#define CHUNK_TRANSFER_SIZE (sizeof(pktbuf) - sizeof(nl_flashcmd))
void push_page(uint8_t dst, uint8_t *buf, size_t size)
{
	uint8_t *ptr = buf;
	int off = 0;
	rfmusb_packetbuffer packetBuffer;
	//-2 == type + address
	//8 == pagenum, addr start, addr end == sizeof(nl_flashcmd)
    uint8_t pktbuf[RFM12_BUFFER_SIZE-2];
    uint16_t crc16;


    //first tx
    ((nl_flashcmd *)&pktbuf)->pagenum = 0;
    ((nl_flashcmd *)&pktbuf)->addr_start = 0;
    printf("L: %i\n");
    if(size < CHUNK_TRANSFER_SIZE)
    {
        ((nl_flashcmd *)&pktbuf)->addr_end = size;
        memcpy(pktbuf + sizeof(nl_flashcmd), ptr, size);
        crc16 = calc_crc(pktbuf + sizeof(nl_flashcmd), size);
        off = size;
    }
    else
    {
        ((nl_flashcmd *)&pktbuf)->addr_end = CHUNK_TRANSFER_SIZE;
        memcpy(pktbuf + sizeof(nl_flashcmd), ptr, CHUNK_TRANSFER_SIZE);
        crc16 = calc_crc(pktbuf + sizeof(nl_flashcmd), CHUNK_TRANSFER_SIZE);
        off += CHUNK_TRANSFER_SIZE;
    }
    printf("L: %i\n");

   /* int halk;
    for(halk = 0; halk < sizeof(pktbuf); halk++)
    {
        printf("%.2x ", pktbuf[halk]);
    }
    printf("\n");*/
    printf("L: %i\n");

    nl_tx_packet(NLPROTO_PAGE_FILL, dst, sizeof(pktbuf), (unsigned char*)&pktbuf);
    printf("L: %i\n");

	while(off < size) {
		while(1)
		{
			int8_t tmp = rfmusb_RxPacket (udhandle, &packetBuffer);

			if (tmp > 0)
			{
			    if ((packetBuffer.buffer[1] == dst) && (packetBuffer.type == NL_PACKETTYPE) && (packetBuffer.buffer[0] == NLPROTO_PAGE_CHKSUM))
			    {
                    if(crc16 != *(uint16_t *)(packetBuffer.buffer + 2))
                    {
                        printf("CRC mismatch! Client: %.4x, Host: %.4x\n", *(uint16_t *)(packetBuffer.buffer + 2), crc16);
                    }

                    break;
			    }

			    printf("ERR 0x%.2x\n", packetBuffer.buffer[0]);
			}

            usleep (250);
		}

		printf(".");

        ((nl_flashcmd *)&pktbuf)->addr_start = off;
        if((size - off) < CHUNK_TRANSFER_SIZE)
        {
            ((nl_flashcmd *)&pktbuf)->addr_end = size;
            memcpy(pktbuf + sizeof(nl_flashcmd), ptr + off, size - off);
            crc16 = calc_crc(pktbuf + sizeof(nl_flashcmd), size - off);
            off = size;
        }
        else
        {
            ((nl_flashcmd *)&pktbuf)->addr_end = off + CHUNK_TRANSFER_SIZE;
            memcpy(pktbuf + sizeof(nl_flashcmd), ptr + off, CHUNK_TRANSFER_SIZE);
            crc16 = calc_crc(pktbuf + sizeof(nl_flashcmd), CHUNK_TRANSFER_SIZE);
            off += CHUNK_TRANSFER_SIZE;
        }

        nl_tx_packet(NLPROTO_PAGE_FILL, dst, sizeof(pktbuf), (unsigned char*)&pktbuf);
	}

	//wait for last ack
    while(1)
    {
        int8_t tmp = rfmusb_RxPacket (udhandle, &packetBuffer);

        if (tmp > 0)
        {
            if ((packetBuffer.buffer[1] == dst) && (packetBuffer.type == NL_PACKETTYPE) && (packetBuffer.buffer[0] == NLPROTO_PAGE_CHKSUM))
            {
                if(crc16 != *(uint16_t *)(packetBuffer.buffer + 2))
                {
                    printf("CRC mismatch! Client: %.4x, Host: %.4x\n", *(uint16_t *)(packetBuffer.buffer + 2), crc16);
                }

                break;
            }

            printf("ERR 0x%.2x\n", packetBuffer.buffer[0]);
        }

        usleep (250);
    }

	printf(".");
}

void dump_page(uint8_t *mem, uint32_t pageStart, uint32_t pageEnd)
{
    int x;
    for(x = pageStart; x < pageEnd; x++)
    {
        printf("%.2x ", mem[x]);
    }
    printf("\n");

    /*for(x = pageStart; x < pageEnd; x++)
    {
        printf("%c", mem[x]);
    }
    printf("\n");*/
}

void flash(char * filename, uint8_t addr, uint16_t pageSize, uint8_t pageCount)
{
    //page transfer related variables
    unsigned int j;
    unsigned int pageNum;

    //hex file reading variables
    size_t size, dst;
    uint8_t *buf;

    //packet stuff
    nl_flashcmd flashcmd;


    //open input file
    printf( "Flashing file: %s\n", filename );
	FILE *fd = fopen(filename,"r");


	//allocate ATMega memory
	uint8_t *mem  = malloc(pageCount * pageSize);
	uint8_t *mask = malloc(pageCount * pageSize);
	memset( mem,  0xff, pageCount * pageSize );
	memset( mask, 0x00, pageCount * pageSize );


    //read in hex file
	while ((buf = read_buf_from_hex(fd, &size, &dst)) != 0)
	{
		memcpy( &mem[dst], buf, size);
		memset( &mask[dst], 0xff, size );
		free(buf);
	}
	if (size != 0)
		goto fileerror;


    //for every page do
	for(pageNum = 0; pageNum < pageCount; pageNum ++)
	{
	    //calculate page boundaries
	    int pageStart = (pageNum * pageSize);
	    int pageEnd = ((pageNum * pageSize) + pageSize);

	    //the j loop is for scanning the page for data-to-write
	    //this transfers a whole page, as long as there's something in it, somewhere
		for(j = pageStart; j < pageEnd; j++)
		{
		    //data found
			if (mask[j] == 0xff)
			{
				// trasfer page stating at i
				printf("Transmitting page 0x%.4x ", pageNum);
				push_page(addr, &(mem[pageStart]), pageSize);
				printf("\n");

                dump_page(mem, pageStart, pageEnd);
                printf("\n");
                break;
			}
		}

        //this skips writing this page, as we found no data
		if (mask[j] != 0xff)
		{
		    continue;
		}

		//commit page
        flashcmd.pagenum = pageNum;
        nl_tx_packet(NLPROTO_PAGE_COMMIT, addr, sizeof(nl_flashcmd), (unsigned char*)&flashcmd);

        while(1)
        {
            int8_t tmp = rfmusb_RxPacket (udhandle, &packetBuffer);

            if (tmp > 0)
            {
                if ((packetBuffer.buffer[1] == addr) && (packetBuffer.type == NL_PACKETTYPE) && (packetBuffer.buffer[0] == NLPROTO_PAGE_COMMITED))
                {
                    //see if pageNum matches
                    if(pageNum != ((nl_flashcmd *)&packetBuffer.buffer[2])->pagenum)
                    {
                        printf("PageNum mismatch! Client: %.4x, Host: %.4x\n", ((nl_flashcmd *)&packetBuffer.buffer[2])->pagenum, pageNum);
                    }

                    break;
                }

                printf("ERR 0x%.2x\n", packetBuffer.buffer[0]);
            }

            usleep (250);
        }
	}


    //cleanup
	free(mem);
	free(mask);
	fclose(fd);


	//send app boot
    nl_tx_packet(NLPROTO_BOOT, addr, 0, NULL);

	return;

fileerror:
	printf( "Given file is not in Intel Hex format");
	return;
}

int main (int argc, char* argv[])
{
        //usb stuff
	int vid, pid, tmp;

	//various variables
        uint8_t tmpchar = 0x00;
	uint_fast8_t ptype;
	uint_fast32_t packetcounter = 0;
	nf_config_t *myconfig;
	nl_config slave_cfg;
	int istate = 0;

        //config stuff
	myconfig = malloc(sizeof(nf_config_t));
	myconfig->verbosity = 0;

        //try to open the device
	if (rfmusb_Connect(udhandle) != 0)
	{
		printf ("Can't find RfmUSB Device!\r\n");
                //FIXME -> what's this?
		//sig_cleanup(__LINE__ * -1);
		return __LINE__ * -1;
	}

//#ifndef WIN32 & 0
#if 0
	signal (SIGINT, nf_exit);
	signal (SIGKILL, nf_exit);
	signal (SIGHUP, nf_exit);
#else
//    atexit((void * )winexit);
#endif
	printf ("L: %i\n", __LINE__);


/*
	if(nf_parse_args (argc, argv, myconfig) == 0)
	{
		nf_usage();
		exit(0);
	}
*/
	myconfig->fname = malloc (255);
	strncpy (myconfig->fname, argv[1], strlen(argv[1]));

	printf ("L: %i\n", __LINE__);

	myconfig->addr = 0xff;

	printf ("Waiting for slave %x\r\n\r\n", myconfig->addr);

	nl_packet_match(NLPROTO_SLAVE_CONFIG, NULL, EXP_ADD);
	nl_packet_match(NLPROTO_MASTER_EHLO, NULL, EXP_ADD);

	while (23)
	{
		//try to fetch a packet from the air
		tmp = rfmusb_RxPacket (udhandle, &packetBuffer);

		if(tmp > 0)
		{
            printf("RX 0x%.2x\n", packetBuffer.buffer[0]);
		}

		//if an error occurs...
		if (tmp < 0)
		{
			fprintf (stderr, "USB error: %s\r\n", usb_strerror());

			exit (__LINE__ * -1);
		}

		//verify that this is a valid packet from the right slave with the right type
		else if ((packetBuffer.buffer[1] == myconfig->addr) && (packetBuffer.type == NL_PACKETTYPE) && ( ptype = nl_packet_match(tmp, &packetBuffer, EXP_MATCH)))
		{
			//see what to do for given packet type
			switch (ptype)
			{
				//slave has sent it's configuration
				case NLPROTO_SLAVE_CONFIG:
                    if((istate == 0) || (istate == 1))
                    {
                        istate = 1;
                        memcpy(&slave_cfg, packetBuffer.buffer + 2, sizeof(nl_config));

                        //printf("got slave config!\nPagesize: %i\nAir Packet Size: %i\nVersion: %i\n", ((nl_config *)(packetBuffer.buffer + 2))->pagesize, ((nl_config *)(packetBuffer.buffer + 2))->rxbufsize, ((nl_config *)(packetBuffer.buffer + 2))->version);
                        printf("Got slave config!\nPagesize: %i\nAir Packet Size: %i\nVersion: %i\nAddress: %i\n\n", slave_cfg.pagesize, slave_cfg.rxbufsize, slave_cfg.version, packetBuffer.buffer[1]);

                        //reply now
                        nl_tx_packet(NLPROTO_MASTER_EHLO, myconfig->addr, 0, NULL);
                    }
				break;

				//slave has ACKed EHLO, ready to accept pagefills
				case NLPROTO_MASTER_EHLO:
                    if(istate == 1)
                    {
                        printf("Slave is ready to be flashed now.\n");

                        //flash
                        flash(myconfig->fname, myconfig->addr , slave_cfg.pagesize, 128);

                        //ready again
                        istate = 2;
                    }
				break;
			}
		}

		//flashing done
		if(istate == 2)
		{
		    printf("Done!\n");

            break;
		}

		//this is done to prevent stressing the usb connection too much
		usleep (1000);
	}
}
