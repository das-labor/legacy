/*
 * Femto OS v 0.91 - Copyright (C) 2008-2009 Ruud Vlaming
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
#define devRAMend         0x0460
#define devFLASHsize      0x2000U
#define devEEPROMsize     0x0200U

/* Communication */
#define devTraceDataPin   PC0
#define devTraceReadyPin  PC1
#define devTraceAckPin    PC2
#define devTraceComPORT   PORTC
#define devTraceComPIN    PINC
#define devTraceComDRR    DDRC
#define devErrorComPORT   PORTB
#define devErrorComDDR    DDRB
#define devLedPORT        PORTB
#define devLedPIN         PINB
#define devLedDRR         DDRB
#define devSwitchPORT     PORTD
#define devSwitchPIN      PIND
#define devSwitchDRR      DDRD

/* Power management */
#define devSMCR           MCUCR
#define devSM2            SM2
#define devSM1            SM1
#define devSM0            SM0
#define devSE             SE

/* Reset management */
#define devMCUSR          MCUCSR
#define devWDRF           WDRF
#define devBORF           BORF
#define devEXTRF          EXTRF
#define devPORF           PORF

/* Clock Settings */
#define devCLKPR          cfgUndefined
#define devCLKPCE         cfgUndefined
#define devCLKPS3         cfgUndefined
#define devCLKPS2         cfgUndefined
#define devCLKPS1         cfgUndefined
#define devCLKPS0         cfgUndefined

/* Watchdog settings */
#define devWDTCR          WDTCR
#define devWDIF           cfgUndefined
#define devWDIE           cfgUndefined
#define devWDP3           cfgUndefined
#define devWDCE           WDTOE
#define devWDE            WDE
#define devWDP2           WDP2
#define devWDP1           WDP1
#define devWDP0           WDP0

/* Timer settings  Timer */
#define devTIMSK          TIMSK
#define devOCIE           OCIE0
#define devTOIE           TOIE0
#define devTIFR           TIFR
#define devOCF            OCF0
#define devTOV            TOV0
#define devTCCRA          TCCR0
#define devWGM            WGM01
#define devTCCRB          TCCR0
#define devOCR            OCR0
#define devTCNT           TCNT0
#define devPrescale_1     0x01
#define devPrescale_8     0x02
#define devPrescale_32    cfgUndefined
#define devPrescale_64    0x03
#define devPrescale_128   cfgUndefined
#define devPrescale_256   0x04
#define devPrescale_1024  0x05

/* EEPROM settings */
#define devEEARH          EEARH
#define devEEARL          EEARL
#define devEEDR           EEDR
#define devEECR           EECR
#define devEEPM1          cfgUndefined
#define devEEPM0          cfgUndefined
#define devEEMPE          EEMWE
#define devEEPE           EEWE
#define devEERE           EERE

/* External interrupt settings */
#define devEIR            GICR
#define devINT            INT0

/* Auxilairy Storage */
#define devAuxEventReg    UBRRL
#define devAuxSysReg      UBRRL
#define devAuxSysRegBit   7


/**
 * Interrupt service routines.
 *
 * Service routines that are not present should not be defined and
 * cannot be defined cfgUndefined.
 */
#define devSigTimerCompare     SIG_OUTPUT_COMPARE0
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

  .section .vectors,"ax"
  __init:
  /* Catch the reset interrupt here. */
    clr  r1
  __do_watchdog_reset:
    out  (_SFR_IO_ADDR(devMCUSR)), r1
    ldi  r24, ((1<<devWDCE) | (1<<devWDE))
    out  (_SFR_IO_ADDR(devWDTCR)), r24
    out  (_SFR_IO_ADDR(devWDTCR)), r1
  __do_clear_bss:
    ldi  r24, lo8(devRAMbegin)
    ldi  r25, hi8(devRAMbegin)
    ldi  r26, lo8(devRAMend)
    ldi  r27, hi8(devRAMend)
  __do_clear_bss_loop:
    st   -X, r1
    cp   r26, r24
    cpc  r27, r25
    brne __do_clear_bss_loop
  __do_device_init:
  /* Nothing to do, no prescaler. */
  __do_stack_def:
    ldi  r30, lo8(xOS+(OSstackInit))
    ldi  r31, hi8(xOS+(OSstackInit))
    out  (_SFR_IO_ADDR(SPL)), r30
    out  (_SFR_IO_ADDR(SPH)), r31
  __do_copy_data:
    jmp    main
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
  /* Catch the time interrupt here: 0x0026 */
    jmp privTickYield

#endif

#endif /* FEMTOOS_DEVICE_ASSEMBLY */

