#ifndef NO_AVR_GCC
void sie_transaction(uint8_t transfer_direction,TOKEN_PACKET_T *tp, DATA_PACKET_T *dp, uint8_t data_packet_length, uint8_t *handshake)
{
TOCKEN_PACKET_LENGTH
tp->

 
}
#endif


#ifndef NO_AVR_GCC
void sie_keepalive()
// Sends a keep alive signal port, same as a low speed EOP
{
    asm(
        "ldi r20, 0x04\n\t"
        "out %0, r20\n\t"
        NOP8
        "ldi r20, 0x04\n\t"
        "out %0, r20\n\t"
        NOP8
        "ldi r20, 0x05\n\t"
        "out %0, r20\n\t"

        : /* no input registers */
        : /* output registers */
          "I" (_SFR_IO_ADDR(USBPORT))
        : /* clobbers */
          "r20"
        );
    
    // Resume idle state.
    sie_idle();
}
#endif


#ifndef NO_AVR_GCC
void sie_reset()
// Holds a SE0 on the lines. It's up to the user to set the idle state after 10 ms.
{
    asm(
        "ldi r20, 0x04\n\t"
        "out %0, r20\n\t"
        : /* no input registers */
        : "I" (_SFR_IO_ADDR(USBPORT)) /* outputs */
        : "r20" /* clobbered */
        );
}
#endif


#ifndef NO_AVR_GCC
void sie_resume()
// Sends a resume signal. It's again up to the user to set the idle state after 20 ms.
{
    asm(
        "ldi r20, 0x06\n\t" // differential 1 (K)
        "out %0, r20\n\t"
        : /* no input registers */
        : "I" (_SFR_IO_ADDR(USBPORT)) /* output */
        : "r20" /* clobbered */
        );
}
#endif


#ifndef NO_AVR_GCC
uint8_t sie_detect_device()
// Detects if there is a low-speed device attached to the USB port.
{
    char result=0;

    /* There is a device connected (or activity on the lines), but we need to figure out what.
     * The values that we could see:
     * 0x40: Low-speed device connected.
     * 0x80: Full-speed device connected.
     * Else: Nothing we know of.
     */
    asm(    "sbis %1, 6\n\t"
            "ldi %0, 1\n\t"
            "sbis %1, 7\n\t"
            "ldi %0, -1\n\t"
            : "=d" (result) /* result gets written. */
            : "I" (_SFR_IO_ADDR(USBPIN))
        );

    return result;
}
#endif
