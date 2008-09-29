/*
	Simple Operating System for Smartcard Education
	Copyright (C) 2002  Matthias Bruestle <m@mbsks.franken.de>

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*! @file
	\brief main() declarations and documentation.

	$Id: main.h,v 1.24 2003/03/30 19:50:58 m Exp $
*/

/*!
	\mainpage SOSSE - Simple Operating System for Smartcard Education

	<H1>Overview</H1>

	<P>SOSSE is a simple operating system for smart cards which is
	opensource and free. It has been developed to provide a smart
	card platform for learning, experimenting and implementing
	inovative concepts. The hardware platform choosen for this
	project are the Funcards based on commercial-of-the-shelf AVR
	microcontrollers, because the hardware documentation is available
	without non-disclosure agreement.</P>

	\image html funcard2.jpg

	<H1>Hardware</H1>

	<P>All of the currently supported cards use the <A
	HREF="http://www.atmel.com/atmel/products/prod23.htm"
	>AVR-Microcontrollers</A> from <A HREF="http://www.atmel.com/"
	>Atmel</A>. The are e.g. called Funcard, Jupiter or Pink card.
	The used AVRs are the AT90S2323, AT90S2343, AT90S8515 and
	AT90S8535.  There are even a few boards with the ATmega161
	available. The data sheets for all Atmel AVR microcontrollers
	are available <A
	HREF="http://www.atmel.com/atmel/products/cgi/locator5.cgi/atmel/products/cgi/locator5.tab"
	>on the Atmel website</A>.</P>

	\image html purple.gif
	\image latex purple.eps "Connection scheme for the AVR cards." width=8cm

	<P>The pinout of the AVR cards is as follows:</P>

	<P>
	<UL>
	<LI>C1 (VCC): Vcc</LI>
	<LI>C2 (RST): RESET</LI>
	<LI>C3 (CLK): XTAL</LI>
	<LI>C4: MOSI</LI>
	<LI>C5 (VSS): Vss</LI>
	<LI>C6: Not connected</LI>
	<LI>C7 (I/O): MISO</LI>
	<LI>C8: SCK</LI>
	</UL>
	</P>
	
	<P>Additional to the smart cards, a programmer is needed. These
	are available at the same shops as the AVR cards.</P>

	<H1>Software</H1>

	<P>SOSSE has been developed using the GCC development
	tools. These are available at <A HREF="http://combio.de/avr/"
	>http://combio.de/avr/</A>. There are the sources, binaries
	and RPMs for Linux and binaries for MS Windows.</P>

	<H1>Design Decisions</H1>

	<H2>Programming Language</H2>

	<P>The C programming language has been choosen instead of assembler,
	because:</P>

	<UL>
	<LI>It is easier to understand C source code.</LI>
	<LI>Modern smart card operating systems are probably all written mostly
		in C. Who does want to write and verify 128kB assembler code?</LI>
	<LI>When new smart card architectures are available for SOSSE, these can
		be utilized more easily.</LI>
	</UL>

	<H1>Building Instructions</H1>

	<H2>Standard Build</H2>

	<OL>

	<LI>Go to the src directory and edit config.h. This file contains
	the configuration data for the build process. The meaning of
	the options is described in the doxygen documention of config.h
	itself. (It is not possible to switch on all features, because
	then the code size is above 8kB.)</LI>

	<LI>You can edit eedata.S if you want to change the EEPROM data
	loaded onto the AVR card. You might want to change the card
	serial number, the start value for the random number generator,
	the keys, the file permissions or the created files.</LI>

	<LI>There are two defines in the Makefile, which could need editing.
	The first is the -mmcu setting in CC and the second is -mtiny-stack
	in CFLAGS. Adjust these to reflect your used AVR card.</LI>

	<LI>To build the HEX files start <EM>make</EM>.</LI>

	<LI>The resulting HEX files sosse.hex and eedata.hex (or BIN files
	sosse.bin and eedata.bin) have then to be loaded on the card with
	the programmer and an apropriate software. The destination for
	sosse.hex is the Flash and for eedata.hex the internal EEPROM.</LI>

	</OL>

	<H2>Debugging</H2>

	<P>Test programms which run natively on the host operating system
	are built running:</P>

	<P>make -f Makefile.emu</P>

	<P>These can be used for comfortable debugging and testing using
	the normal GNU debugger. When the reset sequence is implemented,
	this can be used to simulate a SOSSE smart card via e.g. a Season
	interface.</P>

	<H2>CT-API driver</H2>

	<H3>Building</H3>

	<P>A virtual card sitting in a virtual reader can be build with:</P>

	<P>make -f Makefile.ctapi</P>

	<P>This builds a CT-API driver, which can be used to test applications
	and to debug using a normal debugger.</P>

	<H3>Supported CT-BCS commands</H3>

	<UL>
	<LI>EJECT ICC: Supported with functional unit ICC1. P2 is ignored.</LI>
	<LI>GET STATUS: Supported tag is 80. Possible card states are 01 and
		05.</LI>
	<LI>REQUEST ICC: Supported with functional unit ICC1 and response
		data none and complete ATR. Display settings are ignored.</LI>
	<LI>RESET CT: Supported with functional unit CT and ICC1 and response
		data none and complete ATR. This command is the same as REQUEST ICC,
		but does support reseting the CT, which stops the ICC.</LI>
	</UL>

	<H3>Proprietary command: EEPROM FILE</H3>

	<H3>Header</H3>

	<TABLE>
	<TR>
		<TD>CLA</TD>
		<TD>INS</TD>
		<TD>P1</TD>
		<TD>P2</TD>
		<TD>P3</TD>
	</TR>
	<TR>
		<TD>20</TD>
		<TD>E0</TD>
		<TD>Flags</TD>
		<TD>RFU</TD>
		<TD>Length</TD>
	</TR>
	</TABLE>

	<H3>Command data</H3>

	<TABLE>
	<TR>
		<TD>Byte(s)</TD>
		<TD>Description</TD>
		<TD>Length</TD>
	</TR>
	<TR>
		<TD>1 - Length</TD>
		<TD>File name</TD>
		<TD>Length</TD>
	</TR>
	</TABLE>

	<H3>Status words</H3>

	<TABLE>
	<TR> <TD>SW</TD> <TD>Description</TD> </TR>
	<TR> <TD>90 00</TD> <TD>Command completed successfully</TD> </TR>
	</TABLE>

	<H3>Description</H3>

	<P>This command sets the parameter for EEPROM loading and saving
	for the virtual card.</P>

	<P>If bit 1 of P1 is set, the EEPROM is saved at stopping of the ICC
	and if bit 2 is set, the EEPROM loaded at starting of the ICC. The
	file name for this operation is specified in the data field of the
	command.</P>

	<P><EM>The proprietary commands must be executed before a REQUEST ICC
	and RESET CT, because the parameters are held in RAM, which is separated
	by a fork at card reset.</EM></P>

	<P><EM>A success of the command means only, that the variables have
	correctly been set. It does not verify, that the file is createable,
	readable or writeable.</EM></P>

	<H1>More Information and Download</H1>

	<P>The web page with up to date information about SOSSE is
	at:<BR><A HREF="http://www.mbsks.franken.de/sosse/"
	>http://www.mbsks.franken.de/sosse/</A></P>

	<P>The download location of the SOSSE sources is at:<BR><A
	HREF="ftp://ftp.franken.de/pub/crypt/chipcards/sosse/"
	>ftp://ftp.franken.de/pub/crypt/chipcards/sosse/</A></P>

	<P>The documentation was generated using DoxyGen:<BR><A
	HREF="http://www.doxygen.org/">http://www.doxygen.org/</A></P>

	<H1>Special Files</H1>

	<P>These File are only used, when filesystem support is compiled in.
	Otherwise special absolut locations in EEPROM are used to hold the
	data.</P>

	<H2>EF Ext Auth</H2>

	<P>Path: 3F00FF00</P>

	<P>Content:</P>

	<UL>
	<LI>16 Bytes Key</LI>
	<LI>1 Byte Retry Counter (resets to 10)</LI>
	</UL>

	<H2>EF Int Auth</H2>

	<P>Path: 3F00FF01</P>

	<P>Content:</P>

	<UL>
	<LI>16 Bytes Key</LI>
	<LI>1 Byte Retry Counter (unused)</LI>
	</UL>

	<H2>EF PIN</H2>

	<P>Path: 3F00FF02</P>

	<P>Content:</P>

	<UL>
	<LI>8 Bytes PIN</LI>
	<LI>1 Byte Retry Counter (resets to 3)</LI>
	<LI>8 Bytes PUK</LI>
	<LI>1 Byte Retry Counter (resets to 10)</LI>
	</UL>

	<H1>Commands</H1>

	<H2>Write EEPROM. (Test command)</H2>

	<H3>Header</H3>

	<TABLE>
	<TR>
		<TD>CLA</TD>
		<TD>INS</TD>
		<TD>P1</TD>
		<TD>P2</TD>
		<TD>P3</TD>
	</TR>
	<TR>
		<TD>80</TD>
		<TD>02</TD>
		<TD>Address high</TD>
		<TD>Address low</TD>
		<TD>Length</TD>
	</TR>
	</TABLE>

	<H3>Command data</H3>

	<TABLE>
	<TR>
		<TD>Byte(s)</TD>
		<TD>Description</TD>
		<TD>Length</TD>
	</TR>
	<TR>
		<TD>1 - Length</TD>
		<TD>Data to write to <EM>Address</EM></TD>
		<TD>Length</TD>
	</TR>
	</TABLE>

	<H3>Status words</H3>

	<TABLE>
	<TR> <TD>SW</TD> <TD>Description</TD> </TR>
	<TR> <TD>90 00</TD> <TD>Command completed successfully</TD> </TR>
	<TR> <TD>65 00</TD> <TD>Memory failure (unsuccessful writing)</TD> </TR>
	<TR> <TD>67 00</TD> <TD>Wrong Lc</TD> </TR>
	</TABLE>

	<H3>Description</H3>

	<P>This command can be used to directly write into EEPROM. The
	start address is specified in P1P2 and the data length in P3.
	If there exists an internal and external EEPROM, the address
	convention of the HAL is used. Currently the internal EEPROM
	is in the address range 0 - sizeof internal EEPROM. The external
	EEPROM starts directly afterwards.</P>

	<P><EM>This command should be disabled by setting CONF_WITH_TESTCMDS
	to 0 for a production version.</EM></P>

	<P><EM>Data updates are NOT protected with transactions.</EM></P>

	<H2>Read EEPROM. (Test command)</H2>

	<H3>Header</H3>

	<TABLE>
	<TR>
		<TD>CLA</TD>
		<TD>INS</TD>
		<TD>P1</TD>
		<TD>P2</TD>
		<TD>P3</TD>
	</TR>
	<TR>
		<TD>80</TD>
		<TD>04</TD>
		<TD>Address high</TD>
		<TD>Address low</TD>
		<TD>Length</TD>
	</TR>
	</TABLE>

	<H3>Response data</H3>

	<TABLE>
	<TR>
		<TD>Byte(s)</TD>
		<TD>Description</TD>
		<TD>Length</TD>
	</TR>
	<TR>
		<TD>1 - Length</TD>
		<TD>Data read from <EM>Address</EM></TD>
		<TD>Length</TD>
	</TR>
	</TABLE>

	<H3>Status words</H3>

	<TABLE>
	<TR> <TD>SW</TD> <TD>Description</TD> </TR>
	<TR> <TD>90 00</TD> <TD>Command completed successfully</TD> </TR>
	</TABLE>

	<H3>Description</H3>

	<P>This command can be used to directly read from EEPROM. The
	start address is specified in P1P2 and the data length in P3.
	If there exists an internal and external EEPROM, the address
	convention of the HAL is used. Currently the internal EEPROM
	is in the address range 0 - sizeof internal EEPROM. The external
	EEPROM starts directly afterwards.</P>

	<P><EM>This command should be disabled by setting CONF_WITH_TESTCMDS
	to 0 for a production version.</EM></P>

	<H2>LED Effects (Fun command)</H2>

	<H3>Header</H3>

	<TABLE>
	<TR>
		<TD>CLA</TD>
		<TD>INS</TD>
		<TD>P1</TD>
		<TD>P2</TD>
		<TD>P3</TD>
	</TR>
	<TR>
		<TD>80</TD>
		<TD>06</TD>
		<TD>Effect</TD>
		<TD>Parameter</TD>
		<TD>00</TD>
	</TR>
	</TABLE>

	<H3>Status words</H3>

	<TABLE>
	<TR> <TD>SW</TD> <TD>Description</TD> </TR>
	<TR> <TD>90 00</TD> <TD>Command completed successfully</TD> </TR>
	</TABLE>

	<H3>Description</H3>

	<P>This command does control the LEDs present on some PCB AVR
	cards and shows some nice effects.</P>

	<P>Effect type 0x01 does switch the LEDs specified in P2
	on. Each bit controlls one LED. Which LED is controlled may be
	card dependent. Currently bits 0 - 6 control the leds beginning
	from the left, when looking down at them with the contacts to
	the top.</P>

	<P>Effect type 0x02 does implement a runlight with the left 4
	LED block.  It may need adjustment on other cards to select the
	correct LEDs.</P>

	<P><EM>This command is only enabled when setting CONF_WITH_FUNNY
	to 1.</EM></P>

	\image html jupiter2.jpg

	<H2>Change PIN.</H2>

	<H3>Header</H3>

	<TABLE>
	<TR>
		<TD>CLA</TD>
		<TD>INS</TD>
		<TD>P1</TD>
		<TD>P2</TD>
		<TD>P3</TD>
	</TR>
	<TR>
		<TD>80</TD>
		<TD>24</TD>
		<TD>00</TD>
		<TD>00</TD>
		<TD>10</TD>
	</TR>
	</TABLE>

	<H3>Command data</H3>

	<TABLE>
	<TR>
		<TD>Byte(s)</TD>
		<TD>Description</TD>
		<TD>Length</TD>
	</TR>
	<TR>
		<TD>1 - 8</TD>
		<TD>Current PIN</TD>
		<TD>8</TD>
	</TR>
	<TR>
		<TD>9 - 16</TD>
		<TD>New PIN</TD>
		<TD>8</TD>
	</TR>
	</TABLE>

	<H3>Status words</H3>

	<TABLE>
	<TR> <TD>SW</TD> <TD>Description</TD> </TR>
	<TR> <TD>90 00</TD> <TD>Command completed successfully</TD> </TR>
	<TR> <TD>63 CX</TD> <TD>Verification failed, retry counter specified</TD> </TR>
	<TR> <TD>67 00</TD> <TD>Wrong Lc</TD> </TR>
	<TR> <TD>69 83</TD> <TD>Authentication method blocked</TD> </TR>
	<TR> <TD>6A 86</TD> <TD>Incorrect parameters P1-P2</TD> </TR>
	<TR> <TD>6A 88</TD> <TD>Referenced data not found</TD> </TR>
	</TABLE>

	<H3>Description</H3>

	<P>This command changes the PIN of the card. The first four bytes
	of the data is the current PIN, which is checked for correctness.
	The second four bytes of the data is the new PIN, which will be
	written. On success the AUTH_FLAG_PIN bit in authstate is set,
	on fauilure it is cleared.<P>

	<P><EM>Warning: Currently the retry counter is not handled in
	a secure way. For more information see e.g. Rankl/Effing.</EM></P>

	<P><EM>PIN update is protected with transactions.</EM></P>

	<H2>Create File.</H2>

	<H3>Header</H3>

	<TABLE>
	<TR>
		<TD>CLA</TD>
		<TD>INS</TD>
		<TD>P1</TD>
		<TD>P2</TD>
		<TD>P3</TD>
	</TR>
	<TR>
		<TD>80</TD>
		<TD>E0</TD>
		<TD>00</TD>
		<TD>00</TD>
		<TD>06</TD>
	</TR>
	</TABLE>

	<H3>Command data</H3>

	<TABLE>
	<TR>
		<TD>Byte(s)</TD>
		<TD>Description</TD>
		<TD>Length</TD>
	</TR>
	<TR>
		<TD>1 - 2</TD>
		<TD>Size</TD>
		<TD>2</TD>
	</TR>
	<TR>
		<TD>3 - 4</TD>
		<TD>FID</TD>
		<TD>2</TD>
	</TR>
	<TR>
		<TD>5</TD>
		<TD>File type (DF: 38, EF: 00)</TD>
		<TD>1</TD>
	</TR>
	<TR>
		<TD>6</TD>
		<TD>Access conditions (DF: Create/Delete, EF: Update/Read)</TD>
		<TD>1</TD>
	</TR>
	</TABLE>

	<H3>Status words</H3>

	<TABLE>
	<TR> <TD>SW</TD> <TD>Description</TD> </TR>
	<TR> <TD>90 00</TD> <TD>Command completed successfully</TD> </TR>
	<TR> <TD>67 00</TD> <TD>Wrong Lc</TD> </TR>
	<TR> <TD>69 82</TD> <TD>Security status not satisfied</TD> </TR>
	<TR> <TD>69 86</TD> <TD>Command not allowed (no DF selected)</TD> </TR>
	<TR> <TD>6A 80</TD> <TD>Incorrect arameters in the data field</TD> </TR>
	<TR> <TD>6A 84</TD> <TD>Not enough free space in DF</TD> </TR>
	<TR> <TD>6A 86</TD> <TD>Incorrect parameters P1-P2</TD> </TR>
	<TR> <TD>6A 89</TD> <TD>File exists already</TD> </TR>
	</TABLE>

	<H3>Description</H3>

	<P>This command creates an elementary or dedicated file. To succeed
	the current file must be a dedicated file or the master file and the
	access conditions for file creation must be fulfilled.</P>

	<P>The data of the command is the file header. It consists of a
	2-byte size, 2-byte FID, 1-byte type and 1-byte access conditions.
	Size can't be 0000. The FID can't be 3F00 or FFFF. Valid file types
	are 00 for EF and 38 for DF. The access conditions are described in
	the next tables.</P>

	<TABLE>
	<TR> <TD>File type</TD> <TD>High Nibble</TD> <TD>Low Nibble</TD> </TR>
	<TR> <TD>DF</TD> <TD>Create File</TD> <TD>Delete File</TD> </TR>
	<TR> <TD>EF</TD> <TD>Update</TD> <TD>Read</TD> </TR>
	</TABLE>

	<TABLE>
	<TR> <TD>Nibble Value</TD> <TD>Access condition</TD> </TR>
	<TR> <TD>0</TD> <TD>Always</TD> </TR>
	<TR> <TD>1</TD> <TD>PIN verified</TD> </TR>
	<TR> <TD>2</TD> <TD>External Authentication key verified</TD> </TR>
	<TR> <TD>3</TD> <TD>PIN or External Authentication Key verified</TD> </TR>
	<TR> <TD>4</TD> <TD>PIN and External Authentication Key verified</TD> </TR>
	<TR> <TD>F</TD> <TD>Never</TD> </TR>
	</TABLE>

	<P><EM>File header updates are protected with transactions.</EM></P>

	<H2>Delete File.</H2>

	<H3>Header</H3>

	<TABLE>
	<TR>
		<TD>CLA</TD>
		<TD>INS</TD>
		<TD>P1</TD>
		<TD>P2</TD>
		<TD>P3</TD>
	</TR>
	<TR>
		<TD>80</TD>
		<TD>E4</TD>
		<TD>00</TD>
		<TD>00</TD>
		<TD>02</TD>
	</TR>
	</TABLE>

	<H3>Command data</H3>

	<TABLE>
	<TR>
		<TD>Byte(s)</TD>
		<TD>Description</TD>
		<TD>Length</TD>
	</TR>
	<TR>
		<TD>1 - 2</TD>
		<TD>FID</TD>
		<TD>2</TD>
	</TR>
	</TABLE>

	<H3>Status words</H3>

	<TABLE>
	<TR> <TD>SW</TD> <TD>Description</TD> </TR>
	<TR> <TD>90 00</TD> <TD>Command completed successfully</TD> </TR>
	<TR> <TD>67 00</TD> <TD>Wrong Lc</TD> </TR>
	<TR> <TD>69 82</TD> <TD>Security status not satisfied</TD> </TR>
	<TR> <TD>69 86</TD> <TD>Command not allowed (no DF selected)</TD> </TR>
	<TR> <TD>6A 82</TD> <TD>File not found</TD> </TR>
	<TR> <TD>6A 86</TD> <TD>Incorrect parameters P1-P2</TD> </TR>
	</TABLE>

	<H3>Description</H3>

	<P>The file with the specified FID is deleted from the current file,
	which must be a DF or the MF. Also the access conditions for delete
	must be satisfied and the specified file must be the last one in the
	DF/MF. Deletion of a DF, which contains EFs, is possible.</P>

	<P><EM>File header updates are protected with transactions.</EM></P>

	<H2>External Authentication.</H2>

	<H3>Header</H3>

	<TABLE>
	<TR>
		<TD>CLA</TD>
		<TD>INS</TD>
		<TD>P1</TD>
		<TD>P2</TD>
		<TD>P3</TD>
	</TR>
	<TR>
		<TD>80</TD>
		<TD>82</TD>
		<TD>00</TD>
		<TD>00</TD>
		<TD>08</TD>
	</TR>
	</TABLE>

	<H3>Command data</H3>

	<TABLE>
	<TR>
		<TD>Byte(s)</TD>
		<TD>Description</TD>
		<TD>Length</TD>
	</TR>
	<TR>
		<TD>1 - 8</TD>
		<TD>Encrypted Challenge</TD>
		<TD>8</TD>
	</TR>
	</TABLE>

	<H3>Status words</H3>

	<TABLE>
	<TR> <TD>SW</TD> <TD>Description</TD> </TR>
	<TR> <TD>90 00</TD> <TD>Command completed successfully</TD> </TR>
	<TR> <TD>63 CX</TD> <TD>Verification failed, retry counter specified</TD> </TR>
	<TR> <TD>67 00</TD> <TD>Wrong Lc</TD> </TR>
	<TR> <TD>69 83</TD> <TD>Authentication method blocked</TD> </TR>
	<TR> <TD>69 85</TD> <TD>Conditions of use not satisfied</TD> </TR>
	<TR> <TD>6A 86</TD> <TD>Incorrect parameters P1-P2</TD> </TR>
	<TR> <TD>6A 88</TD> <TD>Referenced data not found</TD> </TR>
	</TABLE>

	<H3>Description</H3>

	<P>Directly before this command a 8 byte challenge must be fetched
	from the card with the Get Challenge command. This challenge must
	then be encrypted with the TEA algorithm and the External Authentication
	key. For authentication the resulting data must then be sent back
	via this command.</P>

	<P><EM>Warning: Currently the retry counter is not handled in
	a secure way. For more information see e.g. Rankl/Effing.</EM></P>

	<H2>Get Challenge</H2>

	<H3>Header</H3>

	<TABLE>
	<TR>
		<TD>CLA</TD>
		<TD>INS</TD>
		<TD>P1</TD>
		<TD>P2</TD>
		<TD>P3</TD>
	</TR>
	<TR>
		<TD>80</TD>
		<TD>84</TD>
		<TD>00</TD>
		<TD>00</TD>
		<TD>08</TD>
	</TR>
	</TABLE>

	<H3>Response data</H3>

	<TABLE>
	<TR>
		<TD>Byte(s)</TD>
		<TD>Description</TD>
		<TD>Length</TD>
	</TR>
	<TR>
		<TD>1 - 8</TD>
		<TD>Random Challenge</TD>
		<TD>8</TD>
	</TR>
	</TABLE>

	<H3>Status words</H3>

	<TABLE>
	<TR> <TD>SW</TD> <TD>Description</TD> </TR>
	<TR> <TD>90 00</TD> <TD>Command completed successfully</TD> </TR>
	<TR> <TD>67 00</TD> <TD>Wrong Le</TD> </TR>
	<TR> <TD>6A 86</TD> <TD>Incorrect parameters P1-P2</TD> </TR>
	</TABLE>

	<H3>Description</H3>

	<P>Returns a challenge usable for the External Authentication command,
	which must then be the next issued command.</P>

	<P><EM>Warning: On the current hardware this challenge is produced with a
	PRNG based on TEA. It is not evaluated, if the current mechanism is
	appropriate for this task.</EM></P>

	<H2>Get Response.</H2>

	<H3>Header</H3>

	<TABLE>
	<TR>
		<TD>CLA</TD>
		<TD>INS</TD>
		<TD>P1</TD>
		<TD>P2</TD>
		<TD>P3</TD>
	</TR>
	<TR>
		<TD>80</TD>
		<TD>C0</TD>
		<TD>00</TD>
		<TD>00</TD>
		<TD>Length</TD>
	</TR>
	</TABLE>

	<H3>Response data after Internal Authenticate</H3>

	<TABLE>
	<TR>
		<TD>Byte(s)</TD>
		<TD>Description</TD>
		<TD>Length</TD>
	</TR>
	<TR>
		<TD>1 - 8</TD>
		<TD>Internal Authentication key encrypted challenge</TD>
		<TD>8</TD>
	</TR>
	</TABLE>

	<H3>Response data after Select</H3>

	<TABLE>
	<TR>
		<TD>Byte(s)</TD>
		<TD>Description</TD>
		<TD>Length</TD>
	</TR>
	<TR>
		<TD>1 - 2</TD>
		<TD>Size</TD>
		<TD>2</TD>
	</TR>
	<TR>
		<TD>3 - 4</TD>
		<TD>FID</TD>
		<TD>2</TD>
	</TR>
	<TR>
		<TD>5</TD>
		<TD>File type (DF: 38, EF: 00)</TD>
		<TD>1</TD>
	</TR>
	<TR>
		<TD>6</TD>
		<TD>Access conditions</TD>
		<TD>1</TD>
	</TR>
	</TABLE>

	<H3>Status words</H3>

	<TABLE>
	<TR> <TD>SW</TD> <TD>Description</TD> </TR>
	<TR> <TD>90 00</TD> <TD>Command completed successfully</TD> </TR>
	<TR> <TD>69 85</TD> <TD>Wrong condition (No data available)</TD> </TR>
	<TR> <TD>6A 86</TD> <TD>Incorrect parameters P1-P2</TD> </TR>
	<TR> <TD>6C XX</TD> <TD>Wrong Le</TD> </TR>
	</TABLE>

	<H3>Description</H3>

	<P>This command fetches data from Internal Authentication and Select.
	These commands signal the availability of data with the status word
	61XX. For more details about the data returned after a Select, see
	at the Create command.</P>

	<H2>Internal Authentication.</H2>

	<H3>Header</H3>

	<TABLE>
	<TR>
		<TD>CLA</TD>
		<TD>INS</TD>
		<TD>P1</TD>
		<TD>P2</TD>
		<TD>P3</TD>
	</TR>
	<TR>
		<TD>80</TD>
		<TD>88</TD>
		<TD>00</TD>
		<TD>Decrypt</TD>
		<TD>08</TD>
	</TR>
	</TABLE>

	<P>This command decrypts the data if P2 is 01. This functionality is
	only enabled when CONF_WITH_DECRYPT is set to 1.</P>

	<H3>Command data</H3>

	<TABLE>
	<TR>
		<TD>Byte(s)</TD>
		<TD>Description</TD>
		<TD>Length</TD>
	</TR>
	<TR>
		<TD>1 - 8</TD>
		<TD>Random Challenge</TD>
		<TD>8</TD>
	</TR>
	</TABLE>

	<H3>Status words</H3>

	<TABLE>
	<TR> <TD>SW</TD> <TD>Description</TD> </TR>
	<TR> <TD>61 08</TD> <TD>Command completed successfully (Data available)</TD> </TR>
	<TR> <TD>67 00</TD> <TD>Wrong Lc</TD> </TR>
	<TR> <TD>69 85</TD> <TD>Conditions of use not satisfied</TD> </TR>
	<TR> <TD>6A 86</TD> <TD>Incorrect parameters P1-P2</TD> </TR>
	<TR> <TD>6A 88</TD> <TD>Referenced data not found</TD> </TR>
	</TABLE>

	<H3>Description</H3>

	<P>Provieds a random challenge to the card. It encryptes the challenge
	with TEA and the Internal Authentication key and makes it available
	via Get Response. For the successful execution either the PIN or the
	External Authentication key must have been successfully verified
	previously.</P>

	<P>Because of possibility to decrypt data, this command can easily
	be used to implement some form of Remotely Keyed Encryption.</P>

	<H2>Read Binary.</H2>

	<H3>Header</H3>

	<TABLE>
	<TR>
		<TD>CLA</TD>
		<TD>INS</TD>
		<TD>P1</TD>
		<TD>P2</TD>
		<TD>P3</TD>
	</TR>
	<TR>
		<TD>80</TD>
		<TD>B0</TD>
		<TD>Offset high</TD>
		<TD>Offest low</TD>
		<TD>Length</TD>
	</TR>
	</TABLE>

	<H3>Response data</H3>

	<TABLE>
	<TR>
		<TD>Byte(s)</TD>
		<TD>Description</TD>
		<TD>Length</TD>
	</TR>
	<TR>
		<TD>1 - Length</TD>
		<TD>Data</TD>
		<TD>Length</TD>
	</TR>
	</TABLE>

	<H3>Status words</H3>

	<TABLE>
	<TR> <TD>SW</TD> <TD>Description</TD> </TR>
	<TR> <TD>90 00</TD> <TD>Command completed successfully</TD> </TR>
	<TR> <TD>69 82</TD> <TD>Security status not satisfied</TD> </TR>
	<TR> <TD>69 86</TD> <TD>Command not allowed (no EF selected)</TD> </TR>
	<TR> <TD>6A 82</TD> <TD>File not found</TD> </TR>
	<TR> <TD>6A 84</TD> <TD>File to short</TD> </TR>
	<TR> <TD>6B 00</TD> <TD>Wrong parameters (offset outside EF)</TD> </TR>
	</TABLE>

	<H3>Description</H3>

	<P>Reads <EM>Lenght</EM> bytes from <EM>Offset</EM> of the currently
	selected EF.</P>

	<H2>Select File.</H2>

	<H3>Header</H3>

	<TABLE>
	<TR>
		<TD>CLA</TD>
		<TD>INS</TD>
		<TD>P1</TD>
		<TD>P2</TD>
		<TD>P3</TD>
	</TR>
	<TR>
		<TD>80</TD>
		<TD>A4</TD>
		<TD>00</TD>
		<TD>00</TD>
		<TD>02</TD>
	</TR>
	</TABLE>

	<H3>Command data</H3>

	<TABLE>
	<TR>
		<TD>Byte(s)</TD>
		<TD>Description</TD>
		<TD>Length</TD>
	</TR>
	<TR>
		<TD>1 - 2</TD>
		<TD>FID</TD>
		<TD>2</TD>
	</TR>
	</TABLE>

	<H3>Status words</H3>

	<TABLE>
	<TR> <TD>SW</TD> <TD>Description</TD> </TR>
	<TR> <TD>61 06</TD> <TD>Command completed successfully</TD> </TR>
	<TR> <TD>67 00</TD> <TD>Wrong Lc</TD> </TR>
	<TR> <TD>6A 82</TD> <TD>File not found</TD> </TR>
	<TR> <TD>6A 86</TD> <TD>Incorrect parameters P1-P2</TD> </TR>
	</TABLE>

	<H3>Description</H3>

	<P>It tries to select a file with the specified FID and searches
	for it in the following order:</P>

	<OL>
	<LI>EF in the current MF/DF</LI>
	<LI>DF in MF</LI>
	</OL>

	<P>If FID is 3F00, the MF is always selected.</P>

	<H2>Unblock PIN.</H2>

	<H3>Header</H3>

	<TABLE>
	<TR>
		<TD>CLA</TD>
		<TD>INS</TD>
		<TD>P1</TD>
		<TD>P2</TD>
		<TD>P3</TD>
	</TR>
	<TR>
		<TD>80</TD>
		<TD>2C</TD>
		<TD>00</TD>
		<TD>00</TD>
		<TD>10</TD>
	</TR>
	</TABLE>

	<H3>Command data</H3>

	<TABLE>
	<TR>
		<TD>Byte(s)</TD>
		<TD>Description</TD>
		<TD>Length</TD>
	</TR>
	<TR>
		<TD>1 - 8</TD>
		<TD>PUK</TD>
		<TD>8</TD>
	</TR>
	<TR>
		<TD>9 - 16</TD>
		<TD>New PIN</TD>
		<TD>8</TD>
	</TR>
	</TABLE>

	<H3>Status words</H3>

	<TABLE>
	<TR> <TD>SW</TD> <TD>Description</TD> </TR>
	<TR> <TD>90 00</TD> <TD>Command completed successfully</TD> </TR>
	<TR> <TD>63 CX</TD> <TD>Verification failed, retry counter specified</TD> </TR>
	<TR> <TD>67 00</TD> <TD>Wrong Lc</TD> </TR>
	<TR> <TD>69 83</TD> <TD>Authentication method blocked</TD> </TR>
	<TR> <TD>6A 86</TD> <TD>Incorrect parameters P1-P2</TD> </TR>
	<TR> <TD>6A 88</TD> <TD>Referenced data not found</TD> </TR>
	</TABLE>

	<H3>Description</H3>

	<P>This command changes the PIN of the card and resets the retry
	counter of this PIN. The first eight bytes of the data is the
	PUK, which is checked for correctness.	The second four bytes
	of the data is the new PIN, which will be written. On success
	the AUTH_FLAG_PIN bit in authstate is set, on fauilure it is
	cleared.<P>

	<P><EM>Warning: Currently the retry counter is not handled in
	a secure way. For more information see e.g. Rankl/Effing.</EM></P>

	<P><EM>PIN update is protected with transactions.</EM></P>

	<H2>Update Binary.</H2>

	<H3>Header</H3>

	<TABLE>
	<TR>
		<TD>CLA</TD>
		<TD>INS</TD>
		<TD>P1</TD>
		<TD>P2</TD>
		<TD>P3</TD>
	</TR>
	<TR>
		<TD>80</TD>
		<TD>D6</TD>
		<TD>Offset high</TD>
		<TD>Offset low</TD>
		<TD>Length</TD>
	</TR>
	</TABLE>

	<H3>Command data</H3>

	<TABLE>
	<TR>
		<TD>Byte(s)</TD>
		<TD>Description</TD>
		<TD>Length</TD>
	</TR>
	<TR>
		<TD>1 - Length</TD>
		<TD>Data</TD>
		<TD>Length</TD>
	</TR>
	</TABLE>

	<H3>Status words</H3>

	<TABLE>
	<TR> <TD>SW</TD> <TD>Description</TD> </TR>
	<TR> <TD>90 00</TD> <TD>Command completed successfully</TD> </TR>
	<TR> <TD>65 00</TD> <TD>Memory failure (unsuccessful writing)</TD> </TR>
	<TR> <TD>67 00</TD> <TD>Wrong Le</TD> </TR>
	<TR> <TD>69 82</TD> <TD>Security status not satisfied</TD> </TR>
	<TR> <TD>69 86</TD> <TD>Command not allowed (no EF selected)</TD> </TR>
	<TR> <TD>6A 82</TD> <TD>File not found</TD> </TR>
	<TR> <TD>6A 84</TD> <TD>File to short</TD> </TR>
	<TR> <TD>6B 00</TD> <TD>Wrong parameters (offset outside EF)</TD> </TR>
	</TABLE>

	<H3>Description</H3>

	<P>Writes <EM>Lenght</EM> bytes to <EM>Offset</EM> of the currently
	selected EF.</P>

	<P><EM>Data updates are NOT protected with transactions.</EM></P>

	<H2>Verify Key</H2>

	<H3>Header</H3>

	<TABLE>
	<TR>
		<TD>CLA</TD>
		<TD>INS</TD>
		<TD>P1</TD>
		<TD>P2</TD>
		<TD>P3</TD>
	</TR>
	<TR>
		<TD>80</TD>
		<TD>2A</TD>
		<TD>00</TD>
		<TD>00</TD>
		<TD>10</TD>
	</TR>
	</TABLE>

	<H3>Command data</H3>

	<TABLE>
	<TR>
		<TD>Byte(s)</TD>
		<TD>Description</TD>
		<TD>Length</TD>
	</TR>
	<TR>
		<TD>1 - 16</TD>
		<TD>External Authentication Key</TD>
		<TD>16</TD>
	</TR>
	</TABLE>

	<H3>Status words</H3>

	<TABLE>
	<TR> <TD>SW</TD> <TD>Description</TD> </TR>
	<TR> <TD>90 00</TD> <TD>Command completed successfully</TD> </TR>
	<TR> <TD>63 CX</TD> <TD>Verification failed, retry counter specified</TD> </TR>
	<TR> <TD>67 00</TD> <TD>Wrong Lc</TD> </TR>
	<TR> <TD>69 83</TD> <TD>Authentication method blocked</TD> </TR>
	<TR> <TD>6A 86</TD> <TD>Incorrect parameters P1-P2</TD> </TR>
	<TR> <TD>6A 88</TD> <TD>Referenced data not found</TD> </TR>
	</TABLE>

	<H3>Description</H3>

	<P>Verifies the External Authentication key as 16 byte plain text
	in the data part of the command.  On success the AUTH_FLAG_KEY
	bit in authstate is set, on failure it is cleared.</P>

	<P><EM>Warning: Currently the retry counter is not handled in
	a secure way. For more information see e.g. Rankl/Effing.</EM></P>

	<H2>Verify PIN</H2>

	<H3>Header</H3>

	<TABLE>
	<TR>
		<TD>CLA</TD>
		<TD>INS</TD>
		<TD>P1</TD>
		<TD>P2</TD>
		<TD>P3</TD>
	</TR>
	<TR>
		<TD>80</TD>
		<TD>20</TD>
		<TD>00</TD>
		<TD>00</TD>
		<TD>08</TD>
	</TR>
	</TABLE>

	<H3>Command data</H3>

	<TABLE>
	<TR>
		<TD>Byte(s)</TD>
		<TD>Description</TD>
		<TD>Length</TD>
	</TR>
	<TR>
		<TD>1 - 8</TD>
		<TD>PIN</TD>
		<TD>8</TD>
	</TR>
	</TABLE>

	<H3>Status words</H3>

	<TABLE>
	<TR> <TD>SW</TD> <TD>Description</TD> </TR>
	<TR> <TD>90 00</TD> <TD>Command completed successfully</TD> </TR>
	<TR> <TD>63 CX</TD> <TD>Verification failed, retry counter specified</TD> </TR>
	<TR> <TD>67 00</TD> <TD>Wrong Lc</TD> </TR>
	<TR> <TD>69 83</TD> <TD>Authentication method blocked</TD> </TR>
	<TR> <TD>6A 86</TD> <TD>Incorrect parameters P1-P2</TD> </TR>
	<TR> <TD>6A 88</TD> <TD>Referenced data not found</TD> </TR>
	</TABLE>

	<H3>Description</H3>

	<P>This command verifies the PIN. The four data bytes contain the
	user supplied PIN, which is checked for correctness.  On success
	the AUTH_FLAG_PIN bit in authstate is set, on fauilure it is
	cleared.<P>

	<P><EM>Warning: Currently the retry counter is not handled in
	a secure way. For more information see e.g. Rankl/Effing.</EM></P>

	<H1>Developers</H1>

	<UL>
	<LI>Matthias Bruestle <m@mbsks.franken.de></LI>
	</UL>

	<H1>License</H1>

	<P>This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License as
	published by the Free Software Foundation; either version 2 of
	the License, or (at your option) any later version.</P>

	<P>This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.</P>

	<P>You should have received a copy of the GNU General Public
	License along with this program; if not, write to the Free
	Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
	MA 02111-1307 USA</P>
*/

#ifndef SOSSE_MAIN_H
#define SOSSE_MAIN_H

#if defined(CTAPI)
void sosse_main( void );
#endif

#endif /* SOSSE_MAIN_H */
