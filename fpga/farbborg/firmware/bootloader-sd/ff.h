/*--------------------------------------------------------------------------/
/  FatFs - FAT file system module include file  R0.03        (C)ChaN, 2006
/---------------------------------------------------------------------------/
/ FatFs module is an experimenal project to implement FAT file system to
/ cheap microcontrollers. This is a free software and is opened for education,
/ research and development. You can use, modify and/or republish it for
/ non-profit or profit use without any restriction under your responsibility.
/---------------------------------------------------------------------------*/

#ifndef _FATFS

#define _BYTE_ACC
/* The _BYTE_ACC enables byte-by-byte access for multi-byte variables. This
/  MUST be defined when multi-byte variable is stored in big-endian and/or
/  address miss-aligned access is prohibited.  */

#define _FS_READONLY
/* Read only configuration. This removes writing functions, f_write, f_sync,
/  f_unlink, f_mkdir, f_chmod, f_rename and f_getfree. */

#define _FS_MINIMIZE	1
/* The _FS_MINIMIZE defines minimization level to remove some functions.
/  0: Not minimized.
/  1: f_stat, f_getfree, f_unlink, f_mkdir, f_chmod and f_rename are removed.
/  2: f_opendir and f_readdir are removed in addition to level 1. */

#define	_USE_SJIS
/* When _USE_SJIS is defined, Shift-JIS code transparency is enabled, otherwise
/  only US-ASCII(7bit) code can be accepted as file/directory name. */


#include "integer.h"


/* Result type for fatfs application interface */
typedef unsigned char	FRESULT;


/* File system object structure */
typedef struct _FATFS {
	BYTE	fs_type;		/* FAT type */
	BYTE	files;			/* Number of files currently opend */
	BYTE	sects_clust;	/* Sectors per cluster */
	BYTE	n_fats;			/* Number of FAT copies */
	WORD	n_rootdir;		/* Number of root directory entry */
	BYTE	winflag;		/* win[] dirty flag (1:must be written back) */
	BYTE	pad1;
	DWORD	sects_fat;		/* Sectors per fat */
	DWORD	max_clust;		/* Maximum cluster# + 1 */
	DWORD	fatbase;		/* FAT start sector */
	DWORD	dirbase;		/* Root directory start sector (cluster# for FAT32) */
	DWORD	database;		/* Data start sector */
	DWORD	winsect;		/* Current sector appearing in the win[] */
	BYTE	win[512];		/* Disk access window for Directory/FAT */
} FATFS;


/* Directory object structure */
typedef struct _DIR {
	DWORD	sclust;		/* Start cluster */
	DWORD	clust;		/* Current cluster */
	DWORD	sect;		/* Current sector */
	WORD	index;		/* Current index */
} DIR;


/* File object structure */
typedef struct _FIL {
	DWORD	fptr;			/* File R/W pointer */
	DWORD	fsize;			/* File size */
	DWORD	org_clust;		/* File start cluster */
	DWORD	curr_clust;		/* Current cluster */
	DWORD	curr_sect;		/* Current sector */
#ifndef _FS_READONLY
	DWORD	dir_sect;		/* Sector containing the directory entry */
	BYTE*	dir_ptr;		/* Ponter to the directory entry in the window */
#endif
	BYTE	flag;			/* File status flags */
	BYTE	sect_clust;		/* Left sectors in cluster */
	BYTE	buffer[512];	/* File R/W buffer */
} FIL;


/* File status structure */
typedef struct _FILINFO {
	DWORD fsize;			/* Size */
	WORD fdate;				/* Date */
	WORD ftime;				/* Time */
	BYTE fattrib;			/* Attribute */
	char fname[8+1+3+1];	/* Name (8.3 format) */
} FILINFO;



/*-----------------------------------------------------*/
/* FatFs module application interface                  */

extern FATFS *FatFs;	/* Pointer to active file system object */

FRESULT f_open (FIL*, const char*, BYTE);			/* Open or create a file */
FRESULT f_read (FIL*, void*, WORD, WORD*);			/* Read file */
FRESULT f_close (FIL*);								/* Close file */
FRESULT f_lseek (FIL*, DWORD);						/* Seek file pointer */
FRESULT f_opendir (DIR*, const char*);				/* Open a directory */
FRESULT f_readdir (DIR*, FILINFO*);					/* Read a directory item */
FRESULT f_stat (const char*, FILINFO*);				/* Get file status */
FRESULT f_getfree (DWORD*);							/* Get number of free clusters */
FRESULT f_mountdrv (void);							/* Force initialized the file system */
FRESULT f_write (FIL*, const void*, WORD, WORD*);	/* Write file */
FRESULT f_sync (FIL*);								/* Flush cached data of a writing file */
FRESULT f_unlink (const char*);						/* Delete a file or directory */
FRESULT	f_mkdir (const char*);						/* Create a directory */
FRESULT f_chmod (const char*, BYTE, BYTE);			/* Change file attriburte */
FRESULT f_rename (const char*, const char*);		/* Rename a file or directory */


/* User defined function to give a current time to fatfs module */

DWORD get_fattime(void);	/* 31-25: Year(0-127 +1980), 24-21: Month(1-12), 20-16: Day(1-31) */
							/* 15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2) */



/* File function return code (FRESULT) */

#define	FR_OK						0
#define	FR_NOT_READY				1
#define	FR_NO_FILE					2
#define	FR_NO_PATH					3
#define	FR_INVALID_NAME				4
#define	FR_DENIED					5
#define	FR_DISK_FULL				6
#define	FR_RW_ERROR					7
#define	FR_INCORRECT_DISK_CHANGE	8
#define	FR_WRITE_PROTECTED			9
#define	FR_NOT_ENABLED				10
#define	FR_NO_FILESYSTEM			11


/* File access control and file status flags (FIL.flag) */

#define	FA_READ				0x01
#define	FA_OPEN_EXISTING	0x00
#ifndef _FS_READONLY
#define	FA_WRITE			0x02
#define	FA_CREATE_ALWAYS	0x08
#define	FA_OPEN_ALWAYS		0x10
#define FA__WRITTEN			0x20
#define FA__DIRTY			0x40
#endif
#define FA__ERROR			0x80


/* FAT type signature (FATFS.fs_type) */

#define FS_FAT12	1
#define FS_FAT16	2
#define FS_FAT32	3


/* File attribute bits for directory entry */

#define	AM_RDO	0x01	/* Read only */
#define	AM_HID	0x02	/* Hidden */
#define	AM_SYS	0x04	/* System */
#define	AM_VOL	0x08	/* Volume label */
#define AM_LFN	0x0F	/* LFN entry */
#define AM_DIR	0x10	/* Directory */
#define AM_ARC	0x20	/* Archive */



/* Multi-byte word access macros  */

#ifdef _BYTE_ACC
#define	LD_WORD(ptr)		(WORD)(((WORD)*(BYTE*)((ptr)+1)<<8)|(WORD)*(BYTE*)(ptr))
#define	LD_DWORD(ptr)		(DWORD)(((DWORD)*(BYTE*)((ptr)+3)<<24)|((DWORD)*(BYTE*)((ptr)+2)<<16)|((WORD)*(BYTE*)((ptr)+1)<<8)|*(BYTE*)(ptr))
#define	ST_WORD(ptr,val)	*(BYTE*)(ptr)=(BYTE)(val); *(BYTE*)((ptr)+1)=(BYTE)((WORD)(val)>>8)
#define	ST_DWORD(ptr,val)	*(BYTE*)(ptr)=(BYTE)(val); *(BYTE*)((ptr)+1)=(BYTE)((WORD)(val)>>8); *(BYTE*)((ptr)+2)=(BYTE)((DWORD)(val)>>16); *(BYTE*)((ptr)+3)=(BYTE)((DWORD)(val)>>24)
#else
#define	LD_WORD(ptr)		(WORD)(*(WORD*)(BYTE*)(ptr))
#define	LD_DWORD(ptr)		(DWORD)(*(DWORD*)(BYTE*)(ptr))
#define	ST_WORD(ptr,val)	*(WORD*)(BYTE*)(ptr)=(WORD)(val)
#define	ST_DWORD(ptr,val)	*(DWORD*)(BYTE*)(ptr)=(DWORD)(val)
#endif


#define _FATFS
#endif
