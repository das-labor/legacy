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

#ifndef FEMTOOS_DEVICE_DEFINITIONS
#define FEMTOOS_DEVICE_DEFINITIONS

/**
 * Parameter definitions. Define the parameters for your device. Parameters that
 * are not present should be defined cfgUndefined.
 */

/* Physical parameters */
#define devClockFrequency 8000000LL
#define devRAMbegin       0x0060
#define devRAMend         0x00E0
#define devFLASHsize      0x0400U
#define devEEPROMsize     0x0080U

/* Communication */
#define devTraceDataPin   PB0
#define devTraceReadyPin  PB1
#define devTraceAckPin    PB2
#define devTraceComPORT   PORTB
#define devTraceComPIN    PINB
#define devTraceComDRR    DDRB
#define devErrorComPORT   PORTA
#define devErrorComDDR    DDRA
#define devLedPORT        PORTA
#define devLedPIN         PINA
#define devLedDRR         DDRA
#define devSwitchPORT     PORTB
#define devSwitchPIN      PINB
#define devSwitchDRR      DDRB

/* Power management */
#define devSMCR           MCUCR
#define devSM2            cfgUndefined
#define devSM1            SM1
#define devSM0            SM0
#define devSE             SE

/* Reset management */
#define devMCUSR          MCUSR
#define devWDRF           WDRF
#define devBORF           BORF
#define devEXTRF          EXTRF
#define devPORF           PORF

/* Clock Settings */
#define devCLKPR          CLKPR
#define devCLKPCE         CLKPCE
#define devCLKPS3         CLKPS3
#define devCLKPS2         CLKPS2
#define devCLKPS1         CLKPS1
#define devCLKPS0         CLKPS0


/* Watchdog settings */
#define devWDTCR          WDTCSR
#define devWDIF           WDIF
#define devWDIE           WDIE
#define devWDP3           WDP3
#define devWDCE           WDCE
#define devWDE            WDE
#define devWDP2           WDP2
#define devWDP1           WDP1
#define devWDP0           WDP0

/* Timer settings  Timer */
#define devTIMSK          TIMSK0
#define devOCIE           OCIE0A
#define devTOIE           TOIE0
#define devTIFR           TIFR0
#define devOCF            OCF0A
#define devTOV            TOV0
#define devTCCRA          TCCR0A
#define devWGM            WGM01
#define devTCCRB          TCCR0B
#define devOCR            OCR0A
#define devTCNT           TCNT0
#define devPrescale_1     0x01
#define devPrescale_8     0x02
#define devPrescale_32    cfgUndefined
#define devPrescale_64    0x03
#define devPrescale_128   cfgUndefined
#define devPrescale_256   0x04
#define devPrescale_1024  0x05

/* EEPROM settings */
#define devEEARH          cfgUndefined
#define devEEARL          EEARL
#define devEEDR           EEDR
#define devEECR           EECR
#define devEEPM1          EEPM1
#define devEEPM0          EEPM0
#define devEEMPE          EEMPE
#define devEEPE           EEPE
#define devEERE           EERE

/* External interrupt settings */
#define devEIR            GIMSK
#define devINT            INT0

/* Auxilairy Storage, devAuxSysReg must be in low IO if
 * used, value of devAuxEventReg may equal to devAuxSysReg. */
#define devAuxEventReg    GPIOR2
#define devAuxSysReg      GPIOR2
#define devAuxSysRegBit   7

/* Features */
#define devSupPINWrite    cfgTrue

/* Interrupt service routines.
 * Service routines that are not present should not be
 * defined and cannot be defined cfgUndefined. */
#define devSigTimerCompare     SIG_OUTPUT_COMPARE0A
#define devSigWatchdogTimeout  SIG_WATCHDOG_TIMEOUT
#define devSigTimerOverflow    SIG_OVERFLOW0
#define devSigExternalInt      SIG_INTERRUPT0

#endif /* FEMTOOS_DEVICE_DEFINITIONS */


#ifdef FEMTOOS_DEVICE_ASSEMBLY

/* In order to let gcc define the stack on to the top of the arrar OsStack.
 * that is &xOS.StackOS[OSstackInit], we must define xOSstackShift. The compiler
 * must be called with the option:
 *   -Wl,--defsym=__stack=xOS+xOSstackShift
 * until we find a way to set __stack to xOS+OSstackInit directly from here.
 * However, it does not seem possible to use
 * .set xOSstackShift, xOs+OSstackInit
 * Not that we do not make use of this facility when (defSysGCCstartup == cfgReplace)
 * or (defSysGCCstartup == cfgOverride) but we must still define the constant.
 */
  .global xOSstackShift
  .set xOSstackShift, (OSstackInit)

/**
 * The section below overwrites the vector table, utilizing the space in a more
 * efficient way. Thus we must implement the expected interrupt exactly on the right
 * place, this code is highly hardware bound. All bookkeeping normally provided
 * by GCC must be done manually. Note there is no need to initialize the Status Register
 * since the initial value is 0x00. Do not forget to change rjmp to jmp when needed.
 * Not that, any code you normally in portInit() must now be put here since the portInit
 * method is replaced by an define. For example, tracing is not possible with cfgReplace
 * since we did not include it here.
 */
#if (defSysGCCstartup == cfgReplace)

  .global __init
  .global __do_clear_bss
  .global __do_copy_data
  .global __do_device_init
  .global __do_watchdog_reset
  .global __do_watchdog_reset_ext

  .section .vectors,"ax"
  __init:
  /* Catch the reset interrupt here. */
    clr  r1
  __do_clear_bss:
    ldi  r26, lo8(devRAMend)
    ldi  r27, hi8(devRAMend)
    rjmp __do_watchdog_reset
  __interrupt_vector_definitions:
  /* Catch the watchdog interrupt here 0x0004. */
  #if (cfgSysSqueezeState == cfgFalse)
    sbi  (_SFR_IO_ADDR(devAuxSysReg)), devAuxSysRegBit
  #else
    set
  #endif
    reti
  __do_watchdog_reset:
    out  (_SFR_IO_ADDR(devMCUSR)), r1
    ldi  r24, ((1<<devWDCE) | (1<<devWDE))
    rjmp __do_watchdog_reset_ext
  /* Catch the timer interrupt here 0x0009. */
    rjmp privTickYield
  __do_watchdog_reset_ext:
    out  (_SFR_IO_ADDR(devWDTCR)), r24
    out  (_SFR_IO_ADDR(devWDTCR)), r1
  __do_device_init:
    ldi  r24, (1<<devCLKPCE)
    out  (_SFR_IO_ADDR(devCLKPR)), r24
    out  (_SFR_IO_ADDR(devCLKPR)), r1
  __do_clear_bss_loop:
    st   -X, r1
    cpi  r26, lo8(devRAMbegin)
    cpc  r27, r1 /* hi8(devRAMbegin) is assumed to be zero! */
    brne __do_clear_bss_loop
  __do_stack_def:
    ldi  r30, lo8(xOS+(OSstackInit))
    out  (_SFR_IO_ADDR(SPL)), r30
  __do_copy_data:
    rjmp    main

#endif

#endif /* FEMTOOS_DEVICE_ASSEMBLY */

