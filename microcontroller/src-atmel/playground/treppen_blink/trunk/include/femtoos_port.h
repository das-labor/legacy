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

#ifndef FEMTOOS_PORT_H
#define FEMTOOS_PORT_H

/**
 * Porting femto OS to other devices.
 *
 * There are two possibilities. Or you want to port this to an other avr device, or to
 * an completely other architecture. In the first case it is probably not needed to alter
 * anything in the femtoo_port.c and femtoo_port.h files. You just need to add an
 * femtoos_ATxxx.asm file to the includes (look in the present files for examples), and
 * you need to add the device in the femtoo_device.h file. The latter case will at least
 * require an overhaul of all that is in the femtoos_port files.
 */

#include "femtoos_globals.h"
#include "femtoos_constants.h"

/**
 * Operation to read one byte (8 bit) from flash, and perform the appropriate type cast.
 * All flash read operations make use of portFlashReadByte, portFlashReadWord or
 * portFlashReadStruc
 */
#define portFlashReadByte(byteType, byteVar) ((byteType) pgm_read_byte(&(byteVar)))


/**
 * Operation to read one word (16bit) from flash, and perform the appropriate type cast.
 * All flash read operations make use of portFlashReadByte, portFlashReadWord or
 * portFlashReadStruc
 */
#define portFlashReadWord(wordType, wordVar) ((wordType) pgm_read_word(&(wordVar)))


/**
 * Operation to read one field from a structure in flash, and perform the appropriate type cast.
 * All flash read operations make use of portFlashReadByte, portFlashReadWord or
 * portFlashReadStruc
 */
#define portFlashReadStruc(strucType, strucPointer, strucFieldType, strucField) ((strucFieldType) pgm_read_word(&(((strucType *)pgm_read_word(&strucPointer))->strucField)))


/**
 * Specify how the status register must be filled upon first start of the task, but do not include
 * the setting of interrupts here, i.e. leave them disabled, and do not make use of any predefined
 * interrupt storage locations (see below), they will be cleared. Since the status will usually be
 * of not great importance at startup, the null byte should be adequate.
 */
#define portInitCPUStatusRegister   0x00


/**
 * Most hardware carry the global interrupt enable bit in there status register. Specify here
 * the bit that must be set to enable the global interrupt. It is combined with the
 * portInitCPUStatusRegister at context startup. Since for the AVR the H bit is used when
 * cfgSysSqueezeState is activated, we include it in the setting. In the latter case the I
 * bit may not be set. Note that this implementation assumes that an interrupt set is represented
 * by a 1 and an interrupt cleared is represented by a 0. Effectively you can only specify the
 * position of the bit here. For unused interrupts specify 0.
 */
#if (cfgSysSqueezeState == cfgTrue) && (cfgIntTickTrack == cfgTrue)
  #define portInitModeInterruptLoc     (0)
  #define portInitGlobalInterruptLoc   (SREG_H)
  #define portInitTickInterruptLoc     (SREG_I)
#elif (cfgSysSqueezeState == cfgTrue) && (cfgIntTickTrack == cfgFalse)
  #define portInitModeInterruptLoc     (0)
  #define portInitGlobalInterruptLoc   (SREG_H)
  #define portInitTickInterruptLoc     (0)
#elif (cfgSysSqueezeState == cfgFalse) && (cfgIntTickTrack == cfgTrue)
  #define portInitModeInterruptLoc     (SREG_I)
  #define portInitGlobalInterruptLoc   (SREG_H)
  #define portInitTickInterruptLoc     (SREG_T)
#elif (cfgSysSqueezeState == cfgFalse) && (cfgIntTickTrack == cfgFalse)
  #define portInitModeInterruptLoc     (0)
  #define portInitGlobalInterruptLoc   (SREG_I)
  #define portInitTickInterruptLoc     (0)
#endif


/**
 * Specify the operation that makes gcc flush all its cached variables. This is needed to
 * make sure the optimizer does not move calls over this barrier.
 * calls this function, it is for your own purposes.
 */
#define portBarrier()                        asm volatile ( "" ::: "memory" )

/**
 * Specify the no operations instruction of your hardware. Note that Femto OS does not explicitly
 * calls this function, it is for your own purposes.
 */
#define portNop()                            asm volatile ( "nop" :: )


/**
 * Specify the instruction to set the global interrupt. All calls from the Femtoos.c regarding
 * setting the state of the global interrupt make use off this function, but that does not hold
 * true for other assembler parts of the OS. The memory clobber ensures uncaching of variables
 * and prevent dislocation.
 */
#define portEnableGlobalInterrupts()         asm volatile ( "sei" ::: "memory" )


/**
 * Specify the instruction to clear the global interrupt. All calls from the Femtoos.c regarding
 * clearing the state of the global interrupt make use off this function, but that does not hold
 * true for other assembler parts of the OS. The memory clobber ensures uncaching of variables
 * and prevent dislocation.
 */
#define portDisableGlobalInterrupts()        asm volatile ( "cli" ::: "memory" )


/**
 * This call is used as preamble for all context switching functions to store the state of
 * the global interrupt as soon as possible. Subsequently the global interrupts are disabled.
 * If we make use of an AuxReg we copy the original of I value to the ARB of the AuxReg,
 * otherwise we copy it to the H bit. We may assume the ARB bit being zero upon entry
 * (it is cleared in the restore context).
 */
#if (cfgSysSqueezeState == cfgFalse)
  /* The devAuxSysRegBit is assumed to be zero upon entry, signaling per default that the
   * interrupts are switched off. The value of the I bit is copied to the ARB bit. */
  #define portResqueGlobalInterruptState()            \
    asm volatile ( "brid 67f                \n\t"     \
                   "cli                     \n\t"     \
                   "sbi %[_ASR_],%[_ARB_]   \n\t"     \
                   "67:          " ::                 \
                   [_ARB_]       "i" (devAuxSysRegBit),     \
                   [_ASR_]       "i" (_SFR_IO_ADDR(devAuxSysReg)))
#else
  /* We signal with the H bit being zero that we originate from a manual Yield. The old
   * value of the I bit is copied to the H bit and the I bit is cleaned. The code below is
   * tricky. First we clean the H bit, and subsequently test if interrupts are enabled. If
   * so we must set the H bit and clean the I bit. The rest is unimportant, since we came from
   * a manual call. To save we first clean H and test for I afterwards. But what if an interrupt
   * comes after cleaning H and before the test? H could be set again when we return, since
   * the H bit may be not conserved due to our own approach. The point is, it does not matter. If
   * I was cleaned before, interrupts are not possible so H stays reset, and if I was not,
   * H will be set anyway. So we do not fear an interrupt here.
   * */
  #define portResqueGlobalInterruptState()                        \
    asm volatile ( "clh           \n\t"      /* Clear H bit  */   \
                   "brid 67f      \n\t"                           \
                   "cli           \n\t"      /* Clean I bit  */   \
                   "seh           \n\t"      /* Set H bit    */   \
                   "67:         " ::                              \
                   [_Hb_]       "i" (preBitSet1(0x00,SREG_H)) )
#endif


/**
 * We call this instead of portResqueGlobalInterruptState to store the global
 * interrupt state on places where we are sure the global interrupts where
 * enabled shortly before, but are already disabled now. The only thing to
 * do it to store that situation, i.e. signal that we come from a place
 * with interrupts activated. For this purpose we can use the auxiliary register
 * bit (save) of sacrifice the status of the H bit (daring). The AuxRegBit
 * is per default zero.
 */
#if (cfgSysSqueezeState == cfgFalse)
  #define portResqueGlobalInterruptActive()            \
    asm volatile ( "sbi %[_ASR_],%[_ARB_]" ::          \
                   [_ARB_]  "i" (devAuxSysRegBit),     \
                   [_ASR_]  "i" (_SFR_IO_ADDR(devAuxSysReg)))
#else
    /* We only need to set the H bit to signal we come from an interrupt. The interrupt is
     * already disabled. The original status of the H bit is permanently lost. */
  #define portResqueGlobalInterruptActive()  \
    asm volatile ( "seh " :: )
#endif


/**
 * Specify the instructions to reboot your hardware. This could be a jump to __init
 * or a soft reset. Naturally this function does not return.
 */
void portReboot(void) __attribute__ ( ( naked, noreturn ) ) ;


/**
 * Specify the instructions that determine if the file system is ready for writing.
 * Femto OS assumes, if this call returns true, the files system remains ready until
 * a write action is initiated by the Femto OS itself. In other words, the system
 * is not capable of discovering other situations which may prevent a successful
 * write. There situations are, for example, but not limited to, writing to flash
 * or raw calls to portFSWriteByte.
 */
#define portFSWriteReady() ((devEECR & (1 << devEEPE)) == 0)


/**
 * Within this call you must save a byte on the file system at the given address.
 * At a file format the Femto FS writes zero's to all valid file locations.
 * Due to the nature of the eeprom, it may be beneficial to invert the byte
 * so that actually 0xFF is stored instead of 0xFF. If you do so, make sure
 * you invert the byte at reading as well.
 * Femto OS assumes this method is the only one writing bytes to the file system
 * and that no other tasks call this method apart from the one that have
 * been granted write access.
 */
void portFSWriteByte(Taddress pAddress, Tbyte bValue);


/**
 * Return the content of the given location in the file system.
 * If needed, invert the byte before returning the value.
 */
Tbyte portFSReadByte(Taddress pAddress);


/**
 * Specify the instructions to obtain the stack pointer.
 *
 * If you have a SP_H field, you must use it here, otherwise this
 * routine may equal portGetReducedStack.
 */
#define portGetFullStack(field) \
  asm volatile ( "in %A0,__SP_L__     \n\t" \
                 "in %B0,__SP_H__   " :"=r" (field) : )

/**
 * Specify the instructions to obtain the reduced stack pointer.
 *
 * Note this is not shorter as the code above since we must supply
 * a valid 16 bit number.
 */
#define portGetReducedStack(field) \
  asm volatile ( "in %A0,__SP_L__     \n\t" \
                 "ldi %B0, 0x00   " :"=r" (field) : )

/**
 * Specify the instructions to initialize the stack pointer.
 *
 * If you have a SP_H field, you must use it here, otherwise this
 * routine may equal portSetReducedStack.
 */
#define portSetFullStack(field) \
  asm volatile ( "out __SP_L__, %A0     \n\t" \
                 "out __SP_H__, %B0 " : : "r" (field) : "memory" )

/**
 * Specify the instructions to set the low byte of the stack pointer.
 *
 * Setting the high byte is not needed since we know it will be zero.
 */
#define portSetReducedStack(field) \
  asm volatile ( "out __SP_L__, %A0 " : : "r" (field) : "memory")


/**
 * Specify the instructions to swap to nibbles in a byte. Some architectures
 * have special instructions for this purpose. Otherwise use:
 * (field = (((field >> 4) & 0x0F) | ((field << 4) & 0xF0)))
 */
#define portSwapNibbles(field)  \
  asm volatile("swap %0" : "=r" (field) : "0" (field));


/**
 * Specify the hardware dependent jump and call instruction. Make sure this instruction
 * always works and does not depend on the distance that must be jumped.
 */

#if (devFLASHsize <= 0x1000UL)
  #define defJUMP  "rjmp"
  #define defCALL  "rcall"
#else
  #define defJUMP  "jmp"
  #define defCALL  "call"
#endif

#define portJump(destination) asm volatile ( defJUMP " " #destination :: )

/**
 * Define the event register. This should be a register that can preferable be read and
 * written one atomic operation (see below) For the AVR the general purpose registers are
 * appropiate. Writing is done using atomic bitset and bitclear operations.
 * If such a register is not available, use a normal register,
 * and make sure you only set events in interrupt protected environments,
 * by using isrFireEvent() or genFireEvent() on the appropiate places.
 * Reading inside the OS is already protected.
 */
#define portEventRegister devAuxEventReg

/**
 * Here you can specify if the portEventRegister can be written and read in an
 * atomic operation. And if the auxiliary system storage coincides with the
 * registers used for events.
 */
#define defEventRegisterAtomicOperation defAuxEventRegLowIO
#define defEqualAuxiliaryRegisters      preEqualMacros(devAuxSysReg,devAuxEventReg)

/**
 * Some devices have a clock prescaler that can be set by software. If so, use
 * cfgSysClockDivider in the configuration parameters to specify which divider
 * you wish. If needed you must adjust the values to the correct bit settings of
 * your hardware. For most AVR's this is organized in the same manner. Some
 * AVR devices have only a prescaling that can be set using fuses. If so, change
 * the devClockFrequency in the xxx.asm port file. This prescaler is not meant for
 * that purpose.
 * Since all AVR devices that do have prescalers work in the same manner, their
 * values are defined here. For other hardware, rewrite the code below.
 */
#if (devCLKPCE == cfgUndefined)
  #if (cfgSysClockDivider != 1)
    #error "cfgSysClockDivider can only be 1 for this device (no clock prescaler)."
  #endif
#else
  #if (cfgSysClockDivider == 1)
    #define defClockPrescaleBits 0x00
  #elif (cfgSysClockDivider == 2)
    #define defClockPrescaleBits 0x01
  #elif (cfgSysClockDivider == 4)
    #define defClockPrescaleBits 0x02
  #elif (cfgSysClockDivider == 8)
    #define defClockPrescaleBits 0x03
  #elif (cfgSysClockDivider == 16)
    #define defClockPrescaleBits 0x04
  #elif (cfgSysClockDivider == 32)
    #define defClockPrescaleBits 0x05
  #elif (cfgSysClockDivider == 64)
    #define defClockPrescaleBits 0x06
  #elif (cfgSysClockDivider == 128)
    #define defClockPrescaleBits 0x07
  #elif (cfgSysClockDivider == 256)
    #define defClockPrescaleBits 0x08
  #else
    #error "cfgSysClockDivider was not recognized for this device."
  #endif
#endif


/**
 * Most hardware make use of some kind of prescaling to feed the timer clocks. We decided
 * not to explicitly do the tick interrupt frequency math in our code, since it will be
 * not general enough anyway. Instead, you can couple the prescale values to the constant
 * which contains the bits that must be set in the prescale register. This coupling is done
 * per device in the device.asm file.
 */
#define defTimerPrescaleBits    (DefPrescaler(cfgSysSubTickDivider))

/**
 * For the delay loop we make use of the interrupt timer in the slowest
 * possible setting.
 */
#if defined(devPrescale_1024)
  #define defDelayPrescaleBits devPrescale_1024
#elif defined(devPrescale_512)
  #define defDelayPrescaleBits devPrescale_512
#elif defined(devPrescale_256)
  #define defDelayPrescaleBits devPrescale_256
#elif defined(devPrescale_128)
  #define defDelayPrescaleBits devPrescale_128
#elif defined(devPrescale_64)
  #define defDelayPrescaleBits devPrescale_64
#elif defined(devPrescale_32)
  #define defDelayPrescaleBits devPrescale_32
#elif defined(devPrescale_16)
  #define defDelayPrescaleBits devPrescale_16
#elif defined(devPrescale_8)
  #define defDelayPrescaleBits devPrescale_8
#elif defined(devPrescale_4)
  #define defDelayPrescaleBits devPrescale_4
#elif defined(devPrescale_2)
  #define defDelayPrescaleBits devPrescale_2
#elif defined(devPrescale_1)
  #define defDelayPrescaleBits devPrescale_1
#else
  #error "No prescaler value for the delay loop possible."
#endif

/**
 * The (device) watchdog can usually be set to different different wait times and
 * is (should be) driven by an independent low power clock. The table below
 * translates the requested waittime into the correct settings for the
 * watchdog prescaler.
 */
#if (cfgNumSleepPeriod < 384)
  #define defWatchDogPrescaleBits  0x04
  #define defSleepRealPeriod        256
#elif (cfgNumSleepPeriod < 768)
  #define defWatchDogPrescaleBits  0x05
  #define defSleepRealPeriod        512
#elif (cfgNumSleepPeriod < 1536)
  #define defWatchDogPrescaleBits  0x06
  #define defSleepRealPeriod       1024
#elif (cfgNumSleepPeriod < 3072) || (devWDP3 == cfgUndefined)
  #define defWatchDogPrescaleBits  0x07
  #define defSleepRealPeriod       2048
#elif (cfgNumSleepPeriod < 6144)
  #define defWatchDogPrescaleBits  0x08
  #define defSleepRealPeriod       4096
#else
  #define defWatchDogPrescaleBits  0x09
  #define defSleepRealPeriod       8192
#endif


/**
 * Specify the functions that the system (femtoos.c) and not your application code (!)
 * must use to enter global critical regions. This code does not need to be
 * nestable, since the OS does not nest such calls.  We explictly forbid inlining
 * since this may lead to funny results when the gcc optimizer is at work. Furthermore,
 * it generates more code too.
 */
void portEnterGlobalInterrupts(void) __attribute__ ( ( naked, noinline ) );


/**
 * Specify the functions that the system (femtoos.c) and not your application code (!)
 * must use to exit global critical regions. This code does not need to be
 * nestable, since the OS does not nest such calls.  We explicitly forbid inlining
 * since this may lead to funny results when the gcc optimizer is at work. Furthermore,
 * it generates more code too.
 */
void portExitGlobalInterrupts(void)  __attribute__ ( ( naked, noinline ) );


/**
 * Specify the functions that the system (femtoos.c) and not your application code (!)
 * must use to enter tick critical regions. This code does not need to be
 * nestable, since the OS does not nest such calls.  We explicitly forbid inlining
 * since this may lead to funny results when the gcc optimizer is at work. Furthermore,
 * it generates more code too.
 */
void portEnterTickInterrupts(void) __attribute__ ( ( naked, noinline ) );


/**
 * Specify the functions that the system (femtoos.c) and not your application code (!)
 * must use to exit tick critical regions. This code does not need to be
 * nestable, since the OS does not nest such calls.  We explicitly forbid inlining
 * since this may lead to funny results when the gcc optimizer is at work. Furthermore,
 * it generates more code too.
 */
void portExitTickInterrupts(void) __attribute__ ( ( naked, noinline ) );

/**
 * Define code to switch off the interrupts of the tick counter. Do not call it
 * directly from application code. This code is called from taskDisableTickInterrupts()
 * and taskEnterTickCritical(). Calls may be nested, but since the bookkeeping is
 * done by the OS, there is no need for any of that. Make sure other bits of the
 * register (containing other timer control bits) are not affected by proper
 * global interrupt protection.
 */
void portDisableTickInterrupts(void) __attribute__ ( ( naked, noinline ) );


/**
 * Define code to switch on the interrupts of the tick counter. Do not call it
 * directly from application code. This code is called from taskEnableTickInterrupts()
 * and taskExitTickCritical(). Calls may be nested, but since the bookkeeping is
 * done by the OS, there is no need for any of that. Make sure other bits of the
 * register (containing other timer control bits) are not affected by proper
 * global interrupt protection.
 */
void portEnableTickInterrupts(void)__attribute__ ( ( naked, noinline ) );

/**
 * Specify all code needed to startup your system. This may include setting the
 * clock frequency, of disabling a watchdog. Do not setup the stack, Femto OS
 * takes care of that.
 */
#if (defSysGCCstartup == cfgReplace)
  #define portInit()
#else
  void portInit(void);
#endif


/**
 * The femto OS is capable of tracing its own and your functions. All actions the
 * OS takes are preceded (as early in the code as possible) with a portTrace(byte) call,
 * where byte contains information of the trace point. Specify your code to get the
 * uiEvent off the chip. This must be done as quickly as possible, for you must not
 * uphold the device.
 */
void portTrace(Tuint08 uiEvent);


/**
 * The Femto OS has no special idle tasks. The idle task is simply a place where the processor
 * must stay until the next tick interrupts occurs. Thus every time the OS goes idle this
 * method is called, and the OS does not expect the method to return. You may implement it
 * as an infinite loop, or some (timer interruptible) power save mode.
 */
void portIdle(void)  __attribute__ ( ( noreturn, naked ) );


/**
 * Besides the possibility to put the device in a power save mode during idle, the OS
 * supports a low power sleep mode which can be entered for a longer period. You are
 * responsible for waking the device up, since this may be very device dependent. The
 * uiTickBlockMinDelay specifies the number of tick blocks (1 tick block == 256 ticks) that
 * can safely be slept without disturbing regular functions (maximum sleep time). This means
 * that, if the low power sleep stays within that period, no delay task shall wakeup
 * and all other tasks are in sleep mode, provided that cfgUseLowPowerOnDelay == cfgTrue.
 * Otherwise, a sleep mode only comes when all tasks are indeed sleeping, and uiTickBlockMinDelay
 * will be set to 0xFF (i.e. there is no minimum). Femto OS wakes all tasks if you
 * exceeded the maximum sleep time if you set cfgUseLowPowerDelayRelease, otherwise,
 * make sure you do not exceed the maximum sleep time. Please note that you remain responsible to
 * adjust the tick timer yourself using genAddtoTickCount(). If you don't, Femto OS will not
 * notice time has past and may try to go to sleep immediately. If you manually put tasks
 * to sleep this may exactly be what you want however.
 * The function must return.
 */
void portSleep(Tuint08 uiTickBlockMinDelay);

/**
 * As long as you are developing, it is useful to keep track of errors that occur
 * (in embedded devices we expect no errors during normal operation) Every error
 * encountered is reported through a call to portShowError. uiMessage contains
 * a 6 bit error message, and uiInfoTask contains (if appropriate) in the first
 * nibble the slot/file or other info and in the last nibble the task number at
 * which the error occurred. You are responsible to get the information off the
 * chip, for example by letting some leds blink. Fatal errors are reported endlessly,
 * non fatal will stop the current task and Femto OS tries to continue operations as
 * far as possible. The system however may crash, hold or reset.
 */
void portShowError(Tuint08 uiMessage, Tuint08 uiCallId, Tuint08 uiInfoTask);

/**
 * The startup code of the Femto OS contains a special call to show a reset took
 * place, this is portShowReset(). Implement it so you can see a reset took place,
 * for example by blinking some leds. This helps you in finding problems with your
 * software since malicious code often gets into a reset loop.
 */
void portShowReset(void);


/**
 * Define portInitContext as privInitContext to make use of the in Femto Os
 * defined privInitContext. This can then be inlined. You can also define your
 * own portInitContext just remove this define (true -> false), and supply the
 * appropriate method inside port.c
 */
#if (true)
  #define portInitContext(TaskStart,StackTop,RegisterUse,InterruptStart) privInitContext(TaskStart,StackTop,RegisterUse,InterruptStart)
#else
  /* This is your prototype. Specify a function which intializes the context of each
   * task and sets the return addresses.  */
  Taddress portInitContext(Taddress pTaskStart, Taddress pStackTop, Tuint08 uiRegisterCount, Tuint08 uiInterruptStart);
#endif


/**
 * Specify a function that switches the stack from task to isr. For implementation
 * details see inside port.c. This method is called
 * by isrEnter() to start executing your interrupt service routine after a context
 * switch is made. Note this function should never inline due to stack manipulation
 * operations.
 */
void portEnterISR(void)  __attribute__ ( ( naked, noinline, used ) );


/**
 * Specify a function that switches the stack from os/task to isr. For implementation
 * details see inside port.c. This method is called
 * by isrBegin() to start executing your interrupt service without any special arrangements.
 * Note this function should never inline due to stack manipulation operations.
 */
void portBeginISR(void)  __attribute__ ( ( naked, noinline, used ) );


/**
 * Specify a function that switches the stack from isr to os/task. For implementation
 * details see inside port.c. This method is called by isrEndReturn() to stop executing
 * your interrupt and continue at the point where operations where interupted.
 * Note this function should never inline due to stack manipulation operations.
 */
void portReturnISR(void)  __attribute__ ( ( naked, noinline, used ) );


/**
 * Specify a function that switches the stack from isr to os/task. For implementation
 * details see inside port.c. This method is called by isrEndReturn() to stop executing
 * your interrupt and immediately performs a context  switch.
 * Note this function should never inline due to stack manipulation operations.
 */
void portYieldISR(void)  __attribute__ ( ( naked, noinline, used ) );


/**
 * Specify a function that saves all relevant registers and the status register
 * on the stack, and subsequently performs a stack change from task to os.
 * For implementation details see inside port.c.
 * Note this function should never inline due to stack manipulation operations.
 */
void portSaveContext(void)  __attribute__ ( ( naked, noinline, used ) );


/**
 * Specify a function that restores all relevant registers and the status register
 * from the stack. For implementation details see inside port.c.
 * Note this function should never inline due to stack manipulation operations.
 */
void portRestoreContext(void)  __attribute__ ( ( naked, noinline, used ) );


/**
 * Specify a function that sets up your hardware so that tick interupts can take
 * place. The method should not activate the interrupts yet.
 */
void portSetupTimerInterrupt(void);


/**
 * Specify a function that returns the number of subticks passed from the
 * subtick timer. If there has been an unhandled tick interrupt the value
 * must not be reset. Femto OS only calls this method with tick interrupts
 * disabled.
 */
Tuint08 portReadTimer(void);


/**
 * Specify a function that returns true if the timer interrupt is due but has
 * not yet been executed, and false otherwise. If is, the method must also clear
 * the flag so that this interrupt is not executed any more. Femto OS only
 * calls this method with tick interrupts disabled.
 */
Tbool portCheckTimer(void);


/**
 * Specify a function that returns the number of subticks passed from the
 * subtick timer, and simultaneously resets the timer with a new interrupt
 * time. Femto OS only calls this method with tick interrupts disabled.
 */
Tuint08 portReadAndResetTimer(Tuint08 uiSubTickInterrupt);


/**
 * Interrupt routine called when the tick interrupt takes place. Implement a jump to privTickYield
 */
void devSigTimerCompare(void) __attribute__ ( ( signal, naked, used, externally_visible ) );


/**
 * Interrupt routine called when the subtick timer overflows. This is an error situation.
 * Implement a jump to privSubtickOverflow
  */
void devSigTimerOverflow(void) __attribute__ ( ( signal, naked, used, externally_visible ) );


/**
 * Interrupt routine called when the watchdog interrupt takes place. This is used for wake up
 * from a low power sleep method, and can be empty (but must be included).
 */
#if defined(devSigWatchdogTimeout)
  void devSigWatchdogTimeout(void) __attribute__ ( ( signal, naked, used, externally_visible ) );
#endif


#endif /* FEMTOOS_PORT_H */
