#include "MicoUartService.h"
#include "MicoUtils.h"
#include "MicoMacros.h"

/*
 ******************************************************************************
 ******************************************************************************
 *                                                                            *
 *                 FILE (CHARACTER MODE) SERVICE                              *
 *                                                                            *
 ******************************************************************************
 ******************************************************************************
*/
typedef struct RS232_UART_FDev_Ctx_st{
    struct st_MicoFileDevice_t fDevReg; /* file-dev registration structure */
}RS232_UART_FDev_Ctx_t;


static RS232_UART_FDev_Ctx_t RS232_UART_FDev[__RS232_UART_MAX_UART_FDEVS__];


/*
 ******************************************************************************
 *                                                                            *
 * UART file-read function                                                    *
 *                                                                            *
 ******************************************************************************
*/
int LatticeMico32_UartFileRead(MicoFileDesc_t *fd, char *buffer, unsigned int bytes)
{
    int iBytes = 0;

    /* retrieve context information */
    MicoUartCtx_t *pUart = (MicoUartCtx_t *)fd->priv;


    /* fetch data */
    while(iBytes < bytes){
        if(MicoUart_getC(pUart, (unsigned char *)&buffer[iBytes]) != 0)
            return(iBytes);
        if((buffer[iBytes] == '\n') || (buffer[iBytes] == '\r')){
            buffer[iBytes] = '\n';
            MicoUart_putC(pUart, '\r');
            MicoUart_putC(pUart, '\n');
            return(iBytes+1);
        }

        /* echo-back data that's entered */
        MicoUart_putC(pUart, buffer[iBytes]);

        iBytes++;
    }


    /* all done */
    return(iBytes);
}


/*
 ******************************************************************************
 *                                                                            *
 * UART file-write function                                                   *
 *                                                                            *
 ******************************************************************************
*/

int LatticeMico32_UartFileWrite(MicoFileDesc_t *fd, const char *buffer, unsigned int bytes)
{
    int iBytes = 0;

    /* retrieve context information */
    MicoUartCtx_t *pUart = (MicoUartCtx_t *)fd->priv;


    /* send data */
    while(iBytes < bytes){
        if(MicoUart_putC(pUart, buffer[iBytes]) != 0)
            return(iBytes);
        iBytes++;
    }


    /* all done */
    return(iBytes);
}



/*
 ******************************************************************************
 *                                                                            *
 * Uart file-isatty function                                                  *
 *                                                                            *
 ******************************************************************************
*/
int LatticeMico32_UartFileIsAtty(MicoFileDesc_t *fd)
{
    /* we're a terminal io device */
    return(1);
}


/*
 ******************************************************************************
 *                                                                            *
 * Uart file-open handler                                                     *
 *                                                                            *
 ******************************************************************************
*/
int LatticeMico32_UartFileOpen(MicoFileDesc_t *fd, const char*filename)
{

    /*
     *---------------------------------------------------------------
     * We don't care about the filename as we don't support
     * filesystems for the LatticeMico32 Uart device (we could!)
     *---------------------------------------------------------------
     */
    MicoUartCtx_t *pUart = (MicoUartCtx_t *)fd->priv;

    /* change to blocking-mode, if it wasn't already set */
    MicoUart_setBlockMode(pUart, 1);

    /* all done */
    return(0);
}


/*
 ******************************************************************************
 *                                                                            *
 * Uart file-registration                                                     *
 *                                                                            *
 ******************************************************************************
*/
void LatticeMico32_UartFileInit(MicoUartCtx_t *pUart)
{
#ifndef _MICOUART_FILESUPPORT_DISABLED_
#ifndef _MICOFILE_SUPPORT_DISABLED_

    static int iDevsRegistered = 0;
    static struct st_MicoFileFnTable_t fileFnTbl;


    /*
     * Do not register this device if maximum rs-232 uart fdevs have
     * been registered
     */
    if(iDevsRegistered >= __RS232_UART_MAX_UART_FDEVS__)
        return;

    /*
     * If input argument is NULL, don't bother doing anything
     */
    if(pUart == (MicoUartCtx_t *)0)
        return;

    /*
     ****************************************************
     * populate the file-device function-table entries:
     * Note: Since registration is same for all UART 
     * instances, it doesn't make any difference clobbering
     * the values over-and-over-again; saves a few bytes
     * of the "if <NOT_INITIALIZED>" logic :)
     ****************************************************
    */
    fileFnTbl.read      = LatticeMico32_UartFileRead;       /* read handler         */
    fileFnTbl.write     = LatticeMico32_UartFileWrite;      /* write handler        */
    fileFnTbl.isatty    = LatticeMico32_UartFileIsAtty;     /* isatty handler       */
    fileFnTbl.open      = LatticeMico32_UartFileOpen;       /* always open          */
    fileFnTbl.close     = 0;                                /* nothing on close     */
    fileFnTbl.lseek     = 0;                                /* cannot seek          */
    fileFnTbl.unlink    = 0;                                /* cannot remove files  */
    fileFnTbl.link      = 0;                                /* cannot "link"        */
    fileFnTbl.stat      = 0;                                /* cannot stat          */
    fileFnTbl.ioctl     = 0;                                /* cannot ioctl         */


    /*
     ***************************************************
     * populate the file-device registration structure *
     ***************************************************
    */
    RS232_UART_FDev[iDevsRegistered].fDevReg.name = pUart->name;
    RS232_UART_FDev[iDevsRegistered].fDevReg.p_FileFnTable = &fileFnTbl;
    RS232_UART_FDev[iDevsRegistered].fDevReg.priv = (void *)pUart;


    /*
     **************************************************
     * register as an available file-device
     **************************************************
    */
    if(MicoRegisterFileDevice(&RS232_UART_FDev[iDevsRegistered].fDevReg) == 0){
        /* increment total registered devices */
        iDevsRegistered++;
    }


    /*
     * all done
     */
#endif
#endif

    return; 
}


