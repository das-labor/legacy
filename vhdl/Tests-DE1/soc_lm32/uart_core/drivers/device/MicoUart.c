#include "MicoUart.h"
#include "MicoMacros.h"
#include "MicoInterrupts.h"
#include "MicoUartService.h"
#include "MicoUtils.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


/*
 ***************************************************************
 * UART ISR: Automatically optimized-out (it's a "static") if
 * _MICOUART_NO_INTERRUPTS_ is defined as part of preprocessor
 * settings.  Note this preprocessor setting affects ALL
 * UART instances.
 ***************************************************************
*/
static void MicoUartISR(unsigned int intrLevel, void *ctx)
{
    MicoUartCtx_t *uart;
    volatile MicoUart_t *dev;
    unsigned int iir;
    unsigned int ier;
    
    uart = (MicoUartCtx_t *)ctx;
    dev = (volatile MicoUart_t *)(uart->base);

    /* interrogate interrupt-cause */
    iir = dev->iir;
    ier = dev->ier;

    /* see if the interrupt is due to rx-data */
    switch(iir){
        case MICOUART_IIR_RXRDY:{
            /* read the data into the buffer and adjust pointers */
            uart->rxBuffer[uart->rxWriteLoc] = dev->rxtx;
            uart->rxWriteLoc++;
            if(uart->rxWriteLoc >= uart->rxBufferSize)
                uart->rxWriteLoc = 0;
            uart->rxDataBytes++;
            if(uart->rxDataBytes == uart->rxBufferSize){
                /* disable rx interrupts as buffer's full */
                uart->ier &= (~MICOUART_IER_RX_INT_MASK);
                dev->ier = uart->ier;
            }
        }break;
        case MICOUART_IIR_TXRDY:{
            /* see if there's stuff to transmit */
            if(uart->txDataBytes == 0){
                /* disable tx interrupts as there's no more data to send */
                uart->ier &= (~MICOUART_IER_TX_INT_MASK);
                dev->ier = uart->ier;
                return;
            }           
            dev->rxtx = uart->txBuffer[uart->txReadLoc];
            uart->txReadLoc++;
            if(uart->txReadLoc >= uart->txBufferSize)
                uart->txReadLoc = 0;
            uart->txDataBytes--;
        }break;
        default:{
            /* 
             * This will never happen unless someone's reading rx
             * in parallel.  If using interrupts, isr is the only
             * code that should be reading rx
             */
        }break;
    }

    /* all done */
    return;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */





/*
 ***************************************************************
 * Initializes UART instance
 ***************************************************************
*/
void MicoUartInit(MicoUartCtx_t *ctx)
{
    volatile MicoUart_t *dev;
    dev = (volatile MicoUart_t *)(ctx->base);

    /* set the baudrate to workaround RTL bug */
    MicoUart_setRate(ctx, ctx->baudrate);

    /* set the data-configuration */
    MicoUart_dataConfig(ctx,
                        ctx->databits,
                        0,
                        0,
                        ctx->stopbits);
 
#ifndef _MICOUART_NO_INTERRUPTS_
    /* If interrupts are available use interrupt-mode */
    if(ctx->intrAvail){

        /* initialize buffer-related parameters */
        ctx->txDataBytes = 0;
        ctx->rxDataBytes = 0;
        ctx->txReadLoc  = 0;
        ctx->txWriteLoc = 0;
        ctx->rxReadLoc  = 0;
        ctx->rxWriteLoc = 0;

        /* 
         * Don't enable transmit interrupt as the write-function takes care of
         * doing that
         */
        ctx->ier = MICOUART_IER_RX_INT_MASK;
        dev->ier = ctx->ier;

        /* Register interrupt-handler for this uart's interrupt */
        MicoRegisterISR(ctx->intrLevel, ctx, MicoUartISR);
    }
#endif


    /* Register this uart as an available UART for lookup services */
    ctx->lookupReg.name = ctx->name;
    ctx->lookupReg.deviceType = "UARTDevice";
    ctx->lookupReg.priv = ctx;
    MicoRegisterDevice( &(ctx->lookupReg) );


    /* attempt initializting this uart for Uart file-services */
    LatticeMico32_UartFileInit(ctx);


    /* all done! */
    return;
}



/*
 ***************************************************************
 * Retrieves a character from the UART
 * Returns 0 if no error.
 * If interrupts are enabled, go through the buffer.
 * Else, interact directly with the uart.
 ***************************************************************
*/
unsigned int MicoUart_getC(MicoUartCtx_t *ctx, unsigned char *pucChar)
{
    unsigned int uiValue;
    volatile MicoUart_t *dev;
    
    if(pucChar == 0)
        return(MICOUART_ERR_INVALID_ARGUMENT);

    dev = (volatile MicoUart_t *)(ctx->base);

    do {
#ifndef _MICOUART_NO_INTERRUPTS_
        if(ctx->intrAvail){

            /* using interrupts */
            if(ctx->rxDataBytes > 0){
                *pucChar = ctx->rxBuffer[ctx->rxReadLoc];
                ctx->rxReadLoc++;
                if(ctx->rxReadLoc >= ctx->rxBufferSize)
                    ctx->rxReadLoc = 0;
                MicoDisableInterrupt(ctx->intrLevel);
                ctx->rxDataBytes--;
                ctx->ier |= MICOUART_IER_RX_INT_MASK;
                dev->ier = ctx->ier;
                MicoEnableInterrupt(ctx->intrLevel);
                return(0);
            }
        }else
#endif      
        {
            /* not using interrupts */
            uiValue = dev->lsr;
            if(uiValue & MICOUART_LSR_RX_RDY_MASK){
                *pucChar = dev->rxtx;
                return(0);
            }
        }

        /* if rx is non-blocking, return immediately */
        if(ctx->blockingRx == 0)
            return(MICOUART_ERR_WOULD_BLOCK);

    }while(1);


    /* this "return" should never happen */
    return(255);
}



/*
 ***************************************************************
 * Sends a character over the UART
 * Returns 0 if no error
 ***************************************************************
*/
unsigned int MicoUart_putC(MicoUartCtx_t *ctx, unsigned char ucChar)
{
    volatile unsigned int uiValue;
    volatile MicoUart_t *dev;

    dev = (volatile MicoUart_t *)(ctx->base);

#ifndef _MICOUART_NO_INTERRUPTS_
    if(ctx->intrAvail){
        /* if tx-buffer's full, wait for it to get empty */
        while(ctx->txDataBytes == ctx->txBufferSize){

            /* if non-blocking tx, return immediately */
            if(ctx->blockingTx == 0)
                return(MICOUART_ERR_WOULD_BLOCK);
        }

        /* stuff data into the write-buffer */
        ctx->txBuffer[ctx->txWriteLoc] = ucChar;
        ctx->txWriteLoc++;
        if(ctx->txWriteLoc >= ctx->txBufferSize)
            ctx->txWriteLoc = 0;

        /* increment tx-databytes and enable transmit interrupt */
        MicoDisableInterrupt(ctx->intrLevel);
        ctx->txDataBytes++;
        ctx->ier |= MICOUART_IER_TX_INT_MASK;
        dev->ier = ctx->ier;
        MicoEnableInterrupt(ctx->intrLevel);

        /* all done */
        return(0);
    }else
#endif
    {
        do{
            /* if uart's ready to accept character, send immediately */
            uiValue = dev->lsr;
            if(uiValue & MICOUART_LSR_TX_RDY_MASK){
                dev->rxtx = ucChar;
                return(0);
            }
    
            /* if non-blocking tx, return immediately */
            if(ctx->blockingTx == 0)
                return(MICOUART_ERR_WOULD_BLOCK);

        }while(1);
    }

    /* all done */
    return(0);
}



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
                                 unsigned int stopbits) 
{

    volatile MicoUart_t *dev;
    unsigned int lcr;

    dev = (volatile MicoUart_t *)(ctx->base);


    /* check data-width value */
    if (dwidth > 8 )
        return(MICOUART_ERR_INVALID_ARGUMENT);
    else if (dwidth < 5)
        return(MICOUART_ERR_INVALID_ARGUMENT);


    /* check stopbit value */
    if((stopbits!=1) && (stopbits != 2))
        return(MICOUART_ERR_INVALID_ARGUMENT);

    /* update context */
    ctx->databits = dwidth;
    ctx->stopbits = stopbits;

    /* configure UART */
    switch(dwidth){
        case 5:{     
                if (stopbits ==2){ lcr=0x4; }
                else {lcr = 0x0;}
            }break;
        case 6:{  
                if (stopbits ==2){ lcr=0x5; }
                else {lcr = 0x01;}
            }break;
        case 7:{
                if (stopbits ==2){ lcr=0x6; }
                else {lcr = 0x2;}
            }break;
        default:{ /* 8 databits */
                if (stopbits ==2){ lcr=0x7; }
                else {lcr = 0x3;}
                ctx->databits = 8;
            }break;
    }

    /* parity control */
    if (parity_en == 1) {
        if (even_odd == 1)
            lcr |= 0x8;
        else
            lcr |= 0x18;
    }
    dev->lcr = lcr;


    /* all done */
    return(0);
}




/*
 ***************************************************************
 * Changes baudrate.
 * NOTE: User must make sure the baudrate passed is correct.
 ***************************************************************
*/
unsigned int MicoUart_setRate(MicoUartCtx_t *ctx, unsigned int baudrate)
{
    unsigned int divisor;
    volatile MicoUart_t *dev;
    dev = (volatile MicoUart_t *)(ctx->base);


    /* it would be nice to have uart stop-receiver/stop-transmitter functionality! */
    /* the only check we do is for zero (to avoid divide-by-zero) */
    if(baudrate == 0)
        return(MICOUART_ERR_INVALID_ARGUMENT);  

#ifndef _MICOUART_NO_INTERRUPTS_
    /* disable interrupt for the UART */
    MicoDisableInterrupt(ctx->intrLevel);
#endif

    /* set to new baudrate */
    ctx->baudrate = baudrate;

    /* Calculate clock-divisor */
    divisor = ((1048576.0 * (baudrate/100)) / (MICO32_CPU_CLOCK_MHZ/100)) + 0.5;

    /*
     * Setup uart:
     * - divisor
     */
    dev->div = divisor;

#ifndef _MICOUART_NO_INTERRUPTS_
    /* re-enable interrupt for the UART */
    MicoEnableInterrupt(ctx->intrLevel);
#endif

    return(0);
}

/*
 ***************************************************************
 * Changes blocking mode to blocking if uiBlock != 0           *
 * else changes blocking-mode to non-blocking if uiBlock == 0  *
 ***************************************************************
*/
unsigned int MicoUart_setBlockMode(MicoUartCtx_t *ctx, unsigned int uiBlock)
{
#ifndef _MICOUART_NO_INTERRUPTS_
    MicoDisableInterrupt(ctx->intrLevel);
#endif

        ctx->blockingTx = 1;
        ctx->blockingRx = 1;

#ifndef _MICOUART_NO_INTERRUPTS_
    MicoEnableInterrupt(ctx->intrLevel);
#endif
    return(0);
}

