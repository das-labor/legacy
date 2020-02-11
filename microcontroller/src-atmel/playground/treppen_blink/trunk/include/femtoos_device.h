/*
 * Femto OS v 0.92 - Copyright (C) 2008-2010 Ruud Vlaming
 *
 * This file is part of the Femto OS distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Please note that, due to the GPLv3 license, for application of this
 * work and/or combined work in embedded systems special obligations apply.
 * If these are not to you liking, please know the Femto OS is dual
 * licensed. A commercial license and support are available.
 * See http://www.femtoos.org/ for details.
 */

/* Port check, see a list of all possible devices at:
 * http://www.nongnu.org/avr-libc/user-manual/using_tools.html */

#if defined(__AVR_ATmega128__)

  /* port is present and tested */
  #include "femtoos_ATmega128.asm"

#elif defined(__AVR_ATmega1280__)

  /* port is present and tested */
  #include "femtoos_ATmega1280.asm"

#elif defined(__AVR_ATmega1281__)

  /* port is present and tested */
  #include "femtoos_ATmega1281.asm"

#elif defined(__AVR_ATmega1284P__)

  /* port is present but untested on the actual device. */
  #warning "Port for this device (ATmega1284P) is untested"
  #include "femtoos_ATmega1284P.asm"

#elif defined(__AVR_ATmega16__)

  /* port is present and tested */
  #include "femtoos_ATmega16A.asm"

#elif defined(__AVR_ATmega162__)

  /* port is present and tested */
  #include "femtoos_ATmega162.asm"

#elif defined(__AVR_ATmega164P__)

  /* port is present and tested */
  #include "femtoos_ATmega164P.asm"

#elif defined(__AVR_ATmega168__)

  /* port is present and tested */
  #include "femtoos_ATmega168.asm"

#elif defined(__AVR_ATmega168P__)

  /* port is present and tested */
  #include "femtoos_ATmega168P.asm"

#elif defined(__AVR_ATmega2560__)

  /* port is present and tested */
  #include "femtoos_ATmega2560.asm"

#elif defined(__AVR_ATmega2561__)

  /* port is present and tested */
  #include "femtoos_ATmega2561.asm"

#elif defined(__AVR_ATmega32__)

  /* port is present and tested */
  #include "femtoos_ATmega32A.asm"

#elif defined(__AVR_ATmega324P__)

  /* port is present and tested */
  #include "femtoos_ATmega324P.asm"

#elif defined(__AVR_ATmega325__)

  /* port is present and tested */
  #include "femtoos_ATmega325.asm"

#elif defined(__AVR_ATmega3250__)

  /* port is present and tested */
  #include "femtoos_ATmega3250.asm"

#elif defined(__AVR_ATmega325P__)

  /* port is present and tested */
  #include "femtoos_ATmega325P.asm"

#elif defined(__AVR_ATmega3250P__)

  /* port is present and tested */
  #include "femtoos_ATmega3250P.asm"

#elif defined(__AVR_ATmega328P__)

  /* port is present and tested */
  #include "femtoos_ATmega328P.asm"

#elif defined(__AVR_ATmega48__)

  /* port is present and tested */
  #include "femtoos_ATmega48.asm"

#elif defined(__AVR_ATmega48P__)

  /* port is present and tested */
  #include "femtoos_ATmega48P.asm"

#elif defined(__AVR_ATmega64__)

  /* port is present and tested */
  #include "femtoos_ATmega64.asm"

#elif defined(__AVR_ATmega645__)

  /* port is present and tested */
  #warning "Port for this device (ATmega645) is untested"
  #include "femtoos_ATmega645.asm"

#elif defined(__AVR_ATmega6450__)

  /* port is present and tested */
  #warning "Port for this device (ATmega6450) is untested"
  #include "femtoos_ATmega6450.asm"

#elif defined(__AVR_ATmega640__)

  /* port is present and tested */
  #include "femtoos_ATmega640.asm"

#elif defined(__AVR_ATmega644__)

  /* port is present and tested */
  #include "femtoos_ATmega644.asm"

#elif defined(__AVR_ATmega644P__)

  /* port is present and tested */
  #include "femtoos_ATmega644P.asm"

#elif defined(__AVR_ATmega8__)

  /* port is present and tested */
  #include "femtoos_ATmega8.asm"

#elif defined(__AVR_ATmega8515__)

  /* port is present and tested */
  #include "femtoos_ATmega8515.asm"

#elif defined(__AVR_ATmega8535__)

  /* port is present and tested */
  #include "femtoos_ATmega8535.asm"

#elif defined(__AVR_ATmega88__)

  /* port is present and tested */
  #include "femtoos_ATmega88.asm"

#elif defined(__AVR_ATmega88P__)

  /* port is present and tested */
 #include "femtoos_ATmega88P.asm"

#elif defined(__AVR_ATtiny2313__)

  /* port is present and tested */
  #include "femtoos_ATtiny2313.asm"

#elif defined(__AVR_ATtiny24__)

  /* port is present and tested */
  #include "femtoos_ATtiny24.asm"

#elif defined(__AVR_ATtiny25__)

  /* port is present and tested */
  #include "femtoos_ATtiny25.asm"

#elif defined(__AVR_ATtiny261__)

  /* port is present and tested */
  #include "femtoos_ATtiny261.asm"

#elif defined(__AVR_ATtiny43U__)

  /* port is present but untested on the actual device. */
  #warning "Port for this device (ATtiny43U) is untested"
  #include "femtoos_ATtiny43U.asm"

#elif defined(__AVR_ATtiny44__)

  /* port is present and tested */
  #include "femtoos_ATtiny44.asm"

#elif defined(__AVR_ATtiny45__)

  /* port is present and tested */
  #include "femtoos_ATtiny45.asm"

#elif defined(__AVR_ATtiny461__)

  /* port is present and tested */
  #include "femtoos_ATtiny461.asm"

#elif defined(__AVR_ATtiny48__)

  /* port is present and tested */
  #include "femtoos_ATtiny48.asm"

#elif defined(__AVR_ATtiny84__)

  /* port is present and tested */
  #include "femtoos_ATtiny84.asm"

#elif defined(__AVR_ATtiny85__)

  /* port is present and tested */
  #include "femtoos_ATtiny85.asm"

#elif defined(__AVR_ATtiny861__)

  /* port is present and tested */
  #include "femtoos_ATtiny861.asm"

#elif defined(__AVR_ATtiny88__)

  /* port is present and tested */
  #include "femtoos_ATtiny88.asm"

#else

  /* ports are not present. */
  #error "Port for this device is absent"

#endif


/**
 * The section below overwrites startup files, utilizing the space in a more
 * efficient way. This is the bookkeeping normally provided by GCC. The vectorspace
 * is not changed. Note there is no need to initialize the Status Register
 * since the initial value is 0x00. For all ports it is identical.
 */
#if defined(FEMTOOS_DEVICE_ASSEMBLY) && (defSysGCCstartup == cfgOverride)

  .global __init
  .global __do_clear_bss
  /* TODO some devices cannot handle the definition of a pseudo method
   * .global __do_copy_data, to suppress the one definned by gcc. I do
   * have a solution up to this moment, but we just take an other name.
   * Unfortunately this costs a lot of bytes. Maybe this is solved in
   * gcc 4.4 but we are not ready for that yet.  */
  .global __start_main

  .section .init0,"ax"
  __init:
    clr  r1
  __do_clear_bss:
  #if (devRAMbegin >= 0x100)
    ldi  r25, hi8(devRAMbegin)
  #endif
    ldi  r26, lo8(devRAMend)
    ldi  r27, hi8(devRAMend)
  __do_clear_bss_loop:
    st   -X, r1
    cpi  r26, lo8(devRAMbegin)
  #if (devRAMbegin >= 0x100)
    cpc  r27, r25
  #else
    cpc  r27, r1
  #endif
    brne __do_clear_bss_loop
  __do_stack_def:
    ldi  r26, lo8(xOS+(OSstackInit))
    out  (_SFR_IO_ADDR(SPL)), r26
  #if (devRAMend > 0x100)
    ldi  r27, hi8(xOS+(OSstackInit))
    out  (_SFR_IO_ADDR(SPH)), r27
  #endif
  __start_main:
  #if (devFLASHsize <= 0x1000U)
    rjmp    main
  #else
    jmp    main
  #endif
/* TODO: gcc still dumps in some junk code here which can never
 * be executed. I don't know how to avoid this. the tip from Bjoern
 * http://www.mail-archive.com/avr-gcc-list@nongnu.org/msg02390.html
 * does not seem to work properly. */

#endif
