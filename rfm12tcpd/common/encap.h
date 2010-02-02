#include <stdint.h>
#include "protocol.h"

#ifndef ENCAP_H
#define ENCAP_H

#define PREDICT_ENCAPSIZE(a) (a+sizeof (tcp2air_t))

tcp2air_t *prep_packet (uint8_t in_dir, uint32_t in_freq, uint16_t in_bw, uint32_t bps, uint32_t len, uint8_t *in_data);
uint8_t *encap_packet (uint32_t *out_len, tcp2air_t *in_packet);
tcp2air_t *decap_packet (uint8_t *in_d);

#endif /* ENCAP_H */
