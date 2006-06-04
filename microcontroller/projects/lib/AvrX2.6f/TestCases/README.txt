AvrX for avr-gcc, version 2.6 test cases

18-Sept-2005

	1. New makefile for GCC, uses dwarf-2 debug format which works well with
	the new Astudio 4.11 or greater.

	2. Pulled "serialio.S" out of AvrX library since it was processor
	dependent (AvrX library is generic code)

	3. Added "hardware.h" to directory to cover major hardware variations
	between CPU families.

8-June-2001

After executing a "make clean" the directory listing is as follows:

BasicTest1.c	- Simple tasking and priority shifting.  Also
		demonstrates hooking into a timer interrupt.

BasicTest2.c	- Runs through all semaphore interfaces and exercises
		the semaphore queueing mechanism.

BasicTest3.c	- Checks out the Timer Queue mechanism, then spins off
		three tasks to hammer on the queue with interrupts enabled.

BasicTest4.c	- Checks out the message queueing system.  Again, spins off
		a couple tasks in the end to hammer on the queue with an
		interrupt handler as well to check out asynchronous handling
		of the queue.

hardware.inc	- some fundamental hardware information - look to makefile
		for the stack location.

makefile	- the makefile

These may or may not be present:

*.avd		- Astudio file defining the target simulation hardware
*.aio		- Astudio file defining the session I/O window layout

MAKEFILE instructions:

The makefile depends upon the environment variable AVRX being
set to the root directory of the AvrX distribution.  E.g. the parent
directory where this ReadMe.txt file is found.

See ../avrx/readme.txt for compiler information

RUNNING TESTS

All the tests toggle bits on PORTB to indicate things are working.  In
general stepping through the main code to insure that each call is working
as expected, then checking the state of the LED bits (set breakpoints on
the outp() command in studio) to insure they are toggling periodically.

NOTE: "hardware.h" defines what ports and bits are actually used.  So, feel
free to change things.
