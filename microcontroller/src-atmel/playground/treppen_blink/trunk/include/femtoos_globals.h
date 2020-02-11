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

#ifndef FEMTOOS_GLOBALS_H
#define FEMTOOS_GLOBALS_H

/* DISCUSSION
 * Unfortunately we must include here some hardware dependent files. avr/io.h could
 * probably be moved to port.h, but this does not hold true for the avr/pgmspace.h.
 * Therefore we kept them together. The point is, storage in flash is not so hardware
 * independent as it may seem. Thus, if you want to port this OS, this is one point
 * to look at.
 */

#include  <avr/io.h>

/* These includes are not compatible with assembler language. But since we need the
 * globals in the startup code, we must explicitly exclude them here.
 */
#ifndef __ASSEMBLER__
  #include  <avr/pgmspace.h>
  #include  <string.h>
#endif

/**
 * Identification string for this OS. Make sure it has 12 bytes.
 */
#define SystemID   "FemtoOS 0.91"

/**
 * Our standard 'bools'.
 */
#define true       1
#define false      0


/* DISCUSSION
 * We don't use primitive data types in the code anywhere. All types are related
 * to the types below. We compile with the option -mint8, otherwise gcc promotes
 * a lot of 8 bit operations to 16 bit. Although this is standard behavior, and
 * can be suppressed with the right casts, to my opinion this has been the wrong
 * choice. C is a machine language and should respect the true lengths of the
 * defined variables. Note that it seems that GCC does not honor the bit lengths
 * defined; so why are the present anyway? With -mint8 int32_t becomes a 16 bit
 * integer. Why? This is totally confusing.
 */
#define Tchar      unsigned char
#define Tbool      Tuint08
#define Taddress   Tchar *
#define Tbyte      uint8_t
#define Tword      uint16_t
#define Tsint08    int8_t
#define Tuint08    uint8_t
#define Tsint16    int16_t
#define Tuint16    uint16_t
#define Tdouble    double
#define Tenum      uint8_t


/** <!--0601-->
 * The highest delay time you may use.
 *
 * The highest delay time you may use. If your tick time is 1ms, and since
 * we have a 16 bit tick counter, the maximum delay is about 65 seconds. In my
 * opinion, this is long enough. If you want to delay tasks longer it is
 * better to get a real time signal somewhere else.
 */
#define defDelayTimeMax        0xFEFF

/** <!--0613-->
 * The lowest delay time you may use.
 *
 * The lowest delay time you may use, which in practice means no
 * delay at all, but it is better to use this constant than 0
 * to make your intentions clear.
 */
#define defDelayTimeZero       0x0000

/** <!--0602-->
 * Use in a synchronizing method to specify it may not timeout
 *
 * Value you can use to indicate that a lock need not to timeout. Use this
 * rather than supply 0 to make your intentions clear.
 */
#define defLockBlockInfinite   0x0000

/** <!--0603-->
 * Use in a synchronizing method to say it must return immediately
 *
 * Value you can use to indicate that a lock need timeout immediately, thus
 * just check if you can obtain the lock, but do not wait for it. Use this
 * rather than supply 1 to make your intentions clear.
 */
#define defLockDoNotBlock      0x0001

/** <!--0604-->
 * Value with which the stack is filled.
 *
 * For testing purposes you can fill the stack with other values that 0x00.
 * This may be handy to see if you are not misusing a register. For the shortest
 * code, leave it to 0x00, so the compiler can rely upon the .bss section cleaning.
 * For a discussion how to use this setting see also cfgSysRegisterCheckByte.
 * Also see the discussion at defStackClean.
 */
#define defStackInitByte       0x00

/** <!--0605-->
 * Value to specify the current task.
 *
 * In all (external) methods accepting the uiTaskNumber parameter you can specify
 * defCurrentTaskNumber to indicate you want to use it for the current task, if
 * you do not know that number (for example when code is run from different
 * tasks).
 * Please be absolutely sure you are inside a real task. If you are inside an
 * isr, inside init or bark routines, you are not in task space and the current
 * task may not be what you expect. However, the system cannot warn you for this
 * situation. Thus, only to be used inside code that was called from appLoop_XXX.
 */
#define defCurrentTaskNumber   0x80

/** <!--0606-->
 * Literal value to specify the current task.
 *
 * In all (external) methods accepting the TaskName parameter you can specify
 * defCurrentTask to indicate you want to use it for the current task, if
 * you do not know that name (for example when code is run from different
 * tasks). Never use it as a number in your code
 * Please be absolutely sure you are inside a real task. If you are inside an
 * isr, inside init or bark routines, you are not in task space and the current
 * task may not be what you expect. However, the system cannot warn you for this
 * situation. Thus, only to be used inside code that was called from appLoop_XXX.
 */
#define defCurrentTaskName     defCurrentTaskName

/** <!--0607-->
 * Literal value to state to start at the beginning of a file.
 *
 * In most file operation calls you can specify an offset. Instead of using zero, you
 * may always use this constant to make your intention clear that you want to start
 * reading or writing from the file start.
 */
#define defFromFileStart        0x00

/** <!--0608-->
 * Literal value to state to start at the end of a file.
 *
 * In most file operation calls you can specify an offset. Instead of using the
 * real file length (which you must know or obtain), you
 * may use this constant to start writing the new bytes at the end of the file.
 * This constant is only available if cfgUseFileSystemMaintainFAT is activated.
 */
#define defFromFileEnd          0xFF

/** <!--0609-->
 * Literal value to state you want to run up to the end of a file.
 *
 * In most file operation calls you can specify an size. Instead of using the
 * real file length (which you must know or obtain), you
 * may use this constant to keep reading bytes up to the end of the file.
 * This constant is only available if cfgUseFileSystemMaintainFAT is activated.
 */
#define defUntilFileEnd         0xFF

/** <!--0610-->
 * Literal value to state to open the file system in read only mode.
 *
 * If cfgUseFileSystemConcurrentRead is activated, it is required to specify if you
 * want to open the file system in read only mode, or in read/write mode. Although, you
 * can just use a boolean there, you may want to use this constant to make your
 * intentions mode explicit.
 */
#define defReadOnly             0x01

/** <!--0611-->
 * Literal value to state to open the file system in read/write mode.
 *
 * If cfgUseFileSystemConcurrentRead is activated, it is required to specify if you
 * want to open the file system in read only mode, or in read/write mode. Although, you
 * can just use a boolean there, you may want to use this constant to make your
 * intentions mode explicit.
 */
#define defReadWrite            0x00

/** <!--0612-->
 * Literal value to fire all events in the genFireEventSet method.
 *
 * If you need to unblock all waiting tasks by firing all all events use this
 * constant in the method genFireEventSet. It makes your intentions clear.
 */
#define defAllEvents            0xFF

/** <!--0714-->
 * Literal value to restart a task in the suspended state.
 *
 * Use this value as restartMode when calling taskRestart(). It restarts
 * the task, and makes sure it starts suspended. This value overrides the
 * value given in the configuration. For tasks with a shared stack, this
 * causes the task to start in a shared state, passing execution on to
 * to the next task with shared stack. Note that the shared stack is not
 * rescheduled any more until resume is called on this stack. Effectively
 * the task is temporarily taken out of the pool of tasks with shared
 * stacks.
 */
#define    defRestartSuspended  0x00

/** <!--0715-->
 * Literal value to restart a task in the sleeping state.
 *
 * Use this value as restartMode when calling taskRestart(). It puts the
 * task to sleep. Restart only takes place after the device has slept.
 * This value overrides the
 * value given in the configuration. For tasks with a shared stack, this
 * causes the task to start in a shared state, passing execution on to
 * to the next task with shared stack. Note that the shared stack is not
 * rescheduled any more until the device has slept. Effectively
 * the task is temporarily taken out of the pool of tasks with shared
 * stacks.
 */
#define    defRestartSleeping   0x40

/** <!--0716-->
 * Literal value to restart a task in the default state.
 *
 * Use this value as restartMode when calling taskRestart(). It puts the
 * task in the start mode as defined in the configuration. For tasks with
 * a shared stack, this causes the task to start in a shared state,
 * passing execution on to to the next task with shared stack. The result
 * thus depends on the setting being cfgStartRunning, cfgStartSuspended
 * of cgfStartSleeping.
 */
#define    defRestartDefault    0x80


/**
 * At this moment this value is for internal use only. When the restart function
 * is called for shared tasks, the standard  options are modified, into shared
 * ones automatically. Do not use this argument when calling taskRestart()
 */
#define    defRestartShared     0x80

/** <!--0717-->
 * Literal value to restart a task in the running state.
 *
 * Use this value as restartMode when calling taskRestart(). It restarts
 * the task, and makes sure it starts running. This value overrides the
 * value given in the configuration. For tasks with a shared stack, this
 * causes the task to start in a shared state, passing execution on to
 * to the next task with shared stack.
 */
#define    defRestartRunning    0xC0


/** <!--0718-->
 * Literal value to suspend a task directly
 *
 * Use this value in taskSuspend() as suspendMode. It will suspend the
 * task immediately. If, at the same time, there was a suspend request
 * present, that will be reset.
 */
#define    defSuspendNow        0x01

/** <!--0719-->
 * Literal value to check for a suspend request.
 *
 * Use this value in taskSuspend() to check if there is a suspend request
 * present. If so, the task will suspend at this point, and the request
 * will be 'consumed'.
 */
#define    defSuspendCheck      0x02

/** <!--0725-->
 * Literal value to reset a suspend request and continue.
 *
 * Use this value in taskSuspend() to check if there is a suspend request
 * present. If so, the task will consume the request, but will not suspend.
 */
#define    defSuspendClear      0x04



/* Definition used to check slot numbers, since it is used in user code,
 * (expended by macro's) it is defined here.  */
#define defCheckSlotTypeError   0xF0


/**
 * DISCUSSION
 * Errors are divided into two categories, fatal and non fatal errors. Production code
 * should of course generate no error at all, but in testing situations, the non fatal
 * errors try to stop the offending task, and continue (but do not release any locks
 * the task was holding). Note this is only possible if
 * the stacks are not damaged, after the error has been reported. Normally a SaveContext
 * will not corrupt the task stack is the context does not fit (it will skip the save in
 * test mode) but reporting the error itself takes quite some stack space and can also
 * corrupt a stack. Fatal errors are reported indefinitely.
 * The error consists off 3x 8bit, with the following format:
 * SSfx xxxx ssss tttt
 *     SS: signbits, used to blink, cleared in the call 10, 01
 *  xxxxx: error number
 *      f: fatal error
 *   tttt: task number
 *   ssss: slot number / other information
 * Errors should, in the current implementation not exceed number 0x2F, since the
 * upper two bits are used for other purposes (blink leds and show info about the
 * type of error. This could reduced to one bit, thereby freeing the numbers until 0x4F.
 */


/**
 * Unused error.
 */
#define  errUnused_0x00                   0x00

/** <!--2001-->
 * Task does not allow this operation.
 *
 * Each task is defined only with a particular set of capabilities,
 * unless you defined to have all capabilities for all tasks. You
 * tried to perform an operation in a task which does not have sufficient
 * capabilities to act.
 * The lowest nibble of the info byte displays the task number.
 * The highest nibble represents the failing capability.
 * -- 0001 cfgCapWatchdog
 * -- 0010 cfgCapEvent
 * -- 0011 cfgCapCritical
 * -- 0100 cfgCapSynchronization
 * -- 0101 cfgCapFileSystem
 * -- 0110 cfgCapDelay
 * -- 0111 cfgCapTimeout
 */
#define  errInsufficientCapabilities      0x01

/**
 * Unused error.
 */
#define  errUnused_0x02                   0x02

/** <!--2003-->
 * The stack of the task will become too small during execution.
 *
 * Femto OS calculated it will not be possible to save the context on the particular
 * moment it tried to do so. That stack probably has not yet really overflowed, but
 * the task cannot be restarted. Other tasks should be fine. The lowest nibble  of the info byte
 * displays the task number.
 *
 * Increase the tasksize.
 */
#define  errTaskStackWillOverflow         0x03

/** <!--2004-->
 * A register is being used which is not saved on the context.
 *
 * Registers that are not saved on the context have altered during execution
 * of the task. This might be an incorrect error. Femto OS cannot determine with 100%
 * certainty that a modified register is actually
 * used, it assumes it is. If you are absolutely certain the register is not used, exclude
 * it from the register check.
 *
 * The lowest nibble  of the info byte  displays the task number. The highest nibble is used to report
 * the highest register block in violation. One (or more) of the registers is offending.
 * The numbering is like this:
 * -- 0000: r0 r1 r2 r3
 * -- 0001: r4 r5 r6 r7
 * -- 0010: r8 r9 r10 r11
 * -- 0011: r12 r13 r14 r15
 * -- 0100: r16 r17 r18 r19
 * -- 0101: r20 r21 r22 r23
 * -- 0110: r24 r25 r26 r27
 * -- 0111: r28 r29 r30 r31
 *
 * Add registers to the context save, or, if the register is not used in
 * interruptible area's, exclude the register block from verification.
 */
#define  errTaskIllegalRegisterUse        0x04

/** <!--2005-->
 * Long task, sub tick overflow may occur, leading to missed ticks.
 *
 * The tick counter is driven by a sub tick counter. The sub tick counter keeps running,
 * even if no tick can be generated. However the sub tick counter itself may not overflow
 * for that may lead to missed ticks. The lowest nibble of the info byte
 * displays the task number.
 *
 * Reduce the time slice for that particular task, or adjust the timing of the system
 * to a lower tick frequency.
 */
#define  errTaskTakesTooLong              0x05

/** <!--2006-->
 * Task cannot enter a critical section
 *
 * Overflow of the number of nesting levels of one of the critical sections. The number
 * is a power of two minus one depending on the number of bits reserved for the counting.
 * Counting is per type, being one of global, switch or tick. The lowest nibble  of the info byte
 * displays the task number.
 *
 * Reduce the levels of nesting, or increase the level counter.
 */
#define  errTaskNestingOverflowed         0x06

/** <!--2007-->
 * Task cannot exit a critical section
 *
 * Underflow of the number of nesting levels of one of the critical sections. You tried
 * to exit a critical section while not inside one. Counting is per type, being one of
 * global, switch or tick. The lowest nibble  of the info byte
 * displays the task number.
 *
 * Review your code, there is flaw in the logic.
 */
#define  errTaskNestingUnderflowed        0x07

/** <!--2008-->
 * This call is illegal in nested critical sections
 *
 * You cannot make certain calls when within nested
 * critical sections. For example, all critical
 * sections must have been left when calling
 * taskRestart()
 */
#define  errTaskNestingPresent            0x08

/**
 *
 */
#define errUnused_0x09                    0x09

/** <!--200A-->
 * Task cannot make use of slot when non was configured.
 *
 * Trying to call an synchronization primitive on a task that was not configured
 * not to hold any slots (Slotsize == 0). This is an error. Define your task to
 * with SlotSize_task to make use of one or more slots. The lowest nibble of the info byte
 * displays the task number.
 */
#define  errTaskHoldsNoSlots              0x0A

/** <!--200B-->
 * Double slots cannot be used for this task.
 *
 * On waits double slots are never allowed. The same applies for mutexes and queu's
 * if cfgSyncDoubleBlock is not activated. This generates this error. The lowest nibble of the info byte
 * displays the task number.
 *
 * Adjust the slot use to include double slots, or change the logic or your code.
 */
#define  errSlotDoubleUsed                0x0B

/** <!--200C-->
 * Writing data to a full queue was forbidden.
 *
 * Trying to write data on an already full queue. This is not an error in strict sense,
 * since bytes put on a full queue are simply ignored. However, you asked to be noted
 * when this situation occurs. This error occurred inside a task (not an isr).
 * The lowest nibble of the info byte displays the task number, this highest the slot number.
 *
 * Review the logic of your code
 */
#define  errQueuOverrun                   0x0C

/** <!--200D-->
 * Reading data from an empty queue was forbidden.
 *
 * Trying to read data from an already empty queue. This is not an error in strict sense,
 * since bytes put on a full queu are simply ignored. However, you asked to be noted
 * when this situation occurs. This error occurred inside a task (not an isr).
 * The lowest nibble of the info byte displays the task number, this highest the slot number.
 *
 * Review the logic of your code
 */
#define  errQueuUnderrun                  0x0D

/** <!--200E-->
 * Tried to set the priority to a value above 7.
 *
 * There are only eight priority levels in the Femto OS. You tried to use a higher priority
 * (this also occurs at the use of a negative priority).  The lowest nibble of the info byte
 * displays the task number.
 *
 * Review the parameter of the function call.
 */
#define  errTaskPriorityTooHigh            0x0E

/** <!--200F-->
 * Delay equals of exceeds 0xFF00 ticks.
 *
 * The delay may not be any longer as 0xFEFF, directly or in internal calculation.
 * Internally a to high delay time arose. The lowest nibble of the info byte
 * displays the task number. This also might be the consequence of a faulty
 * reconstruction of the tick counter after sleeping.
 * The lowest nibble of the info byte
 * displays the task number.
 *
 * Lower the delay or timeout value you used in the call.
 */
#define  errTaskDelayTooLong              0x0F

/** <!--2010-->
 * System cannot handle 'negative' delays.
 *
 * In a delayFromWake the task is not de-activated, because the wake
 * time lies before the current time. This means this particular is running
 * beyond the required reactivation moment. There was no time to put the
 * task into a delay. Actually this is a dangerous situation, if it continues
 * a little longer, the task may get into a very extensive delay.
 * The lowest nibble of the info byte displays the task number.
 *
 * Increase the delayFromWake time, or, reduce the execution time of the code.
 */
#define  errTaskDelayTooShort             0x10

/** <!--2011-->
 *  Slot with number zero is being used, this is forbidden.
 *
 *  Slot with number zero is used for internal purposes and cannot be used
 *  by the application developer.
 *  The lowest nibble of the info byte displays the task number.
 *
 *  Use a valid slot.
 */
#define  errSlotZeroUsed                  0x11

/** <!--2012-->
 * Slot used outside its definition.
 *
 * Slot type mismatch, a function tried to use a slot for an other purpose as it was
 * defined. Mutexes for example cannot be used as queues.
 * The lowest nibble of the info byte displays the task number.
 *
 */
#define  errSlotTypeMismatch              0x12

/** <!--2013-->
 * Two identical slot numbers cannot be used for locking
 *
 * Twin slots. You cannot ask a double lock on two identical slot numbers.
 * The lowest nibble of the info byte displays the task number.
 *
 * Use a single lock, or choose two different slot numbers. Cut/Paste error?
 */
#define  errSlotTwins                     0x13

/** <!--2014-->
 * Task may not block or restart when in dominant state.
 *
 * Calling a method that may block, such as mutex, wait or a the opening
 * of a file with a task that is dominant may lead to a permanent system
 * deadlock. Therefore this is prohibited. Also, when restarting a task,
 * it may not be dominant, for it's state cannot be retained.
 * Also, you may not handle suspend requests in the dominant mode.
 */
#define  errIllegalDominantState          0x14

/** <!--2015-->
 * Request to write more data than space available
 *
 * Your are trying to write more data to a queue than the size of that queue.
 * This call can never succeed and is therefore an error, the call would block
 * indefinitely.
 * The lowest nibble of the info byte displays the task number.
 *
 * Reduce the size of the request.
 */
#define  errQueuWriteLimit                0x15

/** <!--2016-->
 * Request to read more data than space available
 *
 * Your are trying to read more data from a queue than the size of that queue.
 * This call can never succeed and is therefore an error, the call would block
 * indefinitely.
 * The lowest nibble of the info byte displays the task number.
 *
 * Reduce the size of the request.
 */
#define  errQueuReadLimit                 0x16

/** <!--2017-->
 * Trying to remove a slot not present on the slot stack.
 *
 * This error may arise if you relinquish more slots than where claimed.
 * Locking and releasing must always be balanced.
 * The lowest nibble of the info byte displays the task number, this highest the slot number.
 *
 * Review the logic of the code.
 */
#define  errSlotDecreaseFail              0x17

/** <!--2018-->
 * Trying to put more slots on the stack than room for.
 *
 * The slot stack, that is, the memory where the reserved slots are
 * kept for each task is limited and has been exceeded.
 * The lowest nibble of the info byte displays the task number, this highest the slot number.
 *
 * Increase the SlotSize_task parameter for your task.
 */
#define  errSlotIncreaseFail              0x18

/** <!--2019-->
 * File is open when it is not expected or allowed
 *
 * The call you made tried to open a file that is already open, or
 * is not allowed to be made when a file is open.
 * The lowest nibble of the info byte displays the task number.
 *
 * Close the file prior to your call.
 */
#define  errFileOpenMode                  0x19

/** <!--201A-->
 * Trying operations on a file which does not exist.
 *
 * The file you tried to use does not exist as dedicated (named) file
 * and not as default file.
 * The lowest nibble of the info byte displays the task number.
 *
 * Adjust the file number to a valid file, or increase the number of
 * files.
 */
#define  errFileInvalidFileNumber         0x1A

/** <!--201B-->
 * Trying to read or write outside the file.
 *
 * Operations for a particular file number are only allowed inside
 * the EEPROM space reserved for that file. You tried to read or write outside
 * the file. The lowest nibble of the info byte displays the task number.
 *
 * Reduce the read/write data size.
 */
#define  errFileOutOfRange                0x1B

/** <!--201C-->
 * File is closed when it is not expected or allowed
 *
 * The call you made tried to close a file that is already closed, or
 * is not allowed to be made when a file is closed.
 * The lowest nibble of the info byte displays the task number.
 *
 * Open the file prior to your call.
 */
#define  errFileClosedMode                0x1C

/** <!--201D-->
 * File is in the wrong mode for the requested operation
 *
 * The call you made expected to be the file in the read mode,
 * when is was in write, or vice versa. Or, the call is not
 * allowed in specific circumstances, for example with a file
 * open in write mode.
 * The lowest nibble of the info byte displays the task number.
 *
 * Bring the file in the correct mode prior to your call.
 */
#define  errFileWrongMode                 0x1D

/** <!--201E-->
 * Restart may only be called with a valid mode,
 *
 * Only four modes can be used to call restart.
 * These are: defRestartRunning, defRestartSuspended,
 * defRestartSleeping or defRestartDefault. Other
 * values can give bizarre results.
 */
#define  errInvalidRestartMode            0x1E

/** <!--201F-->
 * Suspend may only be called with a valid mode,
 *
 * Only three modes can be used to call suspend.
 * These are: defSuspendNow, defSuspendCheck or
 * defSuspendClear. Other values can give bizarre
 * results.
 */
#define  errInvalidSuspendMode            0x1F


/** <!--2020-->
 * Error of the OS, please report.
 *
 * This error should never occur, and signals a flaw
 * in the OS. If possible, please report the circumstances
 * which led to this situation.
 */
#define  fatInternalError                 0x20

/** <!--2021-->
 * The stack of the OS is under the required minimum.
 *
 * The stack of the OS is under the required minimum. It may
 * not be lower as the space taken by
 * the background variables. The information byte is unused.
 *
 * Increase the StackSizeOS
 */
#define  fatOsStackUnderMinimum           0x21

/** <!--2022-->
 * Overflow of the OS Stack
 *
 * During use the size of the OS stack became to small, other data may
 * be overwritten.The information byte is unused. Note that the OS needs more
 * stack space if checks are switched on.
 *
 * Increase the StackSizeOS
 */
#define  fatOsStackOverflowed             0x22

/** <!--2023-->
 * An illegal event has been fired.
 *
 * You tried to fire an event that is located on the auxiliary bit of the
 * Femto OS. Usually this is the event with the highest number.
 * The lowest nibble of the info byte displays the task number.
 *
 * Use an other event, or separate the event register from the  auxiliary bit
 */
#define  fatIllegalEvent                  0x23

/** <!--2024-->
 * The stack of the task became too small during execution.
 *
 * The stack of the task was not large enough to accommodate the
 * variables and/or the context. Since the task can not reliably
 * be stopped, this error is fatal
 * The lowest nibble of the info byte displays the task number.
 *
 * Increase the stack size of this task.
 */
#define  fatTaskStackOverflowed           0x24

/** <!--2025-->
 * The stack of the isr became too small during execution.
 *
 * The stack of the isr was not large enough to accommodate the
 * variables of the isr. Since the isr cannot be stopped, this error is fatal
 * The information byte is unused.
 *
 * Increase the stack of the isr.
 */
#define  fatIsrStackOverflowed            0x25

/** <!--2026-->
 * Ticks come at to short intervals.
 *
 * The OS takes more then 50% of the processor time regularly. In the equidistant timing model
 * the system is at risk of immediate timer interrupts after context switch. Note that incidental
 * high loads should not be a problem, for instance if a watchdog barks, or at heavy interrupt load.
 * The information byte is unused.
 *
 * Reduce the tick frequency or increase the clock frequency.
 */
#define  fatOsTickRateTooHigh             0x26

/** <!--2027-->
 * The sub tick timer overflowed.
 *
 * A task is too long, or the OS took to long  in the variable timing model, so that it
 * may overflow the sub tick timer. This may lead to missed ticks.
 * The lowest nibble of the info byte displays the task number.
 *
 * Increase the time a sub tick takes, thereby lowering the tick frequency.
 */
#define  fatTaskSubtickOverflow           0x27

/** <!--2028-->
 * A function call used a non existent task number.
 *
 * You may only use valid task numbers. If you make use of task names only, there is
 * no the risk of using undefined task numbers.
 * The lowest nibble of the info byte displays the task number (of the running task,
 * not of the call!)
 *
 * Check the calls, use task names.
 */
#define  fatTaskIllegalTaskNumber         0x28

/** <!---->
 *
 */
#define  fatUnused_0x29                   0x29

/** <!--202A-->
 * No generic functions from isr without OS interrupt protection.
 *
 * A 'genXXXX' function was used from an isr while cfgIntOsProtected was not set.
 * This is forbidden, and may lead to rare and hard to trace weird bugs.
 *
 * Remove the call, or switch on OS protection (cfgIntOsProtected).
 */
#define  fatIllegalCallfromISR            0x2A

/**
 * Unused fatal error.
 */
#define  fatUnused_0x2B                   0x2B

/**
 * Unused fatal error.
 */
#define  fatUnused_0x2C                   0x2C

/**
 * Unused fatal error.
 */
#define  fatUnused_0x2D                   0x2D

/**
 * Unused fatal error.
 */
#define  fatUnused_0x2E                   0x2E

/**
 * Unused fatal error.
 */
#define  fatUnused_0x2F                   0x2F


/**
 * General trace events. Use these in your trace application
 */
#define  traceNull                                    0x00  /* Nullbyte is not used, and may used as separation byte.           */
#define  traceMarker                                  0x01  /* Mark a special point in the tracing.                             */
#define  traceUserByteMessage                         0x02  /* Used to pass a payload information byte                          */
#define  traceUserWordMessage                         0x03  /* Used to pass a payload information word                          */
#define  traceTaskStart                               0x10  /* switched to the task number 0x00 - 0x0F by 0x10 - 0x1F           */
#define  traceOsStart                                 0x20  /* switched to the OS                                               */
#define  traceIdleStart                               0x21  /* switched to the idle task                                        */
#define  traceSleepStart                              0x22  /* switched to low power sleep                                      */
#define  traceTickInterrupt                           0x23  /* a tick interrupt has occurred                                    */
#define  traceWatchdog                                0x24  /* the watchdog barked, see following TaskInit for number           */
#define  traceLockAcquire                             0x25  /* the slot is free and the task may continue                       */
#define  traceLockReject                              0x26  /* the slot has been taken and the task will be blocked             */
#define  traceReset                                   0x27  /* a reset occurred                                                 */
#define  traceISR                                     0x28  /* switched to one of 8 interrupts 0x28 -- 0x2F, define yourself.   */
#define  traceSlotLock                                0x30  /* lock a slot, 2 bytes:   (0x30 | tasknumber), (slot1<<4 | slot2)  */
#define  traceSlotUnlock                              0x40  /* unlock a slot, 2 bytes: (0x40 | tasknumber), (slot1<<4 | slot2)  */
#define  traceFireEvent                               0x50  /* Fire of event number 0..7 given by 0x50 ... 0.57                 */
#define  traceWatermarks                              0x58  /* Watermarks of stacks of OS, ISR and tasks                        */
#define  traceUnused                                  0x59  /* Unused                                                           */
#define  traceREGISTERLEAK_5A                         0x5A  /* If this event occurs, you probably have a register leak          */
#define  traceBurnLock                                0x5B  /* File system not ready for burning                                */
#define  traceBurnFree                                0x5C  /* File system ready for burning                                    */
#define  traceCreateContext                           0x5D  /* construction of a context                                        */
#define  traceLoadInfo                                0x5E  /* Load info on Os, Idle, ISR + tasks, multibyte message            */
#define  traceOsTime                                  0x5F  /* Continuous average load of the Os, 3 byte message                */
#define  traceTaskInit                                0x60  /* Initialized task number 0x00 - 0x0F by 0x60 - 0x6F               */
#define  traceTaskRelease                             0x70  /* A blocked (Sync,File,Event) task may continue 0x70..0x7F         */
#define  traceAPIcallBase                             0x80  /* 0x80 -- 0xCF api called (max 80 functions's)                     */
#define  traceAPIcall1                                0x90  /* See below for definitions                                        */
#define  traceAPIcall2                                0xA0  /*                                                                  */
#define  traceAPIcall3                                0xB0  /*                                                                  */
#define  traceAPIcall4                                0xC0  /*                                                                  */
#define  traceErrorBase                               0xD0  /* 0xD0 -- 0xFF error situation (max 48 error's)                    */
#define  traceError1                                  0xE0  /* See above for definitions                                        */
#define  traceError2                                  0xF0  /*                                                                  */

/**
 * API call ID's. Use these to see which method has been called.
 */
#define  callIdSystem                                 0x00  /** <!----> */
#define  callIdTaskYield                              0x01  /** <!--0125--> */
#define  callIdTaskWaitForEvents                      0x02  /** <!--0805--> */
#define  callIdTaskDelayFromNow                       0x03  /** <!--0116--> */
#define  callIdTaskDelayFromWake                      0x04  /** <!--0117--> */
#define  callIdTaskSuspend                            0x05  /** <!--0120--> */
#define  callIdTaskSleep                              0x06  /** <!--0123--> */
#define  callIdTaskSleepAll                           0x07  /** <!--0124--> */
#define  callIdTaskRecreate                           0x08  /** <!--0121--> */
#define  callIdTaskFeedWatchdog                       0x09  /** <!--0118--> */
#define  callIdTaskKillWatchdog                       0x0A  /** <!--0119--> */
#define  callIdTaskRestart                            0x0B  /** <!--0127--> */
#define  callIdTaskTerminate                          0x0C  /** <!--0107--> */
#define  callIdTaskEnableGlobalInterrupts             0x0D  /** <!--0412--> */
#define  callIdTaskDisableGlobalInterrupts            0x0E  /** <!--0410--> */
#define  callIdTaskEnableTickInterrupts               0x0F  /** <!--0417--> */
#define  callIdTaskDisableTickInterrupts              0x10  /** <!--0418--> */
#define  callIdTaskEnableSwitchTask                   0x11  /** <!--0411--> */
#define  callIdTaskDisableSwitchTask                  0x12  /** <!--0409--> */
#define  callIdTaskEnterGlobalCritical                0x13  /** <!--0413--> */
#define  callIdTaskExitGlobalCritical                 0x14  /** <!--0414--> */
#define  callIdTaskEnterTickCritical                  0x15  /** <!--0419--> */
#define  callIdTaskExitTickCritical                   0x16  /** <!--0420--> */
#define  callIdTaskEnterSwitchCritical                0x17  /** <!--0415--> */
#define  callIdTaskExitSwitchCritical                 0x18  /** <!--0416--> */
#define  callIdTaskFileOpen                           0x19  /** <!--0501--> */
#define  callIdTaskFileClose                          0x1A  /** <!--0502--> */
#define  callIdTaskFileFormat                         0x1B  /** <!--0505--> */
#define  callIdTaskFileGetSize                        0x1C  /** <!--0504--> */
#define  callIdTaskFileSetSize                        0x1D  /** <!--0503--> */
#define  callIdTaskFileReadByte                       0x1E  /** <!--0507--> */
#define  callIdTaskFileWriteByte                      0x1F  /** <!--0508--> */
#define  callIdTaskFileAppendByte                     0x20  /** <!--0509--> */
#define  callIdTaskFileReadPipe                       0x21  /** <!--0510--> */
#define  callIdTaskFileWritePipe                      0x22  /** <!--0511--> */
#define  callIdTaskFileReadBuffer                     0x23  /** <!--0512--> */
#define  callIdTaskFileWriteBuffer                    0x24  /** <!--0513--> */
#define  callIdREGISTERLEAK_A5                        0x25  /** <!----> */
#define  callIdTaskWaitForTasks                       0x26  /** <!--0331--> */
#define  callIdTaskSyncRequest                        0x27  /** <!--0324--> */
#define  callIdTaskSyncRelease                        0x28  /** <!--0325--> */
#define  callIdTaskMemMalloc                          0x29  /** <!----> */
#define  callIdTaskMemFree                            0x2A  /** <!----> */
#define  callIdTaskMemDefrag                          0x2B  /** <!----> */
#define  callIdTaskMemPointer                         0x2C  /** <!----> */
#define  callIdTaskReserved1                          0x2D  /** <!----> */
#define  callIdTaskReserved2                          0x2E  /** <!----> */
#define  callIdTaskReserved3                          0x2F  /** <!----> */
#define  callIdTaskReserved4                          0x30  /** <!----> */
#define  callIdTaskReserved5                          0x31  /** <!----> */
#define  callIdTaskReserved6                          0x32  /** <!----> */
#define  callIdGenSuspend                             0x33  /** <!--0113--> */
#define  callIdGenResume                              0x34  /** <!--0109--> */
#define  callIdGenSetPriority                         0x35  /** <!--0111--> */
#define  callIdGenGetPriority                         0x36  /** <!--0101--> */
#define  callIdGenGetTickCount                        0x37  /** <!--0204--> */
#define  callIdGenGetLastWakeTime                     0x38  /** <!--0103--> */
#define  callIdGenGetTaskname                         0x39  /** <!--0105--> */
#define  callIdGenFireEventSet                        0x3A  /** <!--0801--> */
#define  callIdGenReboot                              0x3B  /** <!--0207--> */
#define  callIdGenLogTask                             0x3C  /** <!--0206--> */
#define  callIdGenLogOs                               0x3D  /** <!--0205--> */
#define  callIdGenWaitRelease                         0x3E  /** <!--0317--> */
#define  callIdGenQueuWrite                           0x3F  /** <!--0309--> */
#define  callIdGenQueuRead                            0x40  /** <!--0312--> */
#define  callIdGenQueuPeek                            0x41  /** <!--0307--> */
#define  callIdGenQueuReadable                        0x42  /** <!--0315--> */
#define  callIdGenQueuWriteable                       0x43  /** <!--0311--> */
#define  callIdGenQueuFull                            0x44  /** <!--0305--> */
#define  callIdGenQueuEmpty                           0x45  /** <!--0303--> */
#define  callIdGenQueuClear                           0x46  /** <!--0301--> */
#define  callIdGenAddtoTickCount                      0x47  /** <!--0203--> */
#define  callIdGenPassFlashString                     0x48  /** <!--0201--> */
#define  callIdGenCountEventBlocks                    0x49  /** <!--0804--> */
#define  callIdGenQueuUnwrite                         0x4A  /** <!--0327--> */
#define  callIdGenQueuUnread                          0x4B  /** <!--0328--> */
#define  callIdGenReserved1                           0x4C  /** <!----> */
#define  callIdGenReserved2                           0x4D  /** <!----> */
#define  callIdGenReserved3                           0x4E  /** <!----> */
#define  callIdGenReserved4                           0x4F  /** <!----> */

/**
 * API call pseudo ID's. These are not actually used, or are 'OnName' functions
 */
#define  callIdTaskStackCheck                         ----  /** <!--0126--> */
#define  callIdGenPipeInt16                           ----  /** <!--0202--> */
#define  callIdGenTraceByteInfo                       ----  /** <!--0208--> */
#define  callIdGenTraceWordInfo                       ----  /** <!--0210--> */
#define  callIdGenTraceMarker                         ----  /** <!--0209--> */
#define  callIdTaskMutexRequestOnName                 0x27  /** <!--0318--> */
#define  callIdTaskMutexReleaseOnName                 0x27  /** <!--0322--> */
#define  callIdTaskQueuWriteRequestOnName             0x27  /** <!--0319--> */
#define  callIdTaskQueuReadRequestOnName              0x27  /** <!--0319--> */
#define  callIdTaskQueuReleaseOnName                  0x28  /** <!--0321--> */
#define  callIdTaskSyncReleaseOnName                  0x28  /** <!--0326--> */
#define  callIdTaskSyncWriteWriteRequestOnName        0x27  /** <!--0320--> */
#define  callIdTaskSyncWriteReadRequestOnName         0x27  /** <!--0320--> */
#define  callIdTaskSyncReadWriteRequestOnName         0x27  /** <!--0320--> */
#define  callIdTaskSyncReadReadRequestOnName          0x27  /** <!--0320--> */
#define  callIdTaskWaitForOtherTask                   0x26  /** <!--0332--> */
#define  callIdIsrBegin                               ----  /** <!--0401--> */
#define  callIdIsrEndReturn                           ----  /** <!--0402--> */
#define  callIdIsrEndYield                            ----  /** <!--0403--> */
#define  callIdIsrEnter                               ----  /** <!--0404--> */
#define  callIdIsrExit                                ----  /** <!--0405--> */
#define  callIdIsrStackCheck                          ----  /** <!--0406--> */
#define  callIdIsrStartLoad                           ----  /** <!--0407--> */
#define  callIdIsrStopLoad                            ----  /** <!--0408--> */
#define  callIdTaskFileClear                          0x1D  /** <!--0516--> */
#define  callIdTaskFileDirectRead                     ----  /** <!--0514--> */
#define  callIdTaskFileDirectWrite                    ----  /** <!--0515--> */
#define  callIdGenFireEvent                           ----  /** <!--0802--> */
#define  callIdIsrFireEvent                           ----  /** <!--0803--> */
#define  callIdTaskWaitForEventSet                    0x02  /** <!--0806--> */


/**
 * Communication bytes. Use this in for example a shell. They take less space than
 * full strings. In you shell app on a pc you can translate them to string
 * messages. All request/response bytes have predefined length for the associated
 * data streams.
 */
#define  defResponseOK                                0x01  /* datalength: 0   */
#define  defResponseUnknownCommand                    0x02  /* datalength: 0   */
#define  defResponseError                             0x03  /* datalength: 0   */
#define  defResponseInvalidTask                       0x04  /* datalength: 0   */
#define  defResponseInvalidPriority                   0x05  /* datalength: 0   */
#define  defResponseFemtoOs                           0x10  /* datalength: 12  */
#define  defResponseLogTask                           0x11  /* datalength: 16  */
#define  defResponseLogOs                             0x12  /* datalength: 10  */
#define  defResponseTickCount                         0x13  /* datalength: 2   */
#define  defResponseDump                              0x14  /* datalength: 2   */
#define  defResponseUptime                            0x15  /* datalength: 5   */
#define  defResponseFile                              0x16  /* datalength: x   */
#define  defResponseInvalidFileNumber                 0x17  /* datalength: x   */

#define  defRequestFemtoOs                            0x90  /* datalength: 0   */
#define  defRequestProcessList                        0x91  /* datalength: 0   */
#define  defRequestTickCount                          0x93  /* datalength: 0   */
#define  defRequestReboot                             0x94  /* datalength: 0   */
#define  defRequestSuspend                            0x95  /* datalength: 1   */
#define  defRequestResume                             0x96  /* datalength: 1   */
#define  defRequestTerminate                          0x97  /* datalength: 1   */
#define  defRequestPriority                           0x98  /* datalength: 2   */
#define  defRequestRecreate                           0x99  /* datalength: 2   */
#define  defRequestDump                               0x9A  /* datalength: 2   */
#define  defRequestStore                              0x9B  /* datalength: x   */
#define  defRequestRecall                             0x9C  /* datalength: x   */
#define  defRequestUptime                             0x9D  /* datalength: 0   */
#define  defRequestTest                               0x9E  /* datalength: 1  */

#define defDumpRAM                                    0x01
#define defDumpEEPROM                                 0x02
#define defDumpFLASH                                  0x03

#endif /* FEMTOOS_GLOBALS_H */
