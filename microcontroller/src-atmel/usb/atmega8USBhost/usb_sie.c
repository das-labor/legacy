

#ifndef NO_AVR_GCC
uint8_t sie_receive(uint8_t *data, uint8_t *data_length)
{


 
}
#endif


#ifndef NO_AVR_GCC
uint8_t sie_send(uint8_t *data, uint8_t data_length)
{


 
}
#endif

#ifndef NO_AVR_GCC
uint8_t sie_idle()
{
  USBDDR=0; //make USBport input
  USBPORT=0;  //remove pullups
}
#endif


#ifndef NO_AVR_GCC
void sie_keepalive()
// Sends a keep alive signal port, same as a low speed EOP (SE0 2bits + idyle 1 bit)
{
   USBDDR=0x3; //make USBport output
    asm(
        "ldi r20, 0x01\n\t"
        "out %0, r20\n\t"
        NOP8
        NOP8
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
  USBDDR=0x3; //make USBport output
    asm(
        "ldi r20, 0x03\n\t"
        "out %0, r20\n\t"
        : /* no input registers */
        : "I" (_SFR_IO_ADDR(USBPORT)) /* outputs */
        : "r20" /* clobbered */
        );
    wait(10);
}
#endif


#ifndef NO_AVR_GCC
void sie_resume()
// Sends a resume signal. It's again up to the user to set the idle state after 20 ms.
{
  USBDDR=0x3;  //make USBport output
    asm(
        "ldi r20, 0x06\n\t" // differential 1 (K)
        "out %0, r20\n\t"
        : /* no input registers */
        : "I" (_SFR_IO_ADDR(USBPORT)) /* output */
        : "r20" /* clobbered */
        );
    wait(20);
}
#endif


#ifndef NO_AVR_GCC
uint8_t sie_detect_device()
// Detects if there is a low-speed device attached to the USB port.
{
    char result=0;
    USBDDR=0x0; //make USBport input
    
    /* There is a device connected (or activity on the lines), but we need to figure out what.
     * The values that we could see:
     * 0x01: Low-speed device connected.
     * 0x02: Full-speed device connected.
     * Else: Nothing we know of.
     */
    asm(    "sbis %1, 0\n\t"
            "ldi %0, 1\n\t"
            "sbis %1, 1\n\t"
            "ldi %0, -1\n\t"
            : "=d" (result) /* result gets written. */
            : "I" (_SFR_IO_ADDR(USBPIN))
        );

    return result;
}
#endif

#ifndef NO_AVR_GCC
void sie_start_keepalive_interrupt()
{
  //timer0 
  OCR0 = 0; //clear timer0 
  TCCR0= _BV(CS00)|_BV(CS01); //prescaler 1:64 = 0,9ms
  TIMSK|= _BV(TOIE0); //Timer0 OVF Match Interrupt Enable
}
#endif

#ifndef NO_AVR_GCC
void sie_stop_keepalive_interrupt()
{
  TCCR0 =0;  //disable timer0 clock source
}
#endif

ISR(TIMER0_OVF_vect)
{
  sie_keepalive();
}
