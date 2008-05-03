USB-servo-2. (aka RobotController)

(a continuation of USB-Servo (aka Mousepointerpointer))

This device was designed to controll standard hobby radio controll servos via a PCs USB port.

Standard RC servos need a power supply of between 4.8 and 6 volts. They also have a Pulse Width Modulation (PWM) signal input which controlls the angle of the servo. This device supplies up to 6 such servos with a 5V supply and the PWM signal to controll the servo. The combined load of your servos should not exceed 3 amps. Check your servo's documentation for pinout and max current draw.

It takes an external power supply of anywhere between 7 and 30 Volts to power the servos. The onboard AVR microcontroller is powered drectly from the USB port.

I built it to controll servos (and PWM motor controllers) on my latest robot. It should work with any RC servo or other device designed to work with radio controll receiver's outputs.

The firmware for the AVR microcontroller is written for the AVR-GCC compiler and as such should compile under either Windows or Linux (but is untested under windows so far).

The device currently has a Linux aplication program to controll the device. There is no reason that this device should not work under windows other than a controlling application or driver has not been written yet.

If anyone has experience programming AVRs in a windows enviroment or knows how to write a windows or Mac driver for this USB controller circuit please contact me so we can document this for others.

Note that this device has not been tested on a wide range of servos or computers. No warrenty or support is offered in any way. It is up to the user to research their choice of hardware and decide wether they are safe. In short, if this destroys your computer and/or servo, don't blame me. On the other hand, mine works fine thank you.

 

Building and Installing.

This page presumes you are able to do the following:

1. Circuit.

The circuit diagram was created using the Freeware version of CadSoft's EAGLE Layout Editor.

The schematic can be found in the subdirectory "circuit".

***Insert circuit diagram here***

 

2. Firmware.

The firmware for this project requires avr-gcc and avr-libc (a   C-library for the AVR controller). Please read the instructions at http://www.nongnu.org/avr-libc/user-manual/install_tools.html for how to install the GNU toolchain (avr-gcc, assembler, linker etc.) and avr-libc.

Once you have the GNU toolchain for AVR microcontrollers installed, you can run "make" in the subdirectory "firmware". You may have to edit the Makefile to use your preferred downloader with "make program". 

If working with a brand-new controller, you may have to set the fuse-bits to use the external crystal:

  make fuse

Afterwards, you can compile and flash to the device:

  make flash

 
If you are new to AVR programming, there is a good starting point here:

http://www.instructables.com/id/E5H5UDWB5UEUKIKV8V?ALLSTEPS

 

3. Commandline client.

The command line tool requires libusb. Please take the packages from your system's distribution or download libusb from http://libusb.sourceforge.net/ and install it before you compile. Change to directory "commandline", check the Makefile and edit the settings if required and type

  make

This will build the unix executable "usb-servo" which can be used to control the device.

 
Usage:

Connect the device to the USB-port. If it isn't already, the servos will all move to the 0-position.

Then use the commandline-client as follows:

  usb-servo status <servo>

  usb-servo set <servo> <angle>

  usb-servo test

Paramiters:

    * servo: The servo to control. 0 is the first servo, 1 is the 2nd, etc.
    * angle: The angle you want to set the servo to. 0 is full left, 255 is full right.

 

Examples.

Set a new angle on a servo:

  usb-servo set 4 23

This sets the servo number 0 to the angle 23. 4 is full left, 255 is full right, so with 23 the servo will be almost on the left side.

Query the last servo set:

  usb-servo status

This will tell you the angle the servo is currently put to.

  Current servo angle: 42

 

Thanks for the Code Guys!

This module is based on the following 3 projects.

I'd like to take this opertunity to thank all the contributers to these projects.

I had the easy job here of copying and pasting their code. Thanks guys! Rocking!

AVR USB A Firmware-Only USB Driver for Atmel AVR Microcontrollers.

USB-Servo (aka Mousepointerpointer) The USB-Servo is a device to control a servo via USB by Ronald Schaten.

Procyon AVRlib C-Language Function Library for Atmel AVR Processors.

Ronald Schaten's USB-Servo project is based on the AVR USB project.

I basically took Ronald's project and in the AVR firmware replaced his single servo timing routine with the more robust AVRlib servo controll routines allowing for controll of more than one servo and better PWM timing.

In the PC based controll programm i then added options for more than one servo and a few checks to make sure the USB-Servo code has not timed out and repeate the command if necisarry.

The only real change i made to the circuit was to add a power source independant from the USB port to prevent too much current draw from the USB port.

 

 

ToDo.

There is still no functionality for the header pins SV1. I intend to make them general purpose I/O pins.

There is still no way to controll the "enable" line from the AVR to the voltage regulator. This will eventually allow all the servos to be powered down when not in use for power saving.

 

 

About the Licence.

My work, as i have allready covered in the previous section, is based on the work of others. Any changes i have made are licensed under the GNU General Public License (GPL).

The others work, except for the USB driver, are licensed under the GNU General Public License (GPL). A copy of the GPL is included in License.txt. The USB driver itself is licensed under a special license by Objective Development. See firmware/usbdrv/License.txt for further info.


