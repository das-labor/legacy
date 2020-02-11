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

#ifndef FEMTOOS_TYPES_H
#define FEMTOOS_TYPES_H

#include "femtoos_globals.h"
#include "femtoos_constants.h"
#include "femtoos_order.h"
#include "femtoos_locals.h"


typedef union
{ Tuint16 MaxSelect;
  struct
  { Tuint08 MaxTask;
    Tuint08 MaxStatus; };
} Tselect;

/**
 * We use two different types of tick, Ttick and TextendedTick. The former is used if we have standard tick
 * interrupts (i.e. equidistant interrupts) and there is no further need for extra storage. The 16 bit tick counter
 * is kept in two separate bytes, which is more efficient as one 16 bit variable for direct access. However, sometimes
 * it is convenient to have the value as 16 bit word at hand, thus we define a union.
 */
typedef union
{ Tuint16 Full;
  struct
  { Tuint08 LowByte;
    Tuint08 HighByte; };
} Ttick;


/**
 * It is assumed that the union matches Full with LowBtye/HighByte. I am not sure if this is portable across
 * compilers, or across architectures. Probably not.
 */
typedef union
{ Tuint16 Full;
  struct
  { Tuint08 LowByte;
    Tuint08 HighByte;
    Tuint08 SubByte; };
} TextendedTick;


#if (cfgUseEquidistantTicks == cfgTrue) && (cfgUseLowPowerSleep == cfgFalse) && (cfgCheckWatermarks == cfgFalse) && (cfgUseLoadMonitor == cfgFalse)
  #define Ttickcount Ttick
#else
  #define Ttickcount TextendedTick
#endif


/**
 * TtaskSave is the structure which holds the "background" variables. These are the variables which
 * are kept while a task is running. In fact, it is a way to communicate information from portRestoreContext
 * to portSaveContext. Also it is used to save some registers temporarily during port(Save/Restore)Context.
 * We have two versions depending on the direction of stack growth. Note that, since some of these variables
 * must be retained until the OS has restarted, some buffer variables are needed. These keep a save
 * distance between the variables needed and the building OS stack. Also, if we have no checking switched
 * on, we can use a shorter version of the TtaskSave.
 * The number of buffer variables is 4 at a minimum (2 addresses, namely a call to privInitOs() and a possible
 * subsequent call to tcbCurrent(). It is assumed that those functions do not push more variables on the
 * stack. This must be checked. In case we have no checking, only one address is pushed so we need only 2
 * buffer variables (one address).
 * Please note the the order of the variables is of the utmost importance, do not change it unless you
 * know what you are doing (and believe me, you don't yet). Just to get you going, mark these facts:
 * - The register-copy variables r31-r28 are filled at the beginning of the save context and restored
 *   at the end, because we must leave all registers untouched. The restore is thus done after the stack
 *   change to OS. Thus these variables may not be overwritten, and therefore the place to store them is
 *   as far as possible from the return address that is placed upon the OS stack.
 * - uiStackTCheck, uiRegisterCheck and uiRegisterUse are used to check the result of the context switch
 *   in privInitOS. Depending on their values, the task may be terminated. So these values should survive
 *   the first method calls to and in privInitOS.
 * - in standard operation, no registers are needed from the background variables after a return from save
 *   context. However, when checks are in place, some variables are read after the return. Those variables
 *   must be preserved upon the return. The return itself destroys at least 2 (3 for mega256) bytes, and
 *   after return there must be room for two consecutive jumps, i.e. 4 (6) bytes.
 *
 *   Note: make sure that any change in this structure is reflected in the defTaskSaveSize constant!
 */

#if (cfgSysStackGrowthUp == cfgTrue)

  #if (cfgCheckTaskStack == cfgTrue) || (cfgCheckRegisters == cfgTrue) || (cfgCheckWatermarks == cfgTrue)

    typedef struct
    {
      #if (cfgUseSynchronization != cfgSyncNon) && (defUseBoolReturns == cfgTrue)
        Tuint08   uiReturn;
      #endif
      Taddress  pcStackLimit;
      Taddress  pcStackLevel;
      Taddress  pcStackOffset;
      Tuint08   uiStackTCheck;
      Tuint08   uiRegisterCheck;
      Tuint08   uiRegisterUse;
      Tuint08   r28;
      Tuint08   r29;
      Tuint08   r30;
      Tuint08   r31;
    } TtaskSave;

  #else

    typedef struct
    {
      #if (cfgUseSynchronization != cfgSyncNon) && (defUseBoolReturns == cfgTrue)
        Tuint08   uiReturn;
      #endif
      Taddress  pcStackLevel;
      Taddress  pcStackOffset;
      Tuint08   uiRegisterUse;
      Tuint08   r30;
      Tuint08   r31;
    } TtaskSave;

  #endif

#else

  #if (cfgCheckTaskStack == cfgTrue) || (cfgCheckRegisters == cfgTrue) || (cfgCheckWatermarks == cfgTrue)

    typedef struct
    { Tuint08   r31;
      Tuint08   r30;
      Tuint08   r29;
      Tuint08   r28;
      Tuint08   uiRegisterUse;
      Tuint08   uiRegisterCheck;
      Tuint08   uiStackTCheck;
      Taddress  pcStackOffset;
      Taddress  pcStackLevel;
      Taddress  pcStackLimit;
      #if (cfgUseSynchronization != cfgSyncNon) && (defUseBoolReturns == cfgTrue)
        Tuint08   uiReturn;
      #endif
    } TtaskSave;

  #else
    typedef struct
    { Tuint08   r31;
      Tuint08   r30;
      Tuint08   uiRegisterUse;
      Taddress  pcStackOffset;
      Taddress  pcStackLevel;
      #if (cfgUseSynchronization != cfgSyncNon) && (defUseBoolReturns == cfgTrue)
        Tuint08   uiReturn;
      #endif
    } TtaskSave;

  #endif

#endif


/**
 * Since at that time a task is running there is no need for the OS stack, we make a union of the
 * background variables and the OS stack. Note that this places boundaries of the StackSize (which
 * can be checked), and implies that the structure of TtaskSave depends on the direction of stack growth
 * and the alignment of the array and the structure (which is assumed to be top-aligned, i.e. the
 * lowest used addresses match). Thus this is a delicate matter. Therefore we have the possibility to
 * use a normal struct as well. Note the StackOS must be the first field, because of the definition of
 * __do_stack_def: in the devices files. (i don't know a way to address xOS.StackOS in assembly directly).
 */
#if (cfgSysReuseOsStack == cfgTrue)
  typedef union
  { Tchar StackOS[StackSizeOS];
    TtaskSave pxSave;
  } TosData;
#else
  typedef struct
  { Tchar StackOS[StackSizeOS];
    TtaskSave pxSave;
  } TosData;
#endif


/**
 * Per task we have one task control block which holds all dynamic information of a task.
 * This includes the status, the delay, the stack level, information about the held locks,
 * if the task is still running, return values, nesting levels of interrupts and lock
 * values of queues. Furthermore some task statistics are kept. Several members of the
 * structure are optional and are only compiled in if that functionality is used.
 */
typedef struct
{ Tstack    pcStackLevel;
  Tuint08   uiTaskStatus;
  #if (defUseDelay == cfgTrue)
    Ttick   uxDelay;
  #endif
  #if (defUseTaskMonitor == cfgTrue)
    Tuint08 uiTaskMonitor;
  #endif
  #if (defUseTaskLevels == cfgTrue)
    Tuint08 uiTaskLevels;
  #endif
  #if (cfgUseEvents == cfgTrue)
    Tuint08 uiTaskEvents;
  #endif
  #if (cfgUseLoadMonitor == cfgTrue)
    Tuint16 uiLoadCollect;
    Tuint16 uiLoadTotal;
  #endif
  #if (cfgCheckWatermarks == cfgTrue)
    Tstack  uiStackMax;
    Tuint08 uiRegisterUse;
  #endif
} TtaskControlBlock;

typedef struct
{ Tstack    pcStackLevel;
  Tuint08   uiTaskStatus;
  #if (defUseDelay == cfgTrue)
    Ttick   uxDelay;
  #endif
  #if (defUseTaskMonitor == cfgTrue)
    Tuint08 uiTaskMonitor;
  #endif
  #if (defUseTaskLevels == cfgTrue)
    Tuint08 uiTaskLevels;
  #endif
  #if (cfgUseEvents == cfgTrue)
    Tuint08 uiTaskEvents;
  #endif
  #if (cfgUseLoadMonitor == cfgTrue)
    Tuint16 uiLoadCollect;
    Tuint16 uiLoadTotal;
  #endif
  #if (cfgCheckWatermarks == cfgTrue)
    Tstack  uiStackMax;
    Tuint08 uiRegisterUse;
  #endif
  #if (defUseQueus == cfgTrue)
    Tsint08 siQueuRightLock;
    #if (cfgUseSynchronization == cfgSyncDoubleBlock)
      Tsint08 siQueuLeftLock;
    #endif
  #endif
  /* This definition must be on the end, for the space is created by an variable sized array */
  #if (defUseTaskSlot == cfgTrue)
    Tuint08 uiTaskSlot;
  #endif
} TtaskExtendedControlBlock;

/**.
 * Function definition of Initialization code which every task may have. This contains
 * all initialization which must be done if a task is started or restarted.
 */
typedef void (*fpInitTask)(void);

/**.
 * Function definition of loop code which every task must have. This contains the
 * workload of a task.
 */
typedef void (*fpLoopTask)(void);

/**.
 * Function definition of bark code which every task may have. This contains the
 * instructions that must be executed if a task gets stuck.
 */
typedef void (*fpBarkTask)(void);


/**
 * Per task we have one task definition block which holds all static information of a task.
 * This includes the use and checking of registers, status, the initial priority, the initial
 * state of the interrupts (global and tick) the maximal stack size, the place where in ram
 * the stack starts off, the length of a time slice, the runtime task name.
 * Not all fields must be present, this depends on the settings of the configuration parameters.
 * The order of fields may not be changed, since this variable is placed in flash, and
 * is filled by preprocessor generated sequences.
 */
typedef struct
{
  #if (defRegisterUseConstant == cfgFalse)
    const Tuint08   uiRegisterUse;
  #endif
  #if (defRegisterCheckConstant == cfgFalse)
    const Tuint08   uiRegisterCheck;
  #endif
  #if (defInitialStatusConstant == cfgFalse)
    const Tuint08   uiInitialStatus;
  #endif
  #if (defStackSizeConstant == cfgFalse) && ( (cfgCheckTaskStack == cfgTrue) || (cfgCheckWatermarks == cfgTrue) || (defStackClean == cfgTrue) )
    const Tstack    uiStackSize;
  #endif
  const Taddress    pcStackOffset;
  #if (defTimeSliceConstant == cfgFalse)
    const Tuint08   uiTimeSlice;
  #endif
  #if (cfgUseSynchronization != cfgSyncNon) && (defSlotDepthConstant == cfgFalse)
    const Tuint08 uiSlotDepth;
  #endif
  #if (cfgUseTasknames == cfgTrue)
    const Taddress  pTaskName;
  #endif
} TtaskDefinitionBlock;


#endif /* FEMTOOS_TYPES_H */
