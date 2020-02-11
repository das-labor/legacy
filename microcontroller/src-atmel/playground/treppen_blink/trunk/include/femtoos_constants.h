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

#ifndef FEMTOOS_CONSTANTS_H
#define FEMTOOS_CONSTANTS_H

/* Definition of constants needed in you configuration file
 * Most of these constants are tightly coupled to the constants
 * defined in locals and cannot be altered. */

/**
 * General definition of true and false for configuration options. We do not
 * use the standard true and false, for we cannot easily check these
 * reliably. Undefined is used for constants that are undefined, but that
 * must be reliably undefined, not just because we forget to set them. */
#define cfgFalse               0x0100U
#define cfgTrue                0x0101U

/**
 * Other singular constants. Do not alter these values without checking their
 * exact function. They signal a special state of a user variable */
#define cfgUndefined           0x0108U
#define cfgOverrideNon         0xFF01U
#define cfgShared              0x8000U

/**
 * Definitions for the initial setting of the interrupts.
 */
#define cfgGlobSet             0x0201U
#define cfgGlobClear           0x0200U
#define cfgTickSet             0x0210U
#define cfgTickClear           0x0200U

/**
 * Definitions for the initial start of a task.
 */
#define cfgExclude             0x0800U
#define cfgStartSuspended      0x0900U
#define cfgStartSleeping       0x0940U
#define cfgStartRunning        0x09C0U

/**
 * Definitions for the different types to fill the slot with.
 */
#define cfgUseAsWait           0x0A01U
#define cfgUseAsMutex          0x0A02U
#define cfgUseAsQueu           0x0A03U
#define cfgUseAsNon            0x0A04U

/**
 * Definitions for the different utilizations of the slotstacks
 */
#define cfgSyncSingleSlot      0x0C01U
#define cfgSyncSingleBlock     0x0C02U
#define cfgSyncDoubleBlock     0x0C03U
#define cfgSyncNon             0x0C04U

/**
 * Definitions for the way gcc should handle generated startup code.
 */
#define cfgKeep                0x0102U
#define cfgOverride            0x0103U
#define cfgReplace             0x0104U

/**
 * Definitions for the way gcc should handle pro/epiloge's
 */
#define cfgNaked               0x0B02U
#define cfgBikini              0x0B03U
#define cfgComplete            0x0B04U

/**
 * Definitions for capabilities
 * Note that the timeout capability also includes a delay since
 * the timeout is kept in the delay variables, which thus must be
 * present.
 */
#define cfgCapNon              0x0D00
#define cfgCapWatchdog         0x0D01
#define cfgCapEvent            0x0D02
#define cfgCapCritical         0x0D04
#define cfgCapSynchronization  0x0D08
#define cfgCapFileSystem       0x0D10
#define cfgCapDelay            0x0D20
#define cfgCapTimeout          0x0D60
#define cfgCapAll              0x0D7F


/**
 * Definitions to define the use of registers (always in pairs of 4).
 */
#define registersNon           0x00    /*  ........  */
#define r00r01r02r03           0x01    /*  .......x  */
#define r04r05r06r07           0x02    /*  ......x.  */
#define r08r09r10r11           0x04    /*  .....x..  */
#define r12r13r14r15           0x08    /*  ....x...  */
#define r16r17r18r19           0x10    /*  ...x....  */
#define r20r21r22r23           0x20    /*  ..x.....  */
#define r24r25r26r27           0x40    /*  .x......  */
#define r28r29r30r31           0x80    /*  x.......  */
#define r00_upto_r07           0x03    /*  ......xx  */
#define r08_upto_r15           0x0C    /*  ....xx..  */
#define r16_upto_r23           0x30    /*  ..xx....  */
#define r24_upto_r31           0xC0    /*  xx......  */
#define r00_upto_r15           0x0F    /*  ....xxxx  */
#define r16_upto_r31           0xF0    /*  xxxx....  */
#define registersAll           0xFF    /*  xxxxxxxx  */

/**
 * This enables the control of the optimization switch from outside, if it
 * is defined it overrides the internal setting, if it is not defined, the
 * internal is used.
 */
#if defined(defExtOptimized)
  #define defSysOptimized (defExtOptimized)
#else
  #define defSysOptimized (cfgSysOptimized)
#endif

/**
 * This enables the control of the startup switch from outside, if it
 * is defined it overrides the internal setting, if it is not defined, the
 * internal is used.
 */
#if defined(defExtGCCstartup)
  #define defSysGCCstartup (defExtGCCstartup)
#else
  #define defSysGCCstartup (cfgSysGCCstartup)
#endif

/**
 * This enables the control of the zero page setting from outside, if it
 * is defined it overrides the internal setting, if it is not defined, the
 * internal is used.
 */
#if defined(defExtZeroPageStack)
  #define defSysZeroPageStack (defExtZeroPageStack)
#else
  #define defSysZeroPageStack (cfgSysZeroPageStack)
#endif


/**
 * Determine the location of some registers for optimization and/or
 * consistency checking.
 */

#include "femtoos_device.h"

#if !defined(__ASSEMBLER__)

#undef  _SFR_IO8
#undef  _SFR_MEM8
#define _SFR_IO8(arg)  (arg)
#define _SFR_MEM8(arg) (arg)

#if (devTIMSK < 0x40)
  #define defTIMSKinIO          cfgTrue
#else
  #define defTIMSKinIO          cfgFalse
#endif

#if (devAuxSysReg < 0x20)
  #define defAuxSysRegLowIO     cfgTrue
#else
  #define defAuxSysRegLowIO     cfgFalse
#endif

#if (devAuxEventReg < 0x20)
  #define defAuxEventRegLowIO   cfgTrue
#else
  #define defAuxEventRegLowIO   cfgFalse
#endif

#undef  _SFR_IO8
#undef  _SFR_MEM8
#define _SFR_IO8(arg)  ( _MMIO_BYTE((arg) + 0x20) )
#define _SFR_MEM8(arg) ( _MMIO_BYTE(arg) )

#endif

/**
 * You may use defThreeByteStackAdd in the configuration files to distinguish
 * between a situation where the stack is increased to or three bytes at call
 * pc push. For example, the required stack space may depend on it.
 */
#if (devFLASHsize > 0x10000UL)
  #define defThreeByteAddress          cfgTrue
  #define defThreeByteStackAdd         1
  #define defAddressSize               3
#else
  #define defThreeByteAddress          cfgFalse
  #define defThreeByteStackAdd         0
  #define defAddressSize               2
#endif

#include "config_application.h"

/* Any derived constants needed to be checked are to be inserted here. */

/* Just to regulate the return value */
#define defUseBoolReturns cfgUseTimeout

/**
 * Because we need the sizes of the structs defined above at preprocessor level, we define
 * defTaskSaveSize. Please make sure that any changes to that struct are reflected here.
 */

#if (cfgUseSynchronization != cfgSyncNon) && (defUseBoolReturns == cfgTrue)
  #define defTaskSaveSizeSync 1
#else
  #define defTaskSaveSizeSync 0
#endif

#if (cfgCheckTaskStack == cfgTrue) || (cfgCheckRegisters == cfgTrue) || (cfgCheckWatermarks == cfgTrue)
  #define defTaskSaveSizeBase 13
#else
  #define defTaskSaveSizeBase 7
#endif

#define defTaskSaveSize (defTaskSaveSizeBase + defTaskSaveSizeSync)

/**
 * This holds the absolute minimum of bytes the task stack must contain. In the present setup
 * this number equals 4 or 6 because the stack must at least accommodate two return addresses.
 * Although the last address is not stored in the context save, it is temporarily needed.
 */
#define defTaskStackMinimum            (2*defAddressSize)

/**
 *  The number of bytes taken by saving one (return) address and the status register.
 */
#define defTaskStackGround             (defAddressSize + 1)


#include "femtoos_check.h"

/**
 * This enables the control of the proEpilog setting from outside, if it
 * is defined it overrides the internal setting, if it is not defined, the
 * internal is used.
 * From GCC 4.3.2 onwards we assume you use the patched version where the
 * OS_task attribute has been included. This replaces the need for a backend
 * gcc hack. It does the same, leaves the frame pointer intact, but does not
 * return or save any unused registers.
 */
#if defined(defExtReduceProEpilogue)

  #if (defExtReduceProEpilogue == cfgNaked)
    #define defSysReduceProEpilogue __attribute__ (( naked ))
    #define defSysReduceMain        __attribute__ (( naked ))
  #elif (defExtReduceProEpilogue == cfgBikini)
    #define defSysReduceProEpilogue  __attribute__ (( OS_task ))
    #define defSysReduceMain         __attribute__ (( OS_main ))
  #elif (defExtReduceProEpilogue == cfgComplete)
    #define defSysReduceProEpilogue
    #define defSysReduceMain
  #else
    #error "undefined option for external parameter defExtReduceProEpilogue"
  #endif

#else

  #if (cfgSysReduceProEpilogue == cfgNaked)
    #define defSysReduceProEpilogue __attribute__ (( naked ))
  #elif (cfgSysReduceProEpilogue == cfgBikini)
    #define defSysReduceProEpilogue  __attribute__ (( OS_task ))
  #else
    #define defSysReduceProEpilogue
  #endif

#endif

/**
 * The wrapper epilog is used for those methods that do a context switch before executing its
 * actions. These are the so-called swtiching methods.
 */
#define defSysWrapperEpilogue __attribute__ ( ( naked , noinline) )

#endif /* FEMTOOS_CONSTANTS_H */
