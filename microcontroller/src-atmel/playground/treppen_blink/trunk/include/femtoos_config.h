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

#ifndef CONFIG_DEFAULT_H
#define CONFIG_DEFAULT_H

/* ========================================================================= */
/* SYSTEM CONFIGURATION ==================================================== */
/* ========================================================================= */

/**
 * This section is used to illustrate the use of the configuration parameters.
 * It is better not to use it in you application code. To that end you may
 * use the template defined below. Make a copy and fill it with you values.
 */

/** <!--1101-->
 * Switch between two sets of configuration parameters
 *
 * Demo applications come in two flavors: 'optimized' and 'not optimized'.
 * This is only about the configuration parameters, the code is identical.
 * Since it is likely the optimization done for a particular version of the
 * compiler and circumstances will lead to incorrect code for an other version,
 * we distribute the examples with optimization switched off. You can simply
 * test the optimized version for all examples by switching this to true.
 *
 * However, it is better to do the optimization per application by hand
 * (following the optimization instructions). Probably you will find slightly
 * different values for the registers used and the stack sizes. This
 * configuration parameter is, and should be, only used inside the
 * config_application header code.
 *
 * This option can be overridden from the command line by using defExtOptimized.
 * If that is set to cfgTrue or cfgFalse its value overrides the value defined
 * in your config file.
 */
#define  cfgSysOptimized                         cfgTrue

/** <!--1102-->
 * Used to debug on the Atmel Studio
 *
 * There is no standard way to debug the OS. The most usable form of
 * debugging i found is making use of a simulator. Some of the example
 * code contains values making is more easy to debug an application by
 * reducing wait times. The AVR simulator from Atmel is not able to
 * simulate the timers, so a trick is used. Note the code compiled with
 * debug == cfgTrue cannot be run on real hardware.
 */
#define  cfgSysDebug                             cfgFalse

/** <!--1103-->
 * Force inlining of some internal methods to reduce OS stack use.
 *
 * Some methods use quite a bit of stack, i.e. they start by saving
 * say 8 or more registers on the context. This implies not only 8 more bytes
 * of ram use, but also 32 extra flash bytes just to store and retrieve
 * the registers. This must be weighted against inlining those methods.
 * Less stack, more flash, but maybe not that much more. It really depends
 * which resource is limited. If you set inline to true you use less OS stack
 * (Typically noticeable when using watchdog, restart of queu functionality)
 * but more flash. Experiment to find the optimal setting. Note that changing
 * this to false may require the need to increase OSstack. Test!
 */
#define  cfgSysInline                            cfgFalse

/** <!--1104-->
 * To suppress the use of the frame pointer even further.
 *
 * Sometimes the GCC compiler uses a frame pointer although we explicitly
 * asked not to do so by -fomit-frame-pointer. It may be unavoidable at
 * times, but GCC is not perfect and sometimes misses he can use a free
 * register instead of a frame pointer. Frame pointers cost a lot of space
 * in terms of flash and ram. For small functions, the avr architecture
 * should normally not need one. If it does so, you can activate the
 * counter measures for the OS. This boils down to binding variables to
 * registers. It is not always successful, for the compiler might ignore
 * such requests.
 */
#define  cfgSysFramePointerCounterMeasures       cfgFalse

/** <!--1105-->
 * Manage the code inside the pro- and epiloge.
 *
 * A lot of functions do not need to save any registers at entrance since
 * they do not return but enter the OS system internal handling. That would
 * be a waste of code. To make all these functions naked, you can the parameter
 * cfgSysReduceProEpilogue to cfgNaked. This works wonderful as long as no
 * frame pointers are used. Normally they are not needed, the functions are
 * small and do not contain a lot of variables, there should be plenty of
 * registers free on the avr. (Of course you compile with -fomit-frame-pointer)
 *
 * However, GCC does sometimes still use a frame pointer, see the discussion on
 * the cfgSysFramePointerCounterMeasures parameter. In that case you should not use
 * naked functions, since naked functions do not contain frame pointer setup
 * code. In that case you have two options if cfgSysFramePointerCounterMeasures
 * do not eliminate the frame pointers. Set cfgSysReduceProEpilogue to cfgBikini
 * or set it to cfgComplete. The latter is always save, but may increase your code
 * dramatically for full pro- and epiloge code is used on all functions.
 *
 * Since gcc 4.3.2 cfgBikini maps onto dressing of the methods with OS_task, which
 * makes sure methods essentially naked, but still define a frame pointer when needed.
 * If you have a fully patched kernel, which recognizes the attribute OS_task, you
 * may safely put cfgBikini here.
 */
#define  cfgSysReduceProEpilogue                 cfgComplete


/** <!--1106-->
 * Specify the byte with which the registers are initially filled.
 *
 * If you make cfgCheckTaskStack == cfgTrue the registers are filled with the
 * value giving at cfgSysRegisterCheckByte. Each time a context switch
 * occurs, registers said not to be used in the RegisterUse by that task
 * are compared with the byte. In theory it is possible that a register
 * was used, but filled with the byte you selected. To be absolutely
 * sure, run your code several times with different values of the
 * cfgSysRegisterCheckByte.
 *
 * If you use tracing, it may be handy to use for this value 0xA5.
 * This value is not defined for regular trace notifications. Thus if
 * it appears in your tracing non the less, you know that a so called non
 * used register leaked trough to the first parameter register, probably
 * you missed some registers needed to be saved.
 *
 * If you want to measure which registers are used, (opposite to analyzing
 * the assembly) you can set this value to the same value as the stack
 * is initialized with (defStackInitByte, usually 0). Now, registers which
 * are context saved but not used cannot be separated from the non-saved
 * registers, so they do not appear it the watermark measurement (activate
 * it). So it gives an indication which registers could be removed from
 * context saving.
 *
 * This method has two drawbacks.
 * (1) Still there may be registers which are changed, but do not have to
 * be saved, since they are only saved in tick interrupt protected code.
 * (2) Some registers may actually be in use, but contain zero for a longer
 * period of time. This is can only be detected by changing defStackInitByte
 * to some other value, but at the risk of contaminating r1.
 */
#define  cfgSysRegisterCheckByte                 0xA5

/** <!--1107-->
 * Specify the direction the stack grows.
 *
 * Depending on the hardware the stack may run upwards (for example AVR or
 * ARM) or downwards. During the development of Femto OS i did not have
 * hardware with the stack running upwards, so the setting
 * cfgSysStackGrowthUp == cfgTrue does not work properly at this
 * moment.
 */
#define  cfgSysStackGrowthUp                     cfgFalse

/** <!--1113-->
 * Reduce the stack pointer when all stacks fit on page zero
 *
 * For very small devices the high byte stack pointer may not be needed. This is
 * comparable to the option -tinystack. However, is not limited to the RAM
 * being limited to 255 bytes. If all stacks present (that is OS stack,
 * ISR stack and all task stacks) fit on the first page of the RAM (note
 * that I/O is located at that place as well) AND the compiler is able to
 * organize the stacks directly after the I/O space, the high byte stack
 * pointer is safely ignored. Even if the actual ram is much larger.
 * Now, the preprocessor can sort out all off the conditions above, with
 * one exception, that is making sure the stacks are located directly after
 * I/O. However, you can be sure the latter is the case if the data section is
 * empty and if shared.o is linked first.  Check the file main.map to be certain.
 * In that case you may set cfgSysZeroPageStack to cfgTrue.
 * Note: eclipse with generated makefiles will not put shared.o first, so do
 * not set cfgSysZeroPageStack to true in that case. Instead use the external
 * switch to control the setting of this option: -DdefExtZeroPageStack=cfgTrue
 * To be on the save side: set this option to cfgFalse.
 */
#define  cfgSysZeroPageStack                     cfgFalse

/** <!--1108-->
 * Makes sure r1 is always cleared after a context switch.
 *
 * Gcc assumes r1 is cleared on AVR architecture at all times. It would be
 * pretty wasteful to store 0 on the context however. Worse, we must not only
 * store r1, but r0, r2 and r3 also. That means four bytes of ram wasted just
 * to store 0. Thus, you may set this to cfgTrue, and the restore context makes
 * sure r1 is cleared at the beginning, if you did not store it. If you do store
 * it, it is not cleaned. This is because there may be instructions (like the
 * multiply on the avr) which actually force you (the compiler) to make use of
 * it.
 *
 * The initial value of r1 equals zero, (because of defStackInitByte) and
 * that should stay that way provided it is not changed by hand.  On the
 * other hand, if you use assembler, you may not want this at all. In general
 * it is save to set this to cfgTrue, even if you store r1 on the context, it
 * costs some extra bytes though. It is not save to leave it to cfgFalse when
 * you do not store the registers r0-r3, in any of your tasks, since gcc still
 * assumes r1 to be zero. This may lead to very funny results and bugs which
 * are very hard to track down.
 */
#define  cfgSysClearUnusedR1                     cfgTrue

/** <!--1114-->
 * Squeeze all info needed by Femto OS in the Status Register.
 *
 * Femto OS uses a trick that saves two bytes of stack space at a context switch.
 * The price to be paid is that it must disable the global interrupts before
 * the portSaveContext() is called. That information can be stored in an auxiliary
 * register (the save way) or on the status register itself. An equivalent situation
 * arises with the internal protection of methods from interrupts.
 *
 * Most devices have an general purpose register in the I/O region that can be
 * used for this purpose. We only need one bit, the other bits are used as event
 * flags. However, if such an register is not available the T and H flags (of the
 * status register) are used to keep this information. This should be save. The T flag
 * (transfer bit) is only used in special cases by gcc, and i verified that they do not
 * collide with the way Femto OS uses the T bit. Upon context switch the T bit is
 * conserved.
 *
 * Likewise, the H bit (half carry flag) may be destroyed on a context switch. As far as
 * i know the backend of gcc does not procedure code that depends explicitly on the H flag.
 * Also using the latter approach the code tends to be a little shorter, but it depends
 * on which methods are used.
 *
 * To be absolutely save, or if you write assembler depending on the H flag,
 * do not choose cfgTrue for this option.
 */
#define  cfgSysSqueezeState                      cfgTrue

/** <!--1116-->
 * Activate this for double use of the OS stack space
 *
 * At the time a task runs, the OS stack space is not used. For the task to run however
 * some variables must be kept (called background variables). These variables can be
 * stored in the space used by the OS stack. This is usually save, but it cannot be
 * absolutely guaranteed that the compiler does not produce code around the stack switch
 * which destroys some of the variables, as a result of (future) optimization actions.
 *
 * Furthermore, if you want measure the use of the OS stack by a watermarking
 * technique, you need to set cfgFalse here too. This is because the background
 * variables are always located at the top of the stack, so the watermark technique
 * fill always find a fully used stack space.
 *
 * If unsure, or when plenty of ram is available, say cfgFalse here, otherwise set it
 * to cfgTrue.
 */
#define  cfgSysReuseOsStack                      cfgTrue

/** <!--1115-->
 * Specify the divider on the clock frequency used in the device.
 *
 * The base frequency (devClockFrequency) is the frequency that is set by the
 * crystal, the pll and other permanent settings such as fuses. That frequency
 * cannot be changed by software so it is defined in the portfile xxx.asm in the
 * devices directory. Some devices have the possibility to reduce the hardware
 * clock by prescaling to a lower value, which is then used to drive the chip.
 * That prescaling can be defined here.
 *
 * Note that you cannot just change the value to your liking. The port
 * file must support it, and it is likely it only supports a few predefined
 * fixed values, such as 1,2,4,8,16,32,64,128,256 for the ATtiny861. See the port
 * files for the allowed values. If there is no prescaler available (ATmega16)
 * set this value to one.
 */
#define  cfgSysClockDivider                      1


/** <!--1109-->
 * Subdivider for the tickcounter
 *
 * The value of the subtick counter at which the tick counter is increased
 * by one. We assume the tick counter is generated in the port by a hardware counter
 * passing a certain value, thereby generating an interrupt. This hardware
 * counter produces the subticks, and the number of Subticks per full tick
 * must be given here. You give the value taking into account a couple of
 * conditions.
 *
 * First, make sure the value of cfgSysSubTicksPerFullTick give rise to
 * the tick time of the desired length. This, by itself cannot be given as
 * a parameter directly. Second, make sure that the hardware  counter is able
 * to count on some more before overflowing, depending on the variation you
 * need for the time slices, so you don not want to make it to high. Third,
 * the accuracy of the delay timers also depend on this number, this you
 * don't want to make it to low either.
 *
 * Rule of the thumb, make it 1/8 of the maximum of your hardware counter.
 * (Thus, 8 bit counters take 32 and for 16 bit counters take 8192.)
 * Then you can have tasks running approximately 7 ticks without being
 * interrupted, and you have timer resolution of approx 3%.
 * Never make it smaller as 8 in absolute sense.
 */
#define  cfgSysSubTicksPerFullTick               32

/** <!--1110-->
 * Setting of the hardware prescaler of the tick counter.
 *
 * The divisor of the system clock to the subtick counter. This is highly
 * system dependent, but for  most architectures the timer clocks are
 * derived form the system clock by some prescaler. You must choose a value
 * here in such a way that the subtick counter gets 'the right speed'. This
 * is your formula:
 *
 * defTickFrequency = devClockFrequency /
 *   (cfgSysClockDivider * cfgSysSubTickDivider * cfgSysSubTicksPerFullTick)
 *
 * Note that you cannot just change the value to your liking. The port
 * file must support it, and it is likely it only supports a few predefined
 * fixed values, such as  1,8,32,64,128,256,1024. See the port files for
 * the allowed values.
 */
#define  cfgSysSubTickDivider                    256

/** <!--1111-->
 * Switch to replace the startup code produced by gcc
 *
 * Gcc produces startup code far from optimal for our (and most other) situation:
 * -- the stack is set, since we don't need it, it is a waste: 8 bytes down the drain
 * -- main is called rather than jumped to, a waste since returning makes no sense:
 *   2 ram bytes gone
 * -- empty .data section is copied: 22 bytes gone
 * -- at least three unneeded jumps: 6 bytes
 * -- an interrupt vector table you might not need, 26 bytes
 *
 * You can define your own startup code and activate it here. The little price you
 * pay is that, at startup, all ram must be cleared, since we don't know exactly
 * which part was used. (information only available at compile time). This is done
 * with the option cfgOverride. You can also completely define your startup files,
 * inclusive your own interrupt vector table. In that case specify cfgReplace.
 * If you want gcc to decide everything set it to cfgKeep.
 *
 * It may not be possible to set this to cfgReplace for certain combination
 * of other configuration settings. This will lead to a clear error.
 *
 * IMPORTANT: if you set this to cfgReplace add the option -nostartfiles to
 * the linker. If you set it to cgfKeep or cfgOverride, DO NOT FORGET to remove
 * that option, otherwise the code cannot be executed.
 * This option can be overridden from the command line by using defExtGCCstartup.
 * If that is set to cfgKeep, cfgOverride or cfgReplace its value overrides the
 * value defined in your config file.
 *
 * Note: the functionality of this option maybe compiler version dependent. Since
 * it is of use only for the smallest devices, i suggest you refrain from using
 * this for devices above 16KB flash.
 *
 * Note: In the current implementation no provision has been made to copy data
 * sections, i.e. if you have predefined data it is not copied from flash to ram
 * unless you alter the implementation in the at***.asm files or use cfgKeep.
 * Imo you should never need anything in the data section, but that is a matter
 * of taste i suppose.
 */
#define  cfgSysGCCstartup                        cfgOverride

/** <!--1112-->
 * Specify the total size of the storage in bytes
 *
 * The total amount of bytes used for the file system should be given here. Usually
 * the file system is defined in onboard EEPROM, and you can use all of it. However,
 * you can also decide to reserve half of it for other purposes for example. Per
 * default the file system starts off at address 0x000.
 */
#define  cfgSysFsStorageSpace                    512




/* ========================================================================= */
/* INTERRUPT HANDLING ====================================================== */
/* ========================================================================= */

/**
 * In this section you specify how the system handles (internal and external
 * interrupts. The choices you make influence the flash as well as ram
 * resources.
 */

/** <!--1201-->
 * To indicate that timer interrupts may be treated as global.
 *
 * Normally blocking for timer interrupts and global interrupts are separate
 * worlds. Most methods of the OS are blocked for timer interrupts but not
 * for global interrupts (except when needed). If you however do not have
 * any other interrupts present, you may decide not to make this distinction.
 * All OS timer interrupt protection code is now replaced by global protection
 * code.  It can shorten the code, but, if you use
 * interrupts for other purposes, it can make you application less responsive,
 * since all functions now generally block all interrupts.
 */
#define  cfgIntGlobalOnly                        cfgFalse

/** <!--1202-->
 * To indicate you use interrupts of your own, possibly with an isr.
 *
 * If you make use of interrupt really executing some code (not counting timer
 * interrupts or watchdog interrupts and so) set this to cfgTrue. It makes the
 * use of the isrXXXX methods possible and makes sure the StackISR is
 * included in the code if needed.
 */
#define  cfgIntUserDefined                       cfgTrue

/** <!--1203-->
 * Set this to let the isr run in OS space.
 *
 * There are two kinds of interrupt handling. One were the interrupt directly
 * forces a context switch and one where the interrupt method itself is
 * responsible for the saving of the registers used. For the former method,
 * you may decide whether you want to make use of the OS stack or the ISR stack.
 * If you use only context switching interrupts, you do not need a ISR stack
 * at all, simple make use of the OS stack and set the parameter to cfgTrue.
 * If you set it to false, the isr makes use of its own ISR stack, of which
 * the size must me defined below. A separate ISR stack is required when
 * the OS is interruptible. ISR's that start with isrBegin() always require
 * an ISR Stack, independent of the setting of cfgIntSwitchUsesOSstack.
 */
#define  cfgIntSwitchUsesOSstack                 cfgFalse

/** <!--1204-->
 * Set this to protect the Femto OS from being interrupted.
 *
 * If your interrupts use ANY of the genXXXX functions, set this to cfgTrue. The
 * internals of the OS and genXXXX functions are now protected. If your
 * interrupts do not use such functions this may (but must not) be set to
 * cfgFalse. Interrupts are now possible on any moment (except during
 * context switch, since we may not have a valid stack there), so that its
 * code is more responsive. You may use your own communication with the
 * tasks (a shared boolean for instance), or events.
 */
#define  cfgIntOsProtected                       cfgFalse

/** <!--1205-->
 * Activate to be able to block tick interrupts during part of a task.
 *
 * If you need protection from context switching during part of your task by disabling
 * the tick interrupts, you need to activate this option. (Note: you can also use the
 * dominant state for that, and leave tick interrupts activated.)
 * Then you can switch off tick interrupts and still task
 * switch to an other task manually. Activating this however blocks the use of
 * other interrupts which start or end with a context switch since that would be
 * a back door, possibly leading to the destruction of registers.
 * Defining tasks as cooperative is also achieved by this activating option, and,
 * for those tasks that are cooperative, set the start interrupts to cfgTickClear.
 *
 * Note that it does not make sense to activate tick tracking when cfgIntGlobalOnly
 * is cfgTrue. This is because all tick enabling / disabling has been mapped to
 * enable / disable of the global interrupt, and the tick interrupt remains unaltered.
 */
#define  cfgIntTickTrack                         cfgTrue

/** <!--1206-->
 * Activate to let the OS manually check for tick interrupts
 *
 * If you use equidistant ticks and all the tasks have the tick interrupts
 * disabled at some moment (or possibly all global interrupts), you have
 * entered a full cooperative mode. This implies no timer interrupts reach
 * the OS any more, so it must start to look manually if the interrupt
 * flags has been set. Since there is no short way Femto OS can detect such
 * a situation, you must indicate here that this might occur. The OS will
 * start monitoring the timer interrupt flag, and increase the tick counter
 * when a interrupt occurs. Note that you remain responsible that the OS
 * is sufficiently often called in order not to miss any ticks.
 *
 * If unsure, say cfgFalse here. This situation is rare in normal use.
 * However it is not an error to activate this setting when interrupts
 * are activated as well, but it ads unnecessary overhead to the system.
 *
 * Note that, if all tasks have shared stacks this is not necessarily a
 * full cooperative mode, since the tasks themselves are still interrupted,
 * although this does not lead to a task switch.
 */
#define  cfgIntManualTicks                       cfgTrue

/* ========================================================================= */
/* QUALITY ASSURANCE ======================================================= */
/* ========================================================================= */

/**
 * In this section you specify if the system should check the way you make
 * use of its facilities. You should develop with check options on. This
 * may increase code size and the use of ram, but is well worth it. If your
 * hardware does not support the amount of flags or ram, consider developing
 * on larger hardware, and see if the code fits the smaller hardware when
 * debugged and compiled with these options of. Developing with check options
 * off is highly inadvisable.
 *
 * Please note that reporting an error may cost considerable amount of stack,
 * thus it may happen that due to an error, a stack overflow error takes
 * place on top. It is not possible to report this error also. Thus make sure
 * that you have ample stack in the testing phase. If a error induced
 * stack overflow error occurs, usually the initial error is reported
 * correctly, after which the system crashes or resets.
 */

/** <!--1313-->
 * Activate internal checks in the Femto OS system
 *
 * Certain design aspects of the Femto OS can be checked by
 * setting this configuration option to true. The error triggered
 * by this option is called internal error an must be considered
 * a flaw or bug in the design of the system. Please file a
 * bug report.
 */
#define  cfgCheckSystem                          cfgTrue

/** <!--1301-->
 * Give a signal through the error port that the system has reset.
 *
 * If you activate this option Femto OS will call portShowReset at every
 * reset. Normally this happens only when you, yes, reset the system
 * or reboot it. However, if your code gets lost somewhere often this
 * results in a system reset. It is very neat when you can spot this.
 */
#define  cfgCheckReset                           cfgTrue

/** <!--1302-->
 * Activate tracing, most OS activities are reported.
 *
 * With this option it is possible to trace what is happening in your system
 * Most actions and api function calls will generate a trace event. Setting this
 * to true may seriously slow down the system, depending on the trace handler
 * implementation. Every task switch, api function call, error etc is reported.
 */
#define  cfgCheckTrace                           cfgTrue

/** <!--1312-->
 * Decide if all reported errors should be treated as fatal.
 *
 * Some error the system can report are fatal by nature, Femto OS cannot
 * recover from them. Some errors however are no so severe, and the
 * OS can, for example, simply terminate the offending task, and try
 * to continue business as usual. Setting this option to cfgTrue
 * makes the system treat all errors as fatal. In the early stage of
 * development this is advised since otherwise you may miss an error.
 *
 * Make sure you have enough OS stack if you set this option to false,
 * since error handle may require a lot of stack on top of the regular
 * stack handling. Os stack checking is not performed inside the error
 * handling.
 */
#define  cfgCheckAlwaysFatal                     cfgFalse

/** <!--1303-->
 * Keep watermarks of the stack and register use.
 *
 * Activate this to see which registers are used and what the highest use of
 * the stack of all tasks may be. These are estimates, measured at certain
 * moments. There is no way to be absolutely sure about the real heights
 * of the stack or real register usage. The watermark measurement of the
 * OS stack is only reliable when cfgSysReuseOsStack is set to cfgFalse.
 * Otherwise it would always give full stack use. Therefore, if cfgSysReuseOsStack
 * is set to cfgTrue, the watermark measurement of the OS will be based on
 * the highest found stacklevel in the deeper area's of the code. This,
 * however, is always a lower(!) bound. (In case non fatal errors occur it
 * is certainly a dozen bytes too low.)
 *
 * Note that if you activate cfgCheckRegisters
 * also, only those registers are collected in the watermark that are checked
 * at, since otherwise this could lead to spurious errors. If cfgCheckRegisters
 * is de-activated the use of all registers is monitored.
 *
 * Furthermore, activating
 * cfgCheckWatermarks alone never leads to an error report, even if the stack
 * overflows or unsaved registers are used. You must activate these options
 * separately.
 */
#define  cfgCheckWatermarks                      cfgTrue

/** <!--1304-->
 * Check if the OS stack keeps within the given limits.
 *
 * Set to cfgTrue to let the OS check its own use of the stack. The free space
 * of the OS stack is measured in the deepest calls of the OS. The
 * StackSafety parameter is NOT taken into account. The use of the OS stack
 * is highly dependent of the capabilities and on the setting of of the
 * cfgCheckTaskStack parameter, but not so much on the used methods of the
 * system.
 *
 * Therefore, it is best to first test with cfgCheckOsStack and
 * cfgCheckTaskStack set to cfgTrue. Then, first switch of cfgCheckTaskStack and
 * reduce the StackOS as much as possible. If the system runs error free, you may
 * switch off cfgCheckOsStack too. Since the call to the method checking the
 * OS is two bytes by itself, sometimes the OS Stack can be reduced two bytes
 * more, but this is risky. Please note that running without errors and/or
 * cfgCheckOsStack with set to cfgTrue is no absolute guaranty the stack will
 * not overflow.
 */
#define  cfgCheckOsStack                         cfgTrue

/** <!--1305-->
 * Check if each task stack keeps with the given limits.
 *
 * This is your life saver. It checks unauthorized use of
 * the stack on every context switch. Usually it reports an error and
 * shuts the task down before the stack overflows.
 * It requires quite some extra code and OS stack space though. This should
 * not be a problem. When all the testing is done
 * you can switch the option to cfgFalse and be reasonable certain,
 * that no further stack overflow occurs.
 *
 * If you have really deep functions where a context switch cannot occur
 * use the option includeTaskStackCheck and implement its function. With
 * this you can check how much space is really left.
 */
#define  cfgCheckTaskStack                       cfgTrue

/** <!--1306-->
 * Check if the isr stack keeps with the given limits.
 *
 * In an isr no context switching occurs, so the only way to check this
 * stack is the option includeIsrStackCheck. Switch this option on
 * if you want to use the checking.
 */
#define  cfgCheckIsrStack                        cfgTrue

/** <!--1307-->
 * Check if use is made of registers not saved on the context.
 *
 * This is a life saver too. It checks unauthorized use of registers on every
 * context switch. It reports an error and
 * shuts the task down before the task is restarted
 * with erroneous registers. Checking on the registers is a
 * little to strict, since registers only used with interrupt protected
 * area's need not to be saved on the context. However, the system cannot
 * make that distinction, so it reports on the save side. If you know
 * exactly what you are doing, you may reduce the number of registers
 * after you have switched off this option.
 * The use of the Tbit is also checked with this option.
 */
#define  cfgCheckRegisters                       cfgTrue

/** <!--1308-->
 * Check if the tick is long enough to let OS and tasks do their jobs.
 *
 * Activate this option to check the issues having to do with timing.
 * For example, check if there is enough time left for the tasks to
 * run, or if the delays are calculated in an orderly manner.
 */
#define  cfgCheckTiming                          cfgTrue

/** <!--1309-->
 * Check if the queu's are not overrun or underrun.
 *
 * Queus may be written to, even if there are full. This is important,
 * since inside an isr you cannot block and wait until the queu is
 * read. The same applies for reading, this may be done on an empty
 * queu. However, you can not expect sensible results when doing so,
 * the only guarantee you have is that the system does not deadlock.
 *
 * Thus, it would be nice if you can see if such a situation occurs.
 * By setting the option to cfgTrue, overrun and underrun of the queus
 * become an error, but only when called from a task. When called
 * from inside an isr environment, superfluous bytes are still silently
 * ignored and zero bytes are provided when the the queus is empty.
 * This is because we do not know how to act after an error in that case.
 */
#define  cfgCheckQueuFilling                     cfgTrue

/** <!--1310-->
 * Checks if you make proper use of the API.
 *
 * There are several rules about how to use the API of the Femto OS. It
 * is your responsibility to honor them. For example, Slot 0 may not
 * be used, priorities may not exceed 7 etc. The OS does not protect you
 * from misuse, since in embedded systems there is usually no one who
 * can click on the OK button when an error is reported. But during
 * development you can check most of the rules on use by switching on
 * this option.
 */
#define  cfgCheckMethodUse                       cfgTrue

/** <!--1311-->
 * Activate checking of the user quality assurance.
 *
 * Use this switch for you own application, to switch on some leds or
 * other means of quality assurance. This option is not used in Femto OS.
 * It is reserved for user application purposes.
 */
#define  cfgCheckApplication                     cfgTrue


/* ========================================================================= */
/* COMMON CAPABILITIES ===================================================== */
/* ========================================================================= */

/**
 * In this section you specify which functionalities should be part of
 * the system. Only activate those parts you need. Some or more function
 * calls may not be present in the system when the accompanying capabilities
 * are not activated. This may lead to compiler errors, even in the Femto
 * os itself.
 */

/** <!--1401-->
 * Make sure every tick takes an equal amount of time.
 *
 * The femto OS uses a feature called Honest Time Slicing. This means that
 * every task gets the full time slice when it starts. This is useful
 * when you have several tasks running indefinitely, just to make sure
 * they all get a fair part of the time.
 *
 * There are reasons however to use the standard tick interrupt system,
 * where all tick interrupts come a fixed times. If all you tasks are
 * blocking, for example, or if you need to derive from that time base.
 * In the latter case set this option to cfgTrue, it saves some code too.
 *
 * Further, Honest Time Slicing introduces some extra rounding errors
 * in the timing model which makes the tick counter less accurate as a
 * system clock as the oscillator crystal would be. If you set this
 * option to cfgTrue however, the tick counter is firmly coupled to the
 * system clock, so that, if you want to use the tick counter as a time
 * source, you only need to calibrate the system clock, i.e. the crystal.
 * Some hardware may be driven by very accurate crystals, and then,
 * with this option set to cfgTrue, you can make an accurate clock.
 */
#define  cfgUseEquidistantTicks                  cfgTrue

/** <!--1403-->
 * If you need delays or timeouts in your code, activate this.
 *
 * If you want to delay some tasks, activate this option. In the system this
 * enables keeping track of task waiting for some time and than being rescheduled.
 * If you make use of timeouts on synchronization primitives or make use
 * of the restart facility, you must also enable this option.
 *
 * Also, if you want to make use of cfgUseCorrectWakeupTimes you must enable
 * it, since it makes use of delayed tasks. If you just use Femto OS as a simple
 * scheduler without delays, you can set this to false. That saves some code
 * and two bytes of ram per task.
 */
#define  cfgUseDelay                             cfgTrue

/** <!--1422-->
 * If you check for suspend when delayed, activate this.
 *
 * Normally, you must manually check for a suspend request, for a task will
 * not just suspend when asked too. The problem with that is that the task
 * may hold locks, of have open files, so that other tasks will effectively
 * become suspended too. See taskSuspend() for more information. However, you
 * almost never will delay a task when it holds locks or has open files, for
 * the same reason. Therefore the delay call is a natural point to check for
 * the suspend request. If you activate this option, the OS will do so for you.
 * Then, you will know on beforehand where the suspend takes place.
 */
#define  cfgUseSuspendOnDelay                    cfgTrue

/** <!--1404-->
 * Activate if you make use of the synchronization primitives.
 *
 * Specify here how you want the synchronization to be used. All synchronization
 * Information is kept in a slotstack, one for each task. The size of that
 * stack can be defined further below. The way this is stack is 'formatted', is
 * chosen here. The more complex the formatting, the more possibilities and the
 * more code generated by the preprocessor. You can choose from cfgSyncSingleSlot,
 * cfgSyncSingleBlock, cfgSyncDoubleBlock and cfgSyncNon.
 *
 * If you do not want
 * to make any use of synchronization set this to cfgSyncNon. Please note that,
 * although no code is included to handle slots, the slot stacks may still claim ram.
 * Set them explicitly to zero if you do not need them. If you choose for
 * cfgSyncSingleSlot, one slot may be occupied per task at every moment and no
 * nesting is allowed. This takes one byte for every task using slots. It makes
 * no sense to define the SlotSizes larger than one (and it is  forbidden, to make
 * sure we can optimize the code).
 *
 * If you choose for cfgSyncSingleBlock, every task may hold one blocking slot
 * at a time, and may use the rest of the bytes in the slot stack for other
 * free locks or nesting. Every byte can hold two slots (one per nibble).
 * If you choose for cfgSyncDoubleBlock, every task may hold two blocking slots,
 * on which it blocks simultaneously. The other bytes in the slot stack are
 * used for free locks.
 */
#define  cfgUseSynchronization                   cfgSyncDoubleBlock

/** <!--1407-->
 * Activate to make sure task are started round robin strictly prioritized
 *
 * Normally, round robin scheduling is applied only on the highest priority
 * that has tasks available that are able to run. The tasks that did (or didn't)
 * run on other priorities are forgotten. Thus, any time the scheduler returns
 * to a new priority, the round (re)starts from the beginning. This is not always
 * desirable, since this may lead to the starvation of some tasks in special
 * circumstances. If these occur, activate this option, to be certain that
 * all tasks on all priorities are given a chance to run.
 */
#define  cfgUseHierarchicalRoundRobin            cfgTrue

/** <!--1405-->
 * If you need to nest critical blocks, activate this.
 *
 * Critical sections can be made by disabling/enabling interrupts over
 * the region you want to protect. However, this approach cannot be nested,
 * since, when the deepest level is left, interrupts are enabled again. This
 * nesting may occur over function calls and is therefore hard to monitor.
 *
 * If you only have one level, there is no need for nesting, but otherwise,
 * you have the option to switch cfgUseNestedCriticals to cfgTrue. Now the
 * system keeps track of the nesting for you. The maximum number of levels
 * depend on the use of the types of interrupts you need to protect. If is
 * is one kind (e.g. either Global or Tick interrupts) you have 16 levels,
 * if you use both, you have 4 levels per interrupt, which may be mixed.
 *
 * If you switch this option to cfgFalse, all enter/exit critical calls
 * are mapped to standard disable/enable calls by the preprocessor.
 * Note, the nesting of mutexes/queu's is handled in a separate option.
 * And second, note that, the OS itself does nest the switching of
 * interrupts, but does not make use of this system, so in fact all
 * levels are available for own use.
 */
#define  cfgUseNestedCriticals                   cfgTrue

/** <!--1402-->
 * If you want unblocking for the highest priority only.
 *
 * When a mutex or queue lock is released by a task the next task
 * may obtain that lock. If you activate this configuration
 * parameter, the task will be the task with the highest possible
 * priority. If there are more tasks with the same priority first
 * the ones with a timeout will be selected. Ofter however, there
 * are not more candidates and the search for these only takes
 * time and space. If you know so on beforehand, set this
 * parameter to cfgFalse.
 */
#define  cfgUsePrioritizedRelease                cfgTrue

/** <!--1406-->
 * If you need to make use of priority inheritance, activate this.
 *
 * A high priority task may block on a low priority task. If that low priority task
 * never gets any processing time because, for example, there is a non blocking task
 * at medium priority, the high priority task hangs too. In that case you may switch
 * on cfgUsePriorityLifting. Blocking tasks sharing the same synchronization lock
 * are lifted to the priority of that of the task with the highest priority.
 * This, however is temporary. As soon as the lock is released, the priorities
 * are restored to there initial value. Beware, the are not restored to there last
 * value, but to the value it started with at the beginning, i.e. the priority
 * of Priority_XXXX.
 */
#define  cfgUsePriorityLifting                   cfgTrue

/** <!--1408-->
 * Activate this to protect a task against infinite loops and waits.
 *
 * You can protect your tasks with a watchdog. If you set this option to cfgTrue,
 * all tasks which contain a bark() method are called when the watchdog
 * expires, if you activated the callAppBark. Shortly before the task is reinitialized
 * (stack/variables cleaned) and afterwards restarted (loop called). Only the priority of
 * the task is retained, the rest of the status info is renewed.
 *
 * The init code of the task, if present, is not called.
 * Redo any work in the bark section, or call init from
 * bark. The watchdog must be activated per task separately, since it is likely you
 * only want to protect only some tasks, bus drivers for example. After a restart
 * the watchdog is deactivated, as it always is when a task starts. Please note that
 * only (!) running tasks are watched. A blocking, delayed or sleeping task can never
 * be barked at. Thus if two tasks are waiting on each other in a block, these tasks
 * may still deadlock. Shared tasks that are not scheduled are not watched, nor is
 * the watchdog decreasing its lifetime.
 */
#define  cfgUseTaskWatchdog                      cfgTrue

/** <!--1409-->
 * Activate to keep track of the last task activation times.
 *
 * Setting this to cfgTrue makes sure that for every take the last wakeup time
 * is recorded correctly. It is recorded correctly always when only delays
 * are used to block a task. However, if tasks are also woke from suspension
 * or sleep, or are deblocked by releasing semafores, these times are not
 * automatically correct. You may need correct times when you use
 * includeTaskDelayFromWake for example. If you activate this option
 * correct wake uptime are kept internally. cfgUseDelay must be activated,
 * for this option to make sense.
 */
#define  cfgUseCorrectWakeupTimes                cfgTrue

/** <!--1410-->
 * Activate to enable timeout on all synchronizing methods.
 *
 * Blocking calls on synchronizing methods may be given a timeout parameter.
 * For this to be effective though, i.e. in order to make sure the code
 * is present for releasing the task on a timeout, this option must be set
 * to cfgTrue. Otherwise the timeout parameter is ignored, and the call or
 * returns with the lock obtained or never returns. The return value itself
 * is undefined, and may even be false, in that case. Activating this implies
 * activation of cfgUseDelay (if cfgUseSynchronization is not equal to
 * cfgSyncNon), even if cfgUseDelay is set to false, internally, but you
 * must still activate cfgUseDelay if you want to make explicit use of delays.
 *
 * TODO: Change the name in cfgUseTimeout
 */
#define  cfgUseTimeout                           cfgTrue

/** <!--1411-->
 * Activate to reserve room in flash for dynamic task names.
 *
 * All tasks have compile time (literal) tasks names. This makes handling more easy.
 * Usually the presence of dynamic task names is a waste of space, since
 * nobody is watching the embedded system. Besides, tasks cannot be
 * dynamically loaded or unloaded, so it is known on any time which tasks
 * are running ;-) However there may be reasons where dynamic task names
 * may be handy, and this is the option you need to set to cfgTrue in order
 * to make use of them. If it is set to cfgFalse, the names are not compiled
 * in, regardless if they are specified below.
 */
#define  cfgUseTasknames                         cfgTrue

/** <!--1412-->
 * Make the Femto OS aware of low power sleep modes.
 *
 * As soon as all tasks are put to sleep or have stopped for an other reason
 * (stopped due to an fatal error, suspended or locked by a semaphore) and
 * thus the only tasks left is the idle task, the system can go to a low
 * power mode instead of the idle task, if this option is set to cfgTrue.
 * Timer interrupts are stopped. Note that, if only this option is set to
 * cfgTrue, the system will not go to sleep if there are still delayed tasks
 * left.)
 *
 * One further point of attention is that interrupts are used to wake up the
 * device from low power sleep. This may be the (internal) watchdog but could
 * as well be some other interrupt you are currently using. Since the Femto OS
 * is in OS mode while sleeping, it is not possible to use the switching
 * interrupts isrEnter() and isrExit(). Interrupt routines have to start with
 * isrBegin() and have to end with isrEndReturn(), and must have their
 * own ISR stack. As an alternative, you can make use of the event system
 * which does not even require isrBegin()/isrEnd(). No stack and registers
 * are used in this case.
 *
 * For the avr, code in the port.c file makes sure the system wakes up again,
 * and correct the tick counter is restored. Please note that passed tick hooks are
 * not catched up, except the appTick16(). Upon wake all sleeping tasks are woken
 * per default. For indefinite sleeps the tick counter cannot be restored.
 */
#define  cfgUseLowPowerSleep                     cfgTrue

/** <!--1413-->
 * Let the OS sleep as soon as all tasks are sufficiently delayed.
 *
 * If the tasks that are not put to sleep are delayed long enough, it may be
 * sensible to put the system in low power too. The minimal delay time for this
 * to happen is set in the parameter cfgNumSleepThreshold. Thus if the system
 * enters idle with all tasks either stopped for any reason or delayed long
 * enough the system will to to sleep if this option is set. Requires the
 * activation of cfgUseDelay.
 *
 * This option may not be activated on devices that do not have a watchdog
 * interrupt. For example the ATmega16 can only reset on watchdog, so the
 * watchdog, used internally to limit the sleeptime cannot be used. Unfortunately
 * there is no other mechanism which can take over this role.
 *
 * The use of the watchdog as time source has one other consequence. The watchdog
 * has its own clock, which is set in milliseconds. So you can set cfgNumSleepPeriod
 * to any value you like, only these device predefined values will be used. The
 * tick clock must be corrected using this values, making it dependent on the
 * real clock frequency. You will obtain shortest code and most accurate results
 * when the real time spend for 256 ticks is a divisor of the used predefined value
 * of the watchdog timer.
 *
 * Last thing you must know that it is your responsibility to correct the tick counter
 * when cfgUseLowPowerOnDelay is activated, otherwise the system wakes up, but the
 * clock is like unaltered, and the delayed task stayed delayed. This could be intentional,
 * but not likely. So better activate includeGenAddtoTickCount. The OS then takes
 * care of adjusting the tick counter.
 */
#define  cfgUseLowPowerOnDelay                   cfgTrue

/** <!--1414-->
 * Activate to correct delay times, if the device over slept.
 *
 * The function call for LowPowerSleep contains a parameter which indicates the
 * maximal sleep time. If the low power sleep takes no longer as this parameter
 * indicates, it is guaranteed that no timer delay tasks have been expired during
 * the sleep so no waking up is necessary. If it is however not certain that the
 * sleep time was equal or shorter as the max sleep time, it may be needed to wake
 * up some delayed tasks (standard sleeping tasks are woken anyhow). Then set this
 * option to cfgTrue. Note, that it is still your responsibility to adjust the tick
 * counter. If you forget, it will be to the system as if no time has passed at all.
 * Requires the activation of cfgUseDelay.
 */
#define  cfgUseLowPowerDelayRelease              cfgTrue

/** <!--1415-->
 * To keep track of the time spend in all tasks, os and isr.
 *
 * With this option it is possible to monitor how much time is spend in each
 * of the tasks. Compare it to the traditional task manager. You must read
 * the accumulated values at hand. The values are measured in subticks.
 * Activating this option increases the ram use a lot. The OS has to keep
 * track of the Time spend in the OS itself (4 bytes) and the idle time
 * (4 bytes) and possible the ISR (5 bytes) and per task spend time
 * must be collected and stored (4 bytes per task). So if you have four tasks
 * or so with an interrupt this facility costs 29 bytes of ram extra!
 */
#define  cfgUseLoadMonitor                       cfgTrue


/** <!--1416-->
 * Activate the Femto File System on EEPROM
 *
 * With this option you can indicate that you want to make use of the Femto
 * File System that is defined as FS on the EEPROM. Synchronized access (single
 * write, multiple read) to the EEPROM is organized by the FFS and there is no
 * need to use slots. Also, the system blocks the writing task until the
 * burnlock on the eeprom has been released.
 */
#define  cfgUseFileSystem                        cfgTrue

/** <!--1417-->
 * Separate read/write regions from read only regions.
 *
 * This option enables you to choose to open a file in read/write mode
 * or in read mode only. The advantage of the latter is that more tasks
 * can simultaneously read from the file system. If an other task wants
 * to write, no new reader tasks are allowed any more until all readers
 * are finished. Then (one) writing task is allowed. This is also known
 * are a singlewritermultiplereader synchronizer. But even if you have one
 * task active at most on the file system it can be handy, since tasks
 * that are only reading on the file system may be put to sleep, whereas
 * writing tasks are never.
 */
#define  cfgUseFileSystemConcurrentRead          cfgTrue

/** <!--1418-->
 * Option to save erase/write cycles on the file system / eeprom
 *
 * Normally writing a bytes triggers an erase before the write. However,
 * depending on the content of the byte written, this may not be necessary.
 * If you activate the economy mode, every byte is read prior to being
 * written and on the basis of the content it is decided if an erase,
 * a write or both are needed. Of course, this increases the code somewhat
 * and introduces some more overhead. On the other side, you the eeprom
 * will last longer (less erases) and the writing time will be shorter on
 * average. This option is only available on devices that support split
 * byte programming, and should be set to cfgFalse in such a feature is
 * not present.
 */
#define  cfgUseFileSystemEconomyMode             cfgTrue

/** <!--1419-->
 * Keep a table with the use of all files
 *
 * If you activate this option, Femto OS will maintain a 'FAT' table at the
 * start of the file space. Every entry of the table contains the length of
 * corresponding files. After a file system format these are all zero. Since
 * files are on fixed locations and their maximum length cannot change, there
 * is no need to keep more information. If you want to append bytes to a file,
 * or want to set the size explicitly, for example, you need this feature.
 */
#define  cfgUseFileSystemMaintainFAT             cfgTrue

/** <!--1420-->
 * Lightweight messaging system between tasks and/or isr.
 *
 * There are eight events which can be given a name. A task may send
 * and event or may block waiting on one or more events to take place.
 * An event may be fired from an isr, even if the OS is made interruptible.
 * The event itself is not stored. As it takes place, all tasks blocking
 * on that particular event are released. If a task blocks after an event
 * has taken place, it has to wait for a new event. Tasks that block on
 * more than one event are released when all events have taken place at
 * least once.
 */
#define  cfgUseEvents                            cfgTrue

/** <!--1421-->
 * Activate if you need to use genFireEvent on variables.
 *
 * In normal circumstances it is most efficient to call genFireEvent on
 * a compile time constant only. Depending on the location of the event
 * register (and other configuration parameters) this may compile to
 * a single (atomic) instruction. The isrFireEvent() method may only
 * be called upon compile time constants! Sometimes however is may be
 * needed to have some variable (e.g. a counter) which determines which
 * event to fire. In that case you need to activate the parameter
 * cfgUseEventsOnVariables. This makes sure the method genFireEvent()
 * is used instead of its macro. It generates a lot of code however.
 * Setting this parameter makes only sense in combination with cfgUseEvents
 * set to cfgTrue.
 */
#define  cfgUseEventsOnVariables                  cfgTrue


/* ========================================================================= */
/* CODE PARAMETERS ========================================================= */
/* ========================================================================= */

/**
 * In this section you specify some quantities, and timer values.
 */

/** <!--1501-->
 * Determines the frequency of watchdog checkups.
 *
 * This is the divisor of the watchdog down counter. In other words, the
 * watchdog counter, which starts at (an internal value of) 3 is decremented
 * by one every (2^n) * 256 ticks. The (2^n)*256 tick boundary is used to
 * determine that moment. Depending on when the watchdog is started, the first
 * decrement may directly be after the first reset of the watchdog, or a full
 * period of (2^n) * 256 - 1 ticks later. If the watchdog counter itself
 * reaches 1 it starts barking. Thus, this mechanism only guarantees
 * a minimum sleep time of the watchdog, namely (2^n)*256 ticks. n is in the
 * interval : 0<=n<=8. Thus, for a given n the minimum watchdog time equals
 * (2^n)*256 ticks, the maximum watchdog time 2*(2^n)*256 -1.
 */
#define  cfgNumWatchdogDiv                       3

/** <!--1502-->
 * Determines the copy frequency of the load monitor.
 *
 * Specify the divisor if the load entry timer. The mechanism is the same as for
 * the watchdog down counter. Since the load logging is a periodic activity it is
 * not really important when the log event occurs. It can thus occur every
 * 256, 512, 1204 etc ticks. Please note that, if you choose your value to high
 * some load counters may overflow. The maximum number of ticks that can be
 * monitored is 65536 / cfgSysSubTicksPerFullTick. For
 * cfgSysSubTicksPerFullTick = 32, this is 2048, leading to a divisor of 3.
 */
#define  cfgNumMonitorDiv                        2

/** <!--1503-->
 * Number of milliseconds the device may continuously sleep
 *
 * When the device is sleeping (and there is a watchdog present in the
 * device) it wakes up at fixed intervals to see it the total sleeping
 * period is already over. If not, a new period of sleep starts. Of course,
 * this repeated wakening only takes place when there is some delay timer
 * which may expire. So both cfgUseLowPowerSleep and cfgUseLowPowerOnDelay
 * must be set to cfgTrue.
 *
 * With this parameter you can set the sleep period in
 * milliseconds. Every wakening costs energy so you don't want to set it
 * to low. However, if you set it to large, the period may exceed the
 * remaining sleep time to quickly, and if that happens, the device stays
 * awake without a real need. The minimum time is 250ms, the maximum time
 * is device dependent, 2 or 8 seconds. In any case, the value you give
 * will be rounded and maximized, so it makes no sense to specify it up
 * to the millisecond.
 *
 * Furthermore, depending on the tick time different values of the cfgNumSleepPeriod
 * imply different calculation methods, which, in turn may have a dramatic
 * impact on the code size. So, experiment with different values.
 */
#define  cfgNumSleepPeriod                       1000

/** <!--1504-->
 * Minimum number of milliseconds the delay must be for sleeping.
 *
 * Set the minimum number of milliseconds the shortest delay must have in order
 * to go to low power sleep. the minimum is about 250 ms,  Note that the option
 * cfgUseLowPowerSleep and cfgUseLowPowerOnDelay must be set to cfgTrue.
 * It makes no sense to set it to a lower as the value of cfgNumSleepPeriod
 * although it does not harm either. Set it high enough to make sleep worthwhile
 * but not to high since otherwise sleep will never occur.
 */
#define  cfgNumSleepThreshold                    2500

/** <!--1505-->
 * Number of bits for counting the depth of global critical nesting.
 *
 * Specify how many bits must be reserver to keep track of the depth
 * of critical nesting. For example, if you specify 3 bits, 8 levels
 * can be used. The number of bits of cfgNumGlobWidth, cfgNumSwitchWidth
 * and cfgNumTickWidth may not exceed 8 bits, for they are stored in one
 * byte. Specify 0 if you do not use this type of critical nesting.
 */
#define  cfgNumGlobWidth                         3

/** <!--1506-->
 * Number of bits for counting the depth of switch critical nesting.
 *
 * Specify how many bits must be reserver to keep track of the depth
 * of critical nesting. For example, if you specify 3 bits, 8 levels
 * can be used. The number of bits of cfgNumGlobWidth, cfgNumSwitchWidth
 * and cfgNumTickWidth may not exceed 8 bits, for they are stored in one
 * byte. Specify 0 if you do not use this type of critical nesting.
 */
#define  cfgNumSwitchWidth                       3

/** <!--1507-->
 * Number of bits for counting the depth of tick critical nesting.
 *
 * Specify how many bits must be reserver to keep track of the depth
 * of critical nesting. For example, if you specify 3 bits, 8 levels
 * can be used. The number of bits of cfgNumGlobWidth, cfgNumSwitchWidth
 * and cfgNumTickWidth may not exceed 8 bits, for they are stored in one
 * byte. Specify 0 if you do not use this type of critical nesting.
 */
#define  cfgNumTickWidth                         2


/* ========================================================================= */
/* EXTERNAL CALLS ========================================================== */
/* ========================================================================= */

/**
 * The femto OS can perform some external calls, apart from the task loops.
 * These are the different tick hooks, and some initialization functions.
 * These are to specified here. Note that the names of these calls are
 * fixed, so you cannot specify more than one hook.
 */

/** <!--1601-->
 * Activates the tick hook that fires every tick.
 *
 * If there is some work that must be done every tick define your own
 * tick00() method and set this option to cfgTrue. Now is is called every tick.
 * Needles to say you must keep the work short. The code runs in OS stack
 * space, and the system cannot check if the OS stack overflows inside your
 * code, so be careful, and check you assembler how much stack is used
 * in the code. Although called every tick, the calls may not be equidistant
 * in time. This heavily depends on the way you set up the time slice
 * parameters.
 *
 * If use is made of cfgUseEquidistantTicks == cfgTrue the calls
 * should be fairly constant over time (aside from interrupts), and if
 * you use variable time slicing, it may get silent for longer timers,
 * up to several times the tick time, and than these are caught up
 * afterwards. Note that you can be sure that on every call the tick counter
 * itself only is incremented by one since the last call. (Not that
 * you can do much with the tick counter, you cannot get it's value
 * form inside the tick hook) The only exception being when the
 * system is in low power sleep. The missed calls due to a low power
 *  sleep are not caught up.
 */
#define  callAppTick00                           cfgFalse

/** <!--1602-->
 * Activates the tick hook that fires every 256 ticks
 *
 * This method is called every 256 ticks, and always after the tick00 has
 * been called. Further the same conditions apply.
 */
#define  callAppTick08                           cfgFalse

/** <!--1603-->
 * Activates the tick hook that fires every 65536 ticks
 *
 * This method is called every 65536 ticks, so when the tick counter overflows.
 * It may be used to extend the tick counter to 32 bits, if needed. Note that,
 * due to the rounding errors in the timer model, it is not particularly
 * well suited to make an accurate clock, only perhaps when
 * cfgUseEquidistantTicks is set to cfgTrue. You still have the drift of the
 * driving crystal though.
 *
 * When in low power sleep, this call is of course skipped. If however, you
 * manually correct the tick counter after sleep, and the sleep time was less
 * than one full round (<65536 ticks) than the appTick16() call is made anyway
 * so that for instance, your 32 tick counter remains accurate.
 */
#define  callAppTick16                           cfgFalse

/** <!--1610-->
 * Enable sleep hook that fires at in-between wakeups.
 *
 * When cfgUseLowPowerOnDelay is activated the device wakes up periodically
 * (with intervals set by cfgNumSleepPeriod) to see if the sleep time is
 * over. You can define a hook on these wake ups to do some bookkeeping.
 * This call takes place in OS space, and the device may go to sleep again
 * afterwards, if the sleeping time was not over yet. Use this for example
 * to keep an external clock in sync, or to flash a heart beat led.
 */
#define  callAppTickSleep                        cfgFalse

/** <!--1604-->
 * Activates the idle hook that fires just before going idle.
 *
 * Just before the system goes idle you can perform some tasks when installing
 * the hook appEnterIdle(). If you do so, activate it here by setting this option
 * to cfgTrue. Your code is run with tick interrupts disabled, so be very brief.
 * Depending on the setting of the interrupt options other interrupts may
 * occur. Of course the call runs in OS space.
 */
#define  callAppEnterIdle                        cfgFalse

/** <!--1605-->
 * Enable sleep hook that fires just before going to low power sleep.
 *
 * Just before the system goes to sleep appEnterSleep() is called. You can
 * perform last tasks before the low power mode is activated. The time spend
 * in your routine is not counted for, so be very brief.  The call runs in
 * OS space, and global interrupts are disabled. If you arrive here, you can
 * be certain the device will not be interrupted before it goes to sleep.
 * The sleep itself however may be short or even absent (if you chose the
 * cfgNumSleepThreshold lower as cfgNumSleepPeriod).
 */
#define  callAppEnterSleep                       cfgFalse

/** <!--1606-->
 * Enable sleep hook that fires just after going to low power sleep.
 *
 * Just after the sleep period is over appExitSleep() is called. You can
 * perform some tasks before the system is restarted. Time spend
 * in your routine is not counted for, so be very brief.  The call runs in
 * OS space, and global interrupts are disabled again, and you can be sure
 * that if you arrive here, no other tasks where started since the wake up.
 * Other interrupts may have occurred and run however.
 */
#define  callAppExitSleep                        cfgFalse


/** <!--1607-->
 * Enable initialization hook called before any context construction.
 *
 * There may be some initialization code that is shared for all tasks. This can
 * be put inside the the function appBoot(). This is called inside OS space,
 * and none of the tasks are already set up at that moment. It is usually a lot
 * more effective to collect the initialization code of the tasks that to give
 * all tasks its own initialization code. Only put code here that only needs to
 * be done at the very start. It is not called again if a particular task is
 * restarted after a watchdog bark for instance.
 */
#define  callAppBoot                             cfgFalse

/** <!--1608-->
 * Enable initialization hook called before task context construction.
 *
 * It may be necessary however to individually give every task its own startup
 * code. If so, set this parameter to cfgTrue. Note that this implies that every
 * task must have an init method, although it may be empty. Note that it is
 * possible to set both callAppBoot and callAppInit to cfgTrue. The appBoot()
 * is called first, followed by init methods of the tasks. In appInit code
 * is placed that must be called before the task may start. If a task is restarted
 * manually this is not re-executed.
 */
#define  callAppInit                             cfgFalse

/** <!--1609-->
 * Activates the watchdog hook called when the watchdog barks.
 *
 * When using the watchdog facility a task that gets stuck is reset. Usually
 * there is some cleaning up to do before the loop is called again. (The
 * init of the task is not called again!) This is done inside appBark(). The task
 * is first re-intialized (cleanup stack and variables) and afterwards
 * restarted (call loop). The bark section is optional however. Decide here
 * if you want such a section. Activate cfgUseTaskWatchdog to make use of
 * the watchdog facility.
 */
#define  callAppBark                             cfgFalse


/* ========================================================================= */
/* API INCLUSIONS ========================================================== */
/* ========================================================================= */

/**
 * This section lists the possible functions that can be used in your code.
 * The function name equals the inclusion definition without the include prefix
 * and starting decapitalized.
 *
 * There are three possible places where these functions can be used, inside
 * normal tasks, inside an interrupt service routine (isr) or inside a bark
 * routine. Functions starting with 'gen' can be used from any of these places,
 * functions starting with 'task' only from with normal tasks, and functions
 * starting with 'isr' only from within isr routines.
 *
 * Calls from inside the tickHooks to the OS are strictly forbidden.
 * Only brief descriptions of the tasks. For more elaborate descriptions, see
 * the function definitions.
 */


/* Standard functions ------------------------------------------------------ */

/**
 * Most of these functions are available without any special capabilities
 * activated.
 */

/** <!--1701-->
 * Manual context switch
 */
#define  includeTaskYield                        cfgTrue

/** <!--1702-->
 * Delay your task for the given number of ticks, starting from now.
 *
 * Activate cfgUseDelay to make use of the this facility
 */
#define  includeTaskDelayFromNow                 cfgTrue

/** <!--1703-->
 * Delay your task, starting at the last wake time.
 *
 * Activate cfgUseDelay to make use of the this facility
 */
#define  includeTaskDelayFromWake                cfgTrue

/** <!--1704-->
 * Suspend a task, but do not invoke a context switch.
 */
#define  includeGenSuspend                       cfgTrue

/** <!--1705-->
 * Suspend this task and invoke a context switch.
 */
#define  includeTaskSuspend                      cfgTrue

/** <!--1706-->
 * Resume the given task from suspension.
 */
#define  includeGenResume                        cfgTrue

/** <!--1707-->
 * Put this task to sleep and invoke a context switch.
 *
 * Activate cfgUseLowPowerSleep to make use of the this facility
 */
#define  includeTaskSleep                        cfgTrue

/** <!--1708-->
 * Put all tasks to sleep.
 *
 * Activate cfgUseLowPowerSleep to make use of the this facility
 */
#define  includeTaskSleepAll                     cfgTrue

/** <!--1710-->
 * Set a new priority for the given task.
 */
#define  includeGenSetPriority                   cfgTrue

/** <!--1711-->
 * Return the current priority of a given task.
 */
#define  includeGenGetPriority                   cfgTrue

/** <!--1712-->
 * Get the tick counter.
 */
#define  includeGenGetTickCount                  cfgTrue

/** <!--1713-->
 * Add the given number of ticks to the tick counter.
 */
#define  includeGenAddtoTickCount                cfgTrue

/** <!--1714-->
 * Return the time the task was last woken.
 *
 * Activate cfgUseDelay to make use of the this facility
 */
#define  includeGenGetLastWakeTime               cfgTrue

/** <!--1715-->
 * Feed the watchdog in order to prevent barking.
 *
 * Activate cfgUseTaskWatchdog to make use of the this facility
 */
#define  includeTaskFeedWatchdog                 cfgTrue

/** <!--1716-->
 * Switch off the watchdog facility for this task.
 *
 * Activate cfgUseTaskWatchdog to make use of the this facility
 */
#define  includeTaskKillWatchdog                 cfgTrue

/** <!--1717-->
 * Get the address from a task name
 *
 * Activate cfgUseTasknames to make use of the this facility
 */
#define  includeGenGetTaskname                   cfgTrue

/** <!--1718-->
 * Stop (in error mode) one particular task.
 */
#define  includeTaskTerminate                    cfgTrue

/** <!--1719-->
 * Reboot the system
 */
#define  includeGenReboot                        cfgTrue


/** <!--1720-->
 * Recreate a task
 */
#define  includeTaskRecreate                     cfgTrue

/** <!--1726-->
 * Restart a task
 */
#define  includeTaskRestart                      cfgTrue

/** <!--1721-->
 * Binary one liners about every task
 */
#define  includeGenLogTask                       cfgTrue

/** <!--1722-->
 * Binary one liner about the os
 */
#define  includeGenLogOs                         cfgTrue

/** <!--1723-->
 * Send a byte, word or marker trough the trace.
 *
 * Activate cfgCheckTarce to make use of the this facility.
 * includes genTraceByteInfo(), genTraceWordInfo() and genTraceMarker().
 */
#define  includeGenTrace                         cfgTrue

/** <!--1724-->
 * Auxiliary function to push a Tint16 on a pipe
 */
#define  includeGenPipeInt16                     cfgFalse

/** <!--1725-->
 * Auxiliary function to push a string on a pipe
 */
#define  includeGenPassFlashString               cfgTrue

/* Event functions --------------------------------------------------------- */

/**
 * These functions require activation of the events facility
 */

/** <!--1752-->
 * Lock this task until the required event is fired.
 *
 * Activate cfgUseEvents to make use of the this facility
 */
#define  includeTaskWaitForEvents                cfgTrue

/** <!--1765-->
 * Fire an event from the isr.
 *
 * Activate cfgUseEvents to make use of the this facility.
 */
#define  includeIsrFireEvent                     cfgTrue

/** <!--1766-->
 * Fire an event from any location.
 *
 * Activate cfgUseEvents to make use of the this facility.
 */
#define  includeGenFireEvent                     cfgTrue

/** <!--1753-->
 * Release all tasks waiting on events.
 *
 * Activate cfgUseEvents to make use of the this facility
 */
#define  includeGenFireEventSet                  cfgTrue

/** <!--1767-->
 * Count number of blocks on a particular combination of events.
 *
 * Activate cfgUseEvents to make use of the this facility
 */
#define  includeGenCountEventBlocks              cfgTrue

/* Synchronization functions ----------------------------------------------- */

/**
 * These functions require activation of the synchronization facility
 */

/** <!--1727-->
 * Lock this task until other tasks pass on the same slot.
 *
 * Activate cfgUseSynchronization to make use of the this facility
 */
#define  includeTaskWaitForTasks                 cfgTrue

/** <!--1728-->
 * Release all wait locks on this slot.
 *
 * Activate cfgUseSynchronization to make use of the this facility
 */
#define  includeGenWaitRelease                   cfgTrue

/** <!--1729-->
 * Try to obtain or release a mutex on the given slot.
 *
 * Call taskMutexRequest() and taskMutexRelease()
 * Activate cfgUseSynchronization to make use of the this facility
 */
#define  includeTaskMutex                        cfgTrue

/** <!--1730-->
 * Try to obtain or release a lock on a queu for n bytes.
 *
 * Call taskQueuRequest() and taskQueuRelease()
 * Activate cfgUseSynchronization to make use of the this facility
 */
#define  includeTaskQueu                         cfgTrue

/* Queu functions --------------------------------------------------------- */

/**
 * The Queu functions require activation of the synchronization facility
 */

/** <!--1731-->
 * Write a byte on the queu.
 *
 * Activate cfgUseSynchronization, and have cfgUseAsQueu defined on at
 * least one task to make use of the this facility.
 */
#define  includeGenQueuWrite                     cfgTrue

/** <!--1732-->
 * Read a byte from the queu.
 *
 * Activate cfgUseSynchronization, and have cfgUseAsQueu defined on at
 * least one task to make use of the this facility.
 */
#define  includeGenQueuRead                      cfgTrue

/** <!--1768-->
 * Unwrite a byte on the queu.
 *
 * Activate cfgUseSynchronization, and have cfgUseAsQueu defined on at
 * least one task to make use of the this facility.
 */
#define  includeGenQueuUnwrite                   cfgTrue

/** <!--1769-->
 * Unread a byte from the queu.
 *
 * Activate cfgUseSynchronization, and have cfgUseAsQueu defined on at
 * least one task to make use of the this facility.
 */
#define  includeGenQueuUnread                    cfgTrue

/** <!--1733-->
 * Clear the queu.
 *
 * Activate cfgUseSynchronization, and have cfgUseAsQueu defined on at
 * least one task to make use of the this facility.
 */
#define  includeGenQueuClear                     cfgFalse

/** <!--1734-->
 * Look ahead what the next byte to read would be.
 *
 * Activate cfgUseSynchronization, and have cfgUseAsQueu defined on at
 * least one task to make use of the this facility.
 */
#define  includeGenQueuPeek                      cfgTrue

/** <!--1735-->
 * See how many bytes can be read from the queu.
 *
 * Activate cfgUseSynchronization, and have cfgUseAsQueu defined on at
 * least one task to make use of the this facility.
 */
#define  includeGenQueuReadable                  cfgTrue

/** <!--1736-->
 * See how many bytes can be written to the queu.
 *
 * Activate cfgUseSynchronization, and have cfgUseAsQueu defined on at
 * least one task to make use of the this facility.
 */
#define  includeGenQueuWriteable                 cfgTrue

/** <!--1737-->
 * See if the queu is full.
 *
 * Activate cfgUseSynchronization, and have cfgUseAsQueu defined on at
 * least one task to make use of the this facility.
 */
#define  includeGenQueuFull                      cfgTrue

/** <!--1738-->
 * See if the queu is empty.
 *
 * Activate cfgUseSynchronization, and have cfgUseAsQueu defined on at
 * least one task to make use of the this facility.
 */
#define  includeGenQueuEmpty                     cfgTrue

/* File System functions --------------------------------------------------- */

/**
 * These function give you the opertunity to read to and write from the
 * file system
 */

/** <!--1754-->
 * Open and close a file.
 *
 * Call taskFileOpen() and taskFileClsoe(), and taskFileDirectRead()
 * and taskFileDirectWrite() for unmanaged raw access to the file system.
 * Activate cfgUseFileSystem to make use of the this facility
 */
#define  includeTaskFileAccess                   cfgTrue

/** <!--1764-->
 * Clear the whole file space (formats the FAT too).
 *
 * Activate cfgUseFileSystem to make use of the this facility
 */
#define  includeTaskFileFormat                   cfgTrue

 /** <!--1755-->
 * Get the size of a file.
 *
 * Activate cfgUseFileSystem to make use of the this facility
 */
#define  includeTaskFileGetSize                  cfgTrue

/** <!--1756-->
 * Set the size of a file.
 *
 * Activate cfgUseFileSystem to make use of the this facility
 * Activate cfgUseFileSystemMaintainFAT to make use of the this facility
 */
#define  includeTaskFileSetSize                  cfgFalse

/** <!--1757-->
 * Read a raw byte from a file
 *
 * Activate cfgUseFileSystem to make use of the this facility
 * Activate cfgUseFileSystemMaintainFAT to make use of the this facility
 */
#define  includeTaskFileReadByte                 cfgTrue

/** <!--1758-->
 * Write a raw byte to a file
 *
 * Activate cfgUseFileSystem to make use of the this facility
 */
#define  includeTaskFileWriteByte                cfgTrue

/** <!--1759-->
 * Append a byte to a file
 *
 * Activate cfgUseFileSystem to make use of the this facility
 */
#define  includeTaskFileAppendByte               cfgTrue

/** <!--1760-->
 * Read bytes in a stream from a file
 *
 * Activate cfgUseFileSystem to make use of the this facility
 */
#define  includeTaskFileReadPipe                 cfgTrue

/** <!--1761-->
 * Write bytes in a stream to a file
 *
 * Activate cfgUseFileSystem to make use of the this facility
 */
#define  includeTaskFileWritePipe                cfgTrue

/** <!--1762-->
 * Read bytes from a file and put them in a buffer
 *
 * Activate cfgUseFileSystem to make use of the this facility
 */
#define  includeTaskFileReadBuffer               cfgTrue

/** <!--1763-->
 * Write bytes from the buffer to a file
 *
 * Activate cfgUseFileSystem to make use of the this facility
 */
#define  includeTaskFileWriteBuffer              cfgTrue


/* Protection functions ---------------------------------------------------- */

/**
 * These functions depend on the settings of the configuration parameters
 * set above.
 */

/** <!--1739-->
 * Enable/disable global interrupts.
 *
 * Enable (with taskEnableGlobalInterrupts()) or disable (with taskDisableGlobalInterrupts())
 * global interrupts, and keep track of the interrupt state over a context switch.
 */
#define  includeTaskProtectGlobalInterrupts      cfgTrue

/** <!--1740-->
 * Enable/disable task switching.
 *
 * Enable (with taskEnableSwitchTask()) or disable (with taskDisableSwitchTask())
 * switching of tasks.
 */
#define  includeTaskProtectSwitchTasks           cfgTrue

/** <!--1770-->
 * Enable/disable tick interrupts.
 *
 * Enable (with taskEnableTickInterrupts()) or disable (with taskDisableTickInterrupts())
 * tick interrupts, and keep track of the interrupt state over a context switch.
 */
#define  includeTaskProtectTickInterrupts        cfgTrue

/** <!--1741-->
 * Disable/enable global interrupts, keep track of the level depth.
 *
 * Call taskEnterGlobalCritical() and taskExitGlobalCritical()
 * Activate cfgUseNestedCriticals to make use of the this facility.
 */
#define  includeTaskProtectGlobalCritical        cfgTrue

/** <!--1742-->
 * Disable/disable task switching, keep track of the level depth.
 *
 * Call taskEnterTickCritical() and taskExitTickCritical()
 * Activate cfgUseNestedCriticals to make use of the this facility.
 */
#define  includeTaskProtectSwitchCritical        cfgTrue

/** <!--1771-->
 * Disable/enable tick interrupts, keep track of the level depth.
 *
 * Call taskEnterTickCritical() and taskExitTickCritical()
 * Activate cfgUseNestedCriticals to make use of the this facility.
 */
#define  includeTaskProtectTickCritical          cfgTrue


/* ISR header/footer functions --------------------------------------------- */

/**
 * These functions are needed when use of external interrupts is made.
 * The corresponding capabilities must be activated.
 */

/** <!--1743-->
 * Start the isr by a context switch.
 *
 * Activate cfgIntUserDefined to make use of the this facility.
 */
#define  includeIsrEnter                         cfgTrue

/** <!--1744-->
 * End the isr, switch to the OS.
 *
 * Activate cfgIntUserDefined to make use of the this facility.
 */
#define  includeIsrExit                          cfgTrue

/** <!--1745-->
 * Start the isr, switch to the isr stack.
 *
 * Activate cfgIntUserDefined to make use of the this facility.
 */
#define  includeIsrBegin                         cfgTrue

/** <!--1746-->
 * Stop the isr, return to the place where the interrupt occurred.
 *
 * Activate cfgIntUserDefined to make use of the this facility.
 */
#define  includeIsrEndReturn                     cfgTrue

/** <!--1747-->
 * Stop the isr, yield as quickly as possible.
 *
 * Activate cfgIntUserDefined to make use of the this facility.
 */
#define  includeIsrEndYield                      cfgTrue

/** <!--1748-->
 * Start the stopwatch measuring time inside an unmanaged isr
 *
 * Activate cfgIntUserDefined, cfgIntOsProtected and includeTaskYield
 * to make use of the this facility.
 * If you do not activate cfgUseLoadMonitor the method is replaced
 * by an empty call.
 */
#define  includeIsrStartLoad                     cfgTrue

/** <!--1749-->
 * Stop the stopwatch measuring time inside an unmanaged isr
 *
 * Activate cfgIntUserDefined to make use of the this facility.
 * If you do not activate cfgUseLoadMonitor the method is replaced
 * by an empty call.
 */
#define  includeIsrStopLoad                      cfgTrue

/* quality assurance functions --------------------------------------------- */

/** <!--1750-->
 * Call to check how much free space is left on the task stack.
 */
#define  includeTaskStackCheck                   cfgTrue

/** <!--1751-->
 * Call to check how much free space is left on the isr stack.
 *
 * Depending on the settings of cfgCheckIsrStack, cfgCheckWatermarks and
 * cfgIntSwitchUsesOSstack the call be be replaced by an empty call.
 */
#define  includeIsrStackCheck                    cfgTrue


/* ========================================================================= */
/* TASK NAMES ============================================================== */
/* ========================================================================= */

/** <!--1801-->
 * Specify the names of the tasks.
 *
 * Internally the Femto OS work with task numbers. These run from 0 to
 * defNumberOfTasks-1. The defNumberOfTasks parameter is defined internally,
 * in this list you define which names you will be using. Furthermore, the
 * task number given to a particular task is done in the order defined below,
 * that is, starting with CN_00 CN_01 etc, if they are to be included in the
 * system (see include tasks).
 *
 * The maximum number of tasks is 16. The task names you define here are
 * used below, all handled by the preprocessor, so theses static names do
 * not cost any ram, and need not to be short.
 * There is no need to remove unused parameters.
 */
#define  CN_00                                   task0
#define  CN_01                                   task1
#define  CN_02                                   task2
#define  CN_03                                   task3
#define  CN_04                                   task4
#define  CN_05                                   task5
#define  CN_06                                   task6
#define  CN_07                                   task7
#define  CN_08                                   task8
#define  CN_09                                   task9
#define  CN_10                                   taskA
#define  CN_11                                   taskB
#define  CN_12                                   taskC
#define  CN_13                                   taskD
#define  CN_14                                   taskE
#define  CN_15                                   taskF


/* ========================================================================= */
/* INCLUDE TASKS =========================================================== */
/* ========================================================================= */

/** <!--1822-->
 * Specify the tasks be included and how they are started.
 *
 * You can specify one You can specify one fixed TaskInclude value for all tasks by using
 * TaskIncludeOverride. If you want to specify the TaskInclude setting per task,
 * set TaskIncludeOverride to cfgOverrideNon, and define the value for each
 * task separately. If you define each task as well as the TaskIncludeOverride
 * the latter takes precedence, so the fixed value overrides values specified
 * per task (if unequal to cfgOverrideNon). Defining this value is not required.
 *
 * See TaskInclude_[taskname] for explanation.
 */
#define  TaskIncludeOverride                     cfgOverrideNon


/** <!--1802-->
 * State which tasks are incorporated and how they are started.
 *
 * Here define which tasks will be actually compiled into your system. The
 * task is excluded by Femto OS if you set the parameter to cfgExclude. The
 * other possibilities are cfgStartSleeping,
 * cfgStartSuspended and cfgStartRunning which behave as can be expected
 * by there definition.
 *
 * Using the function preTaskDefined([TaskName]) you can conditionally include
 * your c code as well. If you need to specify a task number to some method
 * don't you numbers directly, since they may differ according to changed in
 * this list, but use the (preprocessor) function preTaskNumberOf([TaskName])
 * instead, so you keep all calls number independent.
 *
 * Note: The preprocessor cannot check if you define includes here for tasks
 * that have not been previously declared with a task name. Usually this is
 * harmless, but the preTaskDefined() may produce false results in this case.
 */
#define  TaskInclude_task0                       cfgStartRunning
#define  TaskInclude_task1                       cfgExclude
#define  TaskInclude_task2                       cfgExclude
#define  TaskInclude_task3                       cfgExclude
#define  TaskInclude_task4                       cfgExclude
#define  TaskInclude_task5                       cfgExclude
#define  TaskInclude_task6                       cfgExclude
#define  TaskInclude_task7                       cfgExclude
#define  TaskInclude_task8                       cfgExclude
#define  TaskInclude_task9                       cfgExclude
#define  TaskInclude_taskA                       cfgExclude
#define  TaskInclude_taskB                       cfgExclude
#define  TaskInclude_taskC                       cfgExclude
#define  TaskInclude_taskD                       cfgExclude
#define  TaskInclude_taskE                       cfgExclude
#define  TaskInclude_taskF                       cfgExclude

/* ========================================================================= */
/* TASK CAPABILITIES ======================================================= */
/* ========================================================================= */

/** <!--1830-->
 * Specify the capabilities of all tasks at once.
 *
 * You can specify one fixed Capabilities value for all tasks by using
 * CapabilitiesOverride. If you want to specify the Capabilities setting per task,
 * set CapabilitiesOverride to cfgOverrideNon, and define the value for each
 * task separately. If you define each task as well as the CapabilitiesOverride
 * the latter takes precedence, so the fixed value overrides values specified
 * per task (if unequal to cfgOverrideNon). Defining this value is not required.
 *
 * See Capabilities_[taskname] for explanation.
 */
#define  CapabilitiesOverride                    cfgOverrideNon

/** <!--1810-->
 * State per task what its capabilities must be.
 *
 * You can choose to specify per task what the capabilities should be.
 * Some of the capabilities (which must be activated above) can be
 * activated on a task by task basis. The preprocessor can generate
 * more optimal code in some cases. Also, you usually use less ram that
 * way. Also some runtime optimization takes place.
 *
 * You can choose from the following capabilities:
 *  cfgCapWatchdog, cfgCapEvent, cfgCapCritical,  cfgCapSynchronization,
 *  cfgCapFileSystem, cfgCapTimeout, cfgCapDelay, cfgCapAll, cfgCapNon.
 * (cfgCapCritical is only needed for nested critical sections)
 *
 * It is always safe (and usually more convenient while developing) to
 * set all Capabilities to cfgCapAll. Omitting to specify a capability
 * which is used in a particular task may result bugs that are very hard
 * to find, since some ram locations will be used for multiple purposes
 * in that case. Specifying values here other than cfgCapAll for all tasks
 * is on of the last optimizations you should perform. When checkMethodUse
 * is activated Femto OS checks if you specified the correct capabilities
 * on every API call when applicable.
 *
 * Only firing an event from a task does not require the task to have event
 * capabilities, but waiting for an event does.
 */
#define  Capabilities_task0                      cfgCapDelay | cfgCapWatchdog | cfgCapEvent | cfgCapCritical | cfgCapSynchronization | cfgCapFileSystem
#define  Capabilities_task1                      cfgCapDelay | cfgCapWatchdog | cfgCapEvent | cfgCapCritical | cfgCapSynchronization | cfgCapFileSystem | cfgCapTimeout
#define  Capabilities_task2                      cfgCapDelay | cfgCapWatchdog | cfgCapEvent | cfgCapCritical | cfgCapSynchronization | cfgCapFileSystem | cfgCapTimeout
#define  Capabilities_task3                      cfgCapDelay | cfgCapWatchdog | cfgCapEvent | cfgCapCritical | cfgCapSynchronization | cfgCapFileSystem | cfgCapTimeout
#define  Capabilities_task4                      cfgCapDelay | cfgCapWatchdog | cfgCapEvent | cfgCapCritical | cfgCapSynchronization | cfgCapFileSystem | cfgCapTimeout
#define  Capabilities_task5                      cfgCapDelay | cfgCapWatchdog | cfgCapEvent | cfgCapCritical | cfgCapSynchronization | cfgCapFileSystem | cfgCapTimeout
#define  Capabilities_task6                      cfgCapDelay | cfgCapWatchdog | cfgCapEvent | cfgCapCritical | cfgCapSynchronization | cfgCapFileSystem | cfgCapTimeout
#define  Capabilities_task7                      cfgCapDelay | cfgCapWatchdog | cfgCapEvent | cfgCapCritical | cfgCapSynchronization | cfgCapFileSystem | cfgCapTimeout
#define  Capabilities_task8                      cfgCapDelay | cfgCapWatchdog | cfgCapEvent | cfgCapCritical | cfgCapSynchronization | cfgCapFileSystem | cfgCapTimeout
#define  Capabilities_task9                      cfgCapDelay | cfgCapWatchdog | cfgCapEvent | cfgCapCritical | cfgCapSynchronization | cfgCapFileSystem | cfgCapTimeout
#define  Capabilities_taskA                      cfgCapDelay | cfgCapWatchdog | cfgCapEvent | cfgCapCritical | cfgCapSynchronization | cfgCapFileSystem | cfgCapTimeout
#define  Capabilities_taskB                      cfgCapDelay | cfgCapWatchdog | cfgCapEvent | cfgCapCritical | cfgCapSynchronization | cfgCapFileSystem | cfgCapTimeout
#define  Capabilities_taskC                      cfgCapDelay | cfgCapWatchdog | cfgCapEvent | cfgCapCritical | cfgCapSynchronization | cfgCapFileSystem | cfgCapTimeout
#define  Capabilities_taskD                      cfgCapDelay | cfgCapWatchdog | cfgCapEvent | cfgCapCritical | cfgCapSynchronization | cfgCapFileSystem | cfgCapTimeout
#define  Capabilities_taskE                      cfgCapDelay | cfgCapWatchdog | cfgCapEvent | cfgCapCritical | cfgCapSynchronization | cfgCapFileSystem | cfgCapTimeout
#define  Capabilities_taskF                      cfgCapDelay | cfgCapWatchdog | cfgCapEvent | cfgCapCritical | cfgCapSynchronization | cfgCapFileSystem | cfgCapTimeout



/* ========================================================================= */
/* STACK SIZES ============================================================= */
/* ========================================================================= */

/**
 * Here you define the different stack size. And, whatever you do, make sure
 * you run your code with stack protection first.
 */

/** <!--1A01-->
 * Specify the free space required on the stack when checking.
 *
 * This parameter defines the 'danger zone' on the task stacks when
 * checking. Thus, if your task should not require no more than 14 bytes,
 * but you fear it may go as far as 18, define your task stack to be 18 bytes
 * and set StackSafety to 4, it will report an error as soon as it detects
 * values are pushed onto the stack beyond 14 bytes. The task will then
 * be stopped. However, if the surplus stayed with the 18 byte limit, other
 * tasks are not effected. StackSafety may be set to zero,
 * but if you set it to some (small) positive value, this increases the
 * chance the check method catches an error before some the buggy behavior.
 *
 * When your code is finished, and runs bug free for some time, then set
 * StackSafety to zero and reduce all task sizes by the same amount. If then
 * still no errors are reported, you may switch of the cfgCheckTaskStack
 * and start working on the size of the OS stack.
 *
 * This value is not added when checking the OS or ISR stack.
 * Please note that the StackSafety parameter only has effect on the
 * checking itself, it does not create any extra stack or use any ram.
 * The value is limited 255 bytes, even when larger stacks are used.
 * Note that, since the whole safety area is checked at every context
 * switch to see if there is no offensive use, keep the numbers
 * reasonable, or you will notice a degraded performance.
 */
#define  StackSafety                             4

/** <!--1A02-->
 * Specify the stack of the OS.
 *
 * The Femto OS is not really stack hungry and the stack can also be used to hold a
 * number of background variables when the tasks are running. The code for these
 * background variables never uses less than 7 bytes with all protection switched
 * off, and never usesmore than 14 bytes with protection switched on. However, please
 * experiment with the different values and have cfgCheckOsStack == cfgTrue while
 * testing. This is because the stack itself may need to be larger. The StackSizeOS
 * may not be larger than 255.
 *
 * If you have activated checks, and did not treat all errors as fatal, some
 * more OS stack is needed to recover from the error. Typically take 32 bytes
 * as start value.
 */
#define  StackSizeOS                             20

/** <!--1A03-->
 * Specify the stack of the isr.
 *
 * Specify the stack size of the interrupt service routine. Such a stack is
 * not always needed, even when using interrupts. Basically you only need it
 * if you want to allow interrupts while in OS space. If you don't, thus have
 * cfgIntOsProtected == cfgTrue, and use isrBegin - isrEnd constructions you
 * also need a separate stack. If you only use isrEnter - isrExit
 * constructions,  i.e. entering the isr with context switching, you have
 * the choice, see the option cfgIntSwitchUsesOSstack.
 *
 * Check your stack manually with includeIsrStackCheck. Note that the
 * non-switching isr's can be quite stack hungry, since it must push
 * all registers is uses, plus some more.
 * Value must be between 0 and 32767 (inclusive) and is in fact limited
 * by the amount or ram the device has.
 */
#define  StackSizeISR                            8

/** <!--1A07-->
 * Specify the size of the shared stack
 *
 * Cooperative tasks that only use a stack, but leave nothing on that stack,
 * may use a shared stack. Since the task is left in a controlled fashion
 * we can be sure that only the return address of the stack matters. That
 * address is stored on a save place by the OS before the stack is used by
 * an other task.
 *
 * Cooperative stacks may not be interrupted by the tick Thus deactivate
 * tick interrupts at startup for that task. And they may not be interrupted
 * by an other switching interrupt. Thus deactivate global interrupts for
 * this task or make sure no switching interrupts are used.
 *
 * Tasks that want to make use of the shared stack, must define there stacksize
 * as cfgShared. And, of course,  If no task makes use of the shared stack,
 * define 0 here.
 *
 * Note that making use of a shared stack leads to extra code. Therefore it is
 * most beneficial when all tasks are cooperative and share the stack, or when
 * the tasks that have large stacks shared them.
 */
#define  StackSizeShared                         0


/** <!--1823-->
 * Specify the stack size for all tasks at once.
 *
 * You can specify one fixed StackSize value for all tasks by using
 * StackSizeOverride. If you want to specify the StackSize setting per task,
 * set StackSizeOverride to cfgOverrideNon, and define the value for each
 * task separately. If you define each task as well as the StackSizeOverride
 * the latter takes precedence, so the fixed value overrides values specified
 * per task (if unequal to cfgOverrideNon). Defining this value is not required.
 *
 * This fixed value is not used to define StackSizeISR or StackSizeOS.
 * See StackSize_[taskname] for explanation.
 */
#define  StackSizeOverride                       cfgOverrideNon

/** <!--1803-->
 * Specify the stack of each task.
 *
 * This is the most important place. Here you decide between life an death.
 * How much stack should u give a particular task? First, make sure that,
 * whatever you do, run protected first. Really! Now, these are point to
 * take into consideration.
 * -- Two nested calls are necessary to reach the saveContext, thus the minimum
 *    value is 4 bytes, or whatever that takes on you architecture, even if you
 *    save nothing on the stack. Two of these bytes are removed again, the
 *    other contain the return address and are left in place.
 * -- Interrupts are disabled after the first call, so if you yield manually
 *    there is a slim (but non zero chance) the timer interrupt comes just after
 *    the call, and from that point on, 4 bytes are the minimum again, so with
 *    manual yielding, blocking delaying etc, add two bytes to your calculation.
 *    This risk is absent when the task run's 'forever', and is only switched
 *    at a timer interrupt.
 * -- You need one byte for the status register.
 * -- You need 4 bytes per group of 4 registers saved on the context
 * -- If you use any non switching calls to the OS, they usually disable interrupts
 *    after the (nested) call. If the function takes parameters, they are usually
 *    pushed on the stack in between. Count one byte per eight bit parameter.
 * -- Count your own nested calls!
 * -- Note that the testing the Femto OS performs for you only takes account
 *    of the regular depth encountered. However, in real life eventually all
 *    instructions  will be subject to an interrupt / context switch sometime.
 *    Also those you did not encountered on you lab table. Thus, the weird behavior
 *    may start weeks after you installed everything. Beware!
 * -- Have a look at the assembler code! Even if you don't really understand
 *    what is happening inside, calculating the stack depth is relatively easy.
 *    Certainly you know the depth a some locations.
 *
 * There is no need to remove unused parameters, if you deactivate the task, the
 * stack is automatically excluded.
 *
 * If all StackSize values are below 256, i.e. fit in one byte, only one byte
 * is used per task to store the stack level at context switch. Stacks may be larger
 * as 255 bytes however. 32767 at maximum.. In that case two bytes are needed per task to store
 * the information.
 */
#define  StackSize_task0                         8
#define  StackSize_task1                         4
#define  StackSize_task2                         4
#define  StackSize_task3                         4
#define  StackSize_task4                         4
#define  StackSize_task5                         4
#define  StackSize_task6                         4
#define  StackSize_task7                         4
#define  StackSize_task8                         4
#define  StackSize_task9                         4
#define  StackSize_taskA                         4
#define  StackSize_taskB                         4
#define  StackSize_taskC                         4
#define  StackSize_taskD                         4
#define  StackSize_taskE                         4
#define  StackSize_taskF                         4


/* ========================================================================= */
/* TIMES SLICES ============================================================ */
/* ========================================================================= */

/** <!--1A04-->
 * Specify how much run time the idle time gets.
 *
 * The TimeSliceIdleTime must be specified and and may differ
 * from the time other tasks get. For a detailed explanation how this works
 * see  TimeSlice_[taskname].
 *
 * In case of cfgUseEquidistantTicks == cfgTrue
 * you need not specify anything since every tick is equal and the
 * tick may be shared among several tasks.
 * In that case the value specified here is ignored.
 */
#define  TimeSliceIdleTime                      cfgSysSubTicksPerFullTick

/** <!--1824-->
 * Specify the time slices for all tasks at once.
 *
 * You can specify one fixed TimeSlice value for all tasks by using
 * TimeSliceOverride. If you want to specify the TimeSlice setting per task,
 * set TimeSliceOverride to cfgOverrideNon, and define the value for each
 * task separately. If you define each task as well as the TimeSliceOverride
 * the latter takes precedence, so the fixed value overrides values specified
 * per task (if unequal to cfgOverrideNon). Defining this value is not required.
 *
 * This fixed value is not used to define TimeSliceIdleTime.
 * In that case the value specified here is ignored.
 * See TimeSlice_[taskname] for explanation.
 */
#define  TimeSliceOverride                       cfgOverrideNon

/** <!--1804-->
 * Specify how much run time each task gets per turn.
 *
 * Here you determine how many subticks every task may run before the tick
 * interrupt comes. If you choose for cfgSysSubTicksPerFullTick, the task
 * get one full tick at most. If you choose a higher number it may run for
 * more that one tick. The tick counter gets back (lag) but after the task
 * is interrupted this is corrected. Note that the minimum value is
 * defMinTimeSlice and the maximum is approx 0xFF - cfgSysSubTicksPerFullTick
 * since the subtick counter should not overflow, in that case a many ticks
 * are missed. This safety margin is for interrupts and other extra's (some
 * OS operations etc). Many timers cannot handle 0 and it makes not much
 * sense too.
 *
 * These variable time slices only work when, cfgUseEquidistantTicks == cfgFalse
 * except for the TimeSliceIdleTime, which must be set independently. Since
 * you can put the system in some power save mode during idle time too
 * (make sure timer interrupts work) it may make sense to put a longer
 * period there. This mechanism is for busses for example which require
 * and undisturbed time to run.
 *
 * In case of cfgUseEquidistantTicks == cfgTrue
 * you need not specify anything since every tick is equal and the
 * tick may be shared among several tasks. In that case the values specified
 * here are ignored.
 */
#define  TimeSlice_task0                         cfgSysSubTicksPerFullTick
#define  TimeSlice_task1                         cfgSysSubTicksPerFullTick
#define  TimeSlice_task2                         cfgSysSubTicksPerFullTick
#define  TimeSlice_task3                         cfgSysSubTicksPerFullTick
#define  TimeSlice_task4                         cfgSysSubTicksPerFullTick
#define  TimeSlice_task5                         cfgSysSubTicksPerFullTick
#define  TimeSlice_task6                         cfgSysSubTicksPerFullTick
#define  TimeSlice_task7                         cfgSysSubTicksPerFullTick
#define  TimeSlice_task8                         cfgSysSubTicksPerFullTick
#define  TimeSlice_task9                         cfgSysSubTicksPerFullTick
#define  TimeSlice_taskA                         cfgSysSubTicksPerFullTick
#define  TimeSlice_taskB                         cfgSysSubTicksPerFullTick
#define  TimeSlice_taskC                         cfgSysSubTicksPerFullTick
#define  TimeSlice_taskD                         cfgSysSubTicksPerFullTick
#define  TimeSlice_taskE                         cfgSysSubTicksPerFullTick
#define  TimeSlice_taskF                         cfgSysSubTicksPerFullTick

/* ========================================================================= */
/* RUNTIME TASK NAMES ====================================================== */
/* ========================================================================= */

/** <!--1805-->
 * Specify dynamic task names.
 *
 * For some applications it may be handy to have task names. Femto OS does
 * not use them internally, and of course these names consume resources.
 * Set cfgUseTasknames == cfgTrue to include the names in flash.
 * The names are unrelated to the static names.
 *
 * There is no possibility to define a fixed task name.
 */
#define  TaskName_task0                          "taak nul"
#define  TaskName_task1                          "taak een"
#define  TaskName_task2                          "taak twee"
#define  TaskName_task3                          "taak drie"
#define  TaskName_task4                          "taak vier"
#define  TaskName_task5                          "taak vijf"
#define  TaskName_task6                          "taak_zes"
#define  TaskName_task7                          "taak_zeven"
#define  TaskName_task8                          "taak_acht"
#define  TaskName_task9                          "taak_negen"
#define  TaskName_taskA                          "taak_tien"
#define  TaskName_taskB                          "taak_elf"
#define  TaskName_taskC                          "taak_twaalf"
#define  TaskName_taskD                          "taak_dertien"
#define  TaskName_taskE                          "taak_veertien"
#define  TaskName_taskF                          "taak_vijftien"

/* ========================================================================= */
/* TASK PRIORITIES ========================================================= */
/* ========================================================================= */

/** <!--1826-->
 * Specify the priorities for all tasks at once.
 *
 * You can specify one fixed priority value for all tasks by using
 * PriorityOverride. If you want to specify the priority setting per task,
 * set PriorityOverride to cfgOverrideNon, and define the value for each
 * task separately. If you define each task as well as the PriorityOverride
 * the latter takes precedence, so the fixed value overrides values specified
 * per task (if unequal to cfgOverrideNon). Defining this value is not required.
 *
 * See Priority_[taskname] for further explanation.
 */
#define  PriorityOverride                        cfgOverrideNon

/** <!--1806-->
 * Specify the initial priority of each task.
 *
 * Each task starts in a predefined priority 0..7. zero being the lowest
 * priority. All tasks in one priority run round robin, the lowest task number
 * runs first. If in one priority there is one (or more) non blocking task(s),
 * this tasks starves all tasks in lower priorities. This is not a problem
 * from the systems point of view. The idle task runs in a (virtual) priority
 * -1 which cannot be shared with any other task. Which and how many tasks
 * run in a priority does not influence the use of resources. Priorities
 * can be changed at runtime.
 */
#define  Priority_task0                          0
#define  Priority_task1                          3
#define  Priority_task2                          2
#define  Priority_task3                          3
#define  Priority_task4                          2
#define  Priority_task5                          1
#define  Priority_task6                          1
#define  Priority_task7                          1
#define  Priority_task8                          0
#define  Priority_task9                          0
#define  Priority_taskA                          0
#define  Priority_taskB                          0
#define  Priority_taskC                          0
#define  Priority_taskD                          0
#define  Priority_taskE                          0
#define  Priority_taskF                          0


/* ========================================================================= */
/* REGISTER USE ============================================================ */
/* ========================================================================= */

/** <!--1827-->
 * Specify the registers used for all tasks at once.
 *
 * You can specify one fixed RegisterUse value for all tasks by using
 * RegisterUseOverride. If you want to specify the RegisterUse setting per task,
 * set RegisterUseOverride to cfgOverrideNon, and define the value for each
 * task separately. If you define each task as well as the RegisterUseOverride
 * the latter takes precedence, so the fixed value overrides values specified
 * per task (if unequal to cfgOverrideNon). Defining this value is not required.
 *
 * See RegisterUse_[taskname] for further explanation.
 */
#define  RegisterUseOverride                     cfgOverrideNon

/** <!--1807-->
 * Specify which registers are used by each task.
 *
 * The AVR is one example of hardware which contains many registers, most
 * of which you probably don't use. So why save them on the context then?
 * It takes a lot of ram and time too, so we save only those needed.
 * This is called RegisterCompression and works per default.
 *
 * Specify which registers are used in the tasks. These are defined in blocks
 * of four registers. If no use of a register is made, it need not to be
 * saved on the context. If a particular task uses no registers at all,
 * use registersNon, is all registers are used use registersAll.
 *
 * If you have the space it may be wise to develop with all registers saved.
 * Registers only used inside critical sections (and this includes most
 * functions of the Femto OS api) formally need not to be
 * saved. However, this cannot be checked, thus it may be hard to determine
 * if you save enough registers.
 */
#define  RegisterUse_task0                       registersNon
#define  RegisterUse_task1                       r00r01r02r03 | r04r05r06r07
#define  RegisterUse_task2                       r08r09r10r11 | r12r13r14r15
#define  RegisterUse_task3                       r16r17r18r19 | r20r21r22r23
#define  RegisterUse_task4                       r24r25r26r27 | r28r29r30r31
#define  RegisterUse_task5                       r00r01r02r03
#define  RegisterUse_task6                       r04r05r06r07
#define  RegisterUse_task7                       r08r09r10r11
#define  RegisterUse_task8                       r12r13r14r15
#define  RegisterUse_task9                       r16r17r18r19
#define  RegisterUse_taskA                       r20r21r22r23
#define  RegisterUse_taskB                       r24r25r26r27
#define  RegisterUse_taskC                       r28r29r30r31
#define  RegisterUse_taskD                       registersAll
#define  RegisterUse_taskE                       registersAll
#define  RegisterUse_taskF                       registersAll


/* ========================================================================= */
/* REGISTER CHECK ========================================================== */
/* ========================================================================= */

/** <!--1828-->
 * Specify the registers to be checked for all tasks at once.
 *
 * You can specify one fixed RegisterCheck value for all tasks by using
 * RegisterCheckOverride. If you want to specify the RegisterCheck setting per task,
 * set RegisterCheckOverride to cfgOverrideNon, and define the value for each
 * task separately. If you define each task as well as the RegisterCheckOverride
 * the latter takes precedence, so the fixed value overrides values specified
 * per task (if unequal to cfgOverrideNon). Defining this value is not required.
 *
 * See RegisterCheck_[taskname] for further explanation.
 */
#define  RegisterCheckOverride                   cfgOverrideNon

/** <!--1808-->
 * Specify which registers should be checked on each context switch.
 *
 * Specify which registers you want to have checked. Registers that are defined
 * as 'used' above are never checked, so you do not need to exclude them. Thus
 * normally, you would set this to registersAll. However, it may be the case that
 * you know some registers are changed in places where a tick interrupt cannot
 * take place. Such a registers need not to be saved on the context, but may still
 * changed, and thus trigger an error when cfgCheckRegisters is set to cgfTrue.
 * In that case you may exclude such registers here. Even if they are changed, it
 * will not trigger an error. Of course, at your own responsibility. If the register
 * may turn out to be important after all, the system may crash.
 *
 * These values must be provided if you use cfgCheckRegisters, per default
 * choose for registersAll.
 */
#define  RegisterCheck_task0                     registersAll
#define  RegisterCheck_task1                     registersAll
#define  RegisterCheck_task2                     registersAll
#define  RegisterCheck_task3                     registersAll
#define  RegisterCheck_task4                     registersAll
#define  RegisterCheck_task5                     registersAll
#define  RegisterCheck_task6                     registersAll
#define  RegisterCheck_task7                     registersAll
#define  RegisterCheck_task8                     registersAll
#define  RegisterCheck_task9                     registersAll
#define  RegisterCheck_taskA                     registersAll
#define  RegisterCheck_taskB                     registersAll
#define  RegisterCheck_taskC                     registersAll
#define  RegisterCheck_taskD                     registersAll
#define  RegisterCheck_taskE                     registersAll
#define  RegisterCheck_taskF                     registersAll


/* ========================================================================= */
/* INTERRUPT SETTING ======================================================= */
/* ========================================================================= */

/** <!--1829-->
 * Specify the interrupts activated for all tasks at once.
 *
 * You can specify one fixed InterruptStart value for all tasks by using
 * InterruptStartOverride. If you want to specify the InterruptStart setting per task,
 * set InterruptStartOverride to cfgOverrideNon, and define the value for each
 * task separately. If you define each task as well as the InterruptStartOverride
 * the latter takes precedence, so the fixed value overrides values specified
 * per task (if unequal to cfgOverrideNon). Defining this value is not required.
 *
 * See InterruptStart_[taskname] for further explanation.
 */
#define  InterruptStartOverride                  cfgOverrideNon

/** <!--1809-->
 * Specify which interrupts must be activated per task at startup.
 *
 * Specify the interrupt startup value per task. These values are used only
 * when InterruptStartOverride is set to cfgOverrideNon, any other value takes
 * precedence over the definitions per task.
 *
 * Defining the tick interrupts is only useful when cfgIntTickTrack is set to
 * cfgTrue, otherwise tick interrupts are always switched on per default. If
 * cfgIntGlobalOnly is defined to cfgTrue, it makes no sense to set cfgIntTickTrack
 * to cfgTrue since all tick interrupt changing operation are mapped to the
 * global interrupt. If you however still decide to do so make sure you
 * activate tick interrupts per task, since there is no possibility to do
 * so via the OS in a later stage any more.
 *
 * If cfgIntTickTrack is cfgFalse the setting of cfgTickSet / cfgTickClear is
 * irrelevant, since timer interrupts are always activated at the start of a task,
 * or after a context switch. However, since these settings are also relevant to
 * choose between cooperative and preemptive tasks, the setting if cfgTick is
 * mapped to cfgGlob when cfgIntGlobalOnly is defined to cfgTrue, making them
 * of importance after all. Thus, if you want to work cooperatively for a
 * task, and cfgIntGlobalOnly is set, you must still define the task to be
 * cfgTickClear.
 */
#define  InterruptStart_task0                    cfgGlobSet   | cfgTickSet
#define  InterruptStart_task1                    cfgGlobSet   | cfgTickSet
#define  InterruptStart_task2                    cfgGlobSet   | cfgTickSet
#define  InterruptStart_task3                    cfgGlobSet   | cfgTickSet
#define  InterruptStart_task4                    cfgGlobClear | cfgTickSet
#define  InterruptStart_task5                    cfgGlobClear | cfgTickSet
#define  InterruptStart_task6                    cfgGlobClear | cfgTickSet
#define  InterruptStart_task7                    cfgGlobClear | cfgTickSet
#define  InterruptStart_task8                    cfgGlobSet   | cfgTickClear
#define  InterruptStart_task9                    cfgGlobSet   | cfgTickClear
#define  InterruptStart_taskA                    cfgGlobSet   | cfgTickClear
#define  InterruptStart_taskB                    cfgGlobSet   | cfgTickClear
#define  InterruptStart_taskC                    cfgGlobClear | cfgTickClear
#define  InterruptStart_taskD                    cfgGlobClear | cfgTickClear
#define  InterruptStart_taskE                    cfgGlobClear | cfgTickClear
#define  InterruptStart_taskF                    cfgGlobClear | cfgTickClear


/* ========================================================================= */
/* EVENT NAMES ============================================================= */
/* ========================================================================= */

/** <!--1811-->
 * Specify the names of the events.
 *
 * Femto OS has 8 events for free use, when cfgUseEvents is activated. Here
 * they can be given (compile time) names. You would normally use events only
 * through the genFireEventOnName() function. This is because on the avr,
 * only the firing of single constant events generate an atomic instruction.
 * Therefore such a call does not need to be interrupt protected.
 */
#define  EN_00                                   event0
#define  EN_01                                   event1
#define  EN_02                                   event2
#define  EN_03                                   event3
#define  EN_04                                   event4
#define  EN_05                                   event5
#define  EN_06                                   event6
#define  EN_07                                   event7


/* ========================================================================= */
/* SLOT NAMES ============================================================== */
/* ========================================================================= */

/** <!--1901-->
 * Specify the names of the slots.
 *
 * Femto OS has 15 slots for free use, when cfgUseSynchronization is activated
 * they do not use ram by themselves. The slots are numbered 1..15 internally,
 * number 0 is reserved to indicate a particular lock is free. Below you
 * can specify names for each slot. You need not to specify all.  The
 * function preSlotNumberOf([SlotName]) provides you with the possibility to
 * find the number to a slot, needed in the Femto OS API.,
 */
#define  SN_01                                   slot1
#define  SN_02                                   slot2
#define  SN_03                                   slot3
#define  SN_04                                   slot4
#define  SN_05                                   slot5
#define  SN_06                                   slot6
#define  SN_07                                   slot7
#define  SN_08                                   slot8
#define  SN_09                                   slot9
#define  SN_10                                   slotA
#define  SN_11                                   slotB
#define  SN_12                                   slotC
#define  SN_13                                   slotD
#define  SN_14                                   slotE
#define  SN_15                                   slotF


/* ========================================================================= */
/* SLOT SIZES ============================================================== */
/* ========================================================================= */

/** <!--1922-->
 * Specify the slot sizes for all tasks at once.
 *
 * You can specify one fixed SlotSize value for all tasks by using
 * SlotSizeOverride. If you want to specify the SlotSize setting per task,
 * set SlotSizeOverride to cfgOverrideNon, and define the value for each
 * task separately. If you define each task as well as the SlotSizeOverride
 * the latter takes precedence, so the fixed value overrides values specified
 * per task (if unequal to cfgOverrideNon). Defining this value is not required.
 *
 * See SlotSize_[taskname] for explanation.
 */
#define  SlotSizeOverride                        cfgOverrideNon

/** <!--1902-->
 * Specify the size of each slot stack per task.
 *
 * You can specify per task how many simultaneous slots you need to use. To
 * determine how many slots you need, and how much space this will take, you
 * must realize the following:
 * -- A slot can be locked or blocked by a task, or both. Every time a task
 *    locks a slot 4 bits are needed to store the slo tvalue. Two locks occupy
 *    one byte. Nesting of a synchronization primitive is realized by locking
 *    the same slot multiple times.
 * -- Locks are internally stored in the so called slot stack. The slot stack
 *    may contain free locks (i.e. not blocked) only or may hold blocking slots.
 *    A task can block on two slots at most, which are always stored at the
 *    bottom of the stack.
 * -- The mode in which the synchronization is placed determines how the
 * slot stack is understood. The possible modes are: cfgSyncNon,
 * cfgSyncSingleSlot, cfgSyncSingleBlock and cfgSyncDoubleBlock.
 *
 * See also the explanation at cfgUseSynchronization.
 * -- cfgSyncNon:         No slots can be used, it makes no sense to define
 *                        space for them. Set all values below to 0.
 * -- cfgSyncSingleSlot:  Only one slot per task is utilized. Values below
 *                        can be 0 or 1. One byte is occupied for every task
 *                        that makes use of a slot.
 * -- cfgSyncSingleBlock: Every task may only block on one slot, but may hold
 *                        one or more non blocking slots. Specify as many slots
 *                        as you need. The number of free slots the total
 *                        number, or the total minus one if a block is used.
 *                        The minimum number of slots in this case is 2,
 *                        (requires 1 byte)
 * --cfgSyncDoubleBlock:  Every task may block on one or two slots, and may
 *                        hold one or more non blocking slots. Specify as many
 *                        slots as you need. The number of free slots the total
 *                        number, or the total minus two if blocks are used.
 *                        Since in this situation, even in case a single block
 *                        is used, two slot places are reserved, no free locks
 *                        are possible any more. Thus the minimum for this option
 *                        is 4 slots (requiring 2 bytes)
 *
 * There does not exist a memory difference between 3 and 4 slots, or 5 and 6
 * slots. So you made round up to the next even number (except in case of one slot)
 * Tasks that do not need slots get a 0. Note the every nesting for the same
 * slot counts as one, so if you use a lock and reenter it in the same task, that
 * task needs two slots. Please note that any task that makes use of the file
 * system need at least one. At the moment the size maximum is 14 (allowing
 * for 14 simultaneous slots.
 *
 * Important: The preprocessor can generate much shorter code when all tasks
 * that use slots have the same size. So, besides 0 use one other size if
 * possible. Of course, you must weight that against the use of ram.
 */
#define  SlotSize_task0                          4
#define  SlotSize_task1                          4
#define  SlotSize_task2                          4
#define  SlotSize_task3                          4
#define  SlotSize_task4                          4
#define  SlotSize_task5                          4
#define  SlotSize_task6                          4
#define  SlotSize_task7                          4
#define  SlotSize_task8                          4
#define  SlotSize_task9                          4
#define  SlotSize_taskA                          4
#define  SlotSize_taskB                          4
#define  SlotSize_taskC                          4
#define  SlotSize_taskD                          4
#define  SlotSize_taskE                          4
#define  SlotSize_taskF                          4


/* ========================================================================= */
/* SLOT USAGE ============================================================== */
/* ========================================================================= */

/** <!--1923-->
 * Specify the slot use for all tasks at once.
 *
 * You can specify one fixed SlotUse value for all tasks by using
 * SlotUseOverride. If you want to specify the SlotUse setting per task,
 * set SlotUseOverride to cfgOverrideNon, and define the value for each
 * task separately. If you define each task as well as the SlotUseOverride
 * the latter takes precedence, so the fixed value overrides values specified
 * per task (if unequal to cfgOverrideNon). Defining this value is not required.
 *
 * See SlotUse_[slotname] for explanation.
 */
#define  SlotUseOverride                         cfgOverrideNon

/** <!--1903-->
 * State how you will utilize each slot.
 *
 * Specify how you want to utilize your slots. You can choose from cfgUseAsWait,
 * cfgUseAsMutex, cfgUseAsQueu, cfgUseAsNon. Apart from the queus, slots do not
 * take space by themselves, and the difference between Wait (a rendezvous) Mutex
 * and Non is only in the way the use is checked.  In your code you can test the
 * use of a slot using the functions  preWaitDefined([SlotName]),
 * preMutexDefined([SlotName]) or  preQueuDefined([SlotName]) to see if the particular
 * slot was defined in that type.
 *
 * The slots the queus take are always the ones
 * numbered * 1 .. defNumberOfQueus (internally). This does not bother you much,
 * but must be kept in mind is you want to address the queus numerically for a
 * reason. Do not mix the use of slots.
 *
 * Note: The preprocessor cannot check if you define uses here for slots
 * that have not been previously declared with a slot name. Usually this is
 * harmless, but the preQueuDefined(), preMutexDefined() and preWaitDefined() may
 * produce false results in this case.
 */
#define  SlotUse_slot1                           cfgUseAsQueu
#define  SlotUse_slot2                           cfgUseAsMutex
#define  SlotUse_slot3                           cfgUseAsWait
#define  SlotUse_slot4                           cfgUseAsNon
#define  SlotUse_slot5                           cfgUseAsNon
#define  SlotUse_slot6                           cfgUseAsNon
#define  SlotUse_slot7                           cfgUseAsNon
#define  SlotUse_slot8                           cfgUseAsNon
#define  SlotUse_slot9                           cfgUseAsNon
#define  SlotUse_slotA                           cfgUseAsNon
#define  SlotUse_slotB                           cfgUseAsNon
#define  SlotUse_slotC                           cfgUseAsNon
#define  SlotUse_slotD                           cfgUseAsNon
#define  SlotUse_slotE                           cfgUseAsNon
#define  SlotUse_slotF                           cfgUseAsNon


/* ========================================================================= */
/* QUEU SIZES ============================================================== */
/* ========================================================================= */

/** <!--1924-->
 * Specify the sizes for all queues at once.
 *
 * You can specify one fixed QueuSize value for all tasks by using
 * QueuSizeOverride. If you want to specify the QueuSize setting per task,
 * set QueuSizeOverride to cfgOverrideNon, and define the value for each
 * task separately. If you define each task as well as the QueuSizeOverride
 * the latter takes precedence, so the fixed value overrides values specified
 * per task (if unequal to cfgOverrideNon). Defining this value is not required.
 *
 * See QueuSize_[slotname] for explanation.
 */
#define  QueuSizeOverride                        cfgOverrideNon

/** <!--1904-->
 * Specify the size of each queu.
 *
 * Define the sizes of the queus you want to use. Each Queu occupies a slot.
 * The queus of course use resources, for each queue the number of bytes you
 * define (size) plus three extra, which hold the information about locking
 * reading and writing pointers. The maximum size per queue is 127 bytes.
 */
#define  QueuSize_slot1                          8
#define  QueuSize_slot2                          10
#define  QueuSize_slot3                          10
#define  QueuSize_slot4                          10
#define  QueuSize_slot5                          10
#define  QueuSize_slot6                          10
#define  QueuSize_slot7                          10
#define  QueuSize_slot8                          10
#define  QueuSize_slot9                          10
#define  QueuSize_slotA                          10
#define  QueuSize_slotB                          10
#define  QueuSize_slotC                          10
#define  QueuSize_slotD                          10
#define  QueuSize_slotE                          10
#define  QueuSize_slotF                          10


/* ========================================================================= */
/* FILE NAMES ============================================================== */
/* ========================================================================= */

/** <!--1-->
 * Below you can parameterize the file system, if you need one. This is a
 * very basic file system defined on the local EEPROM or alike. Femto OS
 * protects the EEPROM from simultaneous access and has a multiple read,
 * single write synchronizer inside. However, if you large write files in
 * parts, so that you need multiple access to the write functions you need
 * to protect access in between yourself using mutexes. YOu can of course also
 * keep the file open while writing the parts.
 *
 * All files have compile time names (like the tasks) and compile time maximal
 * lengths. Thus the whole format of the storage space is known at compile
 * time. Fragmentation is not possible.
 * Note that if you change any of the parameters below, an already present
 * FS becomes corrupted and must be reformatted. (Simply whipe the EEPROM
 * at flash time).
 *
 * The FS can also get corrupted because of two other reasons.
 *
 * First, if the power goes down while the EEPROM is in a write cycle the CPU
 * might not complete writing the last byte. Since the Femto OS cannot protect
 * you from such a situation, you must take specific hardware measures. For example,
 * activate the brownout detector, so that the device will know it has to complete
 * quickly. Normally that should prevent you from bytes that where written
 * incorrectly.
 *
 * Second, your application might crash, the power might go down or there is
 * some other source of problems. In that case, although whole bytes are
 * written, the byte sequence might not completely be written, leading to
 * corrupt files. Therefore the FS first virtually erases each file, makes
 * the modifications, and than restores the access in the FAT. Since the
 * access specifier in the FAT is one byte, we can be sure it will be written
 * entirely (in which case the file was written correctly) or will not be
 * written at all, in which case the file will be lost. In any case integrity
 * of the FAT will be preserved. Note that if you write a file in parts, you're
 * file may not be completed in the view of your application. The file itself
 * will be valid at all times.
 *
 * To enhance the lifetime of the EEPROM, the FS does only erase a byte when
 * needed. In other words, if a byte can be written without erasing, this is done.
 * Of course the implies reading every byte prior to writing. (This behavior
 * can be changed by changing the port code)
 */

/** <!--1905-->
 * Specify the file names.
 *
 * Files are numbered internally, but can be given names, just like the tasks.
 * Only the first 16 files can have such a names. The rest of the file space
 * is divided into anonymous files of default length.
 * Handle the files using their names in the OnName methods. If the number
 * of a particular named file is needed use preFileNumberOf([FileName]) instead
 * of directly specifying the number, which may be due to change.
 */
#define  FN_00                                   file0
#define  FN_01                                   file1
#define  FN_02                                   file2
#define  FN_03                                   file3
#define  FN_04                                   file4
#define  FN_05                                   file5
#define  FN_06                                   file6
#define  FN_07                                   file7
#define  FN_08                                   file8
#define  FN_09                                   file9
#define  FN_10                                   fileA
#define  FN_11                                   fileB
#define  FN_12                                   fileC
#define  FN_13                                   fileD
#define  FN_14                                   fileE
#define  FN_15                                   fileF


/* ========================================================================= */
/* FILE SIZES ============================================================== */
/* ========================================================================= */

/** <!--1A06-->
 * Specify the standard file size
 *
 * Space not used for regular predefined files is filled up with files of
 * standard size. These files cannot be reached with a compile time name,
 * only under file number.
 *
 * The FAT specifies for each file how much bytes are written.
 */
#define  FileSpaceStandard                       32

/** <!--1832-->
 * Specify the file space for all files at once.
 *
 * You can specify one fixed FileSpace value for all tasks by using
 * FileSpaceOverride. If you want to specify the FileSpace setting per task,
 * set FileSpaceOverride to cfgOverrideNon, and define the value for each
 * task separately. If you define each task as well as the FileSpaceOverride
 * the latter takes precedence, so the fixed value overrides values specified
 * per task (if unequal to cfgOverrideNon). Defining this value is not required.
 *
 * This fixed value is not used to define FileSpaceStandard.
 * See FileSpace_[filename] for explanation.
 */
#define  FileSpaceOverride                       cfgOverrideNon

/** <!--1812-->
 * Specify the space (maximum size) of each file.
 *
 * Here you can define the maximal length of each file. Files may not exceed
 * the given length, and are protected at writing. You can skip a file by
 * setting its space to zero. You need not use all space.
 *
 * The FAT specifies for each file how much bytes are written.
 * Note: The preprocessor cannot check if you define spaces here for files
 * that have not been previously declared with a file name. Usually this is
 * harmless, but the preFileDefined() may produce false results in this case.
 */
#define  FileSpace_file0                         32
#define  FileSpace_file1                         178
#define  FileSpace_file2                         0
#define  FileSpace_file3                         8
#define  FileSpace_file4                         1
#define  FileSpace_file5                         1
#define  FileSpace_file6                         0
#define  FileSpace_file7                         6
#define  FileSpace_file8                         0
#define  FileSpace_file9                         0
#define  FileSpace_fileA                         0
#define  FileSpace_fileB                         0
#define  FileSpace_fileC                         8
#define  FileSpace_fileD                         0
#define  FileSpace_fileE                         12
#define  FileSpace_fileF                         0


#endif /* CONFIG_DEFAULT_H */


#ifdef BelowFindYourTemplate


/* ========================================================================= */
/* SYSTEM CONFIGURATION ==================================================== */
/* ========================================================================= */

#define  cfgSysOptimized                         cfgFalse
#define  cfgSysDebug                             cfgFalse
#define  cfgSysInline                            cfgFalse
#define  cfgSysFramePointerCounterMeasures       cfgFalse
#define  cfgSysReduceProEpilogue                 cfgBikini
#define  cfgSysRegisterCheckByte                 0xA5
#define  cfgSysStackGrowthUp                     cfgFalse
#define  cfgSysZeroPageStack                     cfgFalse
#define  cfgSysClearUnusedR1                     cfgTrue
#define  cfgSysSqueezeState                      defSysOptimized
#define  cfgSysReuseOsStack                      cfgTrue
#define  cfgSysClockDivider                      1
#define  cfgSysSubTicksPerFullTick               32
#define  cfgSysSubTickDivider                    256
#define  cfgSysGCCstartup                        cfgKeep
#define  cfgSysFsStorageSpace                    512


/* ========================================================================= */
/* INTERRUPT HANDLING ====================================================== */
/* ========================================================================= */

#define  cfgIntGlobalOnly                        cfgTrue
#define  cfgIntUserDefined                       cfgFalse
#define  cfgIntSwitchUsesOSstack                 cfgFalse
#define  cfgIntOsProtected                       cfgTrue
#define  cfgIntTickTrack                         cfgFalse
#define  cfgIntManualTicks                       cfgFalse


/* ========================================================================= */
/* QUALITY ASSURANCE ======================================================= */
/* ========================================================================= */

#define  cfgCheckSystem                          cfgFalse
#define  cfgCheckReset                           cfgFalse
#define  cfgCheckTrace                           cfgFalse
#define  cfgCheckAlwaysFatal                     cfgTrue
#define  cfgCheckWatermarks                      cfgFalse
#define  cfgCheckOsStack                         cfgFalse
#define  cfgCheckTaskStack                       cfgFalse
#define  cfgCheckIsrStack                        cfgFalse
#define  cfgCheckRegisters                       cfgFalse
#define  cfgCheckTiming                          cfgFalse
#define  cfgCheckQueuFilling                     cfgFalse
#define  cfgCheckMethodUse                       cfgFalse
#define  cfgCheckApplication                     cfgFalse


/* ========================================================================= */
/* COMMON CAPABILITIES ===================================================== */
/* ========================================================================= */

#define  cfgUseEquidistantTicks                  cfgTrue
#define  cfgUseDelay                             cfgFalse
#define  cfgUseSuspendOnDelay                    cfgFalse
#define  cfgUseSynchronization                   cfgSyncNon
#define  cfgUseHierarchicalRoundRobin            cfgFalse
#define  cfgUseNestedCriticals                   cfgFalse
#define  cfgUsePrioritizedRelease                cfgFalse
#define  cfgUsePriorityLifting                   cfgFalse
#define  cfgUseTaskWatchdog                      cfgFalse
#define  cfgUseCorrectWakeupTimes                cfgFalse
#define  cfgUseTimeout                           cfgFalse
#define  cfgUseTasknames                         cfgFalse
#define  cfgUseLowPowerSleep                     cfgFalse
#define  cfgUseLowPowerOnDelay                   cfgFalse
#define  cfgUseLowPowerDelayRelease              cfgFalse
#define  cfgUseLoadMonitor                       cfgFalse
#define  cfgUseFileSystem                        cfgFalse
#define  cfgUseFileSystemConcurrentRead          cfgFalse
#define  cfgUseFileSystemEconomyMode             cfgFalse
#define  cfgUseFileSystemMaintainFAT             cfgFalse
#define  cfgUseEvents                            cfgFalse
#define  cfgUseEventsOnVariables                 cfgFalse


/* ========================================================================= */
/* CODE PARAMETERS ========================================================= */
/* ========================================================================= */

#define  cfgNumWatchdogDiv                       0
#define  cfgNumMonitorDiv                        0
#define  cfgNumSleepPeriod                       0
#define  cfgNumSleepThreshold                    0
#define  cfgNumGlobWidth                         0
#define  cfgNumSwitchWidth                       0
#define  cfgNumTickWidth                         0


/* ========================================================================= */
/* EXTERNAL CALLS ========================================================== */
/* ========================================================================= */

#define  callAppTick00                           cfgFalse
#define  callAppTick08                           cfgFalse
#define  callAppTick16                           cfgFalse
#define  callAppTickSleep                        cfgFalse
#define  callAppEnterIdle                        cfgFalse
#define  callAppEnterSleep                       cfgFalse
#define  callAppExitSleep                        cfgFalse
#define  callAppBoot                             cfgFalse
#define  callAppInit                             cfgFalse
#define  callAppBark                             cfgFalse


/* ========================================================================= */
/* API INCLUSIONS ========================================================== */
/* ========================================================================= */

/* Standard functions ------------------------------------------------------ */
#define  includeTaskYield                        cfgFalse
#define  includeTaskDelayFromNow                 cfgFalse
#define  includeTaskDelayFromWake                cfgFalse
#define  includeGenSuspend                       cfgFalse
#define  includeTaskSuspend                      cfgFalse
#define  includeGenResume                        cfgFalse
#define  includeTaskSleep                        cfgFalse
#define  includeTaskSleepAll                     cfgFalse
#define  includeGenSetPriority                   cfgFalse
#define  includeGenGetPriority                   cfgFalse
#define  includeGenGetTickCount                  cfgFalse
#define  includeGenAddtoTickCount                cfgFalse
#define  includeGenGetLastWakeTime               cfgFalse
#define  includeTaskFeedWatchdog                 cfgFalse
#define  includeTaskKillWatchdog                 cfgFalse
#define  includeGenGetTaskname                   cfgFalse
#define  includeTaskTerminate                    cfgFalse
#define  includeGenReboot                        cfgFalse
#define  includeTaskRecreate                     cfgFalse
#define  includeTaskRestart                      cfgFalse
#define  includeGenLogTask                       cfgFalse
#define  includeGenLogOs                         cfgFalse
#define  includeGenTrace                         cfgFalse
#define  includeGenPipeInt16                     cfgFalse
#define  includeGenPassFlashString               cfgFalse

/* Event functions --------------------------------------------------------- */
#define  includeTaskWaitForEvents                cfgFalse
#define  includeIsrFireEvent                     cfgFalse
#define  includeGenFireEvent                     cfgFalse
#define  includeGenFireEventSet                  cfgFalse
#define  includeGenCountEventBlocks              cfgFalse

/* Synchronization functions ----------------------------------------------- */
#define  includeTaskWaitForTasks                 cfgFalse
#define  includeGenWaitRelease                   cfgFalse
#define  includeTaskMutex                        cfgFalse
#define  includeTaskQueu                         cfgFalse

/* Queu functions --------------------------------------------------------- */
#define  includeGenQueuWrite                     cfgFalse
#define  includeGenQueuRead                      cfgFalse
#define  includeGenQueuUnwrite                   cfgFalse
#define  includeGenQueuUnread                    cfgFalse
#define  includeGenQueuClear                     cfgFalse
#define  includeGenQueuPeek                      cfgFalse
#define  includeGenQueuReadable                  cfgFalse
#define  includeGenQueuWriteable                 cfgFalse
#define  includeGenQueuFull                      cfgFalse
#define  includeGenQueuEmpty                     cfgFalse

/* File System functions --------------------------------------------------- */
#define  includeTaskFileAccess                   cfgFalse
#define  includeTaskFileFormat                   cfgFalse
#define  includeTaskFileGetSize                  cfgFalse
#define  includeTaskFileSetSize                  cfgFalse
#define  includeTaskFileReadByte                 cfgFalse
#define  includeTaskFileWriteByte                cfgFalse
#define  includeTaskFileAppendByte               cfgFalse
#define  includeTaskFileReadPipe                 cfgFalse
#define  includeTaskFileWritePipe                cfgFalse
#define  includeTaskFileReadBuffer               cfgFalse
#define  includeTaskFileWriteBuffer              cfgFalse

/* Protection functions ---------------------------------------------------- */
#define  includeTaskProtectGlobalInterrupts      cfgFalse
#define  includeTaskProtectSwitchTasks           cfgFalse
#define  includeTaskProtectTickInterrupts        cfgFalse
#define  includeTaskProtectGlobalCritical        cfgFalse
#define  includeTaskProtectSwitchCritical        cfgFalse
#define  includeTaskProtectTickCritical          cfgFalse

/* ISR header/footer functions --------------------------------------------- */
#define  includeIsrEnter                         cfgFalse
#define  includeIsrExit                          cfgFalse
#define  includeIsrBegin                         cfgFalse
#define  includeIsrEndReturn                     cfgFalse
#define  includeIsrEndYield                      cfgFalse
#define  includeIsrStartLoad                     cfgFalse
#define  includeIsrStopLoad                      cfgFalse

/* quality assurance functions --------------------------------------------- */
#define  includeTaskStackCheck                   cfgFalse
#define  includeIsrStackCheck                    cfgFalse


/* ========================================================================= */
/* TASK NAMES ============================================================== */
/* ========================================================================= */

#define  CN_00                                   task0
#define  CN_01                                   task1
#define  CN_02                                   task2
#define  CN_03                                   task3
#define  CN_04                                   task4
#define  CN_05                                   task5
#define  CN_06                                   task6
#define  CN_07                                   task7
#define  CN_08                                   task8
#define  CN_09                                   task9
#define  CN_10                                   taskA
#define  CN_11                                   taskB
#define  CN_12                                   taskC
#define  CN_13                                   taskD
#define  CN_14                                   taskE
#define  CN_15                                   taskF


/* ========================================================================= */
/* INCLUDE TASKS =========================================================== */
/* ========================================================================= */

#define  TaskIncludeOverride                     cfgOverrideNon
#define  TaskInclude_task0                       cfgExclude
#define  TaskInclude_task1                       cfgExclude
#define  TaskInclude_task2                       cfgExclude
#define  TaskInclude_task3                       cfgExclude
#define  TaskInclude_task4                       cfgExclude
#define  TaskInclude_task5                       cfgExclude
#define  TaskInclude_task6                       cfgExclude
#define  TaskInclude_task7                       cfgExclude
#define  TaskInclude_task8                       cfgExclude
#define  TaskInclude_task9                       cfgExclude
#define  TaskInclude_taskA                       cfgExclude
#define  TaskInclude_taskB                       cfgExclude
#define  TaskInclude_taskC                       cfgExclude
#define  TaskInclude_taskD                       cfgExclude
#define  TaskInclude_taskE                       cfgExclude
#define  TaskInclude_taskF                       cfgExclude


/* ========================================================================= */
/* TASK CAPABILITIES ======================================================= */
/* ========================================================================= */

#define  CapabilitiesOverride                    cfgOverrideNon
#define  Capabilities_task0                      cfgCapAll
#define  Capabilities_task1                      cfgCapAll
#define  Capabilities_task2                      cfgCapAll
#define  Capabilities_task3                      cfgCapAll
#define  Capabilities_task4                      cfgCapAll
#define  Capabilities_task5                      cfgCapAll
#define  Capabilities_task6                      cfgCapAll
#define  Capabilities_task7                      cfgCapAll
#define  Capabilities_task8                      cfgCapAll
#define  Capabilities_task9                      cfgCapAll
#define  Capabilities_taskA                      cfgCapAll
#define  Capabilities_taskB                      cfgCapAll
#define  Capabilities_taskC                      cfgCapAll
#define  Capabilities_taskD                      cfgCapAll
#define  Capabilities_taskE                      cfgCapAll
#define  Capabilities_taskF                      cfgCapAll


/* ========================================================================= */
/* STACK SIZES ============================================================= */
/* ========================================================================= */

#define  StackSafety                             0
#define  StackSizeOS                             8
#define  StackSizeISR                            8
#define  StackSizeShared                         0
#define  StackSizeOverride                       cfgOverrideNon
#define  StackSize_task0                         4
#define  StackSize_task1                         4
#define  StackSize_task2                         4
#define  StackSize_task3                         4
#define  StackSize_task4                         4
#define  StackSize_task5                         4
#define  StackSize_task6                         4
#define  StackSize_task7                         4
#define  StackSize_task8                         4
#define  StackSize_task9                         4
#define  StackSize_taskA                         4
#define  StackSize_taskB                         4
#define  StackSize_taskC                         4
#define  StackSize_taskD                         4
#define  StackSize_taskE                         4
#define  StackSize_taskF                         4


/* ========================================================================= */
/* TIMES SLICES ============================================================ */
/* ========================================================================= */

#define  TimeSliceIdleTime                       cfgSysSubTicksPerFullTick
#define  TimeSliceOverride                       cfgOverrideNon
#define  TimeSlice_task0                         cfgSysSubTicksPerFullTick
#define  TimeSlice_task1                         cfgSysSubTicksPerFullTick
#define  TimeSlice_task2                         cfgSysSubTicksPerFullTick
#define  TimeSlice_task3                         cfgSysSubTicksPerFullTick
#define  TimeSlice_task4                         cfgSysSubTicksPerFullTick
#define  TimeSlice_task5                         cfgSysSubTicksPerFullTick
#define  TimeSlice_task6                         cfgSysSubTicksPerFullTick
#define  TimeSlice_task7                         cfgSysSubTicksPerFullTick
#define  TimeSlice_task8                         cfgSysSubTicksPerFullTick
#define  TimeSlice_task9                         cfgSysSubTicksPerFullTick
#define  TimeSlice_taskA                         cfgSysSubTicksPerFullTick
#define  TimeSlice_taskB                         cfgSysSubTicksPerFullTick
#define  TimeSlice_taskC                         cfgSysSubTicksPerFullTick
#define  TimeSlice_taskD                         cfgSysSubTicksPerFullTick
#define  TimeSlice_taskE                         cfgSysSubTicksPerFullTick
#define  TimeSlice_taskF                         cfgSysSubTicksPerFullTick


/* ========================================================================= */
/* RUNTIME TASK NAMES ====================================================== */
/* ========================================================================= */

#define  TaskName_task0                          "taak nul"
#define  TaskName_task1                          "taak een"
#define  TaskName_task2                          "taak twee"
#define  TaskName_task3                          "taak drie"
#define  TaskName_task4                          "taak vier"
#define  TaskName_task5                          "taak vijf"
#define  TaskName_task6                          "taak_zes"
#define  TaskName_task7                          "taak_zeven"
#define  TaskName_task8                          "taak_acht"
#define  TaskName_task9                          "taak_negen"
#define  TaskName_taskA                          "taak_tien"
#define  TaskName_taskB                          "taak_elf"
#define  TaskName_taskC                          "taak_twaalf"
#define  TaskName_taskD                          "taak_dertien"
#define  TaskName_taskE                          "taak_veertien"
#define  TaskName_taskF                          "taak_vijftien"


/* ========================================================================= */
/* TASK PRIORITIES ========================================================= */
/* ========================================================================= */

#define  PriorityOverride                        cfgOverrideNon
#define  Priority_task0                          0
#define  Priority_task1                          3
#define  Priority_task2                          2
#define  Priority_task3                          3
#define  Priority_task4                          2
#define  Priority_task5                          1
#define  Priority_task6                          1
#define  Priority_task7                          1
#define  Priority_task8                          0
#define  Priority_task9                          0
#define  Priority_taskA                          0
#define  Priority_taskB                          0
#define  Priority_taskC                          0
#define  Priority_taskD                          0
#define  Priority_taskE                          0
#define  Priority_taskF                          0


/* ========================================================================= */
/* REGISTER USE ============================================================ */
/* ========================================================================= */

#define  RegisterUseOverride                     cfgOverrideNon
#define  RegisterUse_task0                       registersNon
#define  RegisterUse_task1                       r00r01r02r03 | r04r05r06r07
#define  RegisterUse_task2                       r08r09r10r11 | r12r13r14r15
#define  RegisterUse_task3                       r16r17r18r19 | r20r21r22r23
#define  RegisterUse_task4                       r24r25r26r27 | r28r29r30r31
#define  RegisterUse_task5                       r00r01r02r03
#define  RegisterUse_task6                       r04r05r06r07
#define  RegisterUse_task7                       r08r09r10r11
#define  RegisterUse_task8                       r12r13r14r15
#define  RegisterUse_task9                       r16r17r18r19
#define  RegisterUse_taskA                       r20r21r22r23
#define  RegisterUse_taskB                       r24r25r26r27
#define  RegisterUse_taskC                       r28r29r30r31
#define  RegisterUse_taskD                       registersAll
#define  RegisterUse_taskE                       registersAll
#define  RegisterUse_taskF                       registersAll


/* ========================================================================= */
/* REGISTER CHECK ========================================================== */
/* ========================================================================= */

#define  RegisterCheckOverride                   cfgOverrideNon
#define  RegisterCheck_task0                     registersAll
#define  RegisterCheck_task1                     registersAll
#define  RegisterCheck_task2                     registersAll
#define  RegisterCheck_task3                     registersAll
#define  RegisterCheck_task4                     registersAll
#define  RegisterCheck_task5                     registersAll
#define  RegisterCheck_task6                     registersAll
#define  RegisterCheck_task7                     registersAll
#define  RegisterCheck_task8                     registersAll
#define  RegisterCheck_task9                     registersAll
#define  RegisterCheck_taskA                     registersAll
#define  RegisterCheck_taskB                     registersAll
#define  RegisterCheck_taskC                     registersAll
#define  RegisterCheck_taskD                     registersAll
#define  RegisterCheck_taskE                     registersAll
#define  RegisterCheck_taskF                     registersAll


/* ========================================================================= */
/* INTERRUPT SETTING ======================================================= */
/* ========================================================================= */

#define  InterruptStartOverride                  cfgOverrideNon
#define  InterruptStart_task0                    cfgGlobSet | cfgTickSet
#define  InterruptStart_task1                    cfgGlobSet | cfgTickSet
#define  InterruptStart_task2                    cfgGlobSet | cfgTickSet
#define  InterruptStart_task3                    cfgGlobSet | cfgTickSet
#define  InterruptStart_task4                    cfgGlobSet | cfgTickSet
#define  InterruptStart_task5                    cfgGlobSet | cfgTickSet
#define  InterruptStart_task6                    cfgGlobSet | cfgTickSet
#define  InterruptStart_task7                    cfgGlobSet | cfgTickSet
#define  InterruptStart_task8                    cfgGlobSet | cfgTickSet
#define  InterruptStart_task9                    cfgGlobSet | cfgTickSet
#define  InterruptStart_taskA                    cfgGlobSet | cfgTickSet
#define  InterruptStart_taskB                    cfgGlobSet | cfgTickSet
#define  InterruptStart_taskC                    cfgGlobSet | cfgTickSet
#define  InterruptStart_taskD                    cfgGlobSet | cfgTickSet
#define  InterruptStart_taskE                    cfgGlobSet | cfgTickSet
#define  InterruptStart_taskF                    cfgGlobSet | cfgTickSet


/* ========================================================================= */
/* EVENT NAMES ============================================================= */
/* ========================================================================= */

#define  EN_00                                   event0
#define  EN_01                                   event1
#define  EN_02                                   event2
#define  EN_03                                   event3
#define  EN_04                                   event4
#define  EN_05                                   event5
#define  EN_06                                   event6
#define  EN_07                                   event7


/* ========================================================================= */
/* SLOT NAMES ============================================================== */
/* ========================================================================= */

#define  SN_01                                   slot1
#define  SN_02                                   slot2
#define  SN_03                                   slot3
#define  SN_04                                   slot4
#define  SN_05                                   slot5
#define  SN_06                                   slot6
#define  SN_07                                   slot7
#define  SN_08                                   slot8
#define  SN_09                                   slot9
#define  SN_10                                   slotA
#define  SN_11                                   slotB
#define  SN_12                                   slotC
#define  SN_13                                   slotD
#define  SN_14                                   slotE
#define  SN_15                                   slotF


/* ========================================================================= */
/* SLOT SIZES ============================================================== */
/* ========================================================================= */

#define  SlotSizeOverride                        cfgOverrideNon
#define  SlotSize_task0                          0
#define  SlotSize_task1                          0
#define  SlotSize_task2                          0
#define  SlotSize_task3                          0
#define  SlotSize_task4                          0
#define  SlotSize_task5                          0
#define  SlotSize_task6                          0
#define  SlotSize_task7                          0
#define  SlotSize_task8                          0
#define  SlotSize_task9                          0
#define  SlotSize_taskA                          0
#define  SlotSize_taskB                          0
#define  SlotSize_taskC                          0
#define  SlotSize_taskD                          0
#define  SlotSize_taskE                          0
#define  SlotSize_taskF                          0


/* ========================================================================= */
/* SLOT USAGE ============================================================== */
/* ========================================================================= */

#define  SlotUseOverride                         cfgOverrideNon
#define  SlotUse_slot1                           cfgUseAsNon
#define  SlotUse_slot2                           cfgUseAsNon
#define  SlotUse_slot3                           cfgUseAsNon
#define  SlotUse_slot4                           cfgUseAsNon
#define  SlotUse_slot5                           cfgUseAsNon
#define  SlotUse_slot6                           cfgUseAsNon
#define  SlotUse_slot7                           cfgUseAsNon
#define  SlotUse_slot8                           cfgUseAsNon
#define  SlotUse_slot9                           cfgUseAsNon
#define  SlotUse_slotB                           cfgUseAsNon
#define  SlotUse_slotC                           cfgUseAsNon
#define  SlotUse_slotD                           cfgUseAsNon
#define  SlotUse_slotE                           cfgUseAsNon
#define  SlotUse_slotF                           cfgUseAsNon


/* ========================================================================= */
/* QUEU SIZES ============================================================== */
/* ========================================================================= */

#define  QueuSizeOverride                        cfgOverrideNon
#define  QueuSize_slot1                          0
#define  QueuSize_slot2                          10
#define  QueuSize_slot3                          10
#define  QueuSize_slot4                          10
#define  QueuSize_slot5                          10
#define  QueuSize_slot6                          10
#define  QueuSize_slot7                          10
#define  QueuSize_slot8                          10
#define  QueuSize_slot9                          10
#define  QueuSize_slotA                          10
#define  QueuSize_slotB                          10
#define  QueuSize_slotC                          10
#define  QueuSize_slotD                          10
#define  QueuSize_slotE                          10
#define  QueuSize_slotF                          10


/* ========================================================================= */
/* FILE NAMES ============================================================== */
/* ========================================================================= */

#define  FN_00                                   file0
#define  FN_01                                   file1
#define  FN_02                                   file2
#define  FN_03                                   file3
#define  FN_04                                   file4
#define  FN_05                                   file5
#define  FN_06                                   file6
#define  FN_07                                   file7
#define  FN_08                                   file8
#define  FN_09                                   file9
#define  FN_10                                   fileA
#define  FN_11                                   fileB
#define  FN_12                                   fileC
#define  FN_13                                   fileD
#define  FN_14                                   fileE
#define  FN_15                                   fileF


/* ========================================================================= */
/* FILE SIZES ============================================================== */
/* ========================================================================= */

#define  FileSpaceStandard                       0
#define  FileSpaceOverride                       cfgOverrideNon
#define  FileSpace_file0                         0
#define  FileSpace_file1                         0
#define  FileSpace_file2                         0
#define  FileSpace_file3                         0
#define  FileSpace_file4                         0
#define  FileSpace_file5                         0
#define  FileSpace_file6                         0
#define  FileSpace_file7                         0
#define  FileSpace_file8                         0
#define  FileSpace_file9                         0
#define  FileSpace_fileA                         0
#define  FileSpace_fileB                         0
#define  FileSpace_fileC                         0
#define  FileSpace_fileD                         0
#define  FileSpace_fileE                         0
#define  FileSpace_fileF                         0


#endif
