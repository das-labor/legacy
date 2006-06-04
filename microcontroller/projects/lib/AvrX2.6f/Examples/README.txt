AvrX for avr-gcc & IAR ICCAVR, version 2.6 example code

19-Sep-2005
Updated code for latest WinAvr release (3.4.3)  Not checked out with IAR.  Probably broken.
Added GCC style makefile.  Broke out "serialio.S" from AvrX since it was processor
dependent and built & linked with this makefile.  Left old IAR makefile around.

16-Sep-2002
Updated source code to compiler with the experimental 3.2 compiler.
Examples checked out on an 8535, but not on an mega128 - I am assuming
the work done earlier will still hold (!).  See ..\changes.txt for more
details.

31-May-2002

Updated source code to allow compilation for the ATmega128 chip using
the test release of avr-gcc 3.1 (alpha).

Changes:
    hardware.h - Checked for and equated CS0 and CS00, CS1 and CS10
                for timer0 prescaler bit names.
    All sample code: Added "serialio.h" and removed prototype for InitiSerialIO().

08-June-2001

After executing a "make clean" the directory listing is as follows:

Timers.c	- Shows how to set up the AvrX Timer Queue mechanism,
                start timers and wait for them.

Messages.c	- Application showing the use of multiple messages to
                communicate between applications

MessageAndData.c
        	- Application showing the use of data structure tacked
        	on to a message for passing information between processes.

MessageTimers.c - Application showing the use of EVENTS
                to implement an asyncronous timeout while waiting for a
                message.

MessageTimersAlt.c - an alternate way to structure the code.

*.avd		- Astudio file defining the target simulation hardware
*.aio		- Astudio file defining the session I/O window layout

makefile	- the makefile

MAKEFILE instructions:

The makefile depends upon the environment variable AVRX being
set to the root directory of the AvrX distribution.  E.g. the parent
directory where this ReadMe.txt file is found.

These examples were developed and compiled under avr-gcc 3.0
that can be found at

http://www.combio.de/avr
http://www.avrfreaks.net

This is all for DOS/Windows, of course.  For linux you will probably
have to hack the makefile.

Additional Notes:

The double "AVRX_TASKDEF()" macros are there simply to allow the files
to build for both IAR and GCC compilers.  Naturally one would only use
the macro appropriate for the compiler in use.

The section that includes files is set up for IAR.  It happens to be
compatible with GCC.  For GCC only use, the following should work:

#include "io-avr.h"
#include "sig-avr.h"

As always, when in doubt, LOOK AT THE SOURCE.  The source files are
*always* the last word on how something should work.
