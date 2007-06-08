#ifndef MICO32_UARTSERVICE_HEADER_FILE
#define MICO32_UARTSERVICE_HEADER_FILE

#include "DDStructs.h"
#include "LookupServices.h"
#include "MicoFileDevices.h"
#include "MicoUart.h"



/*
 * If not defined, defines maximum rs-232 uart
 * devices that can be registered as file-devices.
 */
#ifndef __RS232_UART_MAX_UART_FDEVS__
#define __RS232_UART_MAX_UART_FDEVS__       (3)
#endif



#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


/*
 ******************************************************************************
 ******************************************************************************
 *                                                                            *
 *                 FILE (CHARACTER MODE) SERVICE                              *
 *                                                                            *
 ******************************************************************************
 ******************************************************************************
*/


    /* functions used by uart-file-service (user should not call them directly) */
    int LatticeMico32_UartFileRead(MicoFileDesc_t *fd, char *buffer, unsigned int bytes);
    int LatticeMico32_UartFileWrite(MicoFileDesc_t *fd, const char *buffer, unsigned int bytes);
    int LatticeMico32_UartFileIsAtty(MicoFileDesc_t *fd);
    int LatticeMico32_UartFileOpen(MicoFileDesc_t *fd, const char*filename);
    void LatticeMico32_UartFileInit(MicoUartCtx_t *pUart);


#ifdef __cplusplus
};
#endif /* __cplusplus */


#endif /* MICO32_UARTSERVICE_HEADER_FILE */

