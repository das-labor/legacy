#include "nl_flash.h"
//for memcpy and memset
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <endian.h>

#include "../../common/nl_protocol.h"

#ifdef WIN32
#define	usleep(x) Sleep(x)
#define htole32(x) (x)
#define htole16(x) (x)
#define le32toh(x) (x)
#else
#include <unistd.h>
#include <signal.h>
#endif

#undef RFM12_BUFFER_SIZE
#define  RFM12_BUFFER_SIZE 20

//fixme - this shouldn't be a global, i suppose
rfmusb_packetbuffer packetBuffer;


uint8_t *read_buf_from_hex(FILE *f, size_t *size, size_t *offset)
{
	int i, tt;
	uint8_t *buf;


	if (fscanf(f, ":%2x", size) != 1)
{
	printf("bla\n");
		goto error;
}

	if (fscanf(f, "%4x", offset) != 1)
{
	printf("bla1\n");
		goto error;
}

	if (fscanf(f, "%2x", &tt) != 1)
{
	printf("bla2\n");
		goto error;
}

	if(tt == 1)
	{
		*size  = 0;
		return 0;
	}


	i = *size;

	buf = malloc(*size);

	uint8_t *ptr = buf;
	unsigned int tmpbuf;
	for(;i > 0; i--)
	{
		if (fscanf(f, "%2x", &tmpbuf) != 1)
		goto error;
		*ptr++ = (uint8_t)tmpbuf & 0xff;

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
			if (in_len < 2)
				return 0;

			for (i=0;i<listlen;i++)
				if (valid_packet_types[i] == (uint8_t) in_packet->buffer[0])
					return valid_packet_types[i];
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


int nl_tx_packet(rfmusb_dev_handle *udhandle, uint8_t nl_type, uint8_t addr, uint8_t len, uint8_t * data)
{
	nl_packet pkt;
	uint8_t i;

	//CHECK LEN
	//sub 2 for type and 1-byte addr (FIXME)
	if(len > (sizeof(nl_packet) - 2))
		return -1 * __LINE__;

	//form packet
	pkt.pkttype = nl_type;
	pkt.payload[0] = addr;
	memcpy(pkt.payload + 1, data, len);

	//transmit packet
	return rfmusb_TxPacket (udhandle, NL_PACKETTYPE, len + 2, (unsigned char *)&pkt);
}


void nl_dump_page(uint8_t *mem, uint32_t pageStart, uint32_t pageEnd)
{
	int x;
	for(x = pageStart; x < pageEnd; x++)
	{
		printf("%.2x ", mem[x]);
	}
	printf("\n");
}


/* THIS IS SO F****NG CRUDE, STAY AWAY !!! */

void nl_push_page(rfmusb_dev_handle *udhandle, uint8_t dst, uint8_t *buf, size_t size, uint32_t pagenum, uint8_t rxbufsize)
{
	uint8_t *ptr = buf;
	uint32_t off = 0;
	rfmusb_packetbuffer packetBuffer;
	//-2 == type + address
	//8 == pagenum, addr start, addr end == sizeof(nl_flashcmd)
	uint8_t pktbuf[RFM12_BUFFER_SIZE-2];
	uint16_t crc16 = 0;
	uint16_t chunksize = (rxbufsize - 2 - (sizeof(nl_flashcmd) + 1));
	nl_flashcmd txcmd;
	int ret = 0;


	//first tx


	txcmd.pagenum = htole32(pagenum);
	txcmd.addr_start = htole16(0);


	if(size < chunksize)
	{
		txcmd.addr_end = htole16(size);
		memcpy(pktbuf + sizeof(nl_flashcmd), ptr, size);
		crc16 = calc_crc(pktbuf + sizeof(nl_flashcmd), size);
		off = size;
	}
	else
	{
		txcmd.addr_end = htole16(chunksize);
		memcpy(pktbuf + sizeof(nl_flashcmd), ptr, chunksize);
		crc16 = calc_crc(pktbuf + sizeof(nl_flashcmd), chunksize);
		off += chunksize;
	}
	printf("ae: %i, as: %i, p: %i, ts: %i\n", txcmd.addr_end, txcmd.addr_start, txcmd.pagenum, chunksize);
	memcpy (pktbuf, (uint8_t *) &txcmd, sizeof(nl_flashcmd));


	/* int halk;
	for(halk = 0; halk < sizeof(pktbuf); halk++)
	{
	printf("%.2x ", pktbuf[halk]);
	}
	printf("\n");*/

	ret = nl_tx_packet(udhandle, NLPROTO_PAGE_FILL, dst, sizeof(pktbuf), (unsigned char*)&pktbuf);
	printf("braaabret: %i\n", ret);
	
	while(off < size)
	{
		printf("braaar\n");
		while(1)
		{
			int8_t tmp = rfmusb_RxPacket (udhandle, &packetBuffer);

			if (tmp > 0)
			{
				if ((packetBuffer.buffer[1] == dst) && (packetBuffer.type == NL_PACKETTYPE)
					&& (packetBuffer.buffer[0] == NLPROTO_PAGE_CHKSUM))
				{
					if(crc16 != *(uint16_t *)(packetBuffer.buffer + 2))
					{
						printf("WARNING: CRC mismatch! Client: %.4x, Host: %.4x\n",
							*(uint16_t *)(packetBuffer.buffer + 2), crc16);
					}
					printf("crcok");
					
					break;
				}
				
				nl_tx_packet(udhandle, NLPROTO_PAGE_CHKSUM, dst, sizeof(pktbuf), (unsigned char*)&pktbuf);

				printf("WARNING: Unexpected response code from client: 0x%.2x\n",
					packetBuffer.buffer[0]);
			}
			usleep (50);
		}

		txcmd.addr_start = htole16(off);

		if((size - off) < chunksize)
		{
			txcmd.addr_end = htole16(size);
			memcpy(pktbuf + sizeof(nl_flashcmd), ptr + off, size - off);
			crc16 = calc_crc(pktbuf + sizeof(nl_flashcmd), size - off);
			off = size;
		} else
		{
			txcmd.addr_end = htole16(off + chunksize);
			memcpy(pktbuf + sizeof(nl_flashcmd), ptr + off, chunksize);
			crc16 = calc_crc(pktbuf + sizeof(nl_flashcmd), chunksize);
			off += chunksize;
		}

		memcpy (pktbuf, &txcmd, sizeof(nl_flashcmd));
		nl_tx_packet(udhandle, NLPROTO_PAGE_FILL, dst, sizeof(pktbuf), (unsigned char*)&pktbuf);
	}

	//wait for last ack
	while(1)
	{
		int8_t tmp = rfmusb_RxPacket (udhandle, &packetBuffer);

		if (tmp > 0)
		{
			if ((packetBuffer.buffer[1] == dst) &&
				(packetBuffer.type == NL_PACKETTYPE) &&
				(packetBuffer.buffer[0] == NLPROTO_PAGE_CHKSUM))
			{
				if(crc16 != *(uint16_t *)(packetBuffer.buffer + 2))
				{
					printf("CRC mismatch! Client: %.4x, Host: %.4x\n",
						*(uint16_t *)(packetBuffer.buffer + 2), crc16);
				}
				break;
			}

			printf("ERR 0x%.2x\n", packetBuffer.buffer[0]);
		}
		usleep (50);
	}

}


void nl_flash(rfmusb_dev_handle *udhandle, char * filename, uint8_t addr, uint16_t pageSize, uint8_t pageCount, uint8_t rxbufsize)
{
	//page transfer related variables
	unsigned int j;
	uint8_t verbose = 0;
	uint16_t pageNum;

	//hex file reading variables
	size_t size, dst;
	uint8_t *buf;

	//packet stuff
	nl_flashcmd flashcmd;

	buf = malloc (256 * 1024);
	memset (buf, 0xff, 262144);
	//open input file
	printf( "Using file: %s\n", filename );
	int fd = open (filename, O_RDONLY);
	//FILE * fd = fopen(filename, "r");

	if(fd == NULL)
	{
		printf("opening file failed... exiting!\n");
		exit(0);
	}


	//allocate ATMega memory
	uint8_t *mem  = malloc(pageCount * pageSize);
	uint8_t *mask = malloc(pageCount * pageSize);
	memset( mem,  0xff, pageCount * pageSize );
	memset( mask, 0x00, pageCount * pageSize );


	//read in hex file
	/*	while ((buf = read_buf_from_hex(fd, &size, &dst)) != 0)
	{
		memcpy( &mem[dst], buf, size);
		memset( &mask[dst], 0xff, size );
		free(buf);
	}*/
	
	size = read (fd, buf, 262144);

	if (size <= 0)
		goto fileerror;

	//debug
	//printf("%20s, %5i, %i, %i\n", __FILE__, __LINE__, pageCount, pageSize);

	//for every page do
	for(pageNum = 0; pageNum < pageCount; pageNum ++)
	{
		//calculate page boundaries
		int pageStart = htole16((pageNum * pageSize));
		int pageEnd = htole16(((pageNum * pageSize) + pageSize));

		//the j loop is for scanning the page for data-to-write
		//this transfers a whole page, as long as there's something in it, somewhere
		for(j = pageStart; j < pageEnd; j++)
		{
			//data found
			//if (mask[j] == 0xff)
			{
				// trasfer page stating at i
				printf("Transmitting page #%04u [0x%04x .. %04x] ...", pageNum, pageStart, pageEnd);
				nl_push_page(udhandle, addr, buf + (pageNum * pageSize), pageSize, pageNum, rxbufsize);
				if (verbose)
					nl_dump_page(buf, pageStart, pageEnd);
				printf(" done.\n");
				break;

			}
		}

		printf("sending commit command for page #%u\n", pageNum);
		flashcmd.pagenum = htole32(pageNum);
		nl_tx_packet(udhandle, NLPROTO_PAGE_COMMIT, addr, sizeof(nl_flashcmd), (unsigned char*)&flashcmd);

		while(1)
		{
			int8_t tmp = rfmusb_RxPacket (udhandle, &packetBuffer);

			if (tmp > 0)
			{
				if ((packetBuffer.buffer[1] == addr) &&
					(packetBuffer.type == NL_PACKETTYPE) &&
					(packetBuffer.buffer[0] == NLPROTO_PAGE_COMMITED))
				{
					/* see if pageNum matches */
					if(pageNum != le32toh(((nl_flashcmd *)&packetBuffer.buffer[2])->pagenum))
					{
						printf("WARNING: PageNum mismatch! Client: %.4x, Host: %.4x\n",
							((nl_flashcmd *)&packetBuffer.buffer[2])->pagenum, pageNum);
					}

					break;
				}

				printf("WARNING: Unexpected response code from client: 0x%.2x\n", packetBuffer.buffer[0]);
			}
			usleep (50);
		}
	}


	//cleanup
	free(mem);
	free(mask);
	close(fd);
	//fclose(fd);


	j=23;
	//send app boot
	while (j--)
	{
		nl_tx_packet(udhandle, NLPROTO_BOOT, addr, 0, NULL);
		printf("boot..");
		usleep(1);
	}

	printf("\ndone\n");
	return;

	fileerror:
		printf( "Given file is not in Intel Hex format\n");
	return;
}
