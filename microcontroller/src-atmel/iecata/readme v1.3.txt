IEC-ATA v1.3
==========

This short document briefly describes the IEC-ATA hard drive
controller. 
The orginal version of iec-ata is made by: 

Asbjørn Djupdal
Grønngjelet 50
4640 SØGNE
NORWAY


If you need more information, don't hesitate to ask us
directly. We can currently be reached at the following web-address:
www.diensagtreff.de
Or the original outor by e-mail

djupdal@idi.ntnu.no 

Homepage: http://www.stud.ntnu.no/~djupdal/cbm/iecata/

Homepage: http://www.dienstagstreff.de

Introduction
------------

IEC-ATA is a hard drive controller for the Commodore IEC serial bus.
It enables all computers that has an IEC serial bus port (e.g VIC-20,
C-64, C-128) to use cheap, easy to get ATA hard drives (also called
IDE hard drives). 

Features
--------

- Supports standard ATA hard drives up to 128 gigabytes.
  They must have LBA mode.

- The whole disk is available as one big partition

- Supports standard Commodore KERNAL IEC disk commands. This has a
  number of implications:
  - May be connected to any computer with an IEC serial bus port
    (VIC-20, C-64, C-128, and probably others...)
  - No driver software is necessary on the computer, the hard drive
    works like a disk drive with a giant disk in it
  - The IEC bus is very slow, making the hard drive slow. But it is
    considerably faster than the 1541 disk drive. 

- Custom file system that has the following features:
  - Supports PRG, and SEQ file types, and a new DIR file type that
    represents subdirectories
  - Smallest addressable unit: 512 bytes (one ATA logical block)
  - Max file size: 32 megabytes
  - Max number of directory entries: 256
  - Max number of nested subdirectories: unlimited

Usage
-----

Connect the IEC-ATA board to the computer and to the ATA disk drive,
and power on. If the error light does not turn off after some seconds,
try a reset. If that does not help, there is a problem with the hard
drive.

The hard drive has the drive number 10 or outher (as opposed to the 1541 that is
usually drive 8). Other than that, the hard drive is used in the same
way that a 1541 disk drive. 


The following disk commands are supported (through disk channel 15):

- CD:<subdir>
  Change directory
- CD<-
  Dir up
- CD/
  To Root Dir
- MD:<subdir>
  Make subdirectory
- RD:<subdir>
  Remove (delete) subdirectories. The subdirectory must be empty
- S:<file>
  Scratch (delete) files
- R:<new-file>=<old-file>
  Rename file
- N
  Format disk.
- I
  Initialize disk (is actually ignored by IEC-ATA)
- UI+ or -
  Set IEC Busspeed, + slow for C64, - fast for VIC 20
- U0>+chr$(dev)
- Set Devicenumber of Drive temporery form 10 to use dev adress. dev must be smaler then 30
 
The syntax of these commands are the same as on Commodore DOS. Where
appropriate, you may use wildcards (* and ?) just like on Commodore
DOS. 

Loading and saving files work just like on a 1541 disk drive. Note
that only PRG and SEQ files may be created. 

If you read the error channel (channel 15), you get one of these
messages: 

- 00: No error
- 74: Initialize error, the hard drive does not have a valid file
      system
- 25: Create error, the file or directory you try to create can not be
      created
- 61: Not open error, you are trying to access a file that is not open
- 62: Not found error, the file or directory is not found
- 30: Syntax error
- 73: Version message, this is shown after startup and gives the
      version of the IEC-ATA software. 

Examples of use
---------------

Loading a file from BASIC:

  LOAD"filename",10

Saving a file from BASIC:

  SAVE"filename",10

Loading the directory:

  LOAD"$",10

Changing the directory:

  OPEN1,10,15,"CD:mydir":CLOSE1

Formatting the disk:
  OPEN1,10,15,"N":CLOSE1

Hardware
--------

The IEC-ATA controller card contains an AVR mega8515 microcontroller with
32 kilobytes of external SRAM. For more information about the
hardware, see the included schematics.

--
Asbjørn Djupdal, 2002, dienstagtreff, 2005, DasLabor 2005
