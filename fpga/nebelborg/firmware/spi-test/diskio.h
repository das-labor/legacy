/*-----------------------------------------------------------------------
/  Low level disk interface modlue include file  R0.02    (C)ChaN, 2006
/-----------------------------------------------------------------------*/

#ifndef _DISKIF

//#define _READONLY	/* Define this for read-only use */

#include "integer.h"

typedef unsigned char	DSTATUS;
typedef unsigned char	DRESULT;


/*---------------------------------------*/
/* Prototypes for disk control functions */

DSTATUS disk_initialize (void);
DSTATUS disk_shutdown (void);
DSTATUS disk_status (void);
DRESULT disk_read (BYTE*, DWORD, BYTE);
#ifndef	_READONLY
DRESULT disk_write (const BYTE*, DWORD, BYTE);
#endif
DRESULT disk_ioctl (BYTE, void*);
void	disk_timerproc (void);


/* Results of Disk Functions (DRESULT) */

#define RES_OK			0		/* Successful */
#define	RES_ERROR		1		/* R/W Error */
#define	RES_WRPRT		2		/* Write Protected */
#define	RES_NOTRDY		3		/* Not Ready */
#define	RES_PARERR		4		/* Invalid Parameter */


/* Disk Status Bits (DSTATUS) */

#define STA_NOINIT		0x01	/* Drive not initialized */
#define STA_NODISK		0x02	/* No medium in the drive */
#define STA_PROTECT		0x04	/* Write protected */


/* Command code for disk_ioctrl() */

#define GET_SECTORS		1
#define PUT_IDLE_STATE	2
#define MMC_GET_CSD		10
#define MMC_GET_CID		11
#define MMC_GET_OCR		12
#define ATA_GET_REV		20
#define ATA_GET_MODEL	21
#define ATA_GET_SN		22


#define _DISKIF
#endif
