#include "protocol.h"
#include "encap.h"
#include <stdlib.h>
#include <string.h>
#include <endian.h>

tcp2air_t *prep_packet (uint8_t in_dir, uint32_t in_freq, uint16_t in_bw, uint32_t in_bps, uint32_t in_len, uint8_t *in_data)
{
	tcp2air_t *out_pkt;
	out_pkt = malloc (sizeof(tcp2air_t) + in_len);
	out_pkt->len = htonl (in_len);
	out_pkt->bps = htonl (in_bps);
	out_pkt->freq = htonl (in_freq);
	out_pkt->bwidth = htonl (in_bw);
	out_pkt->data = in_data;
	return out_pkt;
}

uint8_t *encap_packet (uint32_t *out_len, tcp2air_t *in_p)
{
	uint8_t *out = malloc (sizeof(tcp2air_t) + in_p->len);

	out[0] = in_p->dir;
	memcpy (out+1, &in_p->freq, 4);
	memcpy (out+5, &in_p->bwidth, 2);
	memcpy (out+7, &in_p->bps, 4);
	memcpy (out+11, &in_p->len, 4);
	memcpy (out+15, &in_p->data, in_p->len);

	return out;
}

tcp2air_t *decap_packet (uint8_t *in_d)
{
	tcp2air_t out_hdr;
	uint8_t *out_data;
	
	out_hdr.dir = (uint8_t) in_d[0];
	out_hdr.freq = ntohl(in_d+1);
	out_hdr.bwidth = ntohs(in_d+5);
	out_hdr.bps = ntohl(in_d+7);
	out_hdr.len = ntohl(in_d+11);
	
	out_data = malloc(out_hdr.len);
	memcpy (out_data, in_d + 15, out_hdr.len);
}
