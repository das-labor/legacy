
/*
 * CRC Algorithms:
 *  NOTE: CRC notation represents binary strings a polynomials; e.g. x^n is the nth bit of the 
 *  binary string x, where the bits of x are zero-indexed (LSB is bit 0).
 *
 *  The general formula is: M(x) * x^n = Q(x) * G(x) + R(x)
 *      M(x) is the original message polynomial
 *      M(x) * x^n is the original message with n zeroes appended to the end (binary multiplication)
 *      G(x) is the degree-n generator polynomial
 *      R(x) is the remainder polynomial, which is used as the CRC checksum
 *      Q(x) is the quotient polynomial (not used for anything)
 *
 *  The sender transmits the message as: (M(x) * x^n) | R(x), putting the remainder in the place
 *  of the zeroes.
 *  The receiver checks whether M(x) * x^n - R(x) is divisible by G(x). If it is, the receiver
 *  assumes that the received message is correct.
 *
 * To decode:
 *      2. CRC16 -- M(x) is 8 bytes (64 bits) maximum
 *          a) Receiver gets [recvd = (M(x) * x^16 + R(x))] -- 80 bits maximum
 *          b) M(x) * x^16 - R(x): [hash_check = recvd & 0x0000 - recvd & 0xffff]
 *          c) if ([hash_check / G(x)] == 0) CRC good
 *          d) else CRC bad
 */


void packet_make_crc5(TOKEN_PACKET_T *token_packet)
// Calculates a CRC5 for the token packet, and places the result in the packet's CRC5 field.
{
    uint16_t input;

    // extract relevant fields
    input = (flip_byte(token_packet->addr, ADDR_BITS) << ENDP_BITS) | 
             flip_byte(token_packet->endp, ENDP_BITS);
    // calculate crc, and place in packet
    token_packet->crc5 = make_crc5(input, ENDP_BITS + ADDR_BITS);
}


uint8_t packet_validate_crc5(TOKEN_PACKET_T *token_packet)
// Validates that the CRC5 for a packet is correct. Returns 0 for incorrect, non-zero for
// correct.
{
    uint16_t input;

    input = (token_packet->addr << ENDP_BITS) | token_packet->endp;
    return token_packet->crc5 == make_crc5(input, ENDP_BITS + ADDR_BITS);
}


uint16_t make_crc5(uint16_t input, uint8_t bit_count)
{
    const uint16_t crc_number = 5; // for crc5
    // the polynomial coefficients
    const uint8_t poly = 0x05; 
    // all 1s of the same length as the polynomial
    const uint8_t crc = 0x1f; 
    // mask for integer's most significant bit
    const uint16_t int_msbit = (1 << (sizeof(uint16_t) * 8 - 1)); 
    // contains the polynomial divisor in its most significant bits
    const uint16_t poly_msb = (poly << (sizeof(uint16_t) * 8 - crc_number));

    // starts as a bunch of 1's in its most significant bits, ends up being the crc value
    uint16_t crc_output = (crc << (sizeof(uint16_t) * 8 - crc_number));
    // the input value, shifted to the most significant bits
    uint16_t input_msb = (input << (sizeof(uint16_t) * 8 - bit_count));

    // iterate through the bits of the input, dividing essentially by polynomial long division
    while (bit_count-- > 0) {
        // check to see if the MSB of the current state of the input is a 1...
        if ( (input_msb ^ crc_output) & int_msbit ) { 
            // shift out the MSB from the remainder
            crc_output <<= 1;
            // subtract the polynomial coefficients (divisor) from the remainder (dividend)
            crc_output ^= poly_msb;
        }
        else { // MSB is a 0
            // just shift out the MSB
            crc_output <<= 1;
        }
        // and then shift out the input's MSB
        input_msb <<= 1;
    }

    // shift back into position
    // shift the remainder back to fill up the least significant bits
    crc_output >>= (sizeof(uint16_t) * 8 - crc_number);

    // invert contents to generate crc field
    return crc_output ^ crc;
}


void packet_make_crc16(DATA_PACKET_T *dp)
// Calculates a CRC16 for the data packet, and places the result in the packet's CRC16 field.
{
    uint8_t data_packet_array[dp->payload_size];
    uint8_t iter;

    for (iter = 0; iter < dp->payload_size; iter++) {
        data_packet_array[iter] = flip_byte(dp->data[iter], CHAR_BITS);
    }
    dp->crc16 = make_crc16(data_packet_array, dp->payload_size * CHAR_BITS);
}


uint8_t packet_validate_crc16(DATA_PACKET_T *dp)
// Validates that the CRC16 for a packet is correct.
{
    // TODO: why is this char * cast necessary?
    return (dp->crc16 == make_crc16((char *) dp->data, dp->payload_size * CHAR_BITS));
}


uint16_t make_crc16(uint8_t *input, uint16_t bit_count)
{
    #define crc_number 16 // for crc16
    #define crc 0xffff // all 1's of the same length as the polynomial
    #define char_msbit (1 << (CHAR_BITS - 1))

    size_t num_bytes = ((bit_count-1) >> 3) + 1; // number of bytes in input array

    uint8_t *poly_msb;
    uint8_t *crc_output;
    uint8_t *input_msb;
    uint16_t crc_return;
    uint8_t i;

    // poly_msb contains the polynomial divisor in its most significant bits
    poly_msb = calloc(num_bytes, sizeof(uint8_t));
    poly_msb[num_bytes - 1] = 0x80;
    poly_msb[num_bytes - 2] = 0x05;

    // starts as a bunch of 1's in its most significant bits, ends up being the crc value
    crc_output = calloc(num_bytes, sizeof(uint8_t));
    crc_output[num_bytes - 1] = 0xff;
    crc_output[num_bytes - 2] = 0xff;

    // input_msb is made a local copy of the input
    input_msb = calloc(num_bytes, sizeof(uint8_t));
    
    // for the last byte we have to shift the least significant bits into the most significant bits
    for(i = 0; i < num_bytes; i++) {
      input_msb[i] = input[i];
    }
   
    // iterate through the bits of the input, dividing essentially by polynomial long division
    while (bit_count-- > 0) {
        // check to see if the MSB of the current state of the input is a 1...
        if ((input_msb[num_bytes - 1] ^ crc_output[num_bytes - 1]) & char_msbit) {
            // shift out the MSB from the remainder
            for(i = num_bytes - 1; i > 0; i--) {
                crc_output[i] = (crc_output[i] << 1) | (crc_output[i-1] >> (CHAR_BITS - 1));
            }
            crc_output[0] <<= 1;

            // subtract the polynomial coefficients (divisor) from the remainder (dividend)
            for(i = 0; i < num_bytes; i++) {
                crc_output[i] ^= poly_msb[i];
            }
        }
        else { // MSB is a 0
            // just shift out the MSB
            for(i = num_bytes - 1; i > 0; i--) {
                crc_output[i] = (crc_output[i] << 1) | (crc_output[i-1] >> (CHAR_BITS - 1));
            }
            crc_output[0] <<= 1;
        }
        // and then shift out the input's MSB
        for( i = num_bytes - 1; i > 0; i-- ) {
            input_msb[i] = (input_msb[i] << 1) | (input_msb[i-1] >> (CHAR_BITS - 1));
        }
        input_msb[0] <<= 1;
    }

    // Shift back into position
    crc_return = crc_output[num_bytes - 2] | (crc_output[num_bytes - 1] << 8); // put the remainder into an int

    return crc_return ^ crc; // invert contents to generate crc field

    #undef char_msbit
    #undef crc
    #undef crc_number
}
