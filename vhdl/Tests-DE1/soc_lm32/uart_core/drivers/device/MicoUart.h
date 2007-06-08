#ifndef MICOUART_HEADER_FILE_
#define MICOUART_HEADER_FILE_
#include "DDStructs.h"

/*
 *----------------------------------------------------------
 * UART SPECIFIC PRECOMPILER DEFINES:
 *----------------------------------------------------------
 * _MICOUART_NO_INTERRUPTS_
 *   This precompiler definition, if defined, compiles-out
 *   code from the driver that relies on interrupt-driven
 *   behavior.  This precompiler setting affects ALL
 *   uart instances i.e. if defined, all uarts will operate
 *   in a non-interrupt mode.  This define tends to reduce
 *   the UART driver code-size as it compiles-out code.
 *   If any of your uart relies on interrupt-driven operation
 *   do not define this precompiler option.
 * 
 * 
 * _MICOUART_FILESUPPORT_DISABLED_
 *   This precompiler definition, if defined, compiles-out
 *   code specific to the UART that allows it to operate as a
 *   file-device.  This precompiler definition affects ALL
 *   uart instances i.e. if defined, none of the UART instances
 *   will be available for file-operations (printf, write, read,
 *   scanf); however, direct UART access through driver API can
 *   still be performed for sending/receiving data over the UART.
 *
 */
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/***************************************************************
 ***************************************************************
 *                                                             *
 *  UART PHYSICAL DEVICE SPECIFIC INFORMATION                  *
 *                                                             *
 ***************************************************************
 ***************************************************************/
    /*
     ------------------------------------------------------
     -  Fudge-factor to bits/word to increase timeout     -
     ------------------------------------------------------
     */
    #define MICOUART_TIMEOUT_FUDGE_BITS (4)


    /*
     ------------------------------------------------------
     - UART registers specific bit definitions used       -
     - in the driver implementation                       -
     ------------------------------------------------------
     */
    #define MICOUART_LSR_RX_RDY_MASK    (0x01)
    #define MICOUART_LSR_TX_RDY_MASK    (0x20)
    #define MICOUART_IER_RX_INT_MASK    (0x01)
    #define MICOUART_IER_TX_INT_MASK    (0x02)
    #define MICOUART_IIR_RXRDY          (0x04)
    #define MICOUART_IIR_TXRDY          (0x02)

    /*
     ------------------------------------------------------
     -                                                    -
     - UART Device Register-map                           -
     -                                                    -
     ------------------------------------------------------
     */
    typedef struct st_MicoUart{
        volatile unsigned int rxtx;
        volatile unsigned int ier;
        volatile unsigned int iir;
        volatile unsigned int lcr;
        volatile unsigned int mcr;
        volatile unsigned int lsr;
        volatile unsigned int msr;
        volatile unsigned int div;
    }MicoUart_t;


/***************************************************************
 ***************************************************************
 *                                                             *
 *  UART SOFTWARE DRIVER SPECIFIC INFORMATION                  *
 *                                                             *
 ***************************************************************
 ***************************************************************/
    /*
     ------------------------------------------------------
     -                                                    -
     - CONSTANTS AND TYPEDEFS                             -
     -                                                    -
     ------------------------------------------------------
     */

    #define MICOUART_ERR_WOULD_BLOCK        (1)
    #define MICOUART_ERR_DEVICE_ERROR       (2)
    #define MICOUART_ERR_INVALID_ARGUMENT   (3)


    /*
     ------------------------------------------------------
     -                                                    -
     - FUNCTIONS                                          -
     -                                                    -
     ------------------------------------------------------
     */
    void MicoUartInit(MicoUartCtx_t *ctx);
    unsigned int MicoUart_putC(MicoUartCtx_t *ctx, unsigned char ucChar);
    unsigned int MicoUart_getC(MicoUartCtx_t *ctx, unsigned char *pucChar);
    unsigned int MicoUart_setRate(MicoUartCtx_t *ctx, unsigned int baudrate);
    unsigned int MicoUart_setBlockMode(MicoUartCtx_t *ctx, unsigned int uiBlock);
    /*
     ***************************************************************
     * Changes datawidth, parity-selection and stop-bit selection
     *
     * Argument:
     *
     * MicoUartCtx_t *ctx: context to uart
     * unsigned int dwidth: data width in bits(5, 6, 7, 8)
     * unsigned int parity_en: 0 => no pareity, 1 => parity enabled
     * unsigned int even_odd (parity): - 1 => even, 0 => odd
     * unsigned int stopbits: stop bits (1,2)
     *
     ***************************************************************
     */
    unsigned int MicoUart_dataConfig(MicoUartCtx_t *ctx,
                                     unsigned int dwidth,
                                     unsigned char parity_en,
                                     unsigned char even_odd,
                                     unsigned int stopbits);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MICOUART_HEADER_FILE_ */

