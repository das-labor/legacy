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
 * Please note 2that, due to the GPLv3 license, for application of this
 * work and/or combined work in embedded systems special obligations apply.
 * If these are not to you liking, please know the Femto OS is dual
 * licensed. A commercial license and support are available.
 * See http://www.femtoos.org/ for details.
 */

#ifndef FEMTOOS_CORE_H
#define FEMTOOS_CORE_H

#include   "femtoos_port.h"

/* DISCUSSION:
 * Several methods are defined through preprocessor definitions only. This
 * applies for the isrXXX methods, depending on the use they are coupled to
 * portXXX or privXXX methods. You cannot, in general make regular functions of
 * these methods, due to balanced stack depth.
 */


/**
 * General information. The method names below are as descriptive as i
 * could think of. The nomenclature is as follows:
 * privXXXX: this method is for internal use only. It is defined here
 *   because it may be called from the portable routines, or the may
 *   be part of a define. Never call them from application code.
 * portXXXX: methods that are part of the portable part of this system.
 *   These are rarely called, but there are some exceptions. It is,
 *   for example possible to directly call portShowError or so.
 *   Of course you never need to call portSaveContext or so, but also
 *   portEnable/DisbaleGlobalInterrupts are butter called through there
 *   equivalents taskEnable/DisableGlobalInterrupts.
 * genXXXXX: these methods contain code save to call from any environment,
 *   a task, an isr or from bark or init. They are non switching, and (tick)
 *   interrupt protected, depending on there function. Note some functions
 *   use quite a lot of stack. Note that, in case we have cfgIntOsProtected
 *   set to false, these functions are forbidden to call from an isr.
 * taskXXXX: these methods can only be called from an task environment. Never
 *   call them from isr or bark or init. They may be switching, i.e. start
 *   with a context switch.
 * isrXXXXX: methods that may only be called from within the isr.
 * appXXXXX: methods that you define as hooks in your application for the
 *   system to call. This includes the application loop, tick hooks etc.
 * Furthermore, quite a lot of methods take a task number or a slot number
 * as parameter. Since these numbers may not be constant, it is better
 * to use task names/slot names and let the preprocessor do the work for
 * you. All methods taking such parameters have defines with the name
 * xxxxOnName which take such parameters. Please consult the
 * example applications to see how to use them.
 *
 * Note that tasks calling methods utilizing a slot, can only hold one
 * slot at the time. Thus, you cannot for example request access to a
 * queue on slot 1 and then wait for a mutex on slot 2. furthermore, a slot
 * can only be used for one purpose at the time. Thus if one task utilizes
 * a slot for a rendezvous, and other task may not use the same slot for
 * a mutex at the same time. If you are sure this use cannot be mixed in
 * time, it is possible, even for queues. However, it is better to utilize
 * the slots for one purpose and avoid mixing. It does not cost extra
 * ram to make use of more or higher slot numbers. Slot number 0 must never
 * be used. See also the discussion in on Queue Usage.
 */


/**
 * Internal use only, do not call it yourself from application code.
 *
 * This method is called directly after a context switch and takes care
 * internal administrative procedures.
 */
#if (defCheckReportingError == cfgTrue)
  void privInitOs(Tuint08 uiErrorControl) __attribute__ (( noinline ));
  #define errOsStateForce        0x40
  #define errOsStateAsIs         0x00
  #define privInitOsReturn()     privInitOs(errOsStateForce)
  #define privInitOsSwitch()     privInitOs(errOsStateAsIs)
#else
  void privInitOs(void) __attribute__ (( noinline ));
  #define privInitOsReturn()     privInitOs()
  #define privInitOsSwitch()     privInitOs()
#endif


/**
 * Internal use only, do not call it yourself from application code.
 *
 * Switches from isr stack to os stack and starts the OS.
 */
void privIsrExit(void) defSysReduceProEpilogue;

/**
 * Internal use only, do not call it yourself from application code.
 *
 * Called from the interrupt which signals the overflow of the subtick timer.
 * Calling this directly results in an error.
 */
void privSubtickOverflow(void);

/**
 * Internal use only, do not call it yourself from application code.
 *
 * Called from the timer interrupt or any place from inside the
 * OS when a Yield is necessary.
 */
void privTickYield(void) defSysReduceProEpilogue;

/**
 * Internal use only, do not call it yourself from application code.
 *
 * Called to check if we can write a byte or if we must block.
 */
void privWaitForFsAccess(void) defSysWrapperEpilogue;

/**
 * Internal use only, do not call it yourself from application code.
 *
 * Starts the OS, changes the stack, and then starts the first task.
 * http://www.mail-archive.com/avr-gcc-list@nongnu.org/msg03812.html
 * Note: we could use defSysReduceMain but the difference is minimal,
 * so for those who have the patch not installed we do not do so.
 */
void main(void) defSysReduceProEpilogue;

/** <!--0125-->
 * Manual context switch
 *
 * Call this function inside your code when you want to force a context switch.
 * Note it makes no real sense to call it if you are the task with the highest
 * priority for it will immediately return. Mostly you use this call as some
 * kind of cooperative escape from part of your code where interrupts are disabled.
 */
void taskYield(void) defSysWrapperEpilogue;

/** <!--0116-->
 * Delay the task a number of ticks, starting from now.
 *
 * taskDelayFromNow delays your task for the given number of ticks. This value
 * represents the minimum delay. Since the delay time is measured from the
 * moment the delay call is issued, it cannot be used to obtain a reliable
 * frequency.
 */
void taskDelayFromNow(Tuint16 uiTicksToWait) defSysWrapperEpilogue;

/** <!--0117-->
 * Delay the task a number of ticks, starting at the last wake time.
 *
 * taskDelayFromWake delays your task for the given number of ticks. The
 * delay time is measured from the moment of last wakeup. This is automatically
 * correct when the tasks are only stopped by delays. If however you want
 * it to be correct in case of suspended and locked tasks as well, set
 * cfgUseCorrectWakeupTimes to true. It can be used to obtain a reliable
 * frequency, within the limits of the system clock and demands of other
 * tasks.
 */
void taskDelayFromWake(Tuint16 uiTicksToWait) defSysWrapperEpilogue;

/** <!--0113-->
 * Suspend a task, but do not invoke a context switch.
 *
 * Call this method if you want to suspend a particular task. The state of
 * that task is not changed to suspended immediately, but this is viewed
 * as a suspend request, to which the task may respond if he likes, by
 * calling taskSuspend(defSuspendCheck). The reason the call does not have
 * immediate effect is that the current state of the target
 * task is unclear. It may be blocking or holding locks or have
 * open files. Suspending such tasks may take down the whole system.
 * If the task is restarted however the state is well defined (it may
 * not hold locks of blocks) the request is always honored, independent
 * of the parameter restart was called with. The latter also implies
 * that this call may used in the bark method of a task, in order to stop
 * the task until the cause of the timeout problems have been eliminated.
 *
 * The call does not force a context switch, thus if you call it with
 * the current running task number the method returns.
 *
 * The call has no effect on terminated or already suspended tasks.
 * Sleeping tasks will receive the request and be suspended when they
 * handle the call after they have waken.
 *
 * When called upon shared tasks, the result depends on the state
 * of the shared task. If it is currently scheduled for execution,
 * if will be treated as a normal task. So if it runs into a
 * taskSuspend(defSuspendCheck) it will be suspended immediately, and
 * no other shared tasks are able to run again. If the shared task
 * hits restart first, the task will be suspended in shared state,
 * thus allowing the other shared states to run. In general it is
 * inadvisable to have a taskSuspend() in a shared task.
 */
void genSuspend(Tuint08 uiTaskNumber);
#define genSuspendOnName(TaskName) genSuspend(preTaskNumberOf(TaskName))

/** <!--0120-->
 * Suspend this task and invoke a context switch.
 *
 * If you need to suspend your task immediately call this method with
 * the parameter defSuspendNow. If you want to handle a suspend request
 * from genSuspend(), call with defSuspendCheck. The
 * suspended state is directly applied, a context switch is performed.
 * This method only returns if an other task (or isr) calls genResume
 * on your task.
 *
 * If you want to remove any suspend requests, call it with defSuspendClear.
 * The task will not suspend and the request will be deleted.
 *
 * Do not use this at places where you may hold resources, such are
 * files, or locks. Although possible, other tasks may be effectively
 * blocked as well. Or know what you are doing. It is an error to call
 * it in the dominant mode, since it will render the switch protection
 * of that mode useless.
 */
void taskSuspend(Tuint08 uiSuspendMode) defSysWrapperEpilogue;

/** <!--0109-->
 * Resume the given task from suspension.
 *
 * A suspended task can be resumed by this call. A task that is in an
 * other state (running (go), terminated, sleeping) remain unchanged.
 * So there is no harm in calling this method on such tasks.
 *
 * Any standing suspend requests are cleared. So you can be certain that
 * the task will not be suspend as a result of prior genSuspend() calls.
 */
void genResume(Tuint08 uiTaskNumber);
#define genResumeOnName(TaskName) genResume(preTaskNumberOf(TaskName))

/** <!--0123-->
 * Put the current task to sleep and invoke a context switch.
 *
 * Call this if you want to put the current task into low power sleep.
 * A sleeping task is not rescheduled for operation, and cannot be
 * woken by the user anymore (it can be recreated though). If all
 * tasks are in sleep state, or some delays are sufficiently long,
 * the device may go to low power sleep, see the options for details.
 * Note that there is no call to put an other specific task to sleep.
 * The method returns when the sleep has taken place.
 * Never call sleep when you have a write lock on the file system, and
 * it is unwise to call it when you hold a read lock (since it will
 * prevent other tasks from writing). Thus call sleep after you have
 * completed file operations (i.e. after fileClose)
 *
 * It is possible to put yourself to sleep in the dominant mode. Although
 * the dominant mode is remembered and revived after the sleep, during
 * sleep other tasks will be activated, which may, request for a dominant
 * mode themselves. This may lead to an unclear situation at wake up.
 *
 * Shared tasks can be put to sleep with this call, but after sleep the
 * call does not return, but the task is restarted in the running mode
 * instead. During sleep of the task, other shared tasks are rescheduled,
 * when present. Because this call works as a restart for shared tasks
 * you are not allowed to call it with any locks.
 *
 */
void taskSleep(void) defSysWrapperEpilogue;

/** <!--0124-->
 * Put all tasks to sleep.
 *
 * Call this is you want give the OS the possibility to go to low
 * power sleep directly. It puts all tasks into low power sleep, which
 * should normally trigger the sleep mode. If any task is using the file
 * system, or is blocking to get access, Femto OS waits until all file
 * write operations have come to an end, and puts the system to sleep
 * directly after. Tasks not using the FS at the moment of calling
 * are put so sleep immediately. The current task may make use of the FS
 * and is treated like other tasks. Thus if the task is in read only
 * mode it will sleep directly; if the task is in read/write mode it will
 * do so at taskFileClose(). See also the discussion on taskSleep().
 * The method returns when the sleep has taken place.
 */
void taskSleepAll(void) defSysWrapperEpilogue;

/** <!--0111-->
 * Set a new priority for the given task.
 *
 * All task run in the initial priority. There are eight priorities running
 * from 0 to 7, 0 being the lowest. The idle task runs in fictive priority -1,
 * thus being the lowest of all. The priority can be raised or lowered at will.
 */
void genSetPriority(Tuint08 uiTaskNumber, Tuint08 uiNewPriority);
#define genSetPriorityOnName(TaskName,NewPriority) genSetPriority(preTaskNumberOf(TaskName),NewPriority)

/** <!--0101-->
 * Return the current priority of a given task.
 *
 * Call this to retrieve the current priority of a task.Of course the information
 * may be outdated the moment it is received. Returns a value between 0 and 7,
 * boundary values included.
 */
Tuint08 genGetPriority(Tuint08 uiTaskNumber);
#define genGetPriorityOnName(TaskName) genGetPriority(preTaskNumberOf(TaskName))

/** <!--0204-->
 * Get the tick counter.
 *
 * Returns the current value of the tick counter. The tick counter is a 16 bit
 * value running from 0 to 0xFFFF. If the tick interrupts come every millisecond
 * we have a timer of approximately 65 seconds.
 */
Tuint16 genGetTickCount(void);

/** <!--0203-->
 * Add the given number of ticks to the tick counter.
 *
 * With this function it is possible to add some value to the tick counter,
 * which is shifted to the future. All missed appTick00() and appTick08() calls
 * are NOT catched up. However an appTick16() call is performed should the
 * tick counter roll over. This method is typically used in situations where
 * the processor is put into a low power sleep.
 */
void genAddtoTickCount(Tuint16 uiSleepTime);

/** <!--0103-->
 * Return the time the task was last woken.
 *
 * When you want to know what the last time was a task was woken call this
 * function. If it was last woken from a delay this time is always correct,
 * otherwise activate the cfgUseCorrectWakeupTimes option. When activated,
 * other wakeup events are recorded as well.
 */
Tuint16 genGetLastWakeTime(Tuint08 uiTaskNumber);
#define genGetLastWakeTimeOnName(TaskName) genGetLastWakeTime(preTaskNumberOf(TaskName))

/** <!--0118-->
 * Feed the watchdog in order to prevent barking.
 *
 * If you activated cfgUseTaskWatchdog and fed the watchdog at least once,
 * it is on alert. Each task has its own watchdog. After a certain time
 * (configurable with cfgNumWatchdogDiv) it starts barking, which means it calls
 * your bark routine if specified and subsequently restarts the task, by
 * re-intializing the context and calling your loop code. To prevent the watchdog
 * from barking, call taskFeedWatchdog() regularly from you code. If you want
 * to stop its activity call taskKillWatchdog().
 * Please note that every loop starts with de-activated watchdog, the watchdog
 * is activated after the first taskFeedWatchdog(). If a task got stuck and is
 * restarted, the watchdog is de-activated! Make sure you activate it early in
 * your code.
 */
void taskFeedWatchdog(void);

/** <!--0119-->
 * Switch off the watchdog facility for this task.
 *
 * See also taskFeedWatchdog(). A watchdog once activated must or be fed or be
 * killed in order to keep it quit. You can call taskKillWatchdog on a already
 * dead watchdog.
 */
void taskKillWatchdog(void);

/** <!--0105-->
 * Get the flash address of the task name
 *
 * Call this method to get the address of the task name of the given task
 * Note that it returns an address in flash, so take appropriate measures
 * read the data, for example read the bytes using portFlashReadByte().
 */
Taddress genGetTaskname(Tuint08 uiTaskNumber);
#define genGetTasknameOnName(TaskName) genGetTaskname(preTaskNumberOf(TaskName))

/** <!--0107-->
 * Stop (in error mode) one particular task.
 *
 * Call this function if you want to stop a task permanently. The task
 * is terminated and cannot be revived by resume or so. In case you have
 * some checking activated, and the system detects an error, the task
 * is brought to the same state. A terminated task can be recreated, provided
 * you compiled the necessary code in. See taskCreate() for more information.
 */
void taskTerminate(Tuint08 uiTaskNumber) defSysWrapperEpilogue;
#define taskTerminateOnName(TaskName) taskTerminate(preTaskNumberOf(TaskName))

/** <!--0121-->
 * Recreate one particular task.
 *
 * Recreate a task by recreating its context. appInit() if present is called
 * again, for (hardware) (re)initialization. The context is
 * cleared, locks are released, and the appLoop() is called, as if the
 * task is new. Your (private) static variables are not cleaned or changed.
 * The task may be terminated, running or in whatever state. You may even
 * call it on you own task, in which case this call does not return.
 */
void taskRecreate(Tuint08 uiTaskNumber) defSysWrapperEpilogue;
#define taskRecreateOnName(TaskName) taskRecreate(preTaskNumberOf(TaskName))

/** <!--0127-->
 * Restart the running task.
 *
 * Restart the task currently running. If present the watchdog is killed and
 * the context is cleared. Subsequently appLoop() is called. appInit() is
 * not called again, this is for one time (hardware) initialization.
 * Watermarks are not cleared. Locks are not released and the priority is kept.
 * Holding a lock at the moment of this call is an error.
 * You can supply a delay, which means that the task is restarted not earlier
 * as the given delay. This call does not return.
 * Your (private) static variables are not cleaned or changed.
 */
void taskRestart(Tuint08 uiRestartMode, Tuint16 uiTicksToWait) defSysWrapperEpilogue __attribute__ ( ( noreturn ) );

/** <!--0207-->
 * Reboot the system
 *
 * This call reboots, but not necessarily resets, the system. The actual
 * result depends on the contents of portReboot(), but on the my avr it
 * simply calls reset, which does not imply a full hardware reset. The
 * software is fully reset though, inclusive cleaning of all variables.
 * As this function may be called from an interrupt it may be used to
 * reactivate a dead system. An other use is a call in a shell. Normally
 * however, there is not much need for such a function. This function
 * should not return.
 */
void genReboot(void) __attribute__ ( ( noreturn ) );

/** <!--0206-->
 * Binary one liners about every task
 *
 * Call this function to obtain information about the a task in  the OS.
 * The information is piped through the given pipe method, which could
 * be a write method on a queue, so there is no need for a separate buffer.
 * The information is in binary form, of the following form:
 * Id(1) TaskNr(1) Status(1) LoadOs/0(2) Name(8) TaskMonitor(1) TaskLevels(1) SlotStack(4) StackMax(2) RegUse(1)
 * the number indicating the number of bytes to be expected.
 */
void genLogTask(Tuint08 uiTaskNumber, void (*pipe)(Tchar));

/** <!--0205-->
 * Binary one liner about the os
 *
 * Call this function to obtain information about the OS itself
 * The information is piped through the given pipe method, which could
 * be a write method on a queue, so there is no need for a separate buffer.
 * The information is in binary form, of the following form:
 * Id(1) StatusOs(1) LoadOs(2) LoadIdle(2) LoadIsr(2) StackOsMax(1) StackIsrMax(2)
 * the number indicating the number of bytes to be expected.
 */
void genLogOs(void (*pipe)(Tchar));

#if (includeGenPipeInt16 == cfgTrue)
/** <!--0202-->
 * Auxiliary function to push a Tint16 on a pipe
 *
 * Inside genLogOs and genLogTask use is made of the function to pipe a 16 bit
 * number. It is available for the user also. Note that this function when used
 * externally uses excessive amount of stack so that is may be more
 * efficient to just duplicate the code and inline it.
 */
void genPipeInt16(Tuint16 uiValue, void (*pipe)(Tchar));
#endif

#if (includeGenPassFlashString == cfgTrue)
/** <!--0201-->
 * Auxiliary function to push a string on a pipe
 *
 * Inside genLogOs and genLogTask use is made of the function to pipe a string
 * from flash. When uiLength == 0 this method continues and scans the string
 * for a terminating null character. If uiLength > 0 the number of chars is
 * exactly uiLength. If the real length of the string is shorter it is augmented
 * with cFilling chars, otherwise the string is just truncated. The terminating
 * null character is never copied. Note that this function when used
 * externally uses excessive amount of stack so that is may be more
 * efficient to just duplicate the code and inline it.
 */
void genPassFlashString(Taddress pString, Tuint08 uiLength, Tchar cFilling, void (*pipe)(Tchar));
#endif

/** <!--0208-->
 * Custom trace facility, sends one byte over the traceline.
 *
 * You can make use of the trace facility yourself by using this routine. It puts out one
 * payload byte after the eventUserByteMessage trace message. If cfgCheckTrace == cfgFalse
 * it is replaced by an empty statement. Include with
 */
#if (cfgCheckTrace == cfgTrue)
  void genTraceByteInfo(Tbyte bUser);
#else
  #define genTraceByteInfo(bUser)  do {} while(false)
#endif

/** <!--0210-->
 * Custom trace facility, sends one byte over the traceline.
 *
 * You can make use of the trace facility yourself by using this routine. It puts out one
 * payload byte after the eventUserByteMessage trace message. If cfgCheckTrace == cfgFalse
 * it is replaced by an empty statement. Include with
 */
#if (cfgCheckTrace == cfgTrue)
  void genTraceWordInfo(Tword wUser);
#else
  #define genTraceWordInfo(bUser)  do {} while(false)
#endif

/** <!--0209-->
 * Custom trace facility, sends a marker traceline.
 *
 * You can make use of the trace facility yourself by using this routine. Use the marker to start
 * tracing a some later point. In the trace program you can specify how many markers must have passed
 * before the tracing starts. If cfgCheckTrace == cfgFalse it is replaced by an empty statement.
 */
#if (cfgCheckTrace == cfgTrue)
 void  genTraceMarker(void);
#else
  #define genTraceMarker()
#endif


#if (cfgUseTimeout == cfgTrue)
  /** <!--0331-->
   * Lock this task until n other tasks pass on the same slot.
   *
   * Use this function to hold one or more tasks and let it wait
   * for the given number of tasks to pass the same lock. In other
   * words it is a rendezvous of N (uiNumberOfTasks) tasks. Make
   * sure N is equal on all calls because as soon as the number of
   * tasks is equal or lower a the value specified all are released.
   * You can specify a timeout, if cfgUseTimeout is activated.
   * The function returns true if the rendezvous took place, false
   * otherwise.
   */
  Tbool taskWaitForTasks(Tuint08 uiSlot, Tuint08 uiNumberOfTasks, Tuint16 uiTicksToWait) defSysWrapperEpilogue;
  #define taskWaitForTasksOnName(SlotName,NumberOfTasks,TicksToWait) taskWaitForTasks(preSlotNumberOf(SlotName),NumberOfTasks,TicksToWait)
  /** <!--0332-->
   * Lock this task until an other tasks pass on the same slot.
   *
   * Use this function to hold one task and let it wait
   * for an other task to pass the same lock. In other
   * words it is a simple rendezvous
   * You can specify a timeout, if cfgUseTimeout is activated.
   * The function returns true if the rendezvous took place, false
   * otherwise.
   */
  #define taskWaitForOtherTask(Slot,TicksToWait)                     taskWaitForTasks(Slot,2,TicksToWait)
  #define taskWaitForOtherTaskOnName(SlotName,TicksToWait)           taskWaitForTasks(preSlotNumberOf(SlotName),2,TicksToWait)
#else
  void taskWaitForTasks(Tuint08 uiSlot, Tuint08 uiNumberOfTasks) defSysWrapperEpilogue;
  #define taskWaitForTasksOnName(SlotName,NumberOfTasks) taskWaitForTasks(preSlotNumberOf(SlotName),NumberOfTasks)
  #define taskWaitForOtherTask(Slot)                     taskWaitForTasks(Slot,2)
  #define taskWaitForOtherTaskOnName(SlotName)           taskWaitForTasks(preSlotNumberOf(SlotName),2)
#endif


/** <!--0317-->
 * Release all wait locks on this slot.
 *
 * Use this call to release a rendezvous started with taskWaitForTasks() or
 * taskWaitForOtherTask(). All waiting tasks are released and return true when
 * cfgUseTimeout is activated. Note this method can be called from within
 * an isr. On other specialty is that this call may also be performed from
 * tasks not holding slots by themselves.
 */
void  genWaitRelease(Tuint08 uiSlot);
#define  genWaitReleaseOnName(SlotName) genWaitRelease(preSlotNumberOf(SlotName))

/** <!--0324-->
 * Try to obtain a lock on a queue , on a mutex or both.
 *
 * The method taskSyncRequest is the general way to obtain a lock on a mutex or queue.
 * It is even possible to obtain a simultaneous lock on two mutexes, two queues or
 * a mutex and a queue. Simultaneous locks hold the task blocked until both resources
 * are free. The uiSlotSlot, may contain one slot as parameter (the least significant
 * nibble) or two slots, one in the low nibble and one in the high nibble. Only a slot
 * in the high nibble is not allowed, even as two identical slots. (Would not make
 * much sense either). For queue's you must specify if , and how many bytes are to
 * be read (>0) or written (<0), maximally 127 bytes.
 *
 * The request blocks until
 * the requested number of bytes is available for reading or writing. If you
 * just want access to the queue, supply 0. For mutexes, supply zero for the free filling
 * parameter (in practice it is ignored, but gives an error if method checking is
 * activated)
 *
 * You can specify a timeout, if cfgUseTimeout is
 * activated, in which case it may return false, indicating a timeout. It returns
 * true if the lock was obtained.
 *
 * Slots must be configured as queue's in the config section. queue always fill up the
 * lowest slot numbers, thus 1..n where n is the maximum number of queue's. The mutexes
 * are numbered n+1 ... n+m, where m is the number of mutexes. But, it is easier to make
 * use of named queues and muytexes. If you do, make use of the predefined macro's for
 * easy access to the method defined below.
 */
#if (cfgUseTimeout == cfgTrue)
  #if (cfgUseSynchronization == cfgSyncDoubleBlock)
    Tbool taskSyncRequest(Tuint08 uiSlotSlot, Tsint08 siFreeLeftFilling, Tsint08 siFreeRightFilling, Tuint16 uiTicksToWait) defSysWrapperEpilogue;
  #else
    Tbool taskSyncRequest(Tuint08 uiSlotSlot, Tsint08 siFreeRightFilling, Tuint16 uiTicksToWait) defSysWrapperEpilogue;
  #endif
#else
  #if (cfgUseSynchronization == cfgSyncDoubleBlock)
    void taskSyncRequest(Tuint08 uiSlotSlot, Tsint08 siFreeLeftFilling, Tsint08 siFreeRightFilling) defSysWrapperEpilogue;
  #else
    void taskSyncRequest(Tuint08 uiSlotSlot, Tsint08 siFreeRightFilling) defSysWrapperEpilogue;
  #endif
#endif


/** <!--0325-->
 * Release a queue, on a mutex or both.
 *
 * After you have taken a queue, mutex or both, you must release them with this
 * call, to unblock other tasks. There is no obligation to release slots which
 * are taken simultaneously in the same manner, but it is allowed. Also, you may
 * use this function to release more than one slot, even if they where not
 * taken simultaneously. This may be used to influence the way other tasks
 * are deblocked. For clarity however, it is best to release in the same manner
 * as the slots where locked.
 */
void taskSyncRelease(Tuint08 uiSlotSlot) defSysWrapperEpilogue;


/**
 * Try to obtain a lock on a queue for n bytes or on a mutex on a give name.
 *
 * These macro define the different ways that locks on queues and mutexes can
 * be defined. At the same time they clarify the reading and writing of
 * queue's. See more at taskSyncRequest(). The macros containing the type of
 * slot contain code which check the type of the slot, if cfgCheckMethodUse
 * is activated. So preferable use these. When making use of double slots
 * the distinction between mutex and queue cannot be made.
 */
#if (cfgUseTimeout == cfgTrue)
  #if (cfgUseSynchronization == cfgSyncDoubleBlock)
    /** <!--0320-->
     * Call to take possession of a queue, mutex or both.
     *
     * Call this if you want to take possession of two queue's, mutexes or
     * one queue and one mutex. Use the (literal)
     * slotnames. You must specify the number of bytes you want to read or
     * write for queues, which can be anything from 0 up to 127, boundaries included.
     * For mutexes specify 0 for the free filling.
     * The task blocks until both resources are available.
     *
     * You must specify a timeout, if cfgUseTimeout is
     * activated. You can specify anything from 2 to defDelayTimeMax. If you need
     * an infinite wait, specify defLockBlockInfinite, and if you want an immediate
     * return specify defLockDoNotBlock, to make your intentions clear.
     * The method returns true or false.
     * True means that the lock has been obtained, false means a timeout.
     */
    #define taskSyncWriteWriteRequestOnName(SlotLeftName,SlotRightName,FreeLeftFilling,FreeRightFilling,TicksToWait) taskSyncRequest((((preSlotNumberOf(SlotLeftName))<<4)|((preSlotNumberOf(SlotRightName)))),(-(FreeLeftFilling)),(-(FreeRightFilling)),TicksToWait)
    #define taskSyncWriteReadRequestOnName(SlotLeftName,SlotRightName,FreeLeftFilling,FreeRightFilling,TicksToWait)  taskSyncRequest((((preSlotNumberOf(SlotLeftName))<<4)|((preSlotNumberOf(SlotRightName)))),(-(FreeLeftFilling)),(FreeRightFilling),TicksToWait)
    #define taskSyncReadWriteRequestOnName(SlotLeftName,SlotRightName,FreeLeftFilling,FreeRightFilling,TicksToWait)  taskSyncRequest((((preSlotNumberOf(SlotLeftName))<<4)|((preSlotNumberOf(SlotRightName)))),(FreeLeftFilling),(-(FreeRightFilling)),TicksToWait)
    #define taskSyncReadReadRequestOnName(SlotLeftName,SlotRightName,FreeLeftFilling,FreeRightFilling,TicksToWait)   taskSyncRequest((((preSlotNumberOf(SlotLeftName))<<4)|((preSlotNumberOf(SlotRightName)))),(FreeLeftFilling),(FreeRightFilling),TicksToWait)
    /** <!--0319-->
     * Call to take possession of a queue on name.
     *
     * Call this if you want to take possession of a queue. Use the (literal)
     * slot name. You must specify the number of bytes you want to read or
     * write, which can be anything from 0 up to 127, boundaries included.
     * The task blocks until the queue is available.
     *
     * You must specify a timeout, if cfgUseTimeout is
     * activated. You can specify anything from 2 to defDelayTimeMax. If you need
     * an infinite wait, specify defLockBlockInfinite, and if you want an immediate
     * return specify defLockDoNotBlock, to make your intentions clear.
     * The method returns true or false.
     * True means that the lock has been obtained, false means a timeout.
     */
    #define taskQueuRequestOnName(SlotName,TicksToWait)                                                              taskSyncRequest(PassIfSlotIsQueu(preSlotNumberOf(SlotName)),0,0,TicksToWait)
    #define taskQueuWriteRequestOnName(SlotName,FreeFilling,TicksToWait)                                             taskSyncRequest(PassIfSlotIsQueu(preSlotNumberOf(SlotName)),0,(-(FreeFilling)),TicksToWait)
    #define taskQueuReadRequestOnName(SlotName,FreeFilling,TicksToWait)                                              taskSyncRequest(PassIfSlotIsQueu(preSlotNumberOf(SlotName)),0,(FreeFilling),TicksToWait)
    /** <!--0318-->
     * Call to take possession of a mutex on name.
     *
     * Call this if you want to take possession of a mutex. Use the (literal)
     * slot name. The task blocks until the mutex is available.
     *
     * You must specify a timeout, if cfgUseTimeout is
     * activated. You can specify anything from 2 to defDelayTimeMax. If you need
     * an infinite wait, specify defLockBlockInfinite, and if you want an immediate
     * return specify defLockDoNotBlock, to make your intentions clear.
     * The method returns true or false.
     * True means that the lock has been obtained, false means a timeout.
     */
    #define taskMutexRequestOnName(SlotName,TicksToWait)                                                             taskSyncRequest(PassIfSlotIsMutex(preSlotNumberOf(SlotName)),0,0,TicksToWait)
  #else
    #define taskQueuRequestOnName(SlotName,TicksToWait)                                                              taskSyncRequest(PassIfSlotIsQueu(preSlotNumberOf(SlotName)),0,TicksToWait)
    #define taskQueuWriteRequestOnName(SlotName,FreeFilling,TicksToWait)                                             taskSyncRequest(PassIfSlotIsQueu(preSlotNumberOf(SlotName)),(-(FreeFilling)),TicksToWait)
    #define taskQueuReadRequestOnName(SlotName,FreeFilling,TicksToWait)                                              taskSyncRequest(PassIfSlotIsQueu(preSlotNumberOf(SlotName)),(FreeFilling),TicksToWait)
    #define taskMutexRequestOnName(SlotName,TicksToWait)                                                             taskSyncRequest(PassIfSlotIsMutex(preSlotNumberOf(SlotName)),0,TicksToWait)
  #endif
#else
  #if (cfgUseSynchronization == cfgSyncDoubleBlock)
    #define taskSyncWriteWriteRequestOnName(SlotLeftName,SlotRightName,FreeLeftFilling,FreeRightFilling)             taskSyncRequest((((preSlotNumberOf(SlotLeftName))<<4)|((preSlotNumberOf(SlotRightName)))),(-(FreeLeftFilling)),(-(FreeRightFilling)))
    #define taskSyncWriteReadRequestOnName(SlotLeftName,SlotRightName,FreeLeftFilling,FreeRightFilling)              taskSyncRequest((((preSlotNumberOf(SlotLeftName))<<4)|((preSlotNumberOf(SlotRightName)))),(-(FreeLeftFilling)),(FreeRightFilling))
    #define taskSyncReadWriteRequestOnName(SlotLeftName,SlotRightName,FreeLeftFilling,FreeRightFilling)              taskSyncRequest((((preSlotNumberOf(SlotLeftName))<<4)|((preSlotNumberOf(SlotRightName)))),(FreeLeftFilling),(-(FreeRightFilling)))
    #define taskSyncReadReadRequestOnName(SlotLeftName,SlotRightName,FreeLeftFilling,FreeRightFilling)               taskSyncRequest((((preSlotNumberOf(SlotLeftName))<<4)|((preSlotNumberOf(SlotRightName)))),(FreeLeftFilling),(FreeRightFilling))
    #define taskQueuRequestOnName(SlotName)                                                                          taskSyncRequest(PassIfSlotIsQueu(preSlotNumberOf(SlotName)),0,0)
    #define taskQueuWriteRequestOnName(SlotName,FreeFilling)                                                         taskSyncRequest(PassIfSlotIsQueu(preSlotNumberOf(SlotName)),0,(-(FreeFilling)))
    #define taskQueuReadRequestOnName(SlotName,FreeFilling)                                                          taskSyncRequest(PassIfSlotIsQueu(preSlotNumberOf(SlotName)),0,(FreeFilling))
    #define taskMutexRequestOnName(SlotName)                                                                         taskSyncRequest(PassIfSlotIsMutex(preSlotNumberOf(SlotName)),0,0)
  #else
    #define taskQueuRequestOnName(SlotName)                                                                          taskSyncRequest(PassIfSlotIsQueu(preSlotNumberOf(SlotName)),0)
    #define taskQueuWriteRequestOnName(SlotName,FreeFilling)                                                         taskSyncRequest(PassIfSlotIsQueu(preSlotNumberOf(SlotName)),(-(FreeFilling)))
    #define taskQueuReadRequestOnName(SlotName,FreeFilling)                                                          taskSyncRequest(PassIfSlotIsQueu(preSlotNumberOf(SlotName)),(FreeFilling))
    #define taskMutexRequestOnName(SlotName)                                                                         taskSyncRequest(PassIfSlotIsMutex(preSlotNumberOf(SlotName)),0)
  #endif
#endif

/** <!--0326-->
 * Release the lock on a queue or mutex, by name.
 *
 * These macro make it possible to release locks on the basis of the name.
 * If other tasks had a lock on the queue the system checks all task with
 * respect to their lock condition. The first task that meets the requirement
 * will be released. Scanning is done in task number order, not in priority order.
 * The macros containing the type of slot contain code which check the type of the
 * slot, if cfgCheckMethodUse is activated. So preferable use these. When making
 * use of double slots the distinction between mutex and queue cannot be made.
 */
#if (cfgUseSynchronization == cfgSyncDoubleBlock)
  #define taskSyncReleaseOnName(SlotLeftName,SlotRightName)   taskSyncRelease((((preSlotNumberOf(SlotLeftName))<<4)|((preSlotNumberOf(SlotRightName)))))
#endif

/** <!--0321-->
 * Release the lock on a queue, by name
 *
 * These macro make it possible to release a lock on a queue on the basis of the name.
 * It performs the same action as taskSyncRelease() but an extra check is included
 * if the slot indeed holds a queue.
 */
#define taskQueuReleaseOnName(SlotName)                       taskSyncRelease(PassIfSlotIsQueu(preSlotNumberOf(SlotName)))

/** <!--0322-->
 * Release the lock on a mutex, by name
 *
 * These macro make it possible to release a lock on a mutex on the basis of the name.
 * It performs the same action as taskSyncRelease() but an extra check is included
 * if the slot indeed holds a mutex.
 */
#define taskMutexReleaseOnName(SlotName)                      taskSyncRelease(PassIfSlotIsMutex(preSlotNumberOf(SlotName)))

/** <!--0309-->
 * Write a byte on the queue.
 *
 * Call this method if you want to write one byte to the queue. The execution
 * takes place in task space, and may require some stackspace. Before you
 * write a byte please first obtain a lock on the queue, or be sure you are
 * the only one writing/reading (in an isr you cannot obtain a lock). The
 * read/write methods are tick/interrupt protected. Thus it is possible to
 * use the queue mechanism without the request/release methods, if you take
 * care of locking and counting of the bytes yourself. This is particularly
 * handy when working with interrupts A queue may filled in the interrupt
 * and read in a task.
 * If you write on a full queue the new bytes are disposed and not placed
 * on the queue.
 */
void genQueuWrite(Tuint08 uiSlot, Tbyte bItem);
#define genQueuWriteOnName(SlotName,bItem) genQueuWrite(preSlotNumberOf(SlotName),bItem)

/** <!--0312-->
 * Read a byte from the queue.
 *
 * Call this method if you want to read one byte from the queue. The execution
 * takes place in task space, and may require some stack space. Before you
 * write a byte please first obtain a lock on the queue, or be sure you are
 * the only one writing/reading (in an isr you cannot obtain a lock). The
 * read/write methods are tick/interrupt protected. Thus it is possible to
 * use the queue mechanism without the request/release methods, if you take
 * care of locking and counting of the bytes yourself. This is particularly
 * handy when working with interrupts. A queue may filled in the interrupt
 * and read in a task.
 * If you read from an empty queue the returned value is 0.
 */
Tbyte genQueuRead(Tuint08 uiSlot);
#define genQueuReadOnName(SlotName) genQueuRead(preSlotNumberOf(SlotName))

/** <!--0327-->
 * Unwrite (remove) bytes from the queue.
 *
 * Call this method if you want to undo the last write operations. The execution
 * takes place in task space, and may require some stack space. Normally you
 * use this directly after several genQueuWrite(), thus inside the same locked
 * environment, to correct erroneous write operations. If done outside the
 * lock, be sure you know what bytes you are removing. You may try to unwrite
 * more bytes as present, this is not an error, and will result all the removal
 * of all bytes written.
 *
 * Note that this action does not restore the original values (from before the
 * write) of the queue. So if this call is followed by an genQueuUnread(),
 * those bytes will not magically return.
 *
 * Typical use is after a communication when it turns out for example that
 * the crc of the bytes received is incorrect.
 */
void genQueuUnwrite(Tuint08 uiSlot, Tuint08 uiUnwriteSize);
#define genQueuUnwriteOnName(SlotName, UnwriteSize) genQueuUnread(preSlotNumberOf(SlotName),UnwriteSize)

/** <!--0328-->
 * Unread (remove) bytes from the queue.
 *
 * Call this method if you want to undo the last read operations. The execution
 * takes place in task space, and may require some stack space. Normally you
 * use this directly after several genQueuRead(), thus inside the same locked
 * environment, to correct erroneous read operations. If done outside the
 * lock, be sure you know what bytes you are removing. You may try to unread
 * more bytes as present, this is not an error, and will result all the removal
 * of all bytes read.
 *
 * After an unread the bytes my be read again, and since the values in the queue
 * are not altered, the same values will reappear.
 *
 * Typical use is after a communication when it turns out for example that
 * the crc of the bytes received is incorrect.
 */
void genQueuUnread(Tuint08 uiSlot, Tuint08 uiUnreadSize);
#define genQueuUnreadOnName(SlotName, UnreadSize) genQueuUnread(preSlotNumberOf(SlotName),UnreadSize)


/** <!--0301-->
 * Clear the queue.
 *
 * Call this to clear the queue. The remaining bytes are not removed,
 * but the read and write pointers are simply reset. Be careful with
 * clearing queues with write blocks on it, for those tasks may block
 * indefinitely (if no timeout has been specified), even if cleaning
 * means that room has become available to write some bytes.
 */
void genQueuClear(Tuint08 uiSlot);
#define genQueuClearOnName(SlotName) genQueuClear(preSlotNumberOf(SlotName))

/** <!--0307-->
 * Look ahead what the next byte to read would be.
 *
 * Instead of reading the byte from the queue, sometimes you want to know
 * which byte is coming without removing that byte. Call this method
 * to peek into the queue. If the queue is empty, it returns 0.
 */
Tbyte genQueuPeek(Tuint08 uiSlot);
#define genQueuPeekOnName(SlotName) genQueuPeek(preSlotNumberOf(SlotName))

/** <!--0315-->
 * See how many bytes can be read from the queue.
 *
 * This method return how may bytes can be read to the queue. This is
 * really a temporarily value, but since you know which tasks read/write
 * on the queue, you can judge how stable it is.
 */
Tuint08 genQueuReadable(Tuint08 uiSlot);
#define genQueuReadableOnName(SlotName) genQueuReadable(preSlotNumberOf(SlotName))

/** <!--0311-->
 * See how many bytes can be written to the queue.
 *
 * This method return how may bytes can be written to the queue. This is
 * really a temporarily value, but since you know which tasks read/write
 * on the queue, you can judge how stable it is.
 */
Tuint08 genQueuWriteable(Tuint08 uiSlot);
#define genQueuWriteableOnName(SlotName) genQueuWriteable(preSlotNumberOf(SlotName))

/** <!--0305-->
 * See if the queue is full.
 *
 * A quick method to see if the queue is full. Interrupts are not blocked
 * inside since the information is obtained in a single atomic read operation.
 */
Tbool genQueuFull(Tuint08 uiSlot);
#define genQueuFullOnName(SlotName) genQueuFull(preSlotNumberOf(SlotName))

/** <!--0303-->
 * See if the queue is empty.
 *
 * A quick method to see if the queue is empty. Interrupts are blocked
 * only shortly inside.
 */
Tbool genQueuEmpty(Tuint08 uiSlot);
#define genQueuEmptyOnName(SlotName) genQueuEmpty(preSlotNumberOf(SlotName))

/** <!--0501-->
 * Open the file system for writing and/or reading.
 *
 * Before you can read or write a file you must request access to the file
 * system. If you need to read only, you can specify so, thereby allowing other
 * tasks read access to the file system too. If read only is false (or absent)
 * you have private access to the file system, for reading and writing. As
 * with other synchronization primitives you may specify a timeout.
 *
 * The function returns true when access
 * has been granted. If you need to use the portReadByte() or portWriteByte()
 * directly, call taskFileOpen() first and then make your calls. Note that, due to
 * the nature of eeprom, fileOpen() works as access protection for the whole
 * file system, and not per file. So you may make use of several files
 * simultaneously within the realm of taskFileOpen() - taskFileClose().
 * taskFileOpen() calls may not be nested.
 */
#if (cfgUseTimeout == cfgTrue)
  #if (cfgUseFileSystemConcurrentRead == cfgTrue)
    Tbool taskFileOpen(Tbool bReadOnly, Tuint16 uiTicksToWait) defSysWrapperEpilogue;
  #else
    Tbool taskFileOpen(Tuint16 uiTicksToWait) defSysWrapperEpilogue;
  #endif
#else
  #if (cfgUseFileSystemConcurrentRead == cfgTrue)
    void taskFileOpen(Tbool bReadOnly) defSysWrapperEpilogue;
  #else
    void taskFileOpen(void) defSysWrapperEpilogue;
  #endif
#endif

/** <!--0502-->
 * Close the file system, so other tasks may obtain access.
 *
 * Every call taskFileOpen() must be closed with a call to taskFileClose(). This signals
 * the Femto OS that you are ready read/writing. It is an error to call taskFileClose() if
 * the file access has not been opened before. If other tasks are waiting to obtain
 * file access, calling taskFileClose() may immediately release such blocked tasks,
 * possibly starving the current task if it runs in lower priority.
 */
void taskFileClose(void) defSysWrapperEpilogue;

/** <!--0503-->
 * Give the file a new size, contents are unchanged.
 *
 * If cfgUseFileSystemMaintainFAT is activated the size of each file is maintained
 * by the Femto OS. It is however possible to change the size using this call. If
 * the file is extended in this way, the new bytes will not be cleaned. Using the
 * raw access taskWriteByte() these may be filled or cleaned on beforehand. It is
 * an error to increase the size beyond the space reserved for the file at compile
 * time. Only use after taskFileOpen().
 */
void taskFileSetSize(Tuint08 uiFileNumber, Tuint08 uiSize);
#define taskFileSetSizeOnName(FileName,Size)  taskFileSetSize(preFileNumberOf(FileName),Size)

/** <!--0516-->
 * Clear the file by setting its size to zero, contents are unchanged.
 *
 * If cfgUseFileSystemMaintainFAT is activated the size of each file is maintained
 * by the Femto OS. It is however possible to change the size to zero using this call.
 * Setting the size to zero does not really remove the file (the bytes stay present),
 * Only use after taskFileOpen().
 */
#define taskFileClear(FileNumber)             taskFileSetSize(FileNumber,0)
#define taskFileClearOnName(FileName)         taskFileSetSize(preFileNumberOf(FileName),0)

/** <!--0504-->
 * Get the present size of a file.
 *
 * If cfgUseFileSystemMaintainFAT is activated the size of each file is maintained
 * by the Femto OS. Using the call you can obtain the size of the file.
 * Only use after taskFileOpen().
 */
Tuint08 taskFileGetSize(Tuint08 uiFileNumber);
#define taskFileGetSizeOnName(FileName) taskFileSize(preFileNumberOf(FileName))

/** <!--0505-->
 * Clean (fill with zero's) the whole disk space.
 *
 * If you need to clean the dis kspace call taskFileFormat. The space used for
 * files (and only that part) is filled with zero's. Since the FAT is situated
 * at the start of this region, all files are set to length zero. If you set the
 * size of these files to other values afterwards, you can be sure that file
 * is filled with 0x00.
 * There is no need to clean format the file system before use, but you must be
 * ensure file system integrity yourself before first use.
 * Only use after taskFileOpen().
 */
void taskFileFormat(void);

/** <!--0507-->
 * Get one byte from a file using a raw read.
 *
 * Call taskFileReadByte() if you need one byte from a file on a random location.
 * It is an error (but possible if checks are switched off) to read beyond the
 * length of the file (if cfgUseFileSystemMaintainFAT is activated) or beyond
 * the space boundary of the file.
 * Only use after taskFileOpen().
 */
Tbyte taskFileReadByte(Tuint08 uiFileNumber, Tuint08 uiOffset);
#define taskFileReadByteOnName(FileName,Offset) taskFileReadByte(preFileNumberOf(FileName),Offset)

/** <!--0508-->
 * Write one byte to a file using a raw write.
 *
 * Call taskFileWriteByte() if you need to write one byte to a file on a random location.
 * The write is raw in the sense that the fileSize is not maintained. You may write bytes
 * beyond the file length, but not beyond the space boundary. For example, you may write,
 * zero's to a file first and afterwards set its length to the desired value.
 * Only use after taskFileOpen().
 */
void taskFileWriteByte(Tuint08 uiFileNumber, Tuint08 uiOffset, Tbyte bItem);
#define taskFileWriteByteOnName(FileName,Offset,Item) taskFileWriteByte(preFileNumberOf(FileName),Offset,Item)

/** <!--0509-->
 * Add one byte to the file, starting at the beginning when full.
 *
 * The call taskFileAppendByte() adds one byte to the file in a size maintained manner
 * (only available if cfgUseFileSystemMaintainFAT is activated). The byte is first written, and
 * subsequently the file size is increased. In case of power failure in between, it is as
 * if the file was never extended. If the file is full, the  byte is written at the
 * beginning, so that this call may be used to implement a ring buffer.  In the latter case,
 * the file size is set to zero first, then the byte is written and subsequently  the size
 * is set to one. Only use after taskFileOpen().
 */
void taskFileAppendByte(Tuint08 uiFileNumber, Tbyte bItem);
#define taskFileAppendByteOnName(FileName,Item) taskFileAppendByte(preFileNumberOf(FileName),Item)

/** <!--0510-->
 * Read several bytes from the file and push them to the pipe.
 *
 * Call this method to read one or more bytes (uiSize) from position uiOffset and push them
 * on the pipe. You can use defFromFileStart for the uiOffset parameter. Furthermore, you
 * can use (if cfgUseFileSystemMaintainFAT is activated) defUntilFileEnd for uiSize. Note that,
 * depending on your design of course, you should not assume a file is present. If there was a
 * power failure at writing, the file length may be set to zero. If the length of the file does
 * not equal zero, you may safely assume writing has been competed, before any power interruption
 * took place.  It is an error to read beyond the file size. Only use after taskFileOpen().
 */
void taskFileReadPipe(Tuint08 uiFileNumber, Tuint08 uiOffset, Tuint08 uiSize, void (*pipe)(Tchar));
#define taskFileReadOnName(FileName,Offset,Size,pipe) taskFileRead(preFileNumberOf(FileName),Offset,Size,pipe)

/** <!--0511-->
 * Read several bytes from the pipe and put the on file.
 *
 * Call this method to read one or more bytes (uiSize) from the pipe and write them to the file, starting
 * at the position uiOffset. You can use defFromFileStart or, if cfgUseFileSystemMaintainFAT is activated,
 * defFromFileEnd for the uiOffset parameter. It is an error to write beyond the file space.
 * Before any writing is done, the size of the file is set to zero. After the writing has been completed,
 * the new size is written to the FAT (when present). The procedure is to ensure file system integrity
 * at power failure. Only use after taskFileOpen().
 */
void taskFileWritePipe(Tuint08 uiFileNumber, Tuint08 uiOffset, Tuint08 uiSize, Tchar (*pipe)(void));
#define taskFileWriteOnName(FileName,Offset,Size,pipe) taskFileWrite(preFileNumberOf(FileName),Offset,Size,pipe)

/** <!--0512-->
 * Read several bytes from the file and push them to the buffer.
 *
 * Call this method to read one or more bytes (uiSize) from position uiOffset and write them
 * in the buffer. You can use defFromFileStart for the uiOffset parameter. Furthermore, you
 * can use (if cfgUseFileSystemMaintainFAT is activated) defUntilFileEnd for uiSize. Note that,
 * depending on your design of course, you should not assume a file is present. If there was a
 * power failure at writing, the file length may be set to zero. If the length of the file does
 * not equal zero, you may safely assume writing has been competed, before any power interruption
 * took place.  It is an error to read beyond the file size. Only use after taskFileOpen().
 */
void taskFileReadBuffer(Tuint08 uiFileNumber, Tuint08 uiOffset, Tuint08 uiSize, Taddress pBuffer);
#define taskFileReadBufferOnName(FileName,Offset,Size,Buffer)  taskFileReadBuffer(preFileNumberOf(FileName),Offset,Size,Buffer)

/** <!--0513-->
 * Read several bytes from the buffer and put the on file.
 *
 * Call this method to read one or more bytes (uiSize) from the buffer and write them to the file, starting
 * at the position uiOffset. You can use defFromFileStart or, if cfgUseFileSystemMaintainFAT is activated,
 * defFromFileEnd for the uiOffset parameter. It is an error to write beyond the file space.
 * Before any writing is done, the size of the file is set to zero. After the writing has been completed,
 * the new size is written to the FAT (when present). The procedure is to ensure file system integrity
 * at power failure. Only use after taskFileOpen().
 */
void taskFileWriteBuffer(Tuint08 uiFileNumber, Tuint08 uiOffset, Tuint08 uiSize, Taddress pBuffer);
#define taskFileWriteBufferOnName(FileName,Offset,Size,Buffer)  taskFileWriteBuffer(preFileNumberOf(FileName),Offset,Size,Buffer)

/** <!--0514-->
 * Direct read function on the file system.
 *
 * Sometimes it is convenient to have direct access to the file system. Instead of using the
 * portFSReadByte, use this macro instead. It returns the value at the given address on the
 * file system. Make sure you only call this if you are certain you have read access.
 * For normal file operations use taskFileReadByte() or taskFileReadBuffer() instead.
 */
#define taskFileDirectRead(Address) portFSReadByte(Address)

/** <!--0515-->
 * Direct write function on the file system.
 *
 * Sometimes it is convenient to have direct access to the file system. Instead of using the
 * portFSWriteByte, use this macro instead. It stores the value at the given address on the
 * file system. Make sure you only call this if you are certain you have write access.
 * For normal file operations use taskFileWriteByte() or taskFileWriteBuffer() instead.
 */
#define taskFileDirectWrite(Address, Value) portFSWriteByte(Address,Value)


#if (includeTaskProtectGlobalInterrupts == cfgTrue) || ((includeTaskProtectGlobalCritical == cfgTrue) && (cfgUseNestedCriticals == cfgFalse) )

  #if (cfgCheckTrace == cfgTrue)
    /** <!--0412-->
     * Enable global interrupts, trace when needed
     *
     * taskEnable/DisableGlobalInterrupts makes it possible to set/clear the global
     * interrupt flag. Don't call portEnable, but call taskEnable to make you code
     * general. Note these calls cannot be nested, i.e. they are absolute and
     * work instantly. If cfgCheckTrace is switched on, the call is traced.
     * Never call Enable/Disable interrupts inside critical sections.
     */
    #define taskEnableGlobalInterrupts()     do { portTrace(traceAPIcallBase | callIdTaskEnableGlobalInterrupts); portEnableGlobalInterrupts(); } while(false)


    /** <!--0410-->
     * Disable global interrupts, trace when needed
     *
     * taskEnable/DisableGlobalInterrupts makes it possible to set/clear the global
     * interrupt flag. Don't call portEnable, but call taskEnable to make you code
     * general. Note these calls cannot be nested, i.e. they are absolute and
     * work instantly. If cfgCheckTrace is switched on, the call is traced.
     * Never call Enable/Disable interrupts inside critical sections.
     */
    #define taskDisableGlobalInterrupts()    do { portDisableGlobalInterrupts(); portTrace(traceAPIcallBase | callIdTaskDisableGlobalInterrupts); } while(false)

  #else

    /**
     * Define for enable tick interrupts, see elsewhere for description.
     */
    #define taskEnableGlobalInterrupts()     portEnableGlobalInterrupts()

    /**
     * Define for disable tick interrupts, see elsewhere for description.
     */
    #define taskDisableGlobalInterrupts()    portDisableGlobalInterrupts()

  #endif

#endif


#if (includeTaskProtectGlobalCritical == cfgTrue)

  #if (cfgUseNestedCriticals == cfgTrue)

    /** <!--0413-->
     * Enter global critical section
     *
     * Use taskEnter/ExitGlobalCritical if you need nested (de)activation
     * of the global interrupts. Nesting is only active when cfgUseNestedCriticals
     * is set, otherwise these calls are replaced by simple taskEnterGlobalCritical.
     * Thus, in fact you can always use this call, and decide later on on its
     * functionality. It is the most general way.
     * The same mechanism about interrupt tracking applies as with taskEnable/
     * DisableGlobalInterrupts.
     */
    void taskEnterGlobalCritical(void);

    /** <!--0414-->
     * Exit global critical section
     *
     * Use taskEnter/ExitGlobalCritical if you need nested (de)activation
     * of the global interrupts. Nesting is only active when cfgUseNestedCriticals
     * is set, otherwise these calls are replaced by simple taskExitGlobalCritical.
     * Thus, in fact you can always use this call, and decide later on on its
     * functionality. It is the most general way.
     * The same mechanism about interrupt tracking applies as with taskEnable/
     * DisableGlobalInterrupts.
     */
    void taskExitGlobalCritical(void);

  #else

    /**
     * Define for enter global interrupts, see function taskEnterGlobalCritical(void)
     */
    #define taskEnterGlobalCritical()        taskDisableGlobalInterrupts()

    /**
     * Define for exit global interrupts, see function taskEnterGlobalCritical(void)
     */
    #define taskExitGlobalCritical()         taskEnableGlobalInterrupts()

  #endif

#endif



#if (includeTaskProtectSwitchTasks == cfgTrue) || ((includeTaskProtectSwitchCritical == cfgTrue) && (cfgUseNestedCriticals == cfgFalse) )

  /** <!--0411-->
   * Enable task switching.
   *
   * Call this to unblock context switching. The task looses its dominant
   * position.  Task switching is restored, and high priority tasks
   * are rescheduled. Call after a taskDisableSwitchTask().
   *
   * Do not call this if you make use of Enter/Exit Switch critical sections.
   * since it disturbs the nesting counting. In fact, it is even better to
   * use taskEnterSwitchCritical() and taskExitSwitchCritical(), even if you
   * do not need nesting. The system then translates this to taskEnableSwitchTask
   * or taskDisableSwitchTask.
   */
  void taskEnableSwitchTask(void);

  /** <!--0409-->
   * Disable task switching.
   *
   * Call this to block context switching. The task becomes dominant. The tick
   * interrupts keep coming, and the tick counter keeps increasing and delayed task
   * wakeup etcetera. Only thing is, there are no task switches, thus by
   * using this method a low priority task can take (keep it temporarily)
   * full processor time, and will behave like a top priority task.
   *
   * The system will prevent switching while possible. Thus, your task will
   * keep complete load as long as you do not block, call sleep, suspend or
   * terminate your task. Calling delay is possible, and the system will
   * go idle during the delay, but will not start an other task. If it is not
   * possible to maintain your task for whatever reason (for example an isr
   * calls suspend on the task) an other task will run, but as soon as the
   * the original task is revived it will regain control when it was sleeping
   * or suspended. When you call a blocking function, it will lose the block,
   * which can be seen as an error. A dominant state may not be restarted
   * either.
   *
   * If a switching interrupt takes place during the time one of the tasks
   * is dominant, context will be saves and the interrupt is handled. Afterwards
   * the dominant task is restarted, even if, for example, the isr revived a
   * task with higher priority.
   *
   * If you call this on a task with shared stack, the task will become
   * dominant as long as the shared task is active.  Cancel the dominant
   * state before you restart. If you all the tasks have a shared stack,
   * the dominant state is without meaning and should not be used.
   *
   * Regarding file system operations the following applies. Opening a file
   * cannot be done while in dominant mode, since other tasks may hold a
   * write lock on the file system so it may block. If only one task is using
   * the file system (defined through the capabilities), blocking on file open
   * cannot occur, and opening a file in dominant mode is allowed.
   * After opening, you may switch on the dominant mode, for all subsequent
   * file operations. All possible write blocks on the file system are handled
   * by spin locks, since their release do not depend on other tasks to act.
   *
   * Do not call this if you make use of Enter/Exit Switch critical sections.
   * since it disturbs the nesting counting. In fact, it is even better to
   * use taskEnterSwitchCritical() and taskExitSwitchCritical(), even if you
   * do not need nesting. The system then translates this to taskEnableSwitchTask
   * or taskDisableSwitchTask.
   */
  void taskDisableSwitchTask(void);

#endif


#if (includeTaskProtectSwitchCritical == cfgTrue)

  #if (cfgUseNestedCriticals == cfgTrue)

    /** <!--0415-->
     * Enter task switch critical section
     *
     * Calling taskEnterSwitchCritical makes the task dominant, and
     * can be called multiple times within the task.
     *
     * Use taskEnter/ExitSwitchCritical if you need nested (de)activation
     * of the task switches. Nesting is only active
     * when cfgUseNestedCriticals is set, otherwise these calls are replaced
     * by simple taskDisableSwitchTask() or taskEnableSwitchTask().
     * Thus, in fact you can always use this call, and decide later on on its
     * functionality. It is the most general way.
     *
     * Please note that if you violate the conditions under which the switch
     * critical section is issued (for example you require a mutex which may block)
     * you may loose the switch block, but the the nesting counting is not reset.
     * Subsequent exits of the critical section remain needed, although the block
     * may have been lost.
     */
    void taskEnterSwitchCritical(void);

    /** <!--0416-->
     * Exit task switch critical section
     *
     * Calling taskExitSwitchCritical() counteracts the corresponding enter
     * calls, and when the number of exit equals the number of enters,
     * the task looses its dominant position.
     *
     * Use taskEnter/ExitSwitchCritical if you need nested (de)activation
     * of the task switches. Nesting is only active when cfgUseNestedCriticals
     * is set, otherwise these calls are replaced by simple taskDisableSwitchTask()
     * or taskEnableSwitchTask().
     * Thus, in fact you can always use this call, and decide later on on its
     * functionality. It is the most general way.
     */
    void taskExitSwitchCritical(void);

  #else

    /**
     * Define for enter tick critical section, see function enter tick critical section(void)
     */
    #define taskEnterSwitchCritical()          taskDisableSwitchTask()

    /**
     * Define for exit tick critical section, see function taskExitTickCritical(void)
     */
    #define taskExitSwitchCritical()           taskEnableSwitchTask()

  #endif

#endif


#if (cfgIntTickTrack == cfgTrue) && ((includeTaskProtectTickInterrupts == cfgTrue) || ((includeTaskProtectTickCritical == cfgTrue) && (cfgUseNestedCriticals == cfgFalse) ))

  #if (cfgCheckTrace == cfgTrue)
    /** <!--0417-->
     * Enable Tick interrupts, trace when needed
     *
     * taskEnable/DisableTickInterrupts makes it possible to set/clear the Tick
     * interrupt flag. Don't call portEnable, but call taskEnable to make you code
     * general. Note these calls cannot be nested, i.e. they are absolute and
     * work instantly. If cfgCheckTrace is switched on, the call is traced.
     * Never call Enable/Disable interrupts inside critical sections.
     */
    #if (cfgIntGlobalOnly == cfgTrue)
      #define taskEnableTickInterrupts()     do { portTrace(traceAPIcallBase | callIdTaskEnableGlobalInterrupts); portEnableGlobalInterrupts(); } while(false)
    #else
      #define taskEnableTickInterrupts()     do { portTrace(traceAPIcallBase | callIdTaskEnableTickInterrupts); portEnableTickInterrupts(); } while(false)
    #endif


    /** <!--0418-->
     * Disable Tick interrupts, trace when needed
     *
     * taskEnable/DisableTickInterrupts makes it possible to set/clear the Tick
     * interrupt flag. Don't call portEnable, but call taskEnable to make you code
     * general. Note these calls cannot be nested, i.e. they are absolute and
     * work instantly. If cfgCheckTrace is switched on, the call is traced.
     * Never call Enable/Disable interrupts inside critical sections.
     */
    #if (cfgIntGlobalOnly == cfgTrue)
      #define taskDisableTickInterrupts()    do { portDisableGlobalInterrupts(); portTrace(traceAPIcallBase | callIdTaskDisableGlobalInterrupts); } while(false)
    #else
      #define taskDisableTickInterrupts()    do { portDisableTickInterrupts(); portTrace(traceAPIcallBase | callIdTaskDisableTickInterrupts); } while(false)
    #endif

  #else

    /**
     * Define for enable tick interrupts, see elsewhere for description.
     */
    #if (cfgIntGlobalOnly == cfgTrue)
      #define taskEnableTickInterrupts()     portEnableGlobalInterrupts()
    #else
      #define taskEnableTickInterrupts()     portEnableTickInterrupts()
    #endif

    /**
     * Define for disable tick interrupts, see elsewhere for description.
     */
    #if (cfgIntGlobalOnly == cfgTrue)
      #define taskDisableTickInterrupts()    portDisableGlobalInterrupts()
    #else
      #define taskDisableTickInterrupts()    portDisableTickInterrupts()
    #endif

  #endif

#endif



#if (cfgIntTickTrack == cfgTrue) && (includeTaskProtectTickCritical == cfgTrue)

  #if (cfgUseNestedCriticals == cfgTrue)

    #if (cfgIntGlobalOnly == cfgTrue)
      #define taskEnterTickCritical()      taskEnterGlobalCritical()
      void taskEnterGlobalCritical(void);
    #else
      /** <!--0419-->
       * Enter Tick critical section
       *
       * Use taskEnter/ExitTickCritical if you need nested (de)activation
       * of the Tick interrupts. Nesting is only active when cfgUseNestedCriticals
       * is set, otherwise these calls are replaced by simple taskEnterTickCritical.
       * Thus, in fact you can always use this call, and decide later on on its
       * functionality. It is the most general way.
       * The same mechanism about interrupt tracking applies as with taskEnable/
       * DisableTickInterrupts.
       */
      void taskEnterTickCritical(void);
    #endif

    #if (cfgIntGlobalOnly == cfgTrue)
      #define taskExitTickCritical()       taskExitGlobalCritical()
      void taskExitGlobalCritical(void);
    #else
      /** <!--0420-->
       * Exit Tick critical section
       *
       * Use taskEnter/ExitTickCritical if you need nested (de)activation
       * of the Tick interrupts. Nesting is only active when cfgUseNestedCriticals
       * is set, otherwise these calls are replaced by simple taskExitTickCritical.
       * Thus, in fact you can always use this call, and decide later on on its
       * functionality. It is the most general way.
       * The same mechanism about interrupt tracking applies as with taskEnable/
       * DisableTickInterrupts.
       */
      void taskExitTickCritical(void);
    #endif

  #else

    /**
     * Define for enter Tick interrupts, see function taskEnterTickCritical(void)
     */
    #define taskEnterTickCritical()        taskDisableTickInterrupts()

    /**
     * Define for exit Tick interrupts, see function taskEnterTickCritical(void)
     */
    #define taskExitTickCritical()         taskEnableTickInterrupts()

  #endif

#endif



#if (cfgIntUserDefined == cfgTrue)
  #if (cfgIntOsProtected == cfgTrue) && (cfgUseLowPowerSleep == cfgFalse) && (cfgIntTickTrack == cfgFalse)
    #if (includeIsrEnter == cfgTrue) && (cfgUseLoadMonitor == cfgTrue)
      /** <!--0404-->
       * Start the isr by a context switch.
       *
       * Use isrEnter() to start an isr with a context switch. The current task is
       * pushed to the context (so this is only available when the interrupts
       * comes when a task is running, i.e. cfgIntOsProtected must be set) and,
       * depending on the setting of cfgIntSwitchUsesOSstack work is continued
       * on the OS or ISR stack. Always close the isr with isrExit(). If selected
       * the time spend in the isr is measured automatically
       */
      #define isrEnter()                   \
        portResqueGlobalInterruptActive(); \
        portSaveContext();                 \
        privInitOsReturn();                \
        isrStartLoad();                    \
        portEnterISR()
    #elif (includeIsrEnter == cfgTrue)
      #define isrEnter()                   \
        portResqueGlobalInterruptActive(); \
        portSaveContext();                 \
        privInitOsReturn();                \
        portEnterISR()
    #endif

    #if (includeIsrExit == cfgTrue) && (cfgUseLoadMonitor == cfgTrue)
      /** <!--0405-->
       * End the isr, switch to the OS.
       *
       * Use isrEnter() to start an isr with a context switch. The current task is
       * pushed to the context (so this is only available when the interrupts
       * comes when a task is running, i.e. cfgIntOsProtected must be set) and,
       * depending on the setting of cfgIntSwitchUsesOSstack work is continued
       * on the OS or ISR stack. Always close the isr with isrExit().
       */
      #define isrExit()         \
        isrStopLoad();          \
        privIsrExit()
    #elif (includeIsrExit == cfgTrue)
      #define isrExit()         privIsrExit()
    #endif

  #endif


  #if (includeIsrBegin == cfgTrue)
    /** <!--0401-->
     * Start the isr, switch to the isr stack.
     *
     * isrBegin() is used when you want the interrupt to be serviced as quickly as possible.
     * No context is saved, so you are responsible for saving and restoring any registers
     * you use, including the status register. You can however let GCC take care of this.
     * Note that, if you use many registers, this may take more time than a context save.
     * Advantage is that you can interrupt the OS if you want, depending on the setting of
     * cfgIntOsProtected. If you want to measure the time spend inside the interrupt you must
     * manually call the irsStartLoad/isrStopLoad routines, and make sure you save the
     * registers utilized inside as well. Note that, in this case, the system is not
     * able to deduce the time spend in the isr from the time measurement for the
     * process (task/os/idle) that was running at the moment. So that time is counted twice.
     */
    #define isrBegin()         portBeginISR()
  #endif

  #if (includeIsrEndReturn == cfgTrue)
    /** <!--0402-->
     * Stop the isr, return to the place where the interrupt occurred.
     *
     * isrEndReturn(): use this to return from a isr to the place the interrupt occurred. It is
     * your only option is you have no OS protection. Restore anything you corrupted, this
     * is your own responsibility. The only thing the isrBegin() / isrReturn() pair takes
     * care of is the stack switch. So inside the isr you have the ISR stack.
     */
    #define isrEndReturn()     portReturnISR()
  #endif

  #if (cfgIntOsProtected == cfgTrue) && (includeIsrEndYield == cfgTrue) && (includeTaskYield == cfgTrue) && (cfgUseLowPowerSleep == cfgFalse)
    /** <!--0403-->
     * Stop the isr, yield as quickly as possible
     *
     * isrEndYield(): use this if you want a context switch directly after the handling of your
     * interrupt. You are still responsible for restoring any registers, so it only makes
     * sense to use isrBegin() / isrYield() over isrEnter() / isrExit() if your response
     * to the interrupt need to be very fast and does not require that many operations.
     * Since this method calls taskYield() that method must be included too.
     */
    #define isrEndYield()      portYieldISR()
  #endif

#endif

/** <!--0126-->
 * Call to check how much free space is left on the task stack.
 *
 * With this function you can check on any place in your code what would happen
 * if a context switch was made here. This function only works when cfgCheckTaskStack
 * set to cfgTrue. Otherwise the function is still defined, but returns 0, so
 * that it does need to be removed every time you switch the cfgCheckTaskStack setting.
 * With the parameter uiExtraStackSafety you can supply how much extra space you
 * require at this place. Use this to compensate for extra stack in calls in which
 * you cannot place this function. The uiExtraStackSafety comes onto the normal
 * StackSafety parameter. Thus the larger the uiExtraStackSafety the quicker an
 * (expected) error occurs.
 *
 * This function returns the amount of free (real) stack space at the moment of
 * calling, that would be left over, if an context save would happen right at this
 * place, apart from the safety parameters, but the stack required for
 * this call included. Thus, if it returns 0 you are living on the edge.
 * This function also operates correctly if the stack level and uiExtraStackSafety
 * together exceed the 255 byte limit.
 */
Tuint16 taskStackCheck(Tuint08 uiExtraStackSafety);

/** <!--0406-->
 * Call to check how much free space is left on the isr stack.
 *
 * With this function you call check the amount of free space in the isr stack.
 * (a context switch never happens here). This function only works when cfgCheckIsrStack
 * set to cfgTrue. Otherwise the function is still defined, but returns 0, so
 * that it does need to be removed every time you switch the cfgCheckIsrStack setting.
 * With the parameter uiExtraStackSafety you can supply how much extra space you
 * require at this place. Note that the total of present isr stack size and
 * uiExtraStackSafety should not exceed the 255 bytes.
 * This function returns the amount of free (real) isr stack space at the moment
 * of calling, apart from the safety parameters. Of course the 2 bytes as result
 * of the call are extra.
 *
 * This function does not operate when cfgIntSwitchUsesOSstack is set to true since
 * in that case the isr makes use of the OS stack.
 */
Tuint16 isrStackCheck(Tuint08 uiExtraStackSafety);


#if (cfgIntUserDefined == cfgTrue)
  #if ((cfgUseLoadMonitor == cfgTrue) && ((includeIsrEnter == cfgTrue) || (includeIsrStartLoad == cfgTrue)))
    /** <!--0407-->
     * Start measuring the time spend in an isr.
     *
     * IsrStartLoad measures the total time spend in interrupts. You only need to call it at the beginning
     * of your interrupt service routine if you took own responsibility of the context save, i.e. if
     * you came at the place with isrBegin. It starts the stopwatch for measuring the time inside an
     * isr. You can leave the call in you code when switching off cfgUseLoadMonitor, it will be replaced
     * by an empty statement.
     */
    void isrStartLoad(void);
  #else
    #define isrStartLoad()
  #endif
#endif

#if (cfgIntUserDefined == cfgTrue)
  #if ((cfgUseLoadMonitor == cfgTrue) && ((includeIsrExit == cfgTrue) || (includeIsrStopLoad == cfgTrue)))
    /** <!--0408-->
     * Stop measuring the time spend in an isr.
     *
     * IsrStopLoad measures the total time spend in interrupts. You only need to call it at the end
     * of your interrupt service routine if you took own responsibility of the context save, i.e. before
     * you leave with isrEndYield() or isrEndReturn().  It stops the stopwatch for measuring the time
     * inside an isr. You can leave the call in you code when switching off cfgUseLoadMonitor, it will be replaced
     * by an empty statement.
     */
    void isrStopLoad(void);
  #else
    #define isrStopLoad()
  #endif
#endif


/** <!--0805-->
 * Call to let the task wait for one specific event in the system.
 *
 * This function is used as a lightweight alternative to synchronization to
 * slots. There are 8 predefined events ready to use. If you call this function
 * the task blocks until the events has fired. Firing event is possible from
 * within an isr, even if the OS is set interruptible. Thus this can make you
 * application very responsive, while not being bound to handle all matters
 * inside an isr. Any task being able to wait for events must have the CapEvent
 * capability.
 *
 * Note, if you use a number, that should be the number of the event itself,
 * thus a number 0 ... 7 (borders included). It is easier to work with names though.
 */
#if (cfgUseTimeout == cfgTrue)
  #define taskWaitForEvent(Event,TicksToWait)            taskWaitForEventSet((1<<(Event)),TicksToWait)
  #define taskWaitForEventOnName(EventName,TicksToWait)  taskWaitForEventSet(preEventLocationOf(EventName),TicksToWait)
#else
  #define taskWaitForEvent(Event)                        taskWaitForEventSet((1<<(Event)))
  #define taskWaitForEventOnName(EventName)              taskWaitForEventSet(preEventLocationOf(EventName))
#endif


/** <!--0806-->
 * Call to let the task wait for more specific events in the system.
 *
 * This function is used as a lightweight alternative to synchronization to
 * slots. There are 8 predefined events ready to use. If you call this function
 * on several events, the task blocks until all of these events
 * have fired. Events are typically fired one at the time, so the task is
 * reactivated when the last event has fired. Firing event is possible from
 * within an isr, even if the OS is set interruptible. Thus this can make you
 * application very responsive, while not being bound to handle all matters
 * inside an isr. Any task being able to wait for events must have the CapEvent
 * capability.
 *
 * The uiEventSet is a binary pattern, the events are represented by a set
 * bit on their location. So if you want to wait on events 5 and 0 supply
 * the byte 0x21. Waiting for event set 0x00 results in a direct context
 * switch, since there is nothing to wait for. It is easier to work with
 * names though.
 */
#if (cfgUseTimeout == cfgTrue)
  Tbool taskWaitForEventSet(Tuint08 uiEventSet, Tuint16 uiTicksToWait) defSysWrapperEpilogue;
  #define taskWaitForEventSetOnNames2(Event0,Event1,TicksToWait)                                    taskWaitForEventSet(preEventLocationOf(Event0)|preEventLocationOf(Event1),TicksToWait)
  #define taskWaitForEventSetOnNames3(Event0,Event1,Event2,TicksToWait)                             taskWaitForEventSet(preEventLocationOf(Event0)|preEventLocationOf(Event1)|preEventLocationOf(Event2),TicksToWait)
  #define taskWaitForEventSetOnNames4(Event0,Event1,Event2,Event3,TicksToWait)                      taskWaitForEventSet(preEventLocationOf(Event0)|preEventLocationOf(Event1)|preEventLocationOf(Event2)|preEventLocationOf(Event3),TicksToWait)
  #define taskWaitForEventSetOnNames5(Event0,Event1,Event2,Event3,Event4,TicksToWait)               taskWaitForEventSet(preEventLocationOf(Event0)|preEventLocationOf(Event1)|preEventLocationOf(Event2)|preEventLocationOf(Event3)|preEventLocationOf(Event4),TicksToWait)
  #define taskWaitForEventSetOnNames6(Event0,Event1,Event2,Event3,Event4,Event5,TicksToWait)        taskWaitForEventSet(preEventLocationOf(Event0)|preEventLocationOf(Event1)|preEventLocationOf(Event2)|preEventLocationOf(Event3)|preEventLocationOf(Event4)|preEventLocationOf(Event5),TicksToWait)
  #define taskWaitForEventSetOnNames7(Event0,Event1,Event2,Event3,Event4,Event5,Event6,TicksToWait) taskWaitForEventSet(preEventLocationOf(Event0)|preEventLocationOf(Event1)|preEventLocationOf(Event2)|preEventLocationOf(Event3)|preEventLocationOf(Event4)|preEventLocationOf(Event5)|preEventLocationOf(Event6),TicksToWait)
  #define taskWaitForAllEvents(TicksToWait)                                                         taskWaitForEventSet(defAllEvents,TicksToWait)
#else
  void taskWaitForEventSet(Tuint08 uiEventSet) defSysWrapperEpilogue;
  #define taskWaitForEventSetOnNames2(Event0,Event1)                                    taskWaitForEventSet(preEventLocationOf(Event0)|preEventLocationOf(Event1))
  #define taskWaitForEventSetOnNames3(Event0,Event1,Event2)                             taskWaitForEventSet(preEventLocationOf(Event0)|preEventLocationOf(Event1)|preEventLocationOf(Event2))
  #define taskWaitForEventSetOnNames4(Event0,Event1,Event2,Event3)                      taskWaitForEventSet(preEventLocationOf(Event0)|preEventLocationOf(Event1)|preEventLocationOf(Event2)|preEventLocationOf(Event3))
  #define taskWaitForEventSetOnNames5(Event0,Event1,Event2,Event3,Event4)               taskWaitForEventSet(preEventLocationOf(Event0)|preEventLocationOf(Event1)|preEventLocationOf(Event2)|preEventLocationOf(Event3)|preEventLocationOf(Event4))
  #define taskWaitForEventSetOnNames6(Event0,Event1,Event2,Event3,Event4,Event5)        taskWaitForEventSet(preEventLocationOf(Event0)|preEventLocationOf(Event1)|preEventLocationOf(Event2)|preEventLocationOf(Event3)|preEventLocationOf(Event4)|preEventLocationOf(Event5))
  #define taskWaitForEventSetOnNames7(Event0,Event1,Event2,Event3,Event4,Event5,Event6) taskWaitForEventSet(preEventLocationOf(Event0)|preEventLocationOf(Event1)|preEventLocationOf(Event2)|preEventLocationOf(Event3)|preEventLocationOf(Event4)|preEventLocationOf(Event5)|preEventLocationOf(Event6))
  #define taskWaitForAllEvents()                                                        taskWaitForEventSet(defAllEvents)
#endif

/** <!--0801-->
 * Call this to release tasks waiting for an different events.
 *
 * Using this call you may fire any combination of events simultaneously. Note it
 * is a real function call and is not compiled to one atomic instruction. Call
 * genFireAllEvents() to release a blocking tasks at once.
 * The execution of this method takes place in a non interruptible realm.
 *
 * Merely firing events does not require the task to have event capabilities, it is
 * even more efficient when the task does not include the capabilities. Waiting for an
 * event however does.
 */
void genFireEventSet(Tuint08 uiEventSet);
#define genFireEventSetOnNames2(Event0,Event1)                                    genFireEventSet(preEventLocationOf(Event0)|preEventLocationOf(Event1))
#define genFireEventSetOnNames3(Event0,Event1,Event2)                             genFireEventSet(preEventLocationOf(Event0)|preEventLocationOf(Event1)|preEventLocationOf(Event2))
#define genFireEventSetOnNames4(Event0,Event1,Event2,Event3)                      genFireEventSet(preEventLocationOf(Event0)|preEventLocationOf(Event1)|preEventLocationOf(Event2)|preEventLocationOf(Event3))
#define genFireEventSetOnNames5(Event0,Event1,Event2,Event3,Event4)               genFireEventSet(preEventLocationOf(Event0)|preEventLocationOf(Event1)|preEventLocationOf(Event2)|preEventLocationOf(Event3)|preEventLocationOf(Event4))
#define genFireEventSetOnNames6(Event0,Event1,Event2,Event3,Event4,Event5)        genFireEventSet(preEventLocationOf(Event0)|preEventLocationOf(Event1)|preEventLocationOf(Event2)|preEventLocationOf(Event3)|preEventLocationOf(Event4)|preEventLocationOf(Event5))
#define genFireEventSetOnNames7(Event0,Event1,Event2,Event3,Event4,Event5,Event6) genFireEventSet(preEventLocationOf(Event0)|preEventLocationOf(Event1)|preEventLocationOf(Event2)|preEventLocationOf(Event3)|preEventLocationOf(Event4)|preEventLocationOf(Event5)|preEventLocationOf(Event6))
#define genFireAllEvents()                                                        genFireEventSet(defAllEvents)

/** <!--0804-->
 * Call to get the number of blocks on a combination of events.
 *
 * You may want to postpone firing an event until one or more tasks
 * are actually blocking on it. Call this to get the current number
 * of tasks blocking on all the events given. If it is called in isr
 * or interrupt protected environment the number is accurate. If not
 * it is a lower bound, when the calling task is the only one capable
 * of firing the particular event.
 */
Tuint08 genCountEventBlocks(Tuint08 uiEventSet);
#define genCountEventBlocksOnNames2(Event0,Event1)                                    genCountEventBlocks(preEventLocationOf(Event0)|preEventLocationOf(Event1))
#define genCountEventBlocksOnNames3(Event0,Event1,Event2)                             genCountEventBlocks(preEventLocationOf(Event0)|preEventLocationOf(Event1)|preEventLocationOf(Event2))
#define genCountEventBlocksOnNames4(Event0,Event1,Event2,Event3)                      genCountEventBlocks(preEventLocationOf(Event0)|preEventLocationOf(Event1)|preEventLocationOf(Event2)|preEventLocationOf(Event3))
#define genCountEventBlocksOnNames5(Event0,Event1,Event2,Event3,Event4)               genCountEventBlocks(preEventLocationOf(Event0)|preEventLocationOf(Event1)|preEventLocationOf(Event2)|preEventLocationOf(Event3)|preEventLocationOf(Event4))
#define genCountEventBlocksOnNames6(Event0,Event1,Event2,Event3,Event4,Event5)        genCountEventBlocks(preEventLocationOf(Event0)|preEventLocationOf(Event1)|preEventLocationOf(Event2)|preEventLocationOf(Event3)|preEventLocationOf(Event4)|preEventLocationOf(Event5))
#define genCountEventBlocksOnNames7(Event0,Event1,Event2,Event3,Event4,Event5,Event6) genCountEventBlocks(preEventLocationOf(Event0)|preEventLocationOf(Event1)|preEventLocationOf(Event2)|preEventLocationOf(Event3)|preEventLocationOf(Event4)|preEventLocationOf(Event5)|preEventLocationOf(Event6))
#define genCountEventBlocksOnAll()                                                    genCountEventBlocks(defAllEvents)


#if (cfgUseEvents == cfgTrue)
  /* Internal base definition for firing an event. */
  #define privFireEvent(Event)             portEventRegister |= (1 << (Event))

  #if (includeIsrFireEvent == cfgTrue)
    /**  <!--0803-->
     * Call this to fire an event from the isr.
     *
     * This method is not a regular method but a define, to be as quick as possible.
     * It is assumed to compiler is able to compile it into a single bit-write instruction
     * (avr). If this is not possible, be sure you only call it when interrupts are
     * disabled, usually from an isr. GCC is able to compile this into an atomic instruction
     * for the AVR provided you supply a constant, or if you use the isrFireEventOnName() call,
     * and the register devAuxEventReg is the lowest I/O section (0x00-0x1F).  So you should
     * not call it with a variable. If you need to fire more events, call this function several
     * times, once for each event. Note that this call is never traced.
     *
     * A specialty of this method is that it can be utilized on the AVR within naked signal
     * handlers without any register saving, provided it compiles to an atomic instruction.
     * This makes interrupt handling extremely fast. Just release some task and go on. The
     * interrupt handles does not need to contain more that two instructions.
     *
     * Merely firing an event does not require the task to have event capabilities, it is
     * even more efficient when the task does not include the capabilities. Waiting for an
     * event however does.
     *
     * If you obtain a "left shift count is negative" from the compiler upon your call
     * you have used an invalid event number, less than zero, bigger then 7 or equal to
     * the AuxRegBit of the Femto OS (the registers for events and the Femto OS auxiliary
     * register may be shared, but in that case you may not use the one bit Femto OS
     * needs). Please correct the situation and do not ignore it just because it is a
     * only warning.
     */
    #define isrFireEvent(Event)              privFireEvent(CheckEventNumber(Event))
    #define isrFireEventOnName(EventName)    isrFireEvent(preEventNumberOf(EventName))
  #endif


  #if (includeGenFireEvent == cfgTrue)
    #if (defEventRegisterAtomicOperation == cfgTrue) && (cfgCheckTrace == cfgFalse) && (cfgUseEventsOnVariables == cfgFalse)
      #define genFireEvent(Event)            privFireEvent(CheckEventNumber(Event))
    #else
      /**  <!--0802-->
       * Call this to fire an event from any place.
       *
       * This method is a regular method or a define, depending on circumstances. If
       * devAuxEventReg is the lowest I/O section (0x00-0x1F), and tracing is deactivated
       * and we make no use of cfgUseEventsOnVariables then this is identical to isrFireEvent.
       * Otherwise a special method is defined which disables global interrupts first,
       * and subsequently traces, checks or otherwise handles the call. It may be used
       * from any location, including an isr, but of course it can be a lot slower/larger.
       * When cfgUseEventsOnVariables is used, you may call isrFireEvent with a variable
       * as parameter.
       *
       * Merely firing an event does not require the task to have event capabilities, it is
       * even more efficient when the task does not include the capabilities. Waiting for an event
       * however does.
       *
       * If you obtain a "left shift count is negative" from the compiler upon your call
       * you have used an invalid event number, less than zero, bigger then 7 or equal to
       * the AuxRegBit of the Femto OS (the registers for events and the Femto OS auxiliary
       * register may be shared, but in that case you may not use the one bit Femto OS
       * needs). Please correct the situation and do not ignore it just because it is only
       * a warning.
       */
      void genFireEvent(Tuint08 uiEvent);
    #endif
    #define genFireEventOnName(EventName)    genFireEvent(preEventNumberOf(EventName))
  #endif

#endif

#endif /* FEMTOOS_CORE_H */

