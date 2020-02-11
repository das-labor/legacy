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

#ifndef TASKS_H
#define TASKS_H

#include "femtoos_shared.h"

/**
 * Declaration of the tick counter
 */
Ttickcount  uxTickCount;


/* Definitions for the uiOsStatus byte. The whole state of the Femto OS is kept in one byte.
 * 8 bit structure: ccnstttt
 * cc:   context state of the OS: This indicates in which state the OS is at the moment.
 * n:    presence of a near wake: one or more tasks have to waked with 256 ticks.
 * s:    switch block,  if set no task switch can be performed.
 * tttt: number of the current task: Number of the task that is currently running.
 */
Tuint08  uiOsStatus;

/**
 * This union, TosDate is the dark art. We use it so pass and keep the background variables AND to hold the
 * OS stack. However some of the background variables are retrieved after this has become stack space again.
 * This should be save if the parameters are
 * directly copied form their location and if they are not located at stack-pointer border. Make sure the
 * OSstack is at least as big as this structure. We tested it, and indeed it can be as small as that.
 * In case of cfgSysStackGrowthUp == cfgTrue and there are problems if can be necessary to place the pxSave
 * to the bottom of the structure, (the compiler normally places it top-aligned, although it has
 * freedom to choose something else.
 * Discussion about the nocommon attribute, see below.
 */
TosData  xOS __attribute__ ((nocommon));

/**
 * Declaration of the FS status byte.
 *
 * The whole state of the Femto File System is kept in one byte.
 * 8 bit structure: rwqbtttt
 * r:    read mode, reading is possible, all blocking FS tasks are write request.
 * w:    write mode, one task has access, further tasks are blocked.
 * q:    request bit, if set, write is requested, no further reads are allowed.
 * b:    burn block. As long as this bit is set, writing is prohibited.
 * tttt: number of the task currently writing the FS in case of writemode
 * tttt: number of tasks holding a readlock in case of readmode
 */
#if (cfgUseFileSystem  ==  cfgTrue)
   Tuint08  uiFsStatus;
#endif


/**
 * If we check the timing of the OS an continuous average is taken over the
 * last for Os Time values. This is stored in uiOsTimeAverage.
 */
#if (cfgCheckTiming == cfgTrue)
  Tuint08  uiOsTimeAverage;
#endif


/**
 * These variables hold the statistics of the OS and the ISR's. The 'collect' variables collect
 * the subticks and these subticks are periodically and simultaneously copied to the 'total'
 * variables.
 */
#if (cfgUseLoadMonitor == cfgTrue)
  Tuint16  uiOsLoadCollect;
  Tuint16  uiOsLoadTotal;
  Tuint16  uiIdleLoadCollect;
  Tuint16  uiIdleLoadTotal;
  #if (cfgIntUserDefined == cfgTrue)
    Tuint08  uiIsrLoadTemp;
    Tuint16  uiIsrLoadCollect;
    Tuint16  uiIsrLoadTotal;
    #if (cfgIntOsProtected == cfgTrue) && (cfgUseLowPowerSleep == cfgFalse) && (cfgIntTickTrack == cfgFalse)
      Tuint08  uiLastOsStatus;
    #endif
  #endif
#endif


/**
 * The 'StackMax' variables hold the highest use of the Stack for the OS and the
 * ISR. These are determined on the bases of the highest stack level as well as
 * on inspection of the content of these arrays.
 */
#if (cfgCheckWatermarks == cfgTrue)
    Tuint08  uiOsStackMax;
  #if (defUseIsrStack == cfgTrue)
    #if ((StackSizeISR) > 0xFF)
      Tuint16  uiIsrStackMax;
    #else
      Tuint08  uiIsrStackMax;
    #endif
  #endif
#endif

/**
 * Define the ISR stack if needed, discussion about nocommon, see below.
 */
#if (defUseIsrStack == cfgTrue)
  Tchar StackISR[(StackSizeISR)] __attribute__ ((nocommon));
#endif


/**
 * Define the Shared stack if needed, discussion about nocommon, see below.
 */
#if (defUseSharedStack == cfgTrue)
  Tchar StackSHR[(StackSizeShared)] __attribute__ ((nocommon));
#endif

#if   (defRegisterUseConstant == cfgTrue)  &&  (defRegisterCheckConstant == cfgTrue)
  #define REGPARAMS(NR)
#elif (defRegisterUseConstant == cfgTrue)  &&  (defRegisterCheckConstant == cfgFalse)
  #define REGPARAMS(NR) (RegisterCheckReduced(TN_ ## NR)),
#elif (defRegisterUseConstant == cfgFalse) &&  (defRegisterCheckConstant == cfgTrue)
  #define REGPARAMS(NR) (RegisterUse(TN_ ## NR)),
#elif (defRegisterUseConstant == cfgFalse) &&  (defRegisterCheckConstant == cfgFalse)
  #define REGPARAMS(NR) (RegisterUse(TN_ ## NR)),(RegisterCheckReduced(TN_ ## NR)),
#else
  #error "Inconsistent parameters, you should not arive here."
#endif

#if (defInitialStatusConstant == cfgTrue)
  #define STATUSPARAMS(NR)
#else
  #define STATUSPARAMS(NR) ( InitialStartModifier( ((TaskInclude(TN_ ## NR)) & defInitialStartGetMask), (InitialShared(TN_ ## NR))) | ((InterruptStart(TN_ ## NR)) & (defInitialInterruptGetMask)) | ( ((Priority(TN_ ## NR)) << defBasePrioShift) & defInitialPriorityGetMask) ),
#endif

#if (defStackSizeConstant == cfgFalse) && ( (cfgCheckTaskStack == cfgTrue) || (cfgCheckWatermarks == cfgTrue) || (defStackClean == cfgTrue) )
  #define STACKSIZEPARAMS(NR) (StackSize(TN_ ## NR)),
#else
  #define STACKSIZEPARAMS(NR)
#endif

#if (cfgSysStackGrowthUp == cfgTrue)
  #define STACKOFFSETPARAMS(NR)       (Stack ## NR)
  #define STACKOFFSETPARAMSSHARED     (StackSHR)
#else
  #define STACKOFFSETPARAMS(NR)       (&Stack ## NR[StackSize(TN_ ## NR) -1])
  #define STACKOFFSETPARAMSSHARED     (&StackSHR[StackSizeShared -1])
#endif

#if (defTimeSliceConstant == cfgTrue)
  #define TIMESLICEPARAMS(NR)
#else
  #define TIMESLICEPARAMS(NR) ,(TimeSlice(TN_ ## NR))
#endif

# if (cfgUseSynchronization != cfgSyncNon) && (defSlotDepthConstant == cfgFalse)
  #define SLOTDEPTHPARAMS(NR) ,( (SlotDepth(TN_ ## NR)) & defSlotDepthMask )
#else
  #define SLOTDEPTHPARAMS(NR)
#endif

#if (cfgUseTasknames == cfgTrue)
  #define TASKNAMEPARAMS(NR) ,(TaskNameStr ## NR)
#else
  #define TASKNAMEPARAMS(NR)
#endif


#define TDB_DEF(NR,SOP) { REGPARAMS(NR) STATUSPARAMS(NR) STACKSIZEPARAMS(NR) SOP TIMESLICEPARAMS(NR) SLOTDEPTHPARAMS(NR) TASKNAMEPARAMS(NR) };


#if (defUseTaskSlot == cfgTrue)
  #define SLOTDEPTHTCBEXTENSION(NR) (SlotDepth(TN_ ## NR))
#else
  #define SLOTDEPTHTCBEXTENSION(NR) (0)
#endif


#if (defUseQueus == cfgTrue) && (defUseTaskSlot == cfgTrue)
  #if (cfgUseSynchronization == cfgSyncDoubleBlock)
    #define QEUETCBEXTENSION(NR) (((SlotSize(TN_ ## NR)) == 0) ? 0 : 2)
  #else
    #define QEUETCBEXTENSION(NR) (((SlotSize(TN_ ## NR)) == 0) ? 0 : 1)
  #endif
#else
  #define QEUETCBEXTENSION(NR)   (0)
#endif



/* By placing the Stacks in nocommon we force gcc to allocate space directly, making
 * sure the stacks are all collected at the beginning of the RAM space, if this file
 * is the first object file linked. Now, if the sum  of all stacks does not exceed 0xFF
 * we safely ignore the stack pointer __SP_H__ for calculations.
 * Note that it is further from the utmost important the linker gets to see the shared.o
 * file first, so it can place all stacks before any other static variables.
 * */


#define TASK_DEF(NR)                                                                                         \
  const Tbyte tcb ## NR[(sizeof(TtaskControlBlock) + SLOTDEPTHTCBEXTENSION(NR) + QEUETCBEXTENSION(NR)) ];    \
  const Tchar Stack ## NR[StackSizeReal(TN_  ## NR)] __attribute__ ((nocommon));                             \
  extern void AppInit(TN_ ## NR) (void);                                                                     \
  extern void AppLoop(TN_ ## NR) (void);                                                                     \
  extern void AppBark(TN_ ## NR) (void);                                                                     \
  const TtaskDefinitionBlock PROGMEM tdb ## NR =  TDB_DEF(NR,STACKOFFSETPARAMS(NR))

#define TASK_DEF_SHARED(NR)                                                                                  \
  const Tbyte tcb ## NR[(sizeof(TtaskControlBlock) + SLOTDEPTHTCBEXTENSION(NR) + QEUETCBEXTENSION(NR)) ];    \
  extern void AppInit(TN_ ## NR) (void);                                                                     \
  extern void AppLoop(TN_ ## NR) (void);                                                                     \
  extern void AppBark(TN_ ## NR) (void);                                                                     \
  const TtaskDefinitionBlock PROGMEM tdb ## NR =  TDB_DEF(NR,STACKOFFSETPARAMSSHARED)

#define QUEU_DEF(NR)                         \
  const Tbyte Queu ## NR[QueuSize(LN_ ## NR)];

#if defNumberOfTasks > 0
 #if cfgUseTasknames == cfgTrue
  const Tchar TaskNameStr00[] PROGMEM = TaskName(TN_00);
 #endif
 #if StackIsShared(TN_00)
  TASK_DEF_SHARED(00)
 #else
  TASK_DEF(00)
 #endif
#endif

#if defNumberOfTasks > 1
 #if cfgUseTasknames == cfgTrue
  const Tchar TaskNameStr01[] PROGMEM = TaskName(TN_01);
 #endif
 #if StackIsShared(TN_01)
  TASK_DEF_SHARED(01)
 #else
  TASK_DEF(01)
 #endif
#endif

#if defNumberOfTasks > 2
 #if cfgUseTasknames == cfgTrue
  const Tchar TaskNameStr02[] PROGMEM = TaskName(TN_02);
 #endif
 #if StackIsShared(TN_02)
  TASK_DEF_SHARED(02)
 #else
  TASK_DEF(02)
 #endif
#endif

#if defNumberOfTasks > 3
 #if cfgUseTasknames == cfgTrue
  const Tchar TaskNameStr03[] PROGMEM = TaskName(TN_03);
 #endif
 #if StackIsShared(TN_03)
  TASK_DEF_SHARED(03)
 #else
  TASK_DEF(03)
 #endif
#endif

#if defNumberOfTasks > 4
 #if cfgUseTasknames == cfgTrue
  const Tchar TaskNameStr04[] PROGMEM = TaskName(TN_04);
 #endif
 #if StackIsShared(TN_04)
  TASK_DEF_SHARED(04)
 #else
  TASK_DEF(04)
 #endif
#endif

#if defNumberOfTasks > 5
 #if cfgUseTasknames == cfgTrue
  const Tchar TaskNameStr05[] PROGMEM = TaskName(TN_05);
 #endif
 #if StackIsShared(TN_05)
  TASK_DEF_SHARED(05)
 #else
  TASK_DEF(05)
 #endif
#endif

#if defNumberOfTasks > 6
 #if cfgUseTasknames == cfgTrue
  const Tchar TaskNameStr06[] PROGMEM = TaskName(TN_06);
 #endif
 #if StackIsShared(TN_06)
  TASK_DEF_SHARED(06)
 #else
  TASK_DEF(06)
 #endif
#endif

#if defNumberOfTasks > 7
 #if cfgUseTasknames == cfgTrue
  const Tchar TaskNameStr07[] PROGMEM = TaskName(TN_07);
 #endif
 #if StackIsShared(TN_07)
  TASK_DEF_SHARED(07)
 #else
  TASK_DEF(07)
 #endif
#endif

#if defNumberOfTasks > 8
 #if cfgUseTasknames == cfgTrue
  const Tchar TaskNameStr08[] PROGMEM = TaskName(TN_08);
 #endif
 #if StackIsShared(TN_08)
  TASK_DEF_SHARED(08)
 #else
  TASK_DEF(08)
 #endif
#endif

#if defNumberOfTasks > 9
 #if cfgUseTasknames == cfgTrue
  const Tchar TaskNameStr09[] PROGMEM = TaskName(TN_09);
 #endif
 #if StackIsShared(TN_09)
  TASK_DEF_SHARED(09)
 #else
  TASK_DEF(09)
 #endif
#endif

#if defNumberOfTasks > 10
 #if cfgUseTasknames == cfgTrue
  const Tchar TaskNameStr10[] PROGMEM = TaskName(TN_10);
 #endif
 #if StackIsShared(TN_10)
  TASK_DEF_SHARED(10)
 #else
  TASK_DEF(10)
 #endif
#endif

#if defNumberOfTasks > 11
 #if cfgUseTasknames == cfgTrue
  const Tchar TaskNameStr11[] PROGMEM = TaskName(TN_11);
 #endif
 #if StackIsShared(TN_11)
  TASK_DEF_SHARED(11)
 #else
  TASK_DEF(11)
 #endif
#endif

#if defNumberOfTasks > 12
 #if cfgUseTasknames == cfgTrue
  const Tchar TaskNameStr12[] PROGMEM = TaskName(TN_12);
 #endif
 #if StackIsShared(TN_12)
  TASK_DEF_SHARED(12)
 #else
  TASK_DEF(12)
 #endif
#endif

#if defNumberOfTasks > 13
 #if cfgUseTasknames == cfgTrue
  const Tchar TaskNameStr13[] PROGMEM = TaskName(TN_13);
 #endif
 #if StackIsShared(TN_13)
  TASK_DEF_SHARED(13)
 #else
  TASK_DEF(13)
 #endif
#endif

#if defNumberOfTasks > 14
 #if cfgUseTasknames == cfgTrue
  const Tchar TaskNameStr14[] PROGMEM = TaskName(TN_14);
 #endif
 #if StackIsShared(TN_14)
  TASK_DEF_SHARED(14)
 #else
  TASK_DEF(14)
 #endif
#endif

#if defNumberOfTasks > 15
 #if cfgUseTasknames == cfgTrue
  const Tchar TaskNameStr15[] PROGMEM = TaskName(TN_15);
 #endif
 #if StackIsShared(TN_15)
  TASK_DEF_SHARED(15)
 #else
  TASK_DEF(15)
 #endif
#endif

/* ===================================================================================================================== */

#if defNumberOfQueus > 0
  Tuint08 uiQueuWrite[defNumberOfQueus];
  Tuint08 uiQueuRead[defNumberOfQueus];
  QUEU_DEF(01)
#endif

#if defNumberOfQueus > 1
  QUEU_DEF(02)
#endif

#if defNumberOfQueus > 2
  QUEU_DEF(03)
#endif

#if defNumberOfQueus > 3
  QUEU_DEF(04)
#endif

#if defNumberOfQueus > 4
  QUEU_DEF(05)
#endif

#if defNumberOfQueus > 5
  QUEU_DEF(06)
#endif

#if defNumberOfQueus > 6
  QUEU_DEF(07)
#endif

#if defNumberOfQueus > 7
  QUEU_DEF(08)
#endif

#if defNumberOfQueus > 8
  QUEU_DEF(09)
#endif

#if defNumberOfQueus > 9
  QUEU_DEF(10)
#endif

#if defNumberOfQueus > 10
  QUEU_DEF(11)
#endif

#if defNumberOfQueus > 11
  QUEU_DEF(12)
#endif

#if defNumberOfQueus > 12
  QUEU_DEF(13)
#endif

#if defNumberOfQueus > 13
  QUEU_DEF(14)
#endif

#if defNumberOfQueus > 14
  QUEU_DEF(15)
#endif

/* ===================================================================================================================== */

#if defNumberOfTasks == 0
  const Tbyte * const pxTCBlist[0] PROGMEM =  { };
#elif defNumberOfTasks == 1
  const Tbyte * const pxTCBlist[1] PROGMEM =  { tcb00 };
#elif defNumberOfTasks == 2
  const Tbyte * const pxTCBlist[2] PROGMEM =  { tcb00,tcb01 };
#elif defNumberOfTasks == 3
  const Tbyte * const pxTCBlist[3] PROGMEM =  { tcb00,tcb01,tcb02 };
#elif defNumberOfTasks == 4
  const Tbyte * const pxTCBlist[4] PROGMEM =  { tcb00,tcb01,tcb02,tcb03 };
#elif defNumberOfTasks == 5
  const Tbyte * const pxTCBlist[5] PROGMEM =  { tcb00,tcb01,tcb02,tcb03,tcb04 };
#elif defNumberOfTasks == 6
  const Tbyte * const pxTCBlist[6] PROGMEM =  { tcb00,tcb01,tcb02,tcb03,tcb04,tcb05 };
#elif defNumberOfTasks == 7
  const Tbyte * const pxTCBlist[7] PROGMEM =  { tcb00,tcb01,tcb02,tcb03,tcb04,tcb05,tcb06 };
#elif defNumberOfTasks == 8
  const Tbyte * const pxTCBlist[8] PROGMEM =  { tcb00,tcb01,tcb02,tcb03,tcb04,tcb05,tcb06,tcb07 };
#elif defNumberOfTasks == 9
  const Tbyte * const pxTCBlist[9] PROGMEM =  { tcb00,tcb01,tcb02,tcb03,tcb04,tcb05,tcb06,tcb07,tcb08 };
#elif defNumberOfTasks == 10
  const Tbyte * const pxTCBlist[10] PROGMEM = { tcb00,tcb01,tcb02,tcb03,tcb04,tcb05,tcb06,tcb07,tcb08,tcb09 };
#elif defNumberOfTasks == 11
  const Tbyte * const pxTCBlist[11] PROGMEM = { tcb00,tcb01,tcb02,tcb03,tcb04,tcb05,tcb06,tcb07,tcb08,tcb09,tcb10 };
#elif defNumberOfTasks == 12
  const Tbyte * const pxTCBlist[12] PROGMEM = { tcb00,tcb01,tcb02,tcb03,tcb04,tcb05,tcb06,tcb07,tcb08,tcb09,tcb10,tcb11 };
#elif defNumberOfTasks == 13
  const Tbyte * const pxTCBlist[13] PROGMEM = { tcb00,tcb01,tcb02,tcb03,tcb04,tcb05,tcb06,tcb07,tcb08,tcb09,tcb10,tcb11,tcb12 };
#elif defNumberOfTasks == 14
  const Tbyte * const pxTCBlist[14] PROGMEM = { tcb00,tcb01,tcb02,tcb03,tcb04,tcb05,tcb06,tcb07,tcb08,tcb09,tcb10,tcb11,tcb12,tcb13 };
#elif defNumberOfTasks == 15
  const Tbyte * const pxTCBlist[15] PROGMEM = { tcb00,tcb01,tcb02,tcb03,tcb04,tcb05,tcb06,tcb07,tcb08,tcb09,tcb10,tcb11,tcb12,tcb13,tcb14 };
#elif defNumberOfTasks == 16
  const Tbyte * const pxTCBlist[16] PROGMEM = { tcb00,tcb01,tcb02,tcb03,tcb04,tcb05,tcb06,tcb07,tcb08,tcb09,tcb10,tcb11,tcb12,tcb13,tcb14,tcb15 };
#endif


#if defNumberOfTasks == 0
  const TtaskDefinitionBlock * const pxTDBlist[0] PROGMEM =  { };
#elif defNumberOfTasks == 1
  const TtaskDefinitionBlock * const pxTDBlist[1] PROGMEM =  { &tdb00 };
#elif defNumberOfTasks == 2
  const TtaskDefinitionBlock * const pxTDBlist[2] PROGMEM =  { &tdb00,&tdb01 };
#elif defNumberOfTasks == 3
  const TtaskDefinitionBlock * const pxTDBlist[3] PROGMEM =  { &tdb00,&tdb01,&tdb02 };
#elif defNumberOfTasks == 4
  const TtaskDefinitionBlock * const pxTDBlist[4] PROGMEM =  { &tdb00,&tdb01,&tdb02,&tdb03 };
#elif defNumberOfTasks == 5
  const TtaskDefinitionBlock * const pxTDBlist[5] PROGMEM =  { &tdb00,&tdb01,&tdb02,&tdb03,&tdb04 };
#elif defNumberOfTasks == 6
  const TtaskDefinitionBlock * const pxTDBlist[6] PROGMEM =  { &tdb00,&tdb01,&tdb02,&tdb03,&tdb04,&tdb05 };
#elif defNumberOfTasks == 7
  const TtaskDefinitionBlock * const pxTDBlist[7] PROGMEM =  { &tdb00,&tdb01,&tdb02,&tdb03,&tdb04,&tdb05,&tdb06 };
#elif defNumberOfTasks == 8
  const TtaskDefinitionBlock * const pxTDBlist[8] PROGMEM =  { &tdb00,&tdb01,&tdb02,&tdb03,&tdb04,&tdb05,&tdb06,&tdb07 };
#elif defNumberOfTasks == 9
  const TtaskDefinitionBlock * const pxTDBlist[9] PROGMEM =  { &tdb00,&tdb01,&tdb02,&tdb03,&tdb04,&tdb05,&tdb06,&tdb07,&tdb08 };
#elif defNumberOfTasks == 10
  const TtaskDefinitionBlock * const pxTDBlist[10] PROGMEM = { &tdb00,&tdb01,&tdb02,&tdb03,&tdb04,&tdb05,&tdb06,&tdb07,&tdb08,&tdb09 };
#elif defNumberOfTasks == 11
  const TtaskDefinitionBlock * const pxTDBlist[11] PROGMEM = { &tdb00,&tdb01,&tdb02,&tdb03,&tdb04,&tdb05,&tdb06,&tdb07,&tdb08,&tdb09,&tdb10 };
#elif defNumberOfTasks == 12
  const TtaskDefinitionBlock * const pxTDBlist[12] PROGMEM = { &tdb00,&tdb01,&tdb02,&tdb03,&tdb04,&tdb05,&tdb06,&tdb07,&tdb08,&tdb09,&tdb10,&tdb11 };
#elif defNumberOfTasks == 13
  const TtaskDefinitionBlock * const pxTDBlist[13] PROGMEM = { &tdb00,&tdb01,&tdb02,&tdb03,&tdb04,&tdb05,&tdb06,&tdb07,&tdb08,&tdb09,&tdb10,&tdb11,&tdb12 };
#elif defNumberOfTasks == 14
  const TtaskDefinitionBlock * const pxTDBlist[14] PROGMEM = { &tdb00,&tdb01,&tdb02,&tdb03,&tdb04,&tdb05,&tdb06,&tdb07,&tdb08,&tdb09,&tdb10,&tdb11,&tdb12,&tdb13 };
#elif defNumberOfTasks == 15
  const TtaskDefinitionBlock * const pxTDBlist[15] PROGMEM = { &tdb00,&tdb01,&tdb02,&tdb03,&tdb04,&tdb05,&tdb06,&tdb07,&tdb08,&tdb09,&tdb10,&tdb11,&tdb12,&tdb13,&tdb14 };
#elif defNumberOfTasks == 16
  const TtaskDefinitionBlock * const pxTDBlist[16] PROGMEM = { &tdb00,&tdb01,&tdb02,&tdb03,&tdb04,&tdb05,&tdb06,&tdb07,&tdb08,&tdb09,&tdb10,&tdb11,&tdb12,&tdb13,&tdb14,&tdb15 };
#endif


#if (callAppInit == cfgTrue )

#if ( defNumberOfTasks == 0 )
  const fpInitTask pxInitlist[0] PROGMEM =  { };
#elif ( defNumberOfTasks == 1 )
  const fpInitTask pxInitlist[1] PROGMEM =  { AppInit(TN_00) };
#elif ( defNumberOfTasks == 2 )
  const fpInitTask pxInitlist[2] PROGMEM =  { AppInit(TN_00),AppInit(TN_01) };
#elif ( defNumberOfTasks == 3 )
  const fpInitTask pxInitlist[3] PROGMEM =  { AppInit(TN_00),AppInit(TN_01),AppInit(TN_02) };
#elif ( defNumberOfTasks == 4 )
  const fpInitTask pxInitlist[4] PROGMEM =  { AppInit(TN_00),AppInit(TN_01),AppInit(TN_02),AppInit(TN_03) };
#elif ( defNumberOfTasks == 5 )
  const fpInitTask pxInitlist[5] PROGMEM =  { AppInit(TN_00),AppInit(TN_01),AppInit(TN_02),AppInit(TN_03),AppInit(TN_04) };
#elif ( defNumberOfTasks == 6 )
  const fpInitTask pxInitlist[6] PROGMEM =  { AppInit(TN_00),AppInit(TN_01),AppInit(TN_02),AppInit(TN_03),AppInit(TN_04),AppInit(TN_05) };
#elif ( defNumberOfTasks == 7 )
  const fpInitTask pxInitlist[7] PROGMEM =  { AppInit(TN_00),AppInit(TN_01),AppInit(TN_02),AppInit(TN_03),AppInit(TN_04),AppInit(TN_05),AppInit(TN_06) };
#elif ( defNumberOfTasks == 8 )
  const fpInitTask pxInitlist[8] PROGMEM =  { AppInit(TN_00),AppInit(TN_01),AppInit(TN_02),AppInit(TN_03),AppInit(TN_04),AppInit(TN_05),AppInit(TN_06),AppInit(TN_07) };
#elif ( defNumberOfTasks == 9 )
  const fpInitTask pxInitlist[9] PROGMEM =  { AppInit(TN_00),AppInit(TN_01),AppInit(TN_02),AppInit(TN_03),AppInit(TN_04),AppInit(TN_05),AppInit(TN_06),AppInit(TN_07),AppInit(TN_08) };
#elif ( defNumberOfTasks == 10 )
  const fpInitTask pxInitlist[10] PROGMEM = { AppInit(TN_00),AppInit(TN_01),AppInit(TN_02),AppInit(TN_03),AppInit(TN_04),AppInit(TN_05),AppInit(TN_06),AppInit(TN_07),AppInit(TN_08),AppInit(TN_09) };
#elif ( defNumberOfTasks == 11 )
  const fpInitTask pxInitlist[11] PROGMEM = { AppInit(TN_00),AppInit(TN_01),AppInit(TN_02),AppInit(TN_03),AppInit(TN_04),AppInit(TN_05),AppInit(TN_06),AppInit(TN_07),AppInit(TN_08),AppInit(TN_09),AppInit(TN_10) };
#elif ( defNumberOfTasks == 12 )
  const fpInitTask pxInitlist[12] PROGMEM = { AppInit(TN_00),AppInit(TN_01),AppInit(TN_02),AppInit(TN_03),AppInit(TN_04),AppInit(TN_05),AppInit(TN_06),AppInit(TN_07),AppInit(TN_08),AppInit(TN_09),AppInit(TN_10),AppInit(TN_11) };
#elif ( defNumberOfTasks == 13 )
  const fpInitTask pxInitlist[13] PROGMEM = { AppInit(TN_00),AppInit(TN_01),AppInit(TN_02),AppInit(TN_03),AppInit(TN_04),AppInit(TN_05),AppInit(TN_06),AppInit(TN_07),AppInit(TN_08),AppInit(TN_09),AppInit(TN_10),AppInit(TN_11),AppInit(TN_12) };
#elif ( defNumberOfTasks == 14 )
  const fpInitTask pxInitlist[14] PROGMEM = { AppInit(TN_00),AppInit(TN_01),AppInit(TN_02),AppInit(TN_03),AppInit(TN_04),AppInit(TN_05),AppInit(TN_06),AppInit(TN_07),AppInit(TN_08),AppInit(TN_09),AppInit(TN_10),AppInit(TN_11),AppInit(TN_12),AppInit(TN_13) };
#elif ( defNumberOfTasks == 15 )
  const fpInitTask pxInitlist[15] PROGMEM = { AppInit(TN_00),AppInit(TN_01),AppInit(TN_02),AppInit(TN_03),AppInit(TN_04),AppInit(TN_05),AppInit(TN_06),AppInit(TN_07),AppInit(TN_08),AppInit(TN_09),AppInit(TN_10),AppInit(TN_11),AppInit(TN_12),AppInit(TN_13),AppInit(TN_14) };
#elif ( defNumberOfTasks == 16 )
  const fpInitTask pxInitlist[16] PROGMEM = { AppInit(TN_00),AppInit(TN_01),AppInit(TN_02),AppInit(TN_03),AppInit(TN_04),AppInit(TN_05),AppInit(TN_06),AppInit(TN_07),AppInit(TN_08),AppInit(TN_09),AppInit(TN_10),AppInit(TN_11),AppInit(TN_12),AppInit(TN_13),AppInit(TN_14),AppInit(TN_15) };
#endif

#endif


#if defNumberOfTasks == 0
  const fpLoopTask pxLooplist[0] PROGMEM =  { };
#elif defNumberOfTasks == 1
  const fpLoopTask pxLooplist[1] PROGMEM =  { AppLoop(TN_00) };
#elif defNumberOfTasks == 2
  const fpLoopTask pxLooplist[2] PROGMEM =  { AppLoop(TN_00),AppLoop(TN_01) };
#elif defNumberOfTasks == 3
  const fpLoopTask pxLooplist[3] PROGMEM =  { AppLoop(TN_00),AppLoop(TN_01),AppLoop(TN_02) };
#elif defNumberOfTasks == 4
  const fpLoopTask pxLooplist[4] PROGMEM =  { AppLoop(TN_00),AppLoop(TN_01),AppLoop(TN_02),AppLoop(TN_03) };
#elif defNumberOfTasks == 5
  const fpLoopTask pxLooplist[5] PROGMEM =  { AppLoop(TN_00),AppLoop(TN_01),AppLoop(TN_02),AppLoop(TN_03),AppLoop(TN_04) };
#elif defNumberOfTasks == 6
  const fpLoopTask pxLooplist[6] PROGMEM =  { AppLoop(TN_00),AppLoop(TN_01),AppLoop(TN_02),AppLoop(TN_03),AppLoop(TN_04),AppLoop(TN_05) };
#elif defNumberOfTasks == 7
  const fpLoopTask pxLooplist[7] PROGMEM =  { AppLoop(TN_00),AppLoop(TN_01),AppLoop(TN_02),AppLoop(TN_03),AppLoop(TN_04),AppLoop(TN_05),AppLoop(TN_06) };
#elif defNumberOfTasks == 8
  const fpLoopTask pxLooplist[8] PROGMEM =  { AppLoop(TN_00),AppLoop(TN_01),AppLoop(TN_02),AppLoop(TN_03),AppLoop(TN_04),AppLoop(TN_05),AppLoop(TN_06),AppLoop(TN_07) };
#elif defNumberOfTasks == 9
  const fpLoopTask pxLooplist[9] PROGMEM =  { AppLoop(TN_00),AppLoop(TN_01),AppLoop(TN_02),AppLoop(TN_03),AppLoop(TN_04),AppLoop(TN_05),AppLoop(TN_06),AppLoop(TN_07),AppLoop(TN_08) };
#elif defNumberOfTasks == 10
  const fpLoopTask pxLooplist[10] PROGMEM = { AppLoop(TN_00),AppLoop(TN_01),AppLoop(TN_02),AppLoop(TN_03),AppLoop(TN_04),AppLoop(TN_05),AppLoop(TN_06),AppLoop(TN_07),AppLoop(TN_08),AppLoop(TN_09) };
#elif defNumberOfTasks == 11
  const fpLoopTask pxLooplist[11] PROGMEM = { AppLoop(TN_00),AppLoop(TN_01),AppLoop(TN_02),AppLoop(TN_03),AppLoop(TN_04),AppLoop(TN_05),AppLoop(TN_06),AppLoop(TN_07),AppLoop(TN_08),AppLoop(TN_09),AppLoop(TN_10) };
#elif defNumberOfTasks == 12
  const fpLoopTask pxLooplist[12] PROGMEM = { AppLoop(TN_00),AppLoop(TN_01),AppLoop(TN_02),AppLoop(TN_03),AppLoop(TN_04),AppLoop(TN_05),AppLoop(TN_06),AppLoop(TN_07),AppLoop(TN_08),AppLoop(TN_09),AppLoop(TN_10),AppLoop(TN_11) };
#elif defNumberOfTasks == 13
  const fpLoopTask pxLooplist[13] PROGMEM = { AppLoop(TN_00),AppLoop(TN_01),AppLoop(TN_02),AppLoop(TN_03),AppLoop(TN_04),AppLoop(TN_05),AppLoop(TN_06),AppLoop(TN_07),AppLoop(TN_08),AppLoop(TN_09),AppLoop(TN_10),AppLoop(TN_11),AppLoop(TN_12) };
#elif defNumberOfTasks == 14
  const fpLoopTask pxLooplist[14] PROGMEM = { AppLoop(TN_00),AppLoop(TN_01),AppLoop(TN_02),AppLoop(TN_03),AppLoop(TN_04),AppLoop(TN_05),AppLoop(TN_06),AppLoop(TN_07),AppLoop(TN_08),AppLoop(TN_09),AppLoop(TN_10),AppLoop(TN_11),AppLoop(TN_12),AppLoop(TN_13) };
#elif defNumberOfTasks == 15
  const fpLoopTask pxLooplist[15] PROGMEM = { AppLoop(TN_00),AppLoop(TN_01),AppLoop(TN_02),AppLoop(TN_03),AppLoop(TN_04),AppLoop(TN_05),AppLoop(TN_06),AppLoop(TN_07),AppLoop(TN_08),AppLoop(TN_09),AppLoop(TN_10),AppLoop(TN_11),AppLoop(TN_12),AppLoop(TN_13),AppLoop(TN_14) };
#elif defNumberOfTasks == 16
  const fpLoopTask pxLooplist[16] PROGMEM = { AppLoop(TN_00),AppLoop(TN_01),AppLoop(TN_02),AppLoop(TN_03),AppLoop(TN_04),AppLoop(TN_05),AppLoop(TN_06),AppLoop(TN_07),AppLoop(TN_08),AppLoop(TN_09),AppLoop(TN_10),AppLoop(TN_11),AppLoop(TN_12),AppLoop(TN_13),AppLoop(TN_14),AppLoop(TN_15) };
#endif


#if (cfgUseTaskWatchdog == cfgTrue ) && (callAppBark == cfgTrue)

#if ( defNumberOfWatchdogTasks == 0 )
  const fpBarkTask pxBarklist[0] PROGMEM =  { };
#elif ( defNumberOfWatchdogTasks == 1 )
  const fpBarkTask pxBarklist[1] PROGMEM =  { AppBark(BN_00) };
#elif ( defNumberOfWatchdogTasks == 2 )
  const fpBarkTask pxBarklist[2] PROGMEM =  { AppBark(BN_00),AppBark(BN_01) };
#elif ( defNumberOfWatchdogTasks == 3 )
  const fpBarkTask pxBarklist[3] PROGMEM =  { AppBark(BN_00),AppBark(BN_01),AppBark(BN_02) };
#elif ( defNumberOfWatchdogTasks == 4 )
  const fpBarkTask pxBarklist[4] PROGMEM =  { AppBark(BN_00),AppBark(BN_01),AppBark(BN_02),AppBark(BN_03) };
#elif ( defNumberOfWatchdogTasks == 5 )
  const fpBarkTask pxBarklist[5] PROGMEM =  { AppBark(BN_00),AppBark(BN_01),AppBark(BN_02),AppBark(BN_03),AppBark(BN_04) };
#elif ( defNumberOfWatchdogTasks == 6 )
  const fpBarkTask pxBarklist[6] PROGMEM =  { AppBark(BN_00),AppBark(BN_01),AppBark(BN_02),AppBark(BN_03),AppBark(BN_04),AppBark(BN_05) };
#elif ( defNumberOfWatchdogTasks == 7 )
  const fpBarkTask pxBarklist[7] PROGMEM =  { AppBark(BN_00),AppBark(BN_01),AppBark(BN_02),AppBark(BN_03),AppBark(BN_04),AppBark(BN_05),AppBark(BN_06) };
#elif ( defNumberOfWatchdogTasks == 8 )
  const fpBarkTask pxBarklist[8] PROGMEM =  { AppBark(BN_00),AppBark(BN_01),AppBark(BN_02),AppBark(BN_03),AppBark(BN_04),AppBark(BN_05),AppBark(BN_06),AppBark(BN_07) };
#elif ( defNumberOfWatchdogTasks == 9 )
  const fpBarkTask pxBarklist[9] PROGMEM =  { AppBark(BN_00),AppBark(BN_01),AppBark(BN_02),AppBark(BN_03),AppBark(BN_04),AppBark(BN_05),AppBark(BN_06),AppBark(BN_07),AppBark(BN_08) };
#elif ( defNumberOfWatchdogTasks == 10 )
  const fpBarkTask pxBarklist[10] PROGMEM = { AppBark(BN_00),AppBark(BN_01),AppBark(BN_02),AppBark(BN_03),AppBark(BN_04),AppBark(BN_05),AppBark(BN_06),AppBark(BN_07),AppBark(BN_08),AppBark(BN_09) };
#elif ( defNumberOfWatchdogTasks == 11 )
  const fpBarkTask pxBarklist[11] PROGMEM = { AppBark(BN_00),AppBark(BN_01),AppBark(BN_02),AppBark(BN_03),AppBark(BN_04),AppBark(BN_05),AppBark(BN_06),AppBark(BN_07),AppBark(BN_08),AppBark(BN_09),AppBark(BN_10) };
#elif ( defNumberOfWatchdogTasks == 12 )
  const fpBarkTask pxBarklist[12] PROGMEM = { AppBark(BN_00),AppBark(BN_01),AppBark(BN_02),AppBark(BN_03),AppBark(BN_04),AppBark(BN_05),AppBark(BN_06),AppBark(BN_07),AppBark(BN_08),AppBark(BN_09),AppBark(BN_10),AppBark(BN_11) };
#elif ( defNumberOfWatchdogTasks == 13 )
  const fpBarkTask pxBarklist[13] PROGMEM = { AppBark(BN_00),AppBark(BN_01),AppBark(BN_02),AppBark(BN_03),AppBark(BN_04),AppBark(BN_05),AppBark(BN_06),AppBark(BN_07),AppBark(BN_08),AppBark(BN_09),AppBark(BN_10),AppBark(BN_11),AppBark(BN_12) };
#elif ( defNumberOfWatchdogTasks == 14 )
  const fpBarkTask pxBarklist[14] PROGMEM = { AppBark(BN_00),AppBark(BN_01),AppBark(BN_02),AppBark(BN_03),AppBark(BN_04),AppBark(BN_05),AppBark(BN_06),AppBark(BN_07),AppBark(BN_08),AppBark(BN_09),AppBark(BN_10),AppBark(BN_11),AppBark(BN_12),AppBark(BN_13) };
#elif ( defNumberOfWatchdogTasks == 15 )
  const fpBarkTask pxBarklist[15] PROGMEM = { AppBark(BN_00),AppBark(BN_01),AppBark(BN_02),AppBark(BN_03),AppBark(BN_04),AppBark(BN_05),AppBark(BN_06),AppBark(BN_07),AppBark(BN_08),AppBark(BN_09),AppBark(BN_10),AppBark(BN_11),AppBark(BN_12),AppBark(BN_13),AppBark(BN_14) };
#elif ( defNumberOfWatchdogTasks == 16 )
  const fpBarkTask pxBarklist[16] PROGMEM = { AppBark(BN_00),AppBark(BN_01),AppBark(BN_02),AppBark(BN_03),AppBark(BN_04),AppBark(BN_05),AppBark(BN_06),AppBark(BN_07),AppBark(BN_08),AppBark(BN_09),AppBark(BN_10),AppBark(BN_11),AppBark(BN_12),AppBark(BN_13),AppBark(BN_14),AppBark(BN_15) };
#endif

#endif


#if (cfgUseSynchronization != cfgSyncNon) && (defUseQueus == cfgTrue)

#if defNumberOfQueus == 1
  const Tbyte * pxQueulist[1] PROGMEM = { Queu01 };
#elif defNumberOfQueus == 2
  const Tbyte * pxQueulist[2] PROGMEM = { Queu01,Queu02 };
#elif defNumberOfQueus == 3
  const Tbyte * pxQueulist[3] PROGMEM = { Queu01,Queu02,Queu03 };
#elif defNumberOfQueus == 4
  const Tbyte * pxQueulist[4] PROGMEM = { Queu01,Queu02,Queu03,Queu04 };
#elif defNumberOfQueus == 5
  const Tbyte * pxQueulist[5] PROGMEM = { Queu01,Queu02,Queu03,Queu04,Queu05 };
#elif defNumberOfQueus == 6
  const Tbyte * pxQueulist[6] PROGMEM = { Queu01,Queu02,Queu03,Queu04,Queu05,Queu06 };
#elif defNumberOfQueus == 7
  const Tbyte * pxQueulist[7] PROGMEM = { Queu01,Queu02,Queu03,Queu04,Queu05,Queu06,Queu07 };
#elif defNumberOfQueus == 8
  const Tbyte * pxQueulist[8] PROGMEM = { Queu01,Queu02,Queu03,Queu04,Queu05,Queu06,Queu07,Queu08 };
#elif defNumberOfQueus == 9
  const Tbyte * pxQueulist[9] PROGMEM = { Queu01,Queu02,Queu03,Queu04,Queu05,Queu06,Queu07,Queu08,Queu09 };
#elif defNumberOfQueus == 10
  const Tbyte * pxQueulist[10] PROGMEM = { Queu01,Queu02,Queu03,Queu04,Queu05,Queu06,Queu07,Queu08,Queu09,Queu10 };
#elif defNumberOfQueus == 11
  const Tbyte * pxQueulist[11] PROGMEM = { Queu01,Queu02,Queu03,Queu04,Queu05,Queu06,Queu07,Queu08,Queu09,Queu10,Queu11 };
#elif defNumberOfQueus == 12
  const Tbyte * pxQueulist[12] PROGMEM = { Queu01,Queu02,Queu03,Queu04,Queu05,Queu06,Queu07,Queu08,Queu09,Queu10,Queu11,Queu12 };
#elif defNumberOfQueus == 13
  const Tbyte * pxQueulist[13] PROGMEM = { Queu01,Queu02,Queu03,Queu04,Queu05,Queu06,Queu07,Queu08,Queu09,Queu10,Queu11,Queu12,Queu13 };
#elif defNumberOfQueus == 14
  const Tbyte * pxQueulist[14] PROGMEM = { Queu01,Queu02,Queu03,Queu04,Queu05,Queu06,Queu07,Queu08,Queu09,Queu10,Queu11,Queu12,Queu13,Queu14 };
#elif defNumberOfQueus == 15
  const Tbyte * pxQueulist[15] PROGMEM = { Queu01,Queu02,Queu03,Queu04,Queu05,Queu06,Queu07,Queu08,Queu09,Queu10,Queu11,Queu12,Queu13,Queu14,Queu15 };
#endif

#endif


#if (cfgUseSynchronization != cfgSyncNon) && (defUseQueus == cfgTrue) && (defQueuSizeConstant == cfgFalse)

#if defNumberOfQueus == 1
  const Tuint08 uiQueuSize[1] PROGMEM = { QueuSize(LN_01) };
#elif defNumberOfQueus == 2
  const Tuint08 uiQueuSize[2] PROGMEM = { QueuSize(LN_01),QueuSize(LN_02) };
#elif defNumberOfQueus == 3
  const Tuint08 uiQueuSize[3] PROGMEM = { QueuSize(LN_01),QueuSize(LN_02),QueuSize(LN_03) };
#elif defNumberOfQueus == 4
  const Tuint08 uiQueuSize[4] PROGMEM = { QueuSize(LN_01),QueuSize(LN_02),QueuSize(LN_03),QueuSize(LN_04) };
#elif defNumberOfQueus == 5
  const Tuint08 uiQueuSize[5] PROGMEM = { QueuSize(LN_01),QueuSize(LN_02),QueuSize(LN_03),QueuSize(LN_04),QueuSize(LN_05) };
#elif defNumberOfQueus == 6
  const Tuint08 uiQueuSize[6] PROGMEM = { QueuSize(LN_01),QueuSize(LN_02),QueuSize(LN_03),QueuSize(LN_04),QueuSize(LN_05),QueuSize(LN_06) };
#elif defNumberOfQueus == 7
  const Tuint08 uiQueuSize[7] PROGMEM = { QueuSize(LN_01),QueuSize(LN_02),QueuSize(LN_03),QueuSize(LN_04),QueuSize(LN_05),QueuSize(LN_06),QueuSize(LN_07) };
#elif defNumberOfQueus == 8
  const Tuint08 uiQueuSize[8] PROGMEM = { QueuSize(LN_01),QueuSize(LN_02),QueuSize(LN_03),QueuSize(LN_04),QueuSize(LN_05),QueuSize(LN_06),QueuSize(LN_07),QueuSize(LN_08) };
#elif defNumberOfQueus == 9
  const Tuint08 uiQueuSize[9] PROGMEM = { QueuSize(LN_01),QueuSize(LN_02),QueuSize(LN_03),QueuSize(LN_04),QueuSize(LN_05),QueuSize(LN_06),QueuSize(LN_07),QueuSize(LN_08),QueuSize(LN_09) };
#elif defNumberOfQueus == 10
  const Tuint08 uiQueuSize[10] PROGMEM = { QueuSize(LN_01),QueuSize(LN_02),QueuSize(LN_03),QueuSize(LN_04),QueuSize(LN_05),QueuSize(LN_06),QueuSize(LN_07),QueuSize(LN_08),QueuSize(LN_09),QueuSize(LN_10) };
#elif defNumberOfQueus == 11
  const Tuint08 uiQueuSize[11] PROGMEM = { QueuSize(LN_01),QueuSize(LN_02),QueuSize(LN_03),QueuSize(LN_04),QueuSize(LN_05),QueuSize(LN_06),QueuSize(LN_07),QueuSize(LN_08),QueuSize(LN_09),QueuSize(LN_10),QueuSize(LN_11) };
#elif defNumberOfQueus == 12
  const Tuint08 uiQueuSize[12] PROGMEM = { QueuSize(LN_01),QueuSize(LN_02),QueuSize(LN_03),QueuSize(LN_04),QueuSize(LN_05),QueuSize(LN_06),QueuSize(LN_07),QueuSize(LN_08),QueuSize(LN_09),QueuSize(LN_10),QueuSize(LN_11),QueuSize(LN_12) };
#elif defNumberOfQueus == 13
  const Tuint08 uiQueuSize[13] PROGMEM = { QueuSize(LN_01),QueuSize(LN_02),QueuSize(LN_03),QueuSize(LN_04),QueuSize(LN_05),QueuSize(LN_06),QueuSize(LN_07),QueuSize(LN_08),QueuSize(LN_09),QueuSize(LN_10),QueuSize(LN_11),QueuSize(LN_12),QueuSize(LN_13) };
#elif defNumberOfQueus == 14
  const Tuint08 uiQueuSize[14] PROGMEM = { QueuSize(LN_01),QueuSize(LN_02),QueuSize(LN_03),QueuSize(LN_04),QueuSize(LN_05),QueuSize(LN_06),QueuSize(LN_07),QueuSize(LN_08),QueuSize(LN_09),QueuSize(LN_10),QueuSize(LN_11),QueuSize(LN_12),QueuSize(LN_13),QueuSize(LN_14) };
#elif defNumberOfQueus == 15
  const Tuint08 uiQueuSize[15] PROGMEM = { QueuSize(LN_01),QueuSize(LN_02),QueuSize(LN_03),QueuSize(LN_04),QueuSize(LN_05),QueuSize(LN_06),QueuSize(LN_07),QueuSize(LN_08),QueuSize(LN_09),QueuSize(LN_10),QueuSize(LN_11),QueuSize(LN_12),QueuSize(LN_13),QueuSize(LN_14),QueuSize(LN_15) };
#endif

#endif

#if (cfgUseFileSystem == cfgTrue) && (defFilePreSpaceConstant == cfgFalse)

#if defFsNumberOfPreFiles == 0
  const Tuint08 uiFileSpace[0] PROGMEM =  { };
#elif defFsNumberOfPreFiles == 1
  const Tuint08 uiFileSpace[1] PROGMEM =  { FileSpace(VN_00) };
#elif defFsNumberOfPreFiles == 2
  const Tuint08 uiFileSpace[2] PROGMEM =  { FileSpace(VN_00),FileSpace(VN_01) };
#elif defFsNumberOfPreFiles == 3
  const Tuint08 uiFileSpace[3] PROGMEM =  { FileSpace(VN_00),FileSpace(VN_01),FileSpace(VN_02) };
#elif defFsNumberOfPreFiles == 4
  const Tuint08 uiFileSpace[4] PROGMEM =  { FileSpace(VN_00),FileSpace(VN_01),FileSpace(VN_02),FileSpace(VN_03) };
#elif defFsNumberOfPreFiles == 5
  const Tuint08 uiFileSpace[5] PROGMEM =  { FileSpace(VN_00),FileSpace(VN_01),FileSpace(VN_02),FileSpace(VN_03),FileSpace(VN_04) };
#elif defFsNumberOfPreFiles == 6
  const Tuint08 uiFileSpace[6] PROGMEM =  { FileSpace(VN_00),FileSpace(VN_01),FileSpace(VN_02),FileSpace(VN_03),FileSpace(VN_04),FileSpace(VN_05) };
#elif defFsNumberOfPreFiles == 7
  const Tuint08 uiFileSpace[7] PROGMEM =  { FileSpace(VN_00),FileSpace(VN_01),FileSpace(VN_02),FileSpace(VN_03),FileSpace(VN_04),FileSpace(VN_05),FileSpace(VN_06) };
#elif defFsNumberOfPreFiles == 8
  const Tuint08 uiFileSpace[8] PROGMEM =  { FileSpace(VN_00),FileSpace(VN_01),FileSpace(VN_02),FileSpace(VN_03),FileSpace(VN_04),FileSpace(VN_05),FileSpace(VN_06),FileSpace(VN_07) };
#elif defFsNumberOfPreFiles == 9
  const Tuint08 uiFileSpace[9] PROGMEM =  { FileSpace(VN_00),FileSpace(VN_01),FileSpace(VN_02),FileSpace(VN_03),FileSpace(VN_04),FileSpace(VN_05),FileSpace(VN_06),FileSpace(VN_07),FileSpace(VN_08) };
#elif defFsNumberOfPreFiles == 10
  const Tuint08 uiFileSpace[10] PROGMEM = { FileSpace(VN_00),FileSpace(VN_01),FileSpace(VN_02),FileSpace(VN_03),FileSpace(VN_04),FileSpace(VN_05),FileSpace(VN_06),FileSpace(VN_07),FileSpace(VN_08),FileSpace(VN_09) };
#elif defFsNumberOfPreFiles == 11
  const Tuint08 uiFileSpace[11] PROGMEM = { FileSpace(VN_00),FileSpace(VN_01),FileSpace(VN_02),FileSpace(VN_03),FileSpace(VN_04),FileSpace(VN_05),FileSpace(VN_06),FileSpace(VN_07),FileSpace(VN_08),FileSpace(VN_09),FileSpace(VN_10) };
#elif defFsNumberOfPreFiles == 12
  const Tuint08 uiFileSpace[12] PROGMEM = { FileSpace(VN_00),FileSpace(VN_01),FileSpace(VN_02),FileSpace(VN_03),FileSpace(VN_04),FileSpace(VN_05),FileSpace(VN_06),FileSpace(VN_07),FileSpace(VN_08),FileSpace(VN_09),FileSpace(VN_10),FileSpace(VN_11) };
#elif defFsNumberOfPreFiles == 13
  const Tuint08 uiFileSpace[13] PROGMEM = { FileSpace(VN_00),FileSpace(VN_01),FileSpace(VN_02),FileSpace(VN_03),FileSpace(VN_04),FileSpace(VN_05),FileSpace(VN_06),FileSpace(VN_07),FileSpace(VN_08),FileSpace(VN_09),FileSpace(VN_10),FileSpace(VN_11),FileSpace(VN_12) };
#elif defFsNumberOfPreFiles == 14
  const Tuint08 uiFileSpace[14] PROGMEM = { FileSpace(VN_00),FileSpace(VN_01),FileSpace(VN_02),FileSpace(VN_03),FileSpace(VN_04),FileSpace(VN_05),FileSpace(VN_06),FileSpace(VN_07),FileSpace(VN_08),FileSpace(VN_09),FileSpace(VN_10),FileSpace(VN_11),FileSpace(VN_12),FileSpace(VN_13) };
#elif defFsNumberOfPreFiles == 15
  const Tuint08 uiFileSpace[15] PROGMEM = { FileSpace(VN_00),FileSpace(VN_01),FileSpace(VN_02),FileSpace(VN_03),FileSpace(VN_04),FileSpace(VN_05),FileSpace(VN_06),FileSpace(VN_07),FileSpace(VN_08),FileSpace(VN_09),FileSpace(VN_10),FileSpace(VN_11),FileSpace(VN_12),FileSpace(VN_13),FileSpace(VN_14) };
#elif defFsNumberOfPreFiles == 16
  const Tuint08 uiFileSpace[16] PROGMEM = { FileSpace(VN_00),FileSpace(VN_01),FileSpace(VN_02),FileSpace(VN_03),FileSpace(VN_04),FileSpace(VN_05),FileSpace(VN_06),FileSpace(VN_07),FileSpace(VN_08),FileSpace(VN_09),FileSpace(VN_10),FileSpace(VN_11),FileSpace(VN_12),FileSpace(VN_13),FileSpace(VN_14),FileSpace(VN_15) };
#endif

#endif


  #if (cfgCheckMethodUse == cfgTrue) && (defCapabilitiesFull == cfgFalse)

#if defNumberOfTasks == 0
  const Tuint08 uiCapabilities[0] PROGMEM =  { };
#elif defNumberOfTasks == 1
  const Tuint08 uiCapabilities[1] PROGMEM =  { (Capabilities(TN_00) & 0xFF) };
#elif defNumberOfTasks == 2
  const Tuint08 uiCapabilities[2] PROGMEM =  { (Capabilities(TN_00) & 0xFF),(Capabilities(TN_01) & 0xFF) };
#elif defNumberOfTasks == 3
  const Tuint08 uiCapabilities[3] PROGMEM =  { (Capabilities(TN_00) & 0xFF),(Capabilities(TN_01) & 0xFF),(Capabilities(TN_02) & 0xFF) };
#elif defNumberOfTasks == 4
  const Tuint08 uiCapabilities[4] PROGMEM =  { (Capabilities(TN_00) & 0xFF),(Capabilities(TN_01) & 0xFF),(Capabilities(TN_02) & 0xFF),(Capabilities(TN_03) & 0xFF) };
#elif defNumberOfTasks == 5
  const Tuint08 uiCapabilities[5] PROGMEM =  { (Capabilities(TN_00) & 0xFF),(Capabilities(TN_01) & 0xFF),(Capabilities(TN_02) & 0xFF),(Capabilities(TN_03) & 0xFF),(Capabilities(TN_04) & 0xFF) };
#elif defNumberOfTasks == 6
  const Tuint08 uiCapabilities[6] PROGMEM =  { (Capabilities(TN_00) & 0xFF),(Capabilities(TN_01) & 0xFF),(Capabilities(TN_02) & 0xFF),(Capabilities(TN_03) & 0xFF),(Capabilities(TN_04) & 0xFF),(Capabilities(TN_05) & 0xFF) };
#elif defNumberOfTasks == 7
  const Tuint08 uiCapabilities[7] PROGMEM =  { (Capabilities(TN_00) & 0xFF),(Capabilities(TN_01) & 0xFF),(Capabilities(TN_02) & 0xFF),(Capabilities(TN_03) & 0xFF),(Capabilities(TN_04) & 0xFF),(Capabilities(TN_05) & 0xFF),(Capabilities(TN_06) & 0xFF) };
#elif defNumberOfTasks == 8
  const Tuint08 uiCapabilities[8] PROGMEM =  { (Capabilities(TN_00) & 0xFF),(Capabilities(TN_01) & 0xFF),(Capabilities(TN_02) & 0xFF),(Capabilities(TN_03) & 0xFF),(Capabilities(TN_04) & 0xFF),(Capabilities(TN_05) & 0xFF),(Capabilities(TN_06) & 0xFF),(Capabilities(TN_07) & 0xFF) };
#elif defNumberOfTasks == 9
  const Tuint08 uiCapabilities[9] PROGMEM =  { (Capabilities(TN_00) & 0xFF),(Capabilities(TN_01) & 0xFF),(Capabilities(TN_02) & 0xFF),(Capabilities(TN_03) & 0xFF),(Capabilities(TN_04) & 0xFF),(Capabilities(TN_05) & 0xFF),(Capabilities(TN_06) & 0xFF),(Capabilities(TN_07) & 0xFF),(Capabilities(TN_08) & 0xFF) };
#elif defNumberOfTasks == 10
  const Tuint08 uiCapabilities[10] PROGMEM = { (Capabilities(TN_00) & 0xFF),(Capabilities(TN_01) & 0xFF),(Capabilities(TN_02) & 0xFF),(Capabilities(TN_03) & 0xFF),(Capabilities(TN_04) & 0xFF),(Capabilities(TN_05) & 0xFF),(Capabilities(TN_06) & 0xFF),(Capabilities(TN_07) & 0xFF),(Capabilities(TN_08) & 0xFF),(Capabilities(TN_09) & 0xFF) };
#elif defNumberOfTasks == 11
  const Tuint08 uiCapabilities[11] PROGMEM = { (Capabilities(TN_00) & 0xFF),(Capabilities(TN_01) & 0xFF),(Capabilities(TN_02) & 0xFF),(Capabilities(TN_03) & 0xFF),(Capabilities(TN_04) & 0xFF),(Capabilities(TN_05) & 0xFF),(Capabilities(TN_06) & 0xFF),(Capabilities(TN_07) & 0xFF),(Capabilities(TN_08) & 0xFF),(Capabilities(TN_09) & 0xFF),(Capabilities(TN_10) & 0xFF) };
#elif defNumberOfTasks == 12
  const Tuint08 uiCapabilities[12] PROGMEM = { (Capabilities(TN_00) & 0xFF),(Capabilities(TN_01) & 0xFF),(Capabilities(TN_02) & 0xFF),(Capabilities(TN_03) & 0xFF),(Capabilities(TN_04) & 0xFF),(Capabilities(TN_05) & 0xFF),(Capabilities(TN_06) & 0xFF),(Capabilities(TN_07) & 0xFF),(Capabilities(TN_08) & 0xFF),(Capabilities(TN_09) & 0xFF),(Capabilities(TN_10) & 0xFF),(Capabilities(TN_11) & 0xFF) };
#elif defNumberOfTasks == 13
  const Tuint08 uiCapabilities[13] PROGMEM = { (Capabilities(TN_00) & 0xFF),(Capabilities(TN_01) & 0xFF),(Capabilities(TN_02) & 0xFF),(Capabilities(TN_03) & 0xFF),(Capabilities(TN_04) & 0xFF),(Capabilities(TN_05) & 0xFF),(Capabilities(TN_06) & 0xFF),(Capabilities(TN_07) & 0xFF),(Capabilities(TN_08) & 0xFF),(Capabilities(TN_09) & 0xFF),(Capabilities(TN_10) & 0xFF),(Capabilities(TN_11) & 0xFF),(Capabilities(TN_12) & 0xFF) };
#elif defNumberOfTasks == 14
  const Tuint08 uiCapabilities[14] PROGMEM = { (Capabilities(TN_00) & 0xFF),(Capabilities(TN_01) & 0xFF),(Capabilities(TN_02) & 0xFF),(Capabilities(TN_03) & 0xFF),(Capabilities(TN_04) & 0xFF),(Capabilities(TN_05) & 0xFF),(Capabilities(TN_06) & 0xFF),(Capabilities(TN_07) & 0xFF),(Capabilities(TN_08) & 0xFF),(Capabilities(TN_09) & 0xFF),(Capabilities(TN_10) & 0xFF),(Capabilities(TN_11) & 0xFF),(Capabilities(TN_12) & 0xFF),(Capabilities(TN_13) & 0xFF) };
#elif defNumberOfTasks == 15
  const Tuint08 uiCapabilities[15] PROGMEM = { (Capabilities(TN_00) & 0xFF),(Capabilities(TN_01) & 0xFF),(Capabilities(TN_02) & 0xFF),(Capabilities(TN_03) & 0xFF),(Capabilities(TN_04) & 0xFF),(Capabilities(TN_05) & 0xFF),(Capabilities(TN_06) & 0xFF),(Capabilities(TN_07) & 0xFF),(Capabilities(TN_08) & 0xFF),(Capabilities(TN_09) & 0xFF),(Capabilities(TN_10) & 0xFF),(Capabilities(TN_11) & 0xFF),(Capabilities(TN_12) & 0xFF),(Capabilities(TN_13) & 0xFF),(Capabilities(TN_14) & 0xFF) };
#elif defNumberOfTasks == 16
  const Tuint08 uiCapabilities[16] PROGMEM = { (Capabilities(TN_00) & 0xFF),(Capabilities(TN_01) & 0xFF),(Capabilities(TN_02) & 0xFF),(Capabilities(TN_03) & 0xFF),(Capabilities(TN_04) & 0xFF),(Capabilities(TN_05) & 0xFF),(Capabilities(TN_06) & 0xFF),(Capabilities(TN_07) & 0xFF),(Capabilities(TN_08) & 0xFF),(Capabilities(TN_09) & 0xFF),(Capabilities(TN_10) & 0xFF),(Capabilities(TN_11) & 0xFF),(Capabilities(TN_12) & 0xFF),(Capabilities(TN_13) & 0xFF),(Capabilities(TN_14) & 0xFF),(Capabilities(TN_15) & 0xFF) };
#endif

#endif

#endif /* TASKS_H */
