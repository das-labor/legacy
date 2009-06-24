#include "nl_flash.h"
//for memcpy and memset
#include <string.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


//fixme - this shouldn't be a global, i suppose
rfmusb_packetbuffer packetBuffer;


uint8_t *read_buf_from_hex(FILE *f, size_t *size, size_t *offset)
{
	int i, tt;
	uint8_t *buf;

	printf("%20s, %5i\n", __FILE__, __LINE__);
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
	printf("%20s, %5i\n", __FILE__, __LINE__);

	i = *size;
	printf("%20s, %5i\n", __FILE__, __LINE__);
	buf = malloc(*size);
	printf("%20s, %5i\n", __FILE__, __LINE__);
	uint8_t *ptr = buf;
	for(;i > 0; i--) {
	printf("%20s, %5i\n", __FILE__, __LINE__);
		if (fscanf(f, "%2x", ptr++) != 1)
			goto error;
	printf("%20s, %5i\n", __FILE__, __LINE__);
	}
	printf("%20s, %5i\n", __FILE__, __LINE__);
	if (fscanf(f, "%2x", &tt) != 1)	 // checksum
		goto error;

	fscanf(f, "\n");
	printf("%20s, %5i\n", __FILE__, __LINE__);

	return buf;

error:
	printf("%20s, %5i\n", __FILE__, __LINE__);
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


int nl_tx_packet(rfmusb_dev_handle *udhandle, uint8_t nl_type, uint8_t addr, uint8_t len, uint8_t * data)
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

#define CHUNK_TRANSFER_SIZE (sizeof(pktbuf) - sizeof(nl_flashcmd))
void nl_push_page(rfmusb_dev_handle *udhandle, uint8_t dst, uint8_t *buf, size_t size)
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


   /* int halk;
    for(halk = 0; halk < sizeof(pktbuf); halk++)
    {
        printf("%.2x ", pktbuf[halk]);
    }
    printf("\n");*/

    nl_tx_packet(udhandle, NLPROTO_PAGE_FILL, dst, sizeof(pktbuf), (unsigned char*)&pktbuf);

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

        nl_tx_packet(udhandle, NLPROTO_PAGE_FILL, dst, sizeof(pktbuf), (unsigned char*)&pktbuf);
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


void nl_flash(rfmusb_dev_handle *udhandle, char * filename, uint8_t addr, uint16_t pageSize, uint8_t pageCount)
{
    //page transfer related variables
    unsigned int j;
    unsigned int pageNum;

    //hex file reading variables
    size_t size, dst;
    uint8_t *buf;

    //packet stuff
    nl_flashcmd flashcmd;

	buf = malloc (256 * 1024);
	memset (buf, 0xff, 262144);
    //open input file
    printf( "Flashing file: %s\n", filename );
	int fd = open (filename, O_RDONLY);


	//allocate ATMega memory
	uint8_t *mem  = malloc(pageCount * pageSize);
	uint8_t *mask = malloc(pageCount * pageSize);
	memset( mem,  0xff, pageCount * pageSize );
	memset( mask, 0x00, pageCount * pageSize );

	printf("%20s, %5i\n", __FILE__, __LINE__);
    //read in hex file
/*	while ((buf = read_buf_from_hex(fd, &size, &dst)) != 0)
	{
		memcpy( &mem[dst], buf, size);
		memset( &mask[dst], 0xff, size );
		free(buf);
	}
*/
	size = read (fd, buf, 262144);
	printf("*********************************************\n\t\t\t\t\tread %i bytes\n", size);

	printf("%20s, %5i\n", __FILE__, __LINE__);
	if (size <= 0)
		goto fileerror;
	printf("%20s, %5i, %i, %i\n", __FILE__, __LINE__, pageCount, pageSize);


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
//			if (mask[j] == 0xff)
			{
				// trasfer page stating at i
				printf("Transmitting page 0x%.4x, %04x, %04x", pageNum, pageStart, pageEnd);
				nl_push_page(udhandle, addr, buf + (pageNum * pageSize), pageSize);
				printf("\n");

                nl_dump_page(buf, pageStart, pageEnd);
                printf("\n");
                break;
			}
		}

        //this skips writing this page, as we found no data
		if (mask[j] != 0xff)
		{
		    // continue;
		}

		//commit page
		printf("commit.\n");
        flashcmd.pagenum = pageNum;
        nl_tx_packet(udhandle, NLPROTO_PAGE_COMMIT, addr, sizeof(nl_flashcmd), (unsigned char*)&flashcmd);

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
	close(fd);


	//send app boot
	while (42)
	{
	        nl_tx_packet(udhandle, NLPROTO_BOOT, addr, 0, NULL);
		printf("boot\n");
		sleep(1);
	}

	return;

fileerror:
	printf( "Given file is not in Intel Hex format");
	return;
}
