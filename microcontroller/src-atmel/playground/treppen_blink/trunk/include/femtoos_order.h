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

#ifndef FEMTOOS_ORDER_H
#define FEMTOOS_ORDER_H

/* Definitions of uses in the system that are directly deduced from the configuration
 * parameters.*/

#if (cfgIntUserDefined == cfgTrue) && (cfgIntSwitchUsesOSstack == cfgFalse)
  #define defUseIsrStack               cfgTrue
#else
  #define defUseIsrStack               cfgFalse
#endif

#if (cfgUseDelay == cfgTrue) || ((cfgUseSynchronization != cfgSyncNon) && (cfgUseTimeout == cfgTrue))
  #define defUseDelay                  cfgTrue
#else
  #define defUseDelay                  cfgFalse
#endif

#if ((cfgUseTaskWatchdog == cfgTrue) || (includeTaskRecreate == cfgTrue) || (includeTaskRestart == cfgTrue) || (includeTaskTerminate == cfgTrue))
  #define defReUseTaskInit             cfgTrue
#else
  #define defReUseTaskInit             cfgFalse
#endif

#if (cfgUseFileSystem == cfgTrue) && ((defReUseTaskInit == cfgTrue) || (cfgUseLowPowerSleep == cfgTrue) || (includeTaskTerminate == cfgTrue) || (cfgCheckMethodUse == cfgTrue))
  #define defUseFsField                cfgTrue
#else
  #define defUseFsField                cfgFalse
#endif

#if (defUseFsField == cfgTrue) || (cfgUseTaskWatchdog == cfgTrue) || (defUseBoolReturns == cfgTrue) || (includeGenSuspend == cfgTrue)
  #define defUseTaskMonitor            cfgTrue
#else
  #define defUseTaskMonitor            cfgFalse
#endif

#if (cfgUseSynchronization  != cfgSyncNon)
  #define defUseTaskSlot               cfgTrue
#else
  #define defUseTaskSlot               cfgFalse
#endif

#if (cfgUseNestedCriticals == cfgTrue)
  #define defUseTaskLevels             cfgTrue
#else
  #define defUseTaskLevels             cfgFalse
#endif

#if (cfgUseSynchronization != cfgSyncNon) && ((defined(SN_01) && (SlotUse(SN_01) == cfgUseAsQueu)) || (defined(SN_02) && (SlotUse(SN_02) == cfgUseAsQueu)) || (defined(SN_03) && (SlotUse(SN_03) == cfgUseAsQueu)) || (defined(SN_04) && (SlotUse(SN_04) == cfgUseAsQueu)) || (defined(SN_05) && (SlotUse(SN_05) == cfgUseAsQueu)) || (defined(SN_06) && (SlotUse(SN_06) == cfgUseAsQueu)) || (defined(SN_07) && (SlotUse(SN_07) == cfgUseAsQueu)) || (defined(SN_08) && (SlotUse(SN_08) == cfgUseAsQueu)) || (defined(SN_09) && (SlotUse(SN_09) == cfgUseAsQueu)) || (defined(SN_10) && (SlotUse(SN_10) == cfgUseAsQueu)) || (defined(SN_11) && (SlotUse(SN_11) == cfgUseAsQueu)) || (defined(SN_12) && (SlotUse(SN_12) == cfgUseAsQueu)) || (defined(SN_13) && (SlotUse(SN_13) == cfgUseAsQueu)) || (defined(SN_14) && (SlotUse(SN_14) == cfgUseAsQueu)) || (defined(SN_15) && (SlotUse(SN_15) == cfgUseAsQueu)))
  #define defUseQueus                  cfgTrue
#else
  #define defUseQueus                  cfgFalse
#endif

#if (cfgUseSynchronization != cfgSyncNon) && ((defined(SN_01) && (SlotUse(SN_01) == cfgUseAsMutex)) || (defined(SN_02) && (SlotUse(SN_02) == cfgUseAsMutex)) || (defined(SN_03) && (SlotUse(SN_03) == cfgUseAsMutex)) || (defined(SN_04) && (SlotUse(SN_04) == cfgUseAsMutex)) || (defined(SN_05) && (SlotUse(SN_05) == cfgUseAsMutex)) || (defined(SN_06) && (SlotUse(SN_06) == cfgUseAsMutex)) || (defined(SN_07) && (SlotUse(SN_07) == cfgUseAsMutex)) || (defined(SN_08) && (SlotUse(SN_08) == cfgUseAsMutex)) || (defined(SN_09) && (SlotUse(SN_09) == cfgUseAsMutex)) || (defined(SN_10) && (SlotUse(SN_10) == cfgUseAsMutex)) || (defined(SN_11) && (SlotUse(SN_11) == cfgUseAsMutex)) || (defined(SN_12) && (SlotUse(SN_12) == cfgUseAsMutex)) || (defined(SN_13) && (SlotUse(SN_13) == cfgUseAsMutex)) || (defined(SN_14) && (SlotUse(SN_14) == cfgUseAsMutex)) || (defined(SN_15) && (SlotUse(SN_15) == cfgUseAsMutex)))
  #define defUseMutexes                cfgTrue
#else
  #define defUseMutexes                cfgFalse
#endif

#if (cfgUseSynchronization != cfgSyncNon) && ((defined(SN_01) && (SlotUse(SN_01) == cfgUseAsWait)) || (defined(SN_02) && (SlotUse(SN_02) == cfgUseAsWait)) || (defined(SN_03) && (SlotUse(SN_03) == cfgUseAsWait)) || (defined(SN_04) && (SlotUse(SN_04) == cfgUseAsWait)) || (defined(SN_05) && (SlotUse(SN_05) == cfgUseAsWait)) || (defined(SN_06) && (SlotUse(SN_06) == cfgUseAsWait)) || (defined(SN_07) && (SlotUse(SN_07) == cfgUseAsWait)) || (defined(SN_08) && (SlotUse(SN_08) == cfgUseAsWait)) || (defined(SN_09) && (SlotUse(SN_09) == cfgUseAsWait)) || (defined(SN_10) && (SlotUse(SN_10) == cfgUseAsWait)) || (defined(SN_11) && (SlotUse(SN_11) == cfgUseAsWait)) || (defined(SN_12) && (SlotUse(SN_12) == cfgUseAsWait)) || (defined(SN_13) && (SlotUse(SN_13) == cfgUseAsWait)) || (defined(SN_14) && (SlotUse(SN_14) == cfgUseAsWait)) || (defined(SN_15) && (SlotUse(SN_15) == cfgUseAsWait)))
  #define defUseWaits                 cfgTrue
#else
  #define defUseWaits                 cfgFalse
#endif


/**
 * defNonSwitchingCallPresent indicates if any call is used which does not switch to OS.
 * If so, we must include some code to enter/exit system critical regions. If you add a
 * function to the Femto OS api, make sure you include it in the conditionals.
 * Usually these routines contain privEnterGlobalCritical() or privEnterSysCritical()
 * TODO: This seems not the most maintainable way to solve this
 */
#if ( (includeGenAddtoTickCount == cfgTrue) || \
      (includeGenCountEventBlocks == cfgTrue) || \
      (includeGenFireEvent  == cfgTrue) || \
      (includeGenFireEventSet == cfgTrue) || \
      (includeGenGetLastWakeTime == cfgTrue) || \
      (includeGenGetPriority == cfgTrue) || \
      (includeGenGetTaskname == cfgTrue) || \
      (includeGenGetTickCount == cfgTrue) || \
      (includeGenLogOs == cfgTrue) || \
      (includeGenLogTask == cfgTrue) || \
      (includeGenLogTask == cfgTrue) || \
      (includeGenQueuClear == cfgTrue) || \
      (includeGenQueuEmpty == cfgTrue) || \
      (includeGenQueuFull == cfgTrue) || \
      (includeGenQueuPeek == cfgTrue) || \
      (includeGenQueuRead == cfgTrue) || \
      (includeGenQueuUnread == cfgTrue) || \
      (includeGenQueuReadable == cfgTrue) || \
      (includeGenQueuWrite == cfgTrue) || \
      (includeGenQueuUnwrite == cfgTrue) || \
      (includeGenQueuWriteable == cfgTrue) || \
      (includeGenResume == cfgTrue) || \
      (includeGenSetPriority == cfgTrue) || \
      (includeGenSuspend == cfgTrue) || \
      (includeGenTrace == cfgTrue) || \
      (includeGenWaitRelease == cfgTrue) || \
      (includeTaskFeedWatchdog == cfgTrue) || \
      (includeTaskFileAppendByte == cfgTrue) || \
      (includeTaskFileFormat == cfgTrue) || \
      (includeTaskFileGetSize == cfgTrue) || \
      ((includeTaskFileReadBuffer == cfgTrue) && (cfgCheckTrace == cfgTrue)) || \
      (includeTaskFileReadByte == cfgTrue) || \
      ((includeTaskFileReadPipe == cfgTrue) && (cfgCheckTrace == cfgTrue)) || \
      (includeTaskFileSetSize == cfgTrue) || \
      ((includeTaskFileWriteBuffer == cfgTrue) && (cfgCheckTrace == cfgTrue)) || \
      (includeTaskFileWriteByte == cfgTrue) || \
      ((includeTaskFileWritePipe == cfgTrue) && (cfgCheckTrace == cfgTrue)) || \
      (includeTaskKillWatchdog == cfgTrue) || \
      (includeTaskProtectSwitchCritical == cfgTrue) || \
      (includeTaskProtectSwitchTasks == cfgTrue) )
  #define defNonSwitchingCallPresent   cfgTrue
#else
  #define defNonSwitchingCallPresent   cfgFalse
#endif



/* File with preprocessor definitions needed to detect which
 * facilities must be activated in the Femto OS core code. First
 * we collect all tasks that make use of a slot, so that those
 * tasks get the lowest tasknumbers (more efficient in looping)
 * and subsequently all other tasks get numbers.
 */

#if (cfgUseSynchronization != cfgSyncNon)

#if defined(CN_00) && (TaskInclude(CN_00) != cfgExclude) && (((Capabilities(CN_00)) & cfgCapSynchronization) == cfgCapSynchronization) && (SlotSize(CN_00) > 0)
  #define TN_00 CN_00
#endif

#if defined(CN_01) && (TaskInclude(CN_01) != cfgExclude) && (((Capabilities(CN_01)) & cfgCapSynchronization) == cfgCapSynchronization) && (SlotSize(CN_01) > 0)
  #if !defined(TN_00)
    #define TN_00 CN_01
  #else
    #define TN_01 CN_01
  #endif
#endif

#if defined(CN_02) && (TaskInclude(CN_02) != cfgExclude) && (((Capabilities(CN_02)) & cfgCapSynchronization) == cfgCapSynchronization) && (SlotSize(CN_02) > 0)
  #if !defined(TN_00)
    #define TN_00 CN_02
  #elif !defined(TN_01)
    #define TN_01 CN_02
  #else
    #define TN_02 CN_02
  #endif
#endif

#if defined(CN_03) && (TaskInclude(CN_03) != cfgExclude) && (((Capabilities(CN_03)) & cfgCapSynchronization) == cfgCapSynchronization) && (SlotSize(CN_03) > 0)
  #if !defined(TN_00)
    #define TN_00 CN_03
  #elif !defined(TN_01)
    #define TN_01 CN_03
  #elif !defined(TN_02)
    #define TN_02 CN_03
  #else
    #define TN_03 CN_03
  #endif
#endif

#if defined(CN_04) && (TaskInclude(CN_04) != cfgExclude) && (((Capabilities(CN_04)) & cfgCapSynchronization) == cfgCapSynchronization) && (SlotSize(CN_04) > 0)
  #if !defined(TN_00)
    #define TN_00 CN_04
  #elif !defined(TN_01)
    #define TN_01 CN_04
  #elif !defined(TN_02)
    #define TN_02 CN_04
  #elif !defined(TN_03)
    #define TN_03 CN_04
  #else
    #define TN_04 CN_04
  #endif
#endif


#if defined(CN_05) && (TaskInclude(CN_05) != cfgExclude) && (((Capabilities(CN_05)) & cfgCapSynchronization) == cfgCapSynchronization) && (SlotSize(CN_05) > 0)
  #if !defined(TN_00)
    #define TN_00 CN_05
  #elif !defined(TN_01)
    #define TN_01 CN_05
  #elif !defined(TN_02)
    #define TN_02 CN_05
  #elif !defined(TN_03)
    #define TN_03 CN_05
  #elif !defined(TN_04)
    #define TN_04 CN_05
  #else
    #define TN_05 CN_05
  #endif
#endif

#if defined(CN_06) && (TaskInclude(CN_06) != cfgExclude) && (((Capabilities(CN_06)) & cfgCapSynchronization) == cfgCapSynchronization) && (SlotSize(CN_06) > 0)
  #if !defined(TN_00)
    #define TN_00 CN_06
  #elif !defined(TN_01)
    #define TN_01 CN_06
  #elif !defined(TN_02)
    #define TN_02 CN_06
  #elif !defined(TN_03)
    #define TN_03 CN_06
  #elif !defined(TN_04)
    #define TN_04 CN_06
  #elif !defined(TN_05)
    #define TN_05 CN_06
  #else
    #define TN_06 CN_06
  #endif
#endif

#if defined(CN_07) && (TaskInclude(CN_07) != cfgExclude) && (((Capabilities(CN_07)) & cfgCapSynchronization) == cfgCapSynchronization) && (SlotSize(CN_07) > 0)
  #if !defined(TN_00)
    #define TN_00 CN_07
  #elif !defined(TN_01)
    #define TN_01 CN_07
  #elif !defined(TN_02)
    #define TN_02 CN_07
  #elif !defined(TN_03)
    #define TN_03 CN_07
  #elif !defined(TN_04)
    #define TN_04 CN_07
  #elif !defined(TN_05)
    #define TN_05 CN_07
  #elif !defined(TN_06)
    #define TN_06 CN_07
  #else
    #define TN_07 CN_07
  #endif
#endif

#if defined(CN_08) && (TaskInclude(CN_08) != cfgExclude) && (((Capabilities(CN_08)) & cfgCapSynchronization) == cfgCapSynchronization) && (SlotSize(CN_08) > 0)
  #if !defined(TN_00)
    #define TN_00 CN_08
  #elif !defined(TN_01)
    #define TN_01 CN_08
  #elif !defined(TN_02)
    #define TN_02 CN_08
  #elif !defined(TN_03)
    #define TN_03 CN_08
  #elif !defined(TN_04)
    #define TN_04 CN_08
  #elif !defined(TN_05)
    #define TN_05 CN_08
  #elif !defined(TN_06)
    #define TN_06 CN_08
  #elif !defined(TN_07)
    #define TN_07 CN_08
  #else
    #define TN_08 CN_08
  #endif
#endif

#if defined(CN_09) && (TaskInclude(CN_09) != cfgExclude) && (((Capabilities(CN_09)) & cfgCapSynchronization) == cfgCapSynchronization) && (SlotSize(CN_09) > 0)
  #if !defined(TN_00)
    #define TN_00 CN_09
  #elif !defined(TN_01)
    #define TN_01 CN_09
  #elif !defined(TN_02)
    #define TN_02 CN_09
  #elif !defined(TN_03)
    #define TN_03 CN_09
  #elif !defined(TN_04)
    #define TN_04 CN_09
  #elif !defined(TN_05)
    #define TN_05 CN_09
  #elif !defined(TN_06)
    #define TN_06 CN_09
  #elif !defined(TN_07)
    #define TN_07 CN_09
  #elif !defined(TN_08)
    #define TN_08 CN_09
  #else
    #define TN_09 CN_09
  #endif
#endif

#if defined(CN_10) && (TaskInclude(CN_10) != cfgExclude) && (((Capabilities(CN_10)) & cfgCapSynchronization) == cfgCapSynchronization) && (SlotSize(CN_10) > 0)
  #if !defined(TN_00)
    #define TN_00 CN_10
  #elif !defined(TN_01)
    #define TN_01 CN_10
  #elif !defined(TN_02)
    #define TN_02 CN_10
  #elif !defined(TN_03)
    #define TN_03 CN_10
  #elif !defined(TN_04)
    #define TN_04 CN_10
  #elif !defined(TN_05)
    #define TN_05 CN_10
  #elif !defined(TN_06)
    #define TN_06 CN_10
  #elif !defined(TN_07)
    #define TN_07 CN_10
  #elif !defined(TN_08)
    #define TN_08 CN_10
  #elif !defined(TN_09)
    #define TN_09 CN_10
  #else
    #define TN_10 CN_10
  #endif
#endif

#if defined(CN_11) && (TaskInclude(CN_11) != cfgExclude) && (((Capabilities(CN_11)) & cfgCapSynchronization) == cfgCapSynchronization) && (SlotSize(CN_11) > 0)
  #if !defined(TN_00)
    #define TN_00 CN_11
  #elif !defined(TN_01)
    #define TN_01 CN_11
  #elif !defined(TN_02)
    #define TN_02 CN_11
  #elif !defined(TN_03)
    #define TN_03 CN_11
  #elif !defined(TN_04)
    #define TN_04 CN_11
  #elif !defined(TN_05)
    #define TN_05 CN_11
  #elif !defined(TN_06)
    #define TN_06 CN_11
  #elif !defined(TN_07)
    #define TN_07 CN_11
  #elif !defined(TN_08)
    #define TN_08 CN_11
  #elif !defined(TN_09)
    #define TN_09 CN_11
  #elif !defined(TN_10)
    #define TN_10 CN_11
  #else
    #define TN_11 CN_11
  #endif
#endif

#if defined(CN_12) && (TaskInclude(CN_12) != cfgExclude) && (((Capabilities(CN_12)) & cfgCapSynchronization) == cfgCapSynchronization) && (SlotSize(CN_12) > 0)
  #if !defined(TN_00)
    #define TN_00 CN_12
  #elif !defined(TN_01)
    #define TN_01 CN_12
  #elif !defined(TN_02)
    #define TN_02 CN_12
  #elif !defined(TN_03)
    #define TN_03 CN_12
  #elif !defined(TN_04)
    #define TN_04 CN_12
  #elif !defined(TN_05)
    #define TN_05 CN_12
  #elif !defined(TN_06)
    #define TN_06 CN_12
  #elif !defined(TN_07)
    #define TN_07 CN_12
  #elif !defined(TN_08)
    #define TN_08 CN_12
  #elif !defined(TN_09)
    #define TN_09 CN_12
  #elif !defined(TN_10)
    #define TN_10 CN_12
  #elif !defined(TN_11)
    #define TN_11 CN_12
  #else
    #define TN_12 CN_12
  #endif
#endif

#if defined(CN_13) && (TaskInclude(CN_13) != cfgExclude) && (((Capabilities(CN_13)) & cfgCapSynchronization) == cfgCapSynchronization) && (SlotSize(CN_13) > 0)
  #if !defined(TN_00)
    #define TN_00 CN_13
  #elif !defined(TN_01)
    #define TN_01 CN_13
  #elif !defined(TN_02)
    #define TN_02 CN_13
  #elif !defined(TN_03)
    #define TN_03 CN_13
  #elif !defined(TN_04)
    #define TN_04 CN_13
  #elif !defined(TN_05)
    #define TN_05 CN_13
  #elif !defined(TN_06)
    #define TN_06 CN_13
  #elif !defined(TN_07)
    #define TN_07 CN_13
  #elif !defined(TN_08)
    #define TN_08 CN_13
  #elif !defined(TN_09)
    #define TN_09 CN_13
  #elif !defined(TN_10)
    #define TN_10 CN_13
  #elif !defined(TN_11)
    #define TN_11 CN_13
  #elif !defined(TN_12)
    #define TN_12 CN_13
  #else
    #define TN_13 CN_13
  #endif
#endif

#if defined(CN_14) && (TaskInclude(CN_14) != cfgExclude) && (((Capabilities(CN_14)) & cfgCapSynchronization) == cfgCapSynchronization) && (SlotSize(CN_14) > 0)
  #if !defined(TN_00)
    #define TN_00 CN_14
  #elif !defined(TN_01)
    #define TN_01 CN_14
  #elif !defined(TN_02)
    #define TN_02 CN_14
  #elif !defined(TN_03)
    #define TN_03 CN_14
  #elif !defined(TN_04)
    #define TN_04 CN_14
  #elif !defined(TN_05)
    #define TN_05 CN_14
  #elif !defined(TN_06)
    #define TN_06 CN_14
  #elif !defined(TN_07)
    #define TN_07 CN_14
  #elif !defined(TN_08)
    #define TN_08 CN_14
  #elif !defined(TN_09)
    #define TN_09 CN_14
  #elif !defined(TN_10)
    #define TN_10 CN_14
  #elif !defined(TN_11)
    #define TN_11 CN_14
  #elif !defined(TN_12)
    #define TN_12 CN_14
  #elif !defined(TN_13)
    #define TN_13 CN_14
  #else
    #define TN_14 CN_14
  #endif
#endif

#if defined(CN_15) && (TaskInclude(CN_15) != cfgExclude) && (((Capabilities(CN_15)) & cfgCapSynchronization) == cfgCapSynchronization) && (SlotSize(CN_15) > 0)
  #if !defined(TN_00)
    #define TN_00 CN_15
  #elif !defined(TN_01)
    #define TN_01 CN_15
  #elif !defined(TN_02)
    #define TN_02 CN_15
  #elif !defined(TN_03)
    #define TN_03 CN_15
  #elif !defined(TN_04)
    #define TN_04 CN_15
  #elif !defined(TN_05)
    #define TN_05 CN_15
  #elif !defined(TN_06)
    #define TN_06 CN_15
  #elif !defined(TN_07)
    #define TN_07 CN_15
  #elif !defined(TN_08)
    #define TN_08 CN_15
  #elif !defined(TN_09)
    #define TN_09 CN_15
  #elif !defined(TN_10)
    #define TN_10 CN_15
  #elif !defined(TN_11)
    #define TN_11 CN_15
  #elif !defined(TN_12)
    #define TN_12 CN_15
  #elif !defined(TN_13)
    #define TN_13 CN_15
  #elif !defined(TN_14)
    #define TN_14 CN_15
  #else
    #define TN_15 CN_15
  #endif
#endif

/**
 * Parameter giving the total number of (included) tasks which hold a slot.
 *
 * The parameter defNumberOfTasksWithSlot is mainly for internal use, and gives,
 * as the name indicates, the number of tasks that hold a slot. The task must not
 * be excluded, since those are not taken into account. The preprocessor
 * takes care that those tasks get the lowest tasknumbers, so that all
 * tasks with a number below  defNumberOfTasksWithSlot indeed hold a slot.
 */

#if !defined(TN_00)
  #define defNumberOfTasksWithSlot 0
#elif !defined(TN_01)
  #define defNumberOfTasksWithSlot 1
#elif !defined(TN_02)
  #define defNumberOfTasksWithSlot 2
#elif !defined(TN_03)
  #define defNumberOfTasksWithSlot 3
#elif !defined(TN_04)
  #define defNumberOfTasksWithSlot 4
#elif !defined(TN_05)
  #define defNumberOfTasksWithSlot 5
#elif !defined(TN_06)
  #define defNumberOfTasksWithSlot 6
#elif !defined(TN_07)
  #define defNumberOfTasksWithSlot 7
#elif !defined(TN_08)
  #define defNumberOfTasksWithSlot 8
#elif !defined(TN_09)
  #define defNumberOfTasksWithSlot 9
#elif !defined(TN_10)
  #define defNumberOfTasksWithSlot 10
#elif !defined(TN_11)
  #define defNumberOfTasksWithSlot 11
#elif !defined(TN_12)
  #define defNumberOfTasksWithSlot 12
#elif !defined(TN_13)
  #define defNumberOfTasksWithSlot 13
#elif !defined(TN_14)
  #define defNumberOfTasksWithSlot 14
#elif !defined(TN_15)
  #define defNumberOfTasksWithSlot 15
#else
  #define defNumberOfTasksWithSlot 16
#endif


#if defined(CN_00) && (TaskInclude(CN_00) != cfgExclude) && ((((Capabilities(CN_00)) & cfgCapSynchronization) != cfgCapSynchronization) || (SlotSize(CN_00) == 0))
  #if !defined(TN_00)
    #define TN_00 CN_00
  #elif !defined(TN_01)
    #define TN_01 CN_00
  #elif !defined(TN_02)
    #define TN_02 CN_00
  #elif !defined(TN_03)
    #define TN_03 CN_00
  #elif !defined(TN_04)
    #define TN_04 CN_00
  #elif !defined(TN_05)
    #define TN_05 CN_00
  #elif !defined(TN_06)
    #define TN_06 CN_00
  #elif !defined(TN_07)
    #define TN_07 CN_00
  #elif !defined(TN_08)
    #define TN_08 CN_00
  #elif !defined(TN_09)
    #define TN_09 CN_00
  #elif !defined(TN_10)
    #define TN_10 CN_00
  #elif !defined(TN_11)
    #define TN_11 CN_00
  #elif !defined(TN_12)
    #define TN_12 CN_00
  #elif !defined(TN_13)
    #define TN_13 CN_00
  #elif !defined(TN_14)
    #define TN_14 CN_00
  #elif !defined(TN_15)
    #define TN_15 CN_00
  #else
    #error "Task left without fields present - you should not arrive here"
  #endif
#endif

#if defined(CN_01) && (TaskInclude(CN_01) != cfgExclude) && ((((Capabilities(CN_01)) & cfgCapSynchronization) != cfgCapSynchronization) || (SlotSize(CN_01) == 0))
  #if !defined(TN_00)
    #define TN_00 CN_01
  #elif !defined(TN_01)
    #define TN_01 CN_01
  #elif !defined(TN_02)
    #define TN_02 CN_01
  #elif !defined(TN_03)
    #define TN_03 CN_01
  #elif !defined(TN_04)
    #define TN_04 CN_01
  #elif !defined(TN_05)
    #define TN_05 CN_01
  #elif !defined(TN_06)
    #define TN_06 CN_01
  #elif !defined(TN_07)
    #define TN_07 CN_01
  #elif !defined(TN_08)
    #define TN_08 CN_01
  #elif !defined(TN_09)
    #define TN_09 CN_01
  #elif !defined(TN_10)
    #define TN_10 CN_01
  #elif !defined(TN_11)
    #define TN_11 CN_01
  #elif !defined(TN_12)
    #define TN_12 CN_01
  #elif !defined(TN_13)
    #define TN_13 CN_01
  #elif !defined(TN_14)
    #define TN_14 CN_01
  #elif !defined(TN_15)
    #define TN_15 CN_01
  #else
    #error "Task left without fields present - you should not arrive here"
  #endif
#endif

#if defined(CN_02) && (TaskInclude(CN_02) != cfgExclude) && ((((Capabilities(CN_02)) & cfgCapSynchronization) != cfgCapSynchronization) || (SlotSize(CN_02) == 0))
  #if !defined(TN_00)
    #define TN_00 CN_02
  #elif !defined(TN_01)
    #define TN_01 CN_02
  #elif !defined(TN_02)
    #define TN_02 CN_02
  #elif !defined(TN_03)
    #define TN_03 CN_02
  #elif !defined(TN_04)
    #define TN_04 CN_02
  #elif !defined(TN_05)
    #define TN_05 CN_02
  #elif !defined(TN_06)
    #define TN_06 CN_02
  #elif !defined(TN_07)
    #define TN_07 CN_02
  #elif !defined(TN_08)
    #define TN_08 CN_02
  #elif !defined(TN_09)
    #define TN_09 CN_02
  #elif !defined(TN_10)
    #define TN_10 CN_02
  #elif !defined(TN_11)
    #define TN_11 CN_02
  #elif !defined(TN_12)
    #define TN_12 CN_02
  #elif !defined(TN_13)
    #define TN_13 CN_02
  #elif !defined(TN_14)
    #define TN_14 CN_02
  #elif !defined(TN_15)
    #define TN_15 CN_02
  #else
    #error "Task left without fields present - you should not arrive here"
  #endif
#endif

#if defined(CN_03) && (TaskInclude(CN_03) != cfgExclude) && ((((Capabilities(CN_03)) & cfgCapSynchronization) != cfgCapSynchronization) || (SlotSize(CN_03) == 0))
  #if !defined(TN_00)
    #define TN_00 CN_03
  #elif !defined(TN_01)
    #define TN_01 CN_03
  #elif !defined(TN_02)
    #define TN_02 CN_03
  #elif !defined(TN_03)
    #define TN_03 CN_03
  #elif !defined(TN_04)
    #define TN_04 CN_03
  #elif !defined(TN_05)
    #define TN_05 CN_03
  #elif !defined(TN_06)
    #define TN_06 CN_03
  #elif !defined(TN_07)
    #define TN_07 CN_03
  #elif !defined(TN_08)
    #define TN_08 CN_03
  #elif !defined(TN_09)
    #define TN_09 CN_03
  #elif !defined(TN_10)
    #define TN_10 CN_03
  #elif !defined(TN_11)
    #define TN_11 CN_03
  #elif !defined(TN_12)
    #define TN_12 CN_03
  #elif !defined(TN_13)
    #define TN_13 CN_03
  #elif !defined(TN_14)
    #define TN_14 CN_03
  #elif !defined(TN_15)
    #define TN_15 CN_03
  #else
    #error "Task left without fields present - you should not arrive here"
  #endif
#endif

#if defined(CN_04) && (TaskInclude(CN_04) != cfgExclude) && ((((Capabilities(CN_04)) & cfgCapSynchronization) != cfgCapSynchronization) || (SlotSize(CN_04) == 0))
  #if !defined(TN_00)
    #define TN_00 CN_04
  #elif !defined(TN_01)
    #define TN_01 CN_04
  #elif !defined(TN_02)
    #define TN_02 CN_04
  #elif !defined(TN_03)
    #define TN_03 CN_04
  #elif !defined(TN_04)
    #define TN_04 CN_04
  #elif !defined(TN_05)
    #define TN_05 CN_04
  #elif !defined(TN_06)
    #define TN_06 CN_04
  #elif !defined(TN_07)
    #define TN_07 CN_04
  #elif !defined(TN_08)
    #define TN_08 CN_04
  #elif !defined(TN_09)
    #define TN_09 CN_04
  #elif !defined(TN_10)
    #define TN_10 CN_04
  #elif !defined(TN_11)
    #define TN_11 CN_04
  #elif !defined(TN_12)
    #define TN_12 CN_04
  #elif !defined(TN_13)
    #define TN_13 CN_04
  #elif !defined(TN_14)
    #define TN_14 CN_04
  #elif !defined(TN_15)
    #define TN_15 CN_04
  #else
    #error "Task left without fields present - you should not arrive here"
  #endif
#endif

#if defined(CN_05) && (TaskInclude(CN_05) != cfgExclude) && ((((Capabilities(CN_05)) & cfgCapSynchronization) != cfgCapSynchronization) || (SlotSize(CN_05) == 0))
  #if !defined(TN_00)
    #define TN_00 CN_05
  #elif !defined(TN_01)
    #define TN_01 CN_05
  #elif !defined(TN_02)
    #define TN_02 CN_05
  #elif !defined(TN_03)
    #define TN_03 CN_05
  #elif !defined(TN_04)
    #define TN_04 CN_05
  #elif !defined(TN_05)
    #define TN_05 CN_05
  #elif !defined(TN_06)
    #define TN_06 CN_05
  #elif !defined(TN_07)
    #define TN_07 CN_05
  #elif !defined(TN_08)
    #define TN_08 CN_05
  #elif !defined(TN_09)
    #define TN_09 CN_05
  #elif !defined(TN_10)
    #define TN_10 CN_05
  #elif !defined(TN_11)
    #define TN_11 CN_05
  #elif !defined(TN_12)
    #define TN_12 CN_05
  #elif !defined(TN_13)
    #define TN_13 CN_05
  #elif !defined(TN_14)
    #define TN_14 CN_05
  #elif !defined(TN_15)
    #define TN_15 CN_05
  #else
    #error "Task left without fields present - you should not arrive here"
  #endif
#endif

#if defined(CN_06) && (TaskInclude(CN_06) != cfgExclude) && ((((Capabilities(CN_06)) & cfgCapSynchronization) != cfgCapSynchronization) || (SlotSize(CN_06) == 0))
  #if !defined(TN_00)
    #define TN_00 CN_06
  #elif !defined(TN_01)
    #define TN_01 CN_06
  #elif !defined(TN_02)
    #define TN_02 CN_06
  #elif !defined(TN_03)
    #define TN_03 CN_06
  #elif !defined(TN_04)
    #define TN_04 CN_06
  #elif !defined(TN_05)
    #define TN_05 CN_06
  #elif !defined(TN_06)
    #define TN_06 CN_06
  #elif !defined(TN_07)
    #define TN_07 CN_06
  #elif !defined(TN_08)
    #define TN_08 CN_06
  #elif !defined(TN_09)
    #define TN_09 CN_06
  #elif !defined(TN_10)
    #define TN_10 CN_06
  #elif !defined(TN_11)
    #define TN_11 CN_06
  #elif !defined(TN_12)
    #define TN_12 CN_06
  #elif !defined(TN_13)
    #define TN_13 CN_06
  #elif !defined(TN_14)
    #define TN_14 CN_06
  #elif !defined(TN_15)
    #define TN_15 CN_06
  #else
    #error "Task left without fields present - you should not arrive here"
  #endif
#endif

#if defined(CN_07) && (TaskInclude(CN_07) != cfgExclude) && ((((Capabilities(CN_07)) & cfgCapSynchronization) != cfgCapSynchronization) || (SlotSize(CN_07) == 0))
  #if !defined(TN_00)
    #define TN_00 CN_07
  #elif !defined(TN_01)
    #define TN_01 CN_07
  #elif !defined(TN_02)
    #define TN_02 CN_07
  #elif !defined(TN_03)
    #define TN_03 CN_07
  #elif !defined(TN_04)
    #define TN_04 CN_07
  #elif !defined(TN_05)
    #define TN_05 CN_07
  #elif !defined(TN_06)
    #define TN_06 CN_07
  #elif !defined(TN_07)
    #define TN_07 CN_07
  #elif !defined(TN_08)
    #define TN_08 CN_07
  #elif !defined(TN_09)
    #define TN_09 CN_07
  #elif !defined(TN_10)
    #define TN_10 CN_07
  #elif !defined(TN_11)
    #define TN_11 CN_07
  #elif !defined(TN_12)
    #define TN_12 CN_07
  #elif !defined(TN_13)
    #define TN_13 CN_07
  #elif !defined(TN_14)
    #define TN_14 CN_07
  #elif !defined(TN_15)
    #define TN_15 CN_07
  #else
    #error "Task left without fields present - you should not arrive here"
  #endif
#endif

#if defined(CN_08) && (TaskInclude(CN_08) != cfgExclude) && ((((Capabilities(CN_08)) & cfgCapSynchronization) != cfgCapSynchronization) || (SlotSize(CN_08) == 0))
  #if !defined(TN_00)
    #define TN_00 CN_08
  #elif !defined(TN_01)
    #define TN_01 CN_08
  #elif !defined(TN_02)
    #define TN_02 CN_08
  #elif !defined(TN_03)
    #define TN_03 CN_08
  #elif !defined(TN_04)
    #define TN_04 CN_08
  #elif !defined(TN_05)
    #define TN_05 CN_08
  #elif !defined(TN_06)
    #define TN_06 CN_08
  #elif !defined(TN_07)
    #define TN_07 CN_08
  #elif !defined(TN_08)
    #define TN_08 CN_08
  #elif !defined(TN_09)
    #define TN_09 CN_08
  #elif !defined(TN_10)
    #define TN_10 CN_08
  #elif !defined(TN_11)
    #define TN_11 CN_08
  #elif !defined(TN_12)
    #define TN_12 CN_08
  #elif !defined(TN_13)
    #define TN_13 CN_08
  #elif !defined(TN_14)
    #define TN_14 CN_08
  #elif !defined(TN_15)
    #define TN_15 CN_08
  #else
    #error "Task left without fields present - you should not arrive here"
  #endif
#endif

#if defined(CN_09) && (TaskInclude(CN_09) != cfgExclude) && ((((Capabilities(CN_09)) & cfgCapSynchronization) != cfgCapSynchronization) || (SlotSize(CN_09) == 0))
  #if !defined(TN_00)
    #define TN_00 CN_09
  #elif !defined(TN_01)
    #define TN_01 CN_09
  #elif !defined(TN_02)
    #define TN_02 CN_09
  #elif !defined(TN_03)
    #define TN_03 CN_09
  #elif !defined(TN_04)
    #define TN_04 CN_09
  #elif !defined(TN_05)
    #define TN_05 CN_09
  #elif !defined(TN_06)
    #define TN_06 CN_09
  #elif !defined(TN_07)
    #define TN_07 CN_09
  #elif !defined(TN_08)
    #define TN_08 CN_09
  #elif !defined(TN_09)
    #define TN_09 CN_09
  #elif !defined(TN_10)
    #define TN_10 CN_09
  #elif !defined(TN_11)
    #define TN_11 CN_09
  #elif !defined(TN_12)
    #define TN_12 CN_09
  #elif !defined(TN_13)
    #define TN_13 CN_09
  #elif !defined(TN_14)
    #define TN_14 CN_09
  #elif !defined(TN_15)
    #define TN_15 CN_09
  #else
    #error "Task left without fields present - you should not arrive here"
  #endif
#endif

#if defined(CN_10) && (TaskInclude(CN_10) != cfgExclude) && ((((Capabilities(CN_10)) & cfgCapSynchronization) != cfgCapSynchronization) || (SlotSize(CN_10) == 0))
  #if !defined(TN_00)
    #define TN_00 CN_10
  #elif !defined(TN_01)
    #define TN_01 CN_10
  #elif !defined(TN_02)
    #define TN_02 CN_10
  #elif !defined(TN_03)
    #define TN_03 CN_10
  #elif !defined(TN_04)
    #define TN_04 CN_10
  #elif !defined(TN_05)
    #define TN_05 CN_10
  #elif !defined(TN_06)
    #define TN_06 CN_10
  #elif !defined(TN_07)
    #define TN_07 CN_10
  #elif !defined(TN_08)
    #define TN_08 CN_10
  #elif !defined(TN_09)
    #define TN_09 CN_10
  #elif !defined(TN_10)
    #define TN_10 CN_10
  #elif !defined(TN_11)
    #define TN_11 CN_10
  #elif !defined(TN_12)
    #define TN_12 CN_10
  #elif !defined(TN_13)
    #define TN_13 CN_10
  #elif !defined(TN_14)
    #define TN_14 CN_10
  #elif !defined(TN_15)
    #define TN_15 CN_10
  #else
    #error "Task left without fields present - you should not arrive here"
  #endif
#endif

#if defined(CN_11) && (TaskInclude(CN_11) != cfgExclude) && ((((Capabilities(CN_11)) & cfgCapSynchronization) != cfgCapSynchronization) || (SlotSize(CN_11) == 0))
  #if !defined(TN_00)
    #define TN_00 CN_11
  #elif !defined(TN_01)
    #define TN_01 CN_11
  #elif !defined(TN_02)
    #define TN_02 CN_11
  #elif !defined(TN_03)
    #define TN_03 CN_11
  #elif !defined(TN_04)
    #define TN_04 CN_11
  #elif !defined(TN_05)
    #define TN_05 CN_11
  #elif !defined(TN_06)
    #define TN_06 CN_11
  #elif !defined(TN_07)
    #define TN_07 CN_11
  #elif !defined(TN_08)
    #define TN_08 CN_11
  #elif !defined(TN_09)
    #define TN_09 CN_11
  #elif !defined(TN_10)
    #define TN_10 CN_11
  #elif !defined(TN_11)
    #define TN_11 CN_11
  #elif !defined(TN_12)
    #define TN_12 CN_11
  #elif !defined(TN_13)
    #define TN_13 CN_11
  #elif !defined(TN_14)
    #define TN_14 CN_11
  #elif !defined(TN_15)
    #define TN_15 CN_11
  #else
    #error "Task left without fields present - you should not arrive here"
  #endif
#endif

#if defined(CN_12) && (TaskInclude(CN_12) != cfgExclude) && ((((Capabilities(CN_12)) & cfgCapSynchronization) != cfgCapSynchronization) || (SlotSize(CN_12) == 0))
  #if !defined(TN_00)
    #define TN_00 CN_12
  #elif !defined(TN_01)
    #define TN_01 CN_12
  #elif !defined(TN_02)
    #define TN_02 CN_12
  #elif !defined(TN_03)
    #define TN_03 CN_12
  #elif !defined(TN_04)
    #define TN_04 CN_12
  #elif !defined(TN_05)
    #define TN_05 CN_12
  #elif !defined(TN_06)
    #define TN_06 CN_12
  #elif !defined(TN_07)
    #define TN_07 CN_12
  #elif !defined(TN_08)
    #define TN_08 CN_12
  #elif !defined(TN_09)
    #define TN_09 CN_12
  #elif !defined(TN_10)
    #define TN_10 CN_12
  #elif !defined(TN_11)
    #define TN_11 CN_12
  #elif !defined(TN_12)
    #define TN_12 CN_12
  #elif !defined(TN_13)
    #define TN_13 CN_12
  #elif !defined(TN_14)
    #define TN_14 CN_12
  #elif !defined(TN_15)
    #define TN_15 CN_12
  #else
    #error "Task left without fields present - you should not arrive here"
  #endif
#endif

#if defined(CN_13) && (TaskInclude(CN_13) != cfgExclude) && ((((Capabilities(CN_13)) & cfgCapSynchronization) != cfgCapSynchronization) || (SlotSize(CN_13) == 0))
  #if !defined(TN_00)
    #define TN_00 CN_13
  #elif !defined(TN_01)
    #define TN_01 CN_13
  #elif !defined(TN_02)
    #define TN_02 CN_13
  #elif !defined(TN_03)
    #define TN_03 CN_13
  #elif !defined(TN_04)
    #define TN_04 CN_13
  #elif !defined(TN_05)
    #define TN_05 CN_13
  #elif !defined(TN_06)
    #define TN_06 CN_13
  #elif !defined(TN_07)
    #define TN_07 CN_13
  #elif !defined(TN_08)
    #define TN_08 CN_13
  #elif !defined(TN_09)
    #define TN_09 CN_13
  #elif !defined(TN_10)
    #define TN_10 CN_13
  #elif !defined(TN_11)
    #define TN_11 CN_13
  #elif !defined(TN_12)
    #define TN_12 CN_13
  #elif !defined(TN_13)
    #define TN_13 CN_13
  #elif !defined(TN_14)
    #define TN_14 CN_13
  #elif !defined(TN_15)
    #define TN_15 CN_13
  #else
    #error "Task left without fields present - you should not arrive here"
  #endif
#endif

#if defined(CN_14) && (TaskInclude(CN_14) != cfgExclude) && ((((Capabilities(CN_14)) & cfgCapSynchronization) != cfgCapSynchronization) || (SlotSize(CN_14) == 0))
  #if !defined(TN_00)
    #define TN_00 CN_14
  #elif !defined(TN_01)
    #define TN_01 CN_14
  #elif !defined(TN_02)
    #define TN_02 CN_14
  #elif !defined(TN_03)
    #define TN_03 CN_14
  #elif !defined(TN_04)
    #define TN_04 CN_14
  #elif !defined(TN_05)
    #define TN_05 CN_14
  #elif !defined(TN_06)
    #define TN_06 CN_14
  #elif !defined(TN_07)
    #define TN_07 CN_14
  #elif !defined(TN_08)
    #define TN_08 CN_14
  #elif !defined(TN_09)
    #define TN_09 CN_14
  #elif !defined(TN_10)
    #define TN_10 CN_14
  #elif !defined(TN_11)
    #define TN_11 CN_14
  #elif !defined(TN_12)
    #define TN_12 CN_14
  #elif !defined(TN_13)
    #define TN_13 CN_14
  #elif !defined(TN_14)
    #define TN_14 CN_14
  #elif !defined(TN_15)
    #define TN_15 CN_14
  #else
    #error "Task left without fields present - you should not arrive here"
  #endif
#endif

#if defined(CN_15) && (TaskInclude(CN_15) != cfgExclude) && ((((Capabilities(CN_15)) & cfgCapSynchronization) != cfgCapSynchronization) || (SlotSize(CN_15) == 0))
  #if !defined(TN_00)
    #define TN_00 CN_15
  #elif !defined(TN_01)
    #define TN_01 CN_15
  #elif !defined(TN_02)
    #define TN_02 CN_15
  #elif !defined(TN_03)
    #define TN_03 CN_15
  #elif !defined(TN_04)
    #define TN_04 CN_15
  #elif !defined(TN_05)
    #define TN_05 CN_15
  #elif !defined(TN_06)
    #define TN_06 CN_15
  #elif !defined(TN_07)
    #define TN_07 CN_15
  #elif !defined(TN_08)
    #define TN_08 CN_15
  #elif !defined(TN_09)
    #define TN_09 CN_15
  #elif !defined(TN_10)
    #define TN_10 CN_15
  #elif !defined(TN_11)
    #define TN_11 CN_15
  #elif !defined(TN_12)
    #define TN_12 CN_15
  #elif !defined(TN_13)
    #define TN_13 CN_15
  #elif !defined(TN_14)
    #define TN_14 CN_15
  #elif !defined(TN_15)
    #define TN_15 CN_15
  #else
    #error "Task left without fields present - you should not arrive here"
  #endif
#endif


#else

#define defNumberOfTasksWithSlot 0

#if defined(CN_00) && (TaskInclude(CN_00) != cfgExclude)
  #define TN_00 CN_00
#endif

#if defined(CN_01) && (TaskInclude(CN_01) != cfgExclude)
  #if !defined(TN_00)
    #define TN_00 CN_01
  #else
    #define TN_01 CN_01
  #endif
#endif

#if defined(CN_02) && (TaskInclude(CN_02) != cfgExclude)
  #if !defined(TN_00)
    #define TN_00 CN_02
  #elif !defined(TN_01)
    #define TN_01 CN_02
  #else
    #define TN_02 CN_02
  #endif
#endif

#if defined(CN_03) && (TaskInclude(CN_03) != cfgExclude)
  #if !defined(TN_00)
    #define TN_00 CN_03
  #elif !defined(TN_01)
    #define TN_01 CN_03
  #elif !defined(TN_02)
    #define TN_02 CN_03
  #else
    #define TN_03 CN_03
  #endif
#endif

#if defined(CN_04) && (TaskInclude(CN_04) != cfgExclude)
  #if !defined(TN_00)
    #define TN_00 CN_04
  #elif !defined(TN_01)
    #define TN_01 CN_04
  #elif !defined(TN_02)
    #define TN_02 CN_04
  #elif !defined(TN_03)
    #define TN_03 CN_04
  #else
    #define TN_04 CN_04
  #endif
#endif


#if defined(CN_05) && (TaskInclude(CN_05) != cfgExclude)
  #if !defined(TN_00)
    #define TN_00 CN_05
  #elif !defined(TN_01)
    #define TN_01 CN_05
  #elif !defined(TN_02)
    #define TN_02 CN_05
  #elif !defined(TN_03)
    #define TN_03 CN_05
  #elif !defined(TN_04)
    #define TN_04 CN_05
  #else
    #define TN_05 CN_05
  #endif
#endif

#if defined(CN_06) && (TaskInclude(CN_06) != cfgExclude)
  #if !defined(TN_00)
    #define TN_00 CN_06
  #elif !defined(TN_01)
    #define TN_01 CN_06
  #elif !defined(TN_02)
    #define TN_02 CN_06
  #elif !defined(TN_03)
    #define TN_03 CN_06
  #elif !defined(TN_04)
    #define TN_04 CN_06
  #elif !defined(TN_05)
    #define TN_05 CN_06
  #else
    #define TN_06 CN_06
  #endif
#endif

#if defined(CN_07) && (TaskInclude(CN_07) != cfgExclude)
  #if !defined(TN_00)
    #define TN_00 CN_07
  #elif !defined(TN_01)
    #define TN_01 CN_07
  #elif !defined(TN_02)
    #define TN_02 CN_07
  #elif !defined(TN_03)
    #define TN_03 CN_07
  #elif !defined(TN_04)
    #define TN_04 CN_07
  #elif !defined(TN_05)
    #define TN_05 CN_07
  #elif !defined(TN_06)
    #define TN_06 CN_07
  #else
    #define TN_07 CN_07
  #endif
#endif

#if defined(CN_08) && (TaskInclude(CN_08) != cfgExclude)
  #if !defined(TN_00)
    #define TN_00 CN_08
  #elif !defined(TN_01)
    #define TN_01 CN_08
  #elif !defined(TN_02)
    #define TN_02 CN_08
  #elif !defined(TN_03)
    #define TN_03 CN_08
  #elif !defined(TN_04)
    #define TN_04 CN_08
  #elif !defined(TN_05)
    #define TN_05 CN_08
  #elif !defined(TN_06)
    #define TN_06 CN_08
  #elif !defined(TN_07)
    #define TN_07 CN_08
  #else
    #define TN_08 CN_08
  #endif
#endif

#if defined(CN_09) && (TaskInclude(CN_09) != cfgExclude)
  #if !defined(TN_00)
    #define TN_00 CN_09
  #elif !defined(TN_01)
    #define TN_01 CN_09
  #elif !defined(TN_02)
    #define TN_02 CN_09
  #elif !defined(TN_03)
    #define TN_03 CN_09
  #elif !defined(TN_04)
    #define TN_04 CN_09
  #elif !defined(TN_05)
    #define TN_05 CN_09
  #elif !defined(TN_06)
    #define TN_06 CN_09
  #elif !defined(TN_07)
    #define TN_07 CN_09
  #elif !defined(TN_08)
    #define TN_08 CN_09
  #else
    #define TN_09 CN_09
  #endif
#endif

#if defined(CN_10) && (TaskInclude(CN_10) != cfgExclude)
  #if !defined(TN_00)
    #define TN_00 CN_10
  #elif !defined(TN_01)
    #define TN_01 CN_10
  #elif !defined(TN_02)
    #define TN_02 CN_10
  #elif !defined(TN_03)
    #define TN_03 CN_10
  #elif !defined(TN_04)
    #define TN_04 CN_10
  #elif !defined(TN_05)
    #define TN_05 CN_10
  #elif !defined(TN_06)
    #define TN_06 CN_10
  #elif !defined(TN_07)
    #define TN_07 CN_10
  #elif !defined(TN_08)
    #define TN_08 CN_10
  #elif !defined(TN_09)
    #define TN_09 CN_10
  #else
    #define TN_10 CN_10
  #endif
#endif

#if defined(CN_11) && (TaskInclude(CN_11) != cfgExclude)
  #if !defined(TN_00)
    #define TN_00 CN_11
  #elif !defined(TN_01)
    #define TN_01 CN_11
  #elif !defined(TN_02)
    #define TN_02 CN_11
  #elif !defined(TN_03)
    #define TN_03 CN_11
  #elif !defined(TN_04)
    #define TN_04 CN_11
  #elif !defined(TN_05)
    #define TN_05 CN_11
  #elif !defined(TN_06)
    #define TN_06 CN_11
  #elif !defined(TN_07)
    #define TN_07 CN_11
  #elif !defined(TN_08)
    #define TN_08 CN_11
  #elif !defined(TN_09)
    #define TN_09 CN_11
  #elif !defined(TN_10)
    #define TN_10 CN_11
  #else
    #define TN_11 CN_11
  #endif
#endif

#if defined(CN_12) && (TaskInclude(CN_12) != cfgExclude)
  #if !defined(TN_00)
    #define TN_00 CN_12
  #elif !defined(TN_01)
    #define TN_01 CN_12
  #elif !defined(TN_02)
    #define TN_02 CN_12
  #elif !defined(TN_03)
    #define TN_03 CN_12
  #elif !defined(TN_04)
    #define TN_04 CN_12
  #elif !defined(TN_05)
    #define TN_05 CN_12
  #elif !defined(TN_06)
    #define TN_06 CN_12
  #elif !defined(TN_07)
    #define TN_07 CN_12
  #elif !defined(TN_08)
    #define TN_08 CN_12
  #elif !defined(TN_09)
    #define TN_09 CN_12
  #elif !defined(TN_10)
    #define TN_10 CN_12
  #elif !defined(TN_11)
    #define TN_11 CN_12
  #else
    #define TN_12 CN_12
  #endif
#endif

#if defined(CN_13) && (TaskInclude(CN_13) != cfgExclude)
  #if !defined(TN_00)
    #define TN_00 CN_13
  #elif !defined(TN_01)
    #define TN_01 CN_13
  #elif !defined(TN_02)
    #define TN_02 CN_13
  #elif !defined(TN_03)
    #define TN_03 CN_13
  #elif !defined(TN_04)
    #define TN_04 CN_13
  #elif !defined(TN_05)
    #define TN_05 CN_13
  #elif !defined(TN_06)
    #define TN_06 CN_13
  #elif !defined(TN_07)
    #define TN_07 CN_13
  #elif !defined(TN_08)
    #define TN_08 CN_13
  #elif !defined(TN_09)
    #define TN_09 CN_13
  #elif !defined(TN_10)
    #define TN_10 CN_13
  #elif !defined(TN_11)
    #define TN_11 CN_13
  #elif !defined(TN_12)
    #define TN_12 CN_13
  #else
    #define TN_13 CN_13
  #endif
#endif

#if defined(CN_14) && (TaskInclude(CN_14) != cfgExclude)
  #if !defined(TN_00)
    #define TN_00 CN_14
  #elif !defined(TN_01)
    #define TN_01 CN_14
  #elif !defined(TN_02)
    #define TN_02 CN_14
  #elif !defined(TN_03)
    #define TN_03 CN_14
  #elif !defined(TN_04)
    #define TN_04 CN_14
  #elif !defined(TN_05)
    #define TN_05 CN_14
  #elif !defined(TN_06)
    #define TN_06 CN_14
  #elif !defined(TN_07)
    #define TN_07 CN_14
  #elif !defined(TN_08)
    #define TN_08 CN_14
  #elif !defined(TN_09)
    #define TN_09 CN_14
  #elif !defined(TN_10)
    #define TN_10 CN_14
  #elif !defined(TN_11)
    #define TN_11 CN_14
  #elif !defined(TN_12)
    #define TN_12 CN_14
  #elif !defined(TN_13)
    #define TN_13 CN_14
  #else
    #define TN_14 CN_14
  #endif
#endif

#if defined(CN_15) && (TaskInclude(CN_15) != cfgExclude)
  #if !defined(TN_00)
    #define TN_00 CN_15
  #elif !defined(TN_01)
    #define TN_01 CN_15
  #elif !defined(TN_02)
    #define TN_02 CN_15
  #elif !defined(TN_03)
    #define TN_03 CN_15
  #elif !defined(TN_04)
    #define TN_04 CN_15
  #elif !defined(TN_05)
    #define TN_05 CN_15
  #elif !defined(TN_06)
    #define TN_06 CN_15
  #elif !defined(TN_07)
    #define TN_07 CN_15
  #elif !defined(TN_08)
    #define TN_08 CN_15
  #elif !defined(TN_09)
    #define TN_09 CN_15
  #elif !defined(TN_10)
    #define TN_10 CN_15
  #elif !defined(TN_11)
    #define TN_11 CN_15
  #elif !defined(TN_12)
    #define TN_12 CN_15
  #elif !defined(TN_13)
    #define TN_13 CN_15
  #elif !defined(TN_14)
    #define TN_14 CN_15
  #else
    #define TN_15 CN_15
  #endif
#endif

#endif

#if !defined(TN_00)
  #define defNumberOfTasks 0
#elif !defined(TN_01)
  #define defNumberOfTasks 1
#elif !defined(TN_02)
  #define defNumberOfTasks 2
#elif !defined(TN_03)
  #define defNumberOfTasks 3
#elif !defined(TN_04)
  #define defNumberOfTasks 4
#elif !defined(TN_05)
  #define defNumberOfTasks 5
#elif !defined(TN_06)
  #define defNumberOfTasks 6
#elif !defined(TN_07)
  #define defNumberOfTasks 7
#elif !defined(TN_08)
  #define defNumberOfTasks 8
#elif !defined(TN_09)
  #define defNumberOfTasks 9
#elif !defined(TN_10)
  #define defNumberOfTasks 10
#elif !defined(TN_11)
  #define defNumberOfTasks 11
#elif !defined(TN_12)
  #define defNumberOfTasks 12
#elif !defined(TN_13)
  #define defNumberOfTasks 13
#elif !defined(TN_14)
  #define defNumberOfTasks 14
#elif !defined(TN_15)
  #define defNumberOfTasks 15
#else
  #define defNumberOfTasks 16
#endif


/* We first define all queus on the slots so that they occupy the
 * lowest slot numbers. Namely those in the range:
 * 0 .. defNumberOfQueus-1  */

#if defined(SN_01) && (SlotUse(SN_01) == cfgUseAsQueu)
  #define LN_01 SN_01
#endif

#if defined(SN_02) && (SlotUse(SN_02) == cfgUseAsQueu)
  #if !defined(LN_01)
    #define LN_01 SN_02
  #else
    #define LN_02 SN_02
  #endif
#endif

#if defined(SN_03) && (SlotUse(SN_03) == cfgUseAsQueu)
  #if !defined(LN_01)
    #define LN_01 SN_03
  #elif !defined(LN_02)
    #define LN_02 SN_03
  #else
    #define LN_03 SN_03
  #endif
#endif

#if defined(SN_04) && (SlotUse(SN_04) == cfgUseAsQueu)
  #if !defined(LN_01)
    #define LN_01 SN_04
  #elif !defined(LN_02)
    #define LN_02 SN_04
  #elif !defined(LN_03)
    #define LN_03 SN_04
  #else
    #define LN_04 SN_04
  #endif
#endif

#if defined(SN_05) && (SlotUse(SN_05) == cfgUseAsQueu)
  #if !defined(LN_01)
    #define LN_01 SN_05
  #elif !defined(LN_02)
    #define LN_02 SN_05
  #elif !defined(LN_03)
    #define LN_03 SN_05
  #elif !defined(LN_04)
    #define LN_04 SN_05
  #else
    #define LN_05 SN_05
  #endif
#endif


#if defined(SN_06) && (SlotUse(SN_06) == cfgUseAsQueu)
  #if !defined(LN_01)
    #define LN_01 SN_06
  #elif !defined(LN_02)
    #define LN_02 SN_06
  #elif !defined(LN_03)
    #define LN_03 SN_06
  #elif !defined(LN_04)
    #define LN_04 SN_06
  #elif !defined(LN_05)
    #define LN_05 SN_06
  #else
    #define LN_06 SN_06
  #endif
#endif

#if defined(SN_07) && (SlotUse(SN_07) == cfgUseAsQueu)
  #if !defined(LN_01)
    #define LN_01 SN_07
  #elif !defined(LN_02)
    #define LN_02 SN_07
  #elif !defined(LN_03)
    #define LN_03 SN_07
  #elif !defined(LN_04)
    #define LN_04 SN_07
  #elif !defined(LN_05)
    #define LN_05 SN_07
  #elif !defined(LN_06)
    #define LN_06 SN_07
  #else
    #define LN_07 SN_07
  #endif
#endif

#if defined(SN_08) && (SlotUse(SN_08) == cfgUseAsQueu)
  #if !defined(LN_01)
    #define LN_01 SN_08
  #elif !defined(LN_02)
    #define LN_02 SN_08
  #elif !defined(LN_03)
    #define LN_03 SN_08
  #elif !defined(LN_04)
    #define LN_04 SN_08
  #elif !defined(LN_05)
    #define LN_05 SN_08
  #elif !defined(LN_06)
    #define LN_06 SN_08
  #elif !defined(LN_07)
    #define LN_07 SN_08
  #else
    #define LN_08 SN_08
  #endif
#endif

#if defined(SN_09) && (SlotUse(SN_09) == cfgUseAsQueu)
  #if !defined(LN_01)
    #define LN_01 SN_09
  #elif !defined(LN_02)
    #define LN_02 SN_09
  #elif !defined(LN_03)
    #define LN_03 SN_09
  #elif !defined(LN_04)
    #define LN_04 SN_09
  #elif !defined(LN_05)
    #define LN_05 SN_09
  #elif !defined(LN_06)
    #define LN_06 SN_09
  #elif !defined(LN_07)
    #define LN_07 SN_09
  #elif !defined(LN_08)
    #define LN_08 SN_09
  #else
    #define LN_09 SN_09
  #endif
#endif

#if defined(SN_10) && (SlotUse(SN_10) == cfgUseAsQueu)
  #if !defined(LN_01)
    #define LN_01 SN_10
  #elif !defined(LN_02)
    #define LN_02 SN_10
  #elif !defined(LN_03)
    #define LN_03 SN_10
  #elif !defined(LN_04)
    #define LN_04 SN_10
  #elif !defined(LN_05)
    #define LN_05 SN_10
  #elif !defined(LN_06)
    #define LN_06 SN_10
  #elif !defined(LN_07)
    #define LN_07 SN_10
  #elif !defined(LN_08)
    #define LN_08 SN_10
  #elif !defined(LN_09)
    #define LN_09 SN_10
  #else
    #define LN_10 SN_10
  #endif
#endif

#if defined(SN_11) && (SlotUse(SN_11) == cfgUseAsQueu)
  #if !defined(LN_01)
    #define LN_01 SN_11
  #elif !defined(LN_02)
    #define LN_02 SN_11
  #elif !defined(LN_03)
    #define LN_03 SN_11
  #elif !defined(LN_04)
    #define LN_04 SN_11
  #elif !defined(LN_05)
    #define LN_05 SN_11
  #elif !defined(LN_06)
    #define LN_06 SN_11
  #elif !defined(LN_07)
    #define LN_07 SN_11
  #elif !defined(LN_08)
    #define LN_08 SN_11
  #elif !defined(LN_09)
    #define LN_09 SN_11
  #elif !defined(LN_10)
    #define LN_10 SN_11
  #else
    #define LN_11 SN_11
  #endif
#endif

#if defined(SN_12) && (SlotUse(SN_12) == cfgUseAsQueu)
  #if !defined(LN_01)
    #define LN_01 SN_12
  #elif !defined(LN_02)
    #define LN_02 SN_12
  #elif !defined(LN_03)
    #define LN_03 SN_12
  #elif !defined(LN_04)
    #define LN_04 SN_12
  #elif !defined(LN_05)
    #define LN_05 SN_12
  #elif !defined(LN_06)
    #define LN_06 SN_12
  #elif !defined(LN_07)
    #define LN_07 SN_12
  #elif !defined(LN_08)
    #define LN_08 SN_12
  #elif !defined(LN_09)
    #define LN_09 SN_12
  #elif !defined(LN_10)
    #define LN_10 SN_12
  #elif !defined(LN_11)
    #define LN_11 SN_12
  #else
    #define LN_12 SN_12
  #endif
#endif

#if defined(SN_13) && (SlotUse(SN_13) == cfgUseAsQueu)
  #if !defined(LN_01)
    #define LN_01 SN_13
  #elif !defined(LN_02)
    #define LN_02 SN_13
  #elif !defined(LN_03)
    #define LN_03 SN_13
  #elif !defined(LN_04)
    #define LN_04 SN_13
  #elif !defined(LN_05)
    #define LN_05 SN_13
  #elif !defined(LN_06)
    #define LN_06 SN_13
  #elif !defined(LN_07)
    #define LN_07 SN_13
  #elif !defined(LN_08)
    #define LN_08 SN_13
  #elif !defined(LN_09)
    #define LN_09 SN_13
  #elif !defined(LN_10)
    #define LN_10 SN_13
  #elif !defined(LN_11)
    #define LN_11 SN_13
  #elif !defined(LN_12)
    #define LN_12 SN_13
  #else
    #define LN_13 SN_13
  #endif
#endif

#if defined(SN_14) && (SlotUse(SN_14) == cfgUseAsQueu)
  #if !defined(LN_01)
    #define LN_01 SN_14
  #elif !defined(LN_02)
    #define LN_02 SN_14
  #elif !defined(LN_03)
    #define LN_03 SN_14
  #elif !defined(LN_04)
    #define LN_04 SN_14
  #elif !defined(LN_05)
    #define LN_05 SN_14
  #elif !defined(LN_06)
    #define LN_06 SN_14
  #elif !defined(LN_07)
    #define LN_07 SN_14
  #elif !defined(LN_08)
    #define LN_08 SN_14
  #elif !defined(LN_09)
    #define LN_09 SN_14
  #elif !defined(LN_10)
    #define LN_10 SN_14
  #elif !defined(LN_11)
    #define LN_11 SN_14
  #elif !defined(LN_12)
    #define LN_12 SN_14
  #elif !defined(LN_13)
    #define LN_13 SN_14
  #else
    #define LN_14 SN_14
  #endif
#endif

#if defined(SN_15) && (SlotUse(SN_15) == cfgUseAsQueu)
  #if !defined(LN_01)
    #define LN_01 SN_15
  #elif !defined(LN_02)
    #define LN_02 SN_15
  #elif !defined(LN_03)
    #define LN_03 SN_15
  #elif !defined(LN_04)
    #define LN_04 SN_15
  #elif !defined(LN_05)
    #define LN_05 SN_15
  #elif !defined(LN_06)
    #define LN_06 SN_15
  #elif !defined(LN_07)
    #define LN_07 SN_15
  #elif !defined(LN_08)
    #define LN_08 SN_15
  #elif !defined(LN_09)
    #define LN_09 SN_15
  #elif !defined(LN_10)
    #define LN_10 SN_15
  #elif !defined(LN_11)
    #define LN_11 SN_15
  #elif !defined(LN_12)
    #define LN_12 SN_15
  #elif !defined(LN_13)
    #define LN_13 SN_15
  #elif !defined(LN_14)
    #define LN_14 SN_15
  #else
    #define LN_15 SN_15
  #endif
#endif


#if !defined(LN_01)
  #define defNumberQueuEnd 1
#elif !defined(LN_02)
  #define defNumberQueuEnd 2
#elif !defined(LN_03)
  #define defNumberQueuEnd 3
#elif !defined(LN_04)
  #define defNumberQueuEnd 4
#elif !defined(LN_05)
  #define defNumberQueuEnd 5
#elif !defined(LN_06)
  #define defNumberQueuEnd 6
#elif !defined(LN_07)
  #define defNumberQueuEnd 7
#elif !defined(LN_08)
  #define defNumberQueuEnd 8
#elif !defined(LN_09)
  #define defNumberQueuEnd 9
#elif !defined(LN_10)
  #define defNumberQueuEnd 10
#elif !defined(LN_11)
  #define defNumberQueuEnd 11
#elif !defined(LN_12)
  #define defNumberQueuEnd 12
#elif !defined(LN_13)
  #define defNumberQueuEnd 13
#elif !defined(LN_14)
  #define defNumberQueuEnd 14
#elif !defined(LN_15)
  #define defNumberQueuEnd 15
#else
  #define defNumberQueuEnd 16
#endif

/* To loop through all queus use the following definitions, do not
 * include the end in the loop */
#define defNumberQueuBegin 1
#define defNumberOfQueus   (defNumberQueuEnd - defNumberQueuBegin)


/* Now we define all mutexed on the slots so that they occupy the
 * next lowest slot numbers. */

#if defined(SN_01) && (SlotUse(SN_01) == cfgUseAsMutex)
  #if !defined(LN_01)
    #define LN_01 SN_01
  #elif !defined(LN_02)
    #define LN_02 SN_01
  #elif !defined(LN_03)
    #define LN_03 SN_01
  #elif !defined(LN_04)
    #define LN_04 SN_01
  #elif !defined(LN_05)
    #define LN_05 SN_01
  #elif !defined(LN_06)
    #define LN_06 SN_01
  #elif !defined(LN_07)
    #define LN_07 SN_01
  #elif !defined(LN_08)
    #define LN_08 SN_01
  #elif !defined(LN_09)
    #define LN_09 SN_01
  #elif !defined(LN_10)
    #define LN_10 SN_01
  #elif !defined(LN_11)
    #define LN_11 SN_01
  #elif !defined(LN_12)
    #define LN_12 SN_01
  #elif !defined(LN_13)
    #define LN_13 SN_01
  #elif !defined(LN_14)
    #define LN_14 SN_01
  #elif !defined(LN_15)
    #define LN_15 SN_01
  #else
    #error "Slot left without fields present - you should not arrive here"
  #endif
#endif

#if defined(SN_02) && (SlotUse(SN_02) == cfgUseAsMutex)
  #if !defined(LN_01)
    #define LN_01 SN_02
  #elif !defined(LN_02)
    #define LN_02 SN_02
  #elif !defined(LN_03)
    #define LN_03 SN_02
  #elif !defined(LN_04)
    #define LN_04 SN_02
  #elif !defined(LN_05)
    #define LN_05 SN_02
  #elif !defined(LN_06)
    #define LN_06 SN_02
  #elif !defined(LN_07)
    #define LN_07 SN_02
  #elif !defined(LN_08)
    #define LN_08 SN_02
  #elif !defined(LN_09)
    #define LN_09 SN_02
  #elif !defined(LN_10)
    #define LN_10 SN_02
  #elif !defined(LN_11)
    #define LN_11 SN_02
  #elif !defined(LN_12)
    #define LN_12 SN_02
  #elif !defined(LN_13)
    #define LN_13 SN_02
  #elif !defined(LN_14)
    #define LN_14 SN_02
  #elif !defined(LN_15)
    #define LN_15 SN_02
  #else
    #error "Slot left without fields present - you should not arrive here"
  #endif
#endif

#if defined(SN_03) && (SlotUse(SN_03) == cfgUseAsMutex)
  #if !defined(LN_01)
    #define LN_01 SN_03
  #elif !defined(LN_02)
    #define LN_02 SN_03
  #elif !defined(LN_03)
    #define LN_03 SN_03
  #elif !defined(LN_04)
    #define LN_04 SN_03
  #elif !defined(LN_05)
    #define LN_05 SN_03
  #elif !defined(LN_06)
    #define LN_06 SN_03
  #elif !defined(LN_07)
    #define LN_07 SN_03
  #elif !defined(LN_08)
    #define LN_08 SN_03
  #elif !defined(LN_09)
    #define LN_09 SN_03
  #elif !defined(LN_10)
    #define LN_10 SN_03
  #elif !defined(LN_11)
    #define LN_11 SN_03
  #elif !defined(LN_12)
    #define LN_12 SN_03
  #elif !defined(LN_13)
    #define LN_13 SN_03
  #elif !defined(LN_14)
    #define LN_14 SN_03
  #elif !defined(LN_15)
    #define LN_15 SN_03
  #else
    #error "Slot left without fields present - you should not arrive here"
  #endif
#endif

#if defined(SN_04) && (SlotUse(SN_04) == cfgUseAsMutex)
  #if !defined(LN_01)
    #define LN_01 SN_04
  #elif !defined(LN_02)
    #define LN_02 SN_04
  #elif !defined(LN_03)
    #define LN_03 SN_04
  #elif !defined(LN_04)
    #define LN_04 SN_04
  #elif !defined(LN_05)
    #define LN_05 SN_04
  #elif !defined(LN_06)
    #define LN_06 SN_04
  #elif !defined(LN_07)
    #define LN_07 SN_04
  #elif !defined(LN_08)
    #define LN_08 SN_04
  #elif !defined(LN_09)
    #define LN_09 SN_04
  #elif !defined(LN_10)
    #define LN_10 SN_04
  #elif !defined(LN_11)
    #define LN_11 SN_04
  #elif !defined(LN_12)
    #define LN_12 SN_04
  #elif !defined(LN_13)
    #define LN_13 SN_04
  #elif !defined(LN_14)
    #define LN_14 SN_04
  #elif !defined(LN_15)
    #define LN_15 SN_04
  #else
    #error "Slot left without fields present - you should not arrive here"
  #endif
#endif

#if defined(SN_05) && (SlotUse(SN_05) == cfgUseAsMutex)
  #if !defined(LN_01)
    #define LN_01 SN_05
  #elif !defined(LN_02)
    #define LN_02 SN_05
  #elif !defined(LN_03)
    #define LN_03 SN_05
  #elif !defined(LN_04)
    #define LN_04 SN_05
  #elif !defined(LN_05)
    #define LN_05 SN_05
  #elif !defined(LN_06)
    #define LN_06 SN_05
  #elif !defined(LN_07)
    #define LN_07 SN_05
  #elif !defined(LN_08)
    #define LN_08 SN_05
  #elif !defined(LN_09)
    #define LN_09 SN_05
  #elif !defined(LN_10)
    #define LN_10 SN_05
  #elif !defined(LN_11)
    #define LN_11 SN_05
  #elif !defined(LN_12)
    #define LN_12 SN_05
  #elif !defined(LN_13)
    #define LN_13 SN_05
  #elif !defined(LN_14)
    #define LN_14 SN_05
  #elif !defined(LN_15)
    #define LN_15 SN_05
  #else
    #error "Slot left without fields present - you should not arrive here"
  #endif
#endif

#if defined(SN_06) && (SlotUse(SN_06) == cfgUseAsMutex)
  #if !defined(LN_01)
    #define LN_01 SN_06
  #elif !defined(LN_02)
    #define LN_02 SN_06
  #elif !defined(LN_03)
    #define LN_03 SN_06
  #elif !defined(LN_04)
    #define LN_04 SN_06
  #elif !defined(LN_05)
    #define LN_05 SN_06
  #elif !defined(LN_06)
    #define LN_06 SN_06
  #elif !defined(LN_07)
    #define LN_07 SN_06
  #elif !defined(LN_08)
    #define LN_08 SN_06
  #elif !defined(LN_09)
    #define LN_09 SN_06
  #elif !defined(LN_10)
    #define LN_10 SN_06
  #elif !defined(LN_11)
    #define LN_11 SN_06
  #elif !defined(LN_12)
    #define LN_12 SN_06
  #elif !defined(LN_13)
    #define LN_13 SN_06
  #elif !defined(LN_14)
    #define LN_14 SN_06
  #elif !defined(LN_15)
    #define LN_15 SN_06
  #else
    #error "Slot left without fields present - you should not arrive here"
  #endif
#endif

#if defined(SN_07) && (SlotUse(SN_07) == cfgUseAsMutex)
  #if !defined(LN_01)
    #define LN_01 SN_07
  #elif !defined(LN_02)
    #define LN_02 SN_07
  #elif !defined(LN_03)
    #define LN_03 SN_07
  #elif !defined(LN_04)
    #define LN_04 SN_07
  #elif !defined(LN_05)
    #define LN_05 SN_07
  #elif !defined(LN_06)
    #define LN_06 SN_07
  #elif !defined(LN_07)
    #define LN_07 SN_07
  #elif !defined(LN_08)
    #define LN_08 SN_07
  #elif !defined(LN_09)
    #define LN_09 SN_07
  #elif !defined(LN_10)
    #define LN_10 SN_07
  #elif !defined(LN_11)
    #define LN_11 SN_07
  #elif !defined(LN_12)
    #define LN_12 SN_07
  #elif !defined(LN_13)
    #define LN_13 SN_07
  #elif !defined(LN_14)
    #define LN_14 SN_07
  #elif !defined(LN_15)
    #define LN_15 SN_07
  #else
    #error "Slot left without fields present - you should not arrive here"
  #endif
#endif

#if defined(SN_08) && (SlotUse(SN_08) == cfgUseAsMutex)
  #if !defined(LN_01)
    #define LN_01 SN_08
  #elif !defined(LN_02)
    #define LN_02 SN_08
  #elif !defined(LN_03)
    #define LN_03 SN_08
  #elif !defined(LN_04)
    #define LN_04 SN_08
  #elif !defined(LN_05)
    #define LN_05 SN_08
  #elif !defined(LN_06)
    #define LN_06 SN_08
  #elif !defined(LN_07)
    #define LN_07 SN_08
  #elif !defined(LN_08)
    #define LN_08 SN_08
  #elif !defined(LN_09)
    #define LN_09 SN_08
  #elif !defined(LN_10)
    #define LN_10 SN_08
  #elif !defined(LN_11)
    #define LN_11 SN_08
  #elif !defined(LN_12)
    #define LN_12 SN_08
  #elif !defined(LN_13)
    #define LN_13 SN_08
  #elif !defined(LN_14)
    #define LN_14 SN_08
  #elif !defined(LN_15)
    #define LN_15 SN_08
  #else
    #error "Slot left without fields present - you should not arrive here"
  #endif
#endif

#if defined(SN_09) && (SlotUse(SN_09) == cfgUseAsMutex)
  #if !defined(LN_01)
    #define LN_01 SN_09
  #elif !defined(LN_02)
    #define LN_02 SN_09
  #elif !defined(LN_03)
    #define LN_03 SN_09
  #elif !defined(LN_04)
    #define LN_04 SN_09
  #elif !defined(LN_05)
    #define LN_05 SN_09
  #elif !defined(LN_06)
    #define LN_06 SN_09
  #elif !defined(LN_07)
    #define LN_07 SN_09
  #elif !defined(LN_08)
    #define LN_08 SN_09
  #elif !defined(LN_09)
    #define LN_09 SN_09
  #elif !defined(LN_10)
    #define LN_10 SN_09
  #elif !defined(LN_11)
    #define LN_11 SN_09
  #elif !defined(LN_12)
    #define LN_12 SN_09
  #elif !defined(LN_13)
    #define LN_13 SN_09
  #elif !defined(LN_14)
    #define LN_14 SN_09
  #elif !defined(LN_15)
    #define LN_15 SN_09
  #else
    #error "Slot left without fields present - you should not arrive here"
  #endif
#endif

#if defined(SN_10) && (SlotUse(SN_10) == cfgUseAsMutex)
  #if !defined(LN_01)
    #define LN_01 SN_10
  #elif !defined(LN_02)
    #define LN_02 SN_10
  #elif !defined(LN_03)
    #define LN_03 SN_10
  #elif !defined(LN_04)
    #define LN_04 SN_10
  #elif !defined(LN_05)
    #define LN_05 SN_10
  #elif !defined(LN_06)
    #define LN_06 SN_10
  #elif !defined(LN_07)
    #define LN_07 SN_10
  #elif !defined(LN_08)
    #define LN_08 SN_10
  #elif !defined(LN_09)
    #define LN_09 SN_10
  #elif !defined(LN_10)
    #define LN_10 SN_10
  #elif !defined(LN_11)
    #define LN_11 SN_10
  #elif !defined(LN_12)
    #define LN_12 SN_10
  #elif !defined(LN_13)
    #define LN_13 SN_10
  #elif !defined(LN_14)
    #define LN_14 SN_10
  #elif !defined(LN_15)
    #define LN_15 SN_10
  #else
    #error "Slot left without fields present - you should not arrive here"
  #endif
#endif

#if defined(SN_11) && (SlotUse(SN_11) == cfgUseAsMutex)
  #if !defined(LN_01)
    #define LN_01 SN_11
  #elif !defined(LN_02)
    #define LN_02 SN_11
  #elif !defined(LN_03)
    #define LN_03 SN_11
  #elif !defined(LN_04)
    #define LN_04 SN_11
  #elif !defined(LN_05)
    #define LN_05 SN_11
  #elif !defined(LN_06)
    #define LN_06 SN_11
  #elif !defined(LN_07)
    #define LN_07 SN_11
  #elif !defined(LN_08)
    #define LN_08 SN_11
  #elif !defined(LN_09)
    #define LN_09 SN_11
  #elif !defined(LN_10)
    #define LN_10 SN_11
  #elif !defined(LN_11)
    #define LN_11 SN_11
  #elif !defined(LN_12)
    #define LN_12 SN_11
  #elif !defined(LN_13)
    #define LN_13 SN_11
  #elif !defined(LN_14)
    #define LN_14 SN_11
  #elif !defined(LN_15)
    #define LN_15 SN_11
  #else
    #error "Slot left without fields present - you should not arrive here"
  #endif
#endif

#if defined(SN_12) && (SlotUse(SN_12) == cfgUseAsMutex)
  #if !defined(LN_01)
    #define LN_01 SN_12
  #elif !defined(LN_02)
    #define LN_02 SN_12
  #elif !defined(LN_03)
    #define LN_03 SN_12
  #elif !defined(LN_04)
    #define LN_04 SN_12
  #elif !defined(LN_05)
    #define LN_05 SN_12
  #elif !defined(LN_06)
    #define LN_06 SN_12
  #elif !defined(LN_07)
    #define LN_07 SN_12
  #elif !defined(LN_08)
    #define LN_08 SN_12
  #elif !defined(LN_09)
    #define LN_09 SN_12
  #elif !defined(LN_10)
    #define LN_10 SN_12
  #elif !defined(LN_11)
    #define LN_11 SN_12
  #elif !defined(LN_12)
    #define LN_12 SN_12
  #elif !defined(LN_13)
    #define LN_13 SN_12
  #elif !defined(LN_14)
    #define LN_14 SN_12
  #elif !defined(LN_15)
    #define LN_15 SN_12
  #else
    #error "Slot left without fields present - you should not arrive here"
  #endif
#endif

#if defined(SN_13) && (SlotUse(SN_13) == cfgUseAsMutex)
  #if !defined(LN_01)
    #define LN_01 SN_13
  #elif !defined(LN_02)
    #define LN_02 SN_13
  #elif !defined(LN_03)
    #define LN_03 SN_13
  #elif !defined(LN_04)
    #define LN_04 SN_13
  #elif !defined(LN_05)
    #define LN_05 SN_13
  #elif !defined(LN_06)
    #define LN_06 SN_13
  #elif !defined(LN_07)
    #define LN_07 SN_13
  #elif !defined(LN_08)
    #define LN_08 SN_13
  #elif !defined(LN_09)
    #define LN_09 SN_13
  #elif !defined(LN_10)
    #define LN_10 SN_13
  #elif !defined(LN_11)
    #define LN_11 SN_13
  #elif !defined(LN_12)
    #define LN_12 SN_13
  #elif !defined(LN_13)
    #define LN_13 SN_13
  #elif !defined(LN_14)
    #define LN_14 SN_13
  #elif !defined(LN_15)
    #define LN_15 SN_13
  #else
    #error "Slot left without fields present - you should not arrive here"
  #endif
#endif

#if defined(SN_14) && (SlotUse(SN_14) == cfgUseAsMutex)
  #if !defined(LN_01)
    #define LN_01 SN_14
  #elif !defined(LN_02)
    #define LN_02 SN_14
  #elif !defined(LN_03)
    #define LN_03 SN_14
  #elif !defined(LN_04)
    #define LN_04 SN_14
  #elif !defined(LN_05)
    #define LN_05 SN_14
  #elif !defined(LN_06)
    #define LN_06 SN_14
  #elif !defined(LN_07)
    #define LN_07 SN_14
  #elif !defined(LN_08)
    #define LN_08 SN_14
  #elif !defined(LN_09)
    #define LN_09 SN_14
  #elif !defined(LN_10)
    #define LN_10 SN_14
  #elif !defined(LN_11)
    #define LN_11 SN_14
  #elif !defined(LN_12)
    #define LN_12 SN_14
  #elif !defined(LN_13)
    #define LN_13 SN_14
  #elif !defined(LN_14)
    #define LN_14 SN_14
  #elif !defined(LN_15)
    #define LN_15 SN_14
  #else
    #error "Slot left without fields present - you should not arrive here"
  #endif
#endif

#if defined(SN_15) && (SlotUse(SN_15) == cfgUseAsMutex)
  #if !defined(LN_01)
    #define LN_01 SN_15
  #elif !defined(LN_02)
    #define LN_02 SN_15
  #elif !defined(LN_03)
    #define LN_03 SN_15
  #elif !defined(LN_04)
    #define LN_04 SN_15
  #elif !defined(LN_05)
    #define LN_05 SN_15
  #elif !defined(LN_06)
    #define LN_06 SN_15
  #elif !defined(LN_07)
    #define LN_07 SN_15
  #elif !defined(LN_08)
    #define LN_08 SN_15
  #elif !defined(LN_09)
    #define LN_09 SN_15
  #elif !defined(LN_10)
    #define LN_10 SN_15
  #elif !defined(LN_11)
    #define LN_11 SN_15
  #elif !defined(LN_12)
    #define LN_12 SN_15
  #elif !defined(LN_13)
    #define LN_13 SN_15
  #elif !defined(LN_14)
    #define LN_14 SN_15
  #elif !defined(LN_15)
    #define LN_15 SN_15
  #else
    #error "Slot left without fields present - you should not arrive here"
  #endif
#endif


#if !defined(LN_01)
  #define defNumberMutexEnd 1
#elif !defined(LN_02)
  #define defNumberMutexEnd 2
#elif !defined(LN_03)
  #define defNumberMutexEnd 3
#elif !defined(LN_04)
  #define defNumberMutexEnd 4
#elif !defined(LN_05)
  #define defNumberMutexEnd 5
#elif !defined(LN_06)
  #define defNumberMutexEnd 6
#elif !defined(LN_07)
  #define defNumberMutexEnd 7
#elif !defined(LN_08)
  #define defNumberMutexEnd 8
#elif !defined(LN_09)
  #define defNumberMutexEnd 9
#elif !defined(LN_10)
  #define defNumberMutexEnd 10
#elif !defined(LN_11)
  #define defNumberMutexEnd 11
#elif !defined(LN_12)
  #define defNumberMutexEnd 12
#elif !defined(LN_13)
  #define defNumberMutexEnd 13
#elif !defined(LN_14)
  #define defNumberMutexEnd 14
#elif !defined(LN_15)
  #define defNumberMutexEnd 15
#else
  #define defNumberMutexEnd 16
#endif

/* To loop through all queus use the following definitions, do not
 * include the end in the loop */
#define defNumberMutexBegin defNumberQueuEnd
#define defNumberOfMutexes  (defNumberMutexEnd - defNumberMutexBegin)

/* Last we define all waits on the slots so that they occupy the
 * highest slot numbers. */


#if defined(SN_01) && (SlotUse(SN_01) == cfgUseAsWait)
  #if !defined(LN_01)
    #define LN_01 SN_01
  #elif !defined(LN_02)
    #define LN_02 SN_01
  #elif !defined(LN_03)
    #define LN_03 SN_01
  #elif !defined(LN_04)
    #define LN_04 SN_01
  #elif !defined(LN_05)
    #define LN_05 SN_01
  #elif !defined(LN_06)
    #define LN_06 SN_01
  #elif !defined(LN_07)
    #define LN_07 SN_01
  #elif !defined(LN_08)
    #define LN_08 SN_01
  #elif !defined(LN_09)
    #define LN_09 SN_01
  #elif !defined(LN_10)
    #define LN_10 SN_01
  #elif !defined(LN_11)
    #define LN_11 SN_01
  #elif !defined(LN_12)
    #define LN_12 SN_01
  #elif !defined(LN_13)
    #define LN_13 SN_01
  #elif !defined(LN_14)
    #define LN_14 SN_01
  #elif !defined(LN_15)
    #define LN_15 SN_01
  #else
    #error "Slot left without fields present - you should not arrive here"
  #endif
#endif

#if defined(SN_02) && (SlotUse(SN_02) == cfgUseAsWait)
  #if !defined(LN_01)
    #define LN_01 SN_02
  #elif !defined(LN_02)
    #define LN_02 SN_02
  #elif !defined(LN_03)
    #define LN_03 SN_02
  #elif !defined(LN_04)
    #define LN_04 SN_02
  #elif !defined(LN_05)
    #define LN_05 SN_02
  #elif !defined(LN_06)
    #define LN_06 SN_02
  #elif !defined(LN_07)
    #define LN_07 SN_02
  #elif !defined(LN_08)
    #define LN_08 SN_02
  #elif !defined(LN_09)
    #define LN_09 SN_02
  #elif !defined(LN_10)
    #define LN_10 SN_02
  #elif !defined(LN_11)
    #define LN_11 SN_02
  #elif !defined(LN_12)
    #define LN_12 SN_02
  #elif !defined(LN_13)
    #define LN_13 SN_02
  #elif !defined(LN_14)
    #define LN_14 SN_02
  #elif !defined(LN_15)
    #define LN_15 SN_02
  #else
    #error "Slot left without fields present - you should not arrive here"
  #endif
#endif

#if defined(SN_03) && (SlotUse(SN_03) == cfgUseAsWait)
  #if !defined(LN_01)
    #define LN_01 SN_03
  #elif !defined(LN_02)
    #define LN_02 SN_03
  #elif !defined(LN_03)
    #define LN_03 SN_03
  #elif !defined(LN_04)
    #define LN_04 SN_03
  #elif !defined(LN_05)
    #define LN_05 SN_03
  #elif !defined(LN_06)
    #define LN_06 SN_03
  #elif !defined(LN_07)
    #define LN_07 SN_03
  #elif !defined(LN_08)
    #define LN_08 SN_03
  #elif !defined(LN_09)
    #define LN_09 SN_03
  #elif !defined(LN_10)
    #define LN_10 SN_03
  #elif !defined(LN_11)
    #define LN_11 SN_03
  #elif !defined(LN_12)
    #define LN_12 SN_03
  #elif !defined(LN_13)
    #define LN_13 SN_03
  #elif !defined(LN_14)
    #define LN_14 SN_03
  #elif !defined(LN_15)
    #define LN_15 SN_03
  #else
    #error "Slot left without fields present - you should not arrive here"
  #endif
#endif

#if defined(SN_04) && (SlotUse(SN_04) == cfgUseAsWait)
  #if !defined(LN_01)
    #define LN_01 SN_04
  #elif !defined(LN_02)
    #define LN_02 SN_04
  #elif !defined(LN_03)
    #define LN_03 SN_04
  #elif !defined(LN_04)
    #define LN_04 SN_04
  #elif !defined(LN_05)
    #define LN_05 SN_04
  #elif !defined(LN_06)
    #define LN_06 SN_04
  #elif !defined(LN_07)
    #define LN_07 SN_04
  #elif !defined(LN_08)
    #define LN_08 SN_04
  #elif !defined(LN_09)
    #define LN_09 SN_04
  #elif !defined(LN_10)
    #define LN_10 SN_04
  #elif !defined(LN_11)
    #define LN_11 SN_04
  #elif !defined(LN_12)
    #define LN_12 SN_04
  #elif !defined(LN_13)
    #define LN_13 SN_04
  #elif !defined(LN_14)
    #define LN_14 SN_04
  #elif !defined(LN_15)
    #define LN_15 SN_04
  #else
    #error "Slot left without fields present - you should not arrive here"
  #endif
#endif

#if defined(SN_05) && (SlotUse(SN_05) == cfgUseAsWait)
  #if !defined(LN_01)
    #define LN_01 SN_05
  #elif !defined(LN_02)
    #define LN_02 SN_05
  #elif !defined(LN_03)
    #define LN_03 SN_05
  #elif !defined(LN_04)
    #define LN_04 SN_05
  #elif !defined(LN_05)
    #define LN_05 SN_05
  #elif !defined(LN_06)
    #define LN_06 SN_05
  #elif !defined(LN_07)
    #define LN_07 SN_05
  #elif !defined(LN_08)
    #define LN_08 SN_05
  #elif !defined(LN_09)
    #define LN_09 SN_05
  #elif !defined(LN_10)
    #define LN_10 SN_05
  #elif !defined(LN_11)
    #define LN_11 SN_05
  #elif !defined(LN_12)
    #define LN_12 SN_05
  #elif !defined(LN_13)
    #define LN_13 SN_05
  #elif !defined(LN_14)
    #define LN_14 SN_05
  #elif !defined(LN_15)
    #define LN_15 SN_05
  #else
    #error "Slot left without fields present - you should not arrive here"
  #endif
#endif

#if defined(SN_06) && (SlotUse(SN_06) == cfgUseAsWait)
  #if !defined(LN_01)
    #define LN_01 SN_06
  #elif !defined(LN_02)
    #define LN_02 SN_06
  #elif !defined(LN_03)
    #define LN_03 SN_06
  #elif !defined(LN_04)
    #define LN_04 SN_06
  #elif !defined(LN_05)
    #define LN_05 SN_06
  #elif !defined(LN_06)
    #define LN_06 SN_06
  #elif !defined(LN_07)
    #define LN_07 SN_06
  #elif !defined(LN_08)
    #define LN_08 SN_06
  #elif !defined(LN_09)
    #define LN_09 SN_06
  #elif !defined(LN_10)
    #define LN_10 SN_06
  #elif !defined(LN_11)
    #define LN_11 SN_06
  #elif !defined(LN_12)
    #define LN_12 SN_06
  #elif !defined(LN_13)
    #define LN_13 SN_06
  #elif !defined(LN_14)
    #define LN_14 SN_06
  #elif !defined(LN_15)
    #define LN_15 SN_06
  #else
    #error "Slot left without fields present - you should not arrive here"
  #endif
#endif

#if defined(SN_07) && (SlotUse(SN_07) == cfgUseAsWait)
  #if !defined(LN_01)
    #define LN_01 SN_07
  #elif !defined(LN_02)
    #define LN_02 SN_07
  #elif !defined(LN_03)
    #define LN_03 SN_07
  #elif !defined(LN_04)
    #define LN_04 SN_07
  #elif !defined(LN_05)
    #define LN_05 SN_07
  #elif !defined(LN_06)
    #define LN_06 SN_07
  #elif !defined(LN_07)
    #define LN_07 SN_07
  #elif !defined(LN_08)
    #define LN_08 SN_07
  #elif !defined(LN_09)
    #define LN_09 SN_07
  #elif !defined(LN_10)
    #define LN_10 SN_07
  #elif !defined(LN_11)
    #define LN_11 SN_07
  #elif !defined(LN_12)
    #define LN_12 SN_07
  #elif !defined(LN_13)
    #define LN_13 SN_07
  #elif !defined(LN_14)
    #define LN_14 SN_07
  #elif !defined(LN_15)
    #define LN_15 SN_07
  #else
    #error "Slot left without fields present - you should not arrive here"
  #endif
#endif

#if defined(SN_08) && (SlotUse(SN_08) == cfgUseAsWait)
  #if !defined(LN_01)
    #define LN_01 SN_08
  #elif !defined(LN_02)
    #define LN_02 SN_08
  #elif !defined(LN_03)
    #define LN_03 SN_08
  #elif !defined(LN_04)
    #define LN_04 SN_08
  #elif !defined(LN_05)
    #define LN_05 SN_08
  #elif !defined(LN_06)
    #define LN_06 SN_08
  #elif !defined(LN_07)
    #define LN_07 SN_08
  #elif !defined(LN_08)
    #define LN_08 SN_08
  #elif !defined(LN_09)
    #define LN_09 SN_08
  #elif !defined(LN_10)
    #define LN_10 SN_08
  #elif !defined(LN_11)
    #define LN_11 SN_08
  #elif !defined(LN_12)
    #define LN_12 SN_08
  #elif !defined(LN_13)
    #define LN_13 SN_08
  #elif !defined(LN_14)
    #define LN_14 SN_08
  #elif !defined(LN_15)
    #define LN_15 SN_08
  #else
    #error "Slot left without fields present - you should not arrive here"
  #endif
#endif

#if defined(SN_09) && (SlotUse(SN_09) == cfgUseAsWait)
  #if !defined(LN_01)
    #define LN_01 SN_09
  #elif !defined(LN_02)
    #define LN_02 SN_09
  #elif !defined(LN_03)
    #define LN_03 SN_09
  #elif !defined(LN_04)
    #define LN_04 SN_09
  #elif !defined(LN_05)
    #define LN_05 SN_09
  #elif !defined(LN_06)
    #define LN_06 SN_09
  #elif !defined(LN_07)
    #define LN_07 SN_09
  #elif !defined(LN_08)
    #define LN_08 SN_09
  #elif !defined(LN_09)
    #define LN_09 SN_09
  #elif !defined(LN_10)
    #define LN_10 SN_09
  #elif !defined(LN_11)
    #define LN_11 SN_09
  #elif !defined(LN_12)
    #define LN_12 SN_09
  #elif !defined(LN_13)
    #define LN_13 SN_09
  #elif !defined(LN_14)
    #define LN_14 SN_09
  #elif !defined(LN_15)
    #define LN_15 SN_09
  #else
    #error "Slot left without fields present - you should not arrive here"
  #endif
#endif

#if defined(SN_10) && (SlotUse(SN_10) == cfgUseAsWait)
  #if !defined(LN_01)
    #define LN_01 SN_10
  #elif !defined(LN_02)
    #define LN_02 SN_10
  #elif !defined(LN_03)
    #define LN_03 SN_10
  #elif !defined(LN_04)
    #define LN_04 SN_10
  #elif !defined(LN_05)
    #define LN_05 SN_10
  #elif !defined(LN_06)
    #define LN_06 SN_10
  #elif !defined(LN_07)
    #define LN_07 SN_10
  #elif !defined(LN_08)
    #define LN_08 SN_10
  #elif !defined(LN_09)
    #define LN_09 SN_10
  #elif !defined(LN_10)
    #define LN_10 SN_10
  #elif !defined(LN_11)
    #define LN_11 SN_10
  #elif !defined(LN_12)
    #define LN_12 SN_10
  #elif !defined(LN_13)
    #define LN_13 SN_10
  #elif !defined(LN_14)
    #define LN_14 SN_10
  #elif !defined(LN_15)
    #define LN_15 SN_10
  #else
    #error "Slot left without fields present - you should not arrive here"
  #endif
#endif

#if defined(SN_11) && (SlotUse(SN_11) == cfgUseAsWait)
  #if !defined(LN_01)
    #define LN_01 SN_11
  #elif !defined(LN_02)
    #define LN_02 SN_11
  #elif !defined(LN_03)
    #define LN_03 SN_11
  #elif !defined(LN_04)
    #define LN_04 SN_11
  #elif !defined(LN_05)
    #define LN_05 SN_11
  #elif !defined(LN_06)
    #define LN_06 SN_11
  #elif !defined(LN_07)
    #define LN_07 SN_11
  #elif !defined(LN_08)
    #define LN_08 SN_11
  #elif !defined(LN_09)
    #define LN_09 SN_11
  #elif !defined(LN_10)
    #define LN_10 SN_11
  #elif !defined(LN_11)
    #define LN_11 SN_11
  #elif !defined(LN_12)
    #define LN_12 SN_11
  #elif !defined(LN_13)
    #define LN_13 SN_11
  #elif !defined(LN_14)
    #define LN_14 SN_11
  #elif !defined(LN_15)
    #define LN_15 SN_11
  #else
    #error "Slot left without fields present - you should not arrive here"
  #endif
#endif

#if defined(SN_12) && (SlotUse(SN_12) == cfgUseAsWait)
  #if !defined(LN_01)
    #define LN_01 SN_12
  #elif !defined(LN_02)
    #define LN_02 SN_12
  #elif !defined(LN_03)
    #define LN_03 SN_12
  #elif !defined(LN_04)
    #define LN_04 SN_12
  #elif !defined(LN_05)
    #define LN_05 SN_12
  #elif !defined(LN_06)
    #define LN_06 SN_12
  #elif !defined(LN_07)
    #define LN_07 SN_12
  #elif !defined(LN_08)
    #define LN_08 SN_12
  #elif !defined(LN_09)
    #define LN_09 SN_12
  #elif !defined(LN_10)
    #define LN_10 SN_12
  #elif !defined(LN_11)
    #define LN_11 SN_12
  #elif !defined(LN_12)
    #define LN_12 SN_12
  #elif !defined(LN_13)
    #define LN_13 SN_12
  #elif !defined(LN_14)
    #define LN_14 SN_12
  #elif !defined(LN_15)
    #define LN_15 SN_12
  #else
    #error "Slot left without fields present - you should not arrive here"
  #endif
#endif

#if defined(SN_13) && (SlotUse(SN_13) == cfgUseAsWait)
  #if !defined(LN_01)
    #define LN_01 SN_13
  #elif !defined(LN_02)
    #define LN_02 SN_13
  #elif !defined(LN_03)
    #define LN_03 SN_13
  #elif !defined(LN_04)
    #define LN_04 SN_13
  #elif !defined(LN_05)
    #define LN_05 SN_13
  #elif !defined(LN_06)
    #define LN_06 SN_13
  #elif !defined(LN_07)
    #define LN_07 SN_13
  #elif !defined(LN_08)
    #define LN_08 SN_13
  #elif !defined(LN_09)
    #define LN_09 SN_13
  #elif !defined(LN_10)
    #define LN_10 SN_13
  #elif !defined(LN_11)
    #define LN_11 SN_13
  #elif !defined(LN_12)
    #define LN_12 SN_13
  #elif !defined(LN_13)
    #define LN_13 SN_13
  #elif !defined(LN_14)
    #define LN_14 SN_13
  #elif !defined(LN_15)
    #define LN_15 SN_13
  #else
    #error "Slot left without fields present - you should not arrive here"
  #endif
#endif

#if defined(SN_14) && (SlotUse(SN_14) == cfgUseAsWait)
  #if !defined(LN_01)
    #define LN_01 SN_14
  #elif !defined(LN_02)
    #define LN_02 SN_14
  #elif !defined(LN_03)
    #define LN_03 SN_14
  #elif !defined(LN_04)
    #define LN_04 SN_14
  #elif !defined(LN_05)
    #define LN_05 SN_14
  #elif !defined(LN_06)
    #define LN_06 SN_14
  #elif !defined(LN_07)
    #define LN_07 SN_14
  #elif !defined(LN_08)
    #define LN_08 SN_14
  #elif !defined(LN_09)
    #define LN_09 SN_14
  #elif !defined(LN_10)
    #define LN_10 SN_14
  #elif !defined(LN_11)
    #define LN_11 SN_14
  #elif !defined(LN_12)
    #define LN_12 SN_14
  #elif !defined(LN_13)
    #define LN_13 SN_14
  #elif !defined(LN_14)
    #define LN_14 SN_14
  #elif !defined(LN_15)
    #define LN_15 SN_14
  #else
    #error "Slot left without fields present - you should not arrive here"
  #endif
#endif

#if defined(SN_15) && (SlotUse(SN_15) == cfgUseAsWait)
  #if !defined(LN_01)
    #define LN_01 SN_15
  #elif !defined(LN_02)
    #define LN_02 SN_15
  #elif !defined(LN_03)
    #define LN_03 SN_15
  #elif !defined(LN_04)
    #define LN_04 SN_15
  #elif !defined(LN_05)
    #define LN_05 SN_15
  #elif !defined(LN_06)
    #define LN_06 SN_15
  #elif !defined(LN_07)
    #define LN_07 SN_15
  #elif !defined(LN_08)
    #define LN_08 SN_15
  #elif !defined(LN_09)
    #define LN_09 SN_15
  #elif !defined(LN_10)
    #define LN_10 SN_15
  #elif !defined(LN_11)
    #define LN_11 SN_15
  #elif !defined(LN_12)
    #define LN_12 SN_15
  #elif !defined(LN_13)
    #define LN_13 SN_15
  #elif !defined(LN_14)
    #define LN_14 SN_15
  #elif !defined(LN_15)
    #define LN_15 SN_15
  #else
    #error "Slot left without fields present - you should not arrive here"
  #endif
#endif


#if !defined(LN_01)
  #define defNumberWaitEnd 1
#elif !defined(LN_02)
  #define defNumberWaitEnd 2
#elif !defined(LN_03)
  #define defNumberWaitEnd 3
#elif !defined(LN_04)
  #define defNumberWaitEnd 4
#elif !defined(LN_05)
  #define defNumberWaitEnd 5
#elif !defined(LN_06)
  #define defNumberWaitEnd 6
#elif !defined(LN_07)
  #define defNumberWaitEnd 7
#elif !defined(LN_08)
  #define defNumberWaitEnd 8
#elif !defined(LN_09)
  #define defNumberWaitEnd 9
#elif !defined(LN_10)
  #define defNumberWaitEnd 10
#elif !defined(LN_11)
  #define defNumberWaitEnd 11
#elif !defined(LN_12)
  #define defNumberWaitEnd 12
#elif !defined(LN_13)
  #define defNumberWaitEnd 13
#elif !defined(LN_14)
  #define defNumberWaitEnd 14
#elif !defined(LN_15)
  #define defNumberWaitEnd 15
#else
  #define defNumberWaitEnd 16
#endif

/* To loop through all waits use the following definitions, do not
 * include the end in the loop */
#define defNumberWaitBegin defNumberMutexEnd
#define defNumberOfWaits   (defNumberWaitEnd - defNumberWaitBegin)

/** <!--0724-->
 * Use to check if two macro's are equal.
 *
 * This is a handy function to test if two macros are equal if they
 * do not expand down to a number. Note it can only be used in c-like
 * if statements (not in preprocessor code) but that does not matter
 * since the compiler optimizer sees that the argument can be fully
 * evaluated at compile time.
 *
 */
#define preEqualMacros(A,B)      (strcmp(DefToString(A),DefToString(B))==0)

/**
 * The maximum number of slots. The slots range is 1..15. Slot 0 is
 * used to indicate a particular slot is free. This number gives the
 * number of queus, mutexes and waits.
 */
#define defNumberOfSlots   (defNumberWaitEnd - defNumberQueuBegin)

/** <!--0701-->
 * Use to see if a task is defined and included in the source
 *
 * Use this function to see if a task is defined and included in the source
 * Can be used in preproccessor statements
 */
#define preTaskDefined(TaskName) ( (TaskInclude(TaskName) != cfgExclude) )

/** <!--0702-->
 * Use to see if a slot is in use.
 *
 * Use this function to see if a slot is in use.
 * Can be used in preproccessor statements
 */
#define preSlotDefined(SlotName) ( (SlotUse(SlotName) != cfgUseAsNon) )

/** <!--0703-->
 * Use to see if a queu is defined on a slot name.
 *
 * Use this function to see if a queu is defined on a slot name.
 * Can be used in preproccessor statements
 */
#define preQueuDefined(SlotName) ( (SlotUse(SlotName) == cfgUseAsQueu) )

/** <!--0704-->
 * Use to see if a mutex is defined on a slot name.
 *
 * Use this function to see if a mutex is defined on a slot name.
 * Can be used in preproccessor statements
 */
#define preMutexDefined(SlotName) ( (SlotUse(SlotName) == cfgUseAsMutex) )

/** <!--0705-->
 * Use this function to see if a wait is defined on a slot name.
 *
 * Use this function to see if a wait is defined on a slot name.
 * Can be used in preproccessor statements
 */
#define preWaitDefined(SlotName) ( (SlotUse(SlotName) == cfgUseAsWait) )

/** <!--0706-->
 * Use this function to see if a file is defined on a file name.
 *
 * Use this function to see if a file is defined on a file name.
 * Can be used in preproccessor statements
 */
#define preFileDefined(FileName) ( (FileSpace(FileName) > 0 ) )

/** <!--0710-->
 * Use this function as general bit setter.
 *
 * Use this function to set a one or more bits on a variable, or to
 * modify one or more bits on a variable. For the AVR, if you use something
 * like PORTA = preBitSet1(PORTA,3), it is compiled to an atomic instruction.
 * The method is used internally since it can also deal with the cfgUndefined
 * constant (which is ignored).
 *
 * Note: n0 .. n7 should be constants and not variables.
 */
#define preBitSet
#define preBitSet1(p,n0)                      ( (p) | (Tuint08)(0xFF & ( ((Tuint16)1<<((n0) & 0xFF))) ) )
#define preBitSet2(p,n0,n1)                   ( (p) | (Tuint08)(0xFF & ( ((Tuint16)1<<((n0) & 0xFF)) | ((Tuint16)1<<((n1) & 0xFF))) ) )
#define preBitSet3(p,n0,n1,n2)                ( (p) | (Tuint08)(0xFF & ( ((Tuint16)1<<((n0) & 0xFF)) | ((Tuint16)1<<((n1) & 0xFF)) | ((Tuint16)1<<((n2) & 0xFF))) ) )
#define preBitSet4(p,n0,n1,n2,n3)             ( (p) | (Tuint08)(0xFF & ( ((Tuint16)1<<((n0) & 0xFF)) | ((Tuint16)1<<((n1) & 0xFF)) | ((Tuint16)1<<((n2) & 0xFF)) | ((Tuint16)1<<((n3) & 0xFF))) ) )
#define preBitSet5(p,n0,n1,n2,n3,n4)          ( (p) | (Tuint08)(0xFF & ( ((Tuint16)1<<((n0) & 0xFF)) | ((Tuint16)1<<((n1) & 0xFF)) | ((Tuint16)1<<((n2) & 0xFF)) | ((Tuint16)1<<((n3) & 0xFF)) | ((Tuint16)1<<((n4) & 0xFF))) ) )
#define preBitSet6(p,n0,n1,n2,n3,n4,n5)       ( (p) | (Tuint08)(0xFF & ( ((Tuint16)1<<((n0) & 0xFF)) | ((Tuint16)1<<((n1) & 0xFF)) | ((Tuint16)1<<((n2) & 0xFF)) | ((Tuint16)1<<((n3) & 0xFF)) | ((Tuint16)1<<((n4) & 0xFF)) | ((Tuint16)1<<((n5) & 0xFF))) ) )
#define preBitSet7(p,n0,n1,n2,n3,n4,n5,n6)    ( (p) | (Tuint08)(0xFF & ( ((Tuint16)1<<((n0) & 0xFF)) | ((Tuint16)1<<((n1) & 0xFF)) | ((Tuint16)1<<((n2) & 0xFF)) | ((Tuint16)1<<((n3) & 0xFF)) | ((Tuint16)1<<((n4) & 0xFF)) | ((Tuint16)1<<((n5) & 0xFF)) | ((Tuint16)1<<((n6) & 0xFF))) ) )
#define preBitSet8(p,n0,n1,n2,n3,n4,n5,n6,n7) ( (p) | (Tuint08)(0xFF & ( ((Tuint16)1<<((n0) & 0xFF)) | ((Tuint16)1<<((n1) & 0xFF)) | ((Tuint16)1<<((n2) & 0xFF)) | ((Tuint16)1<<((n3) & 0xFF)) | ((Tuint16)1<<((n4) & 0xFF)) | ((Tuint16)1<<((n5) & 0xFF)) | ((Tuint16)1<<((n6) & 0xFF)) | ((Tuint16)1<<((n7) & 0xFF))) ) )

/** <!--0711-->
 * Use this function as general method to clear bits.
 *
 * Use this function to clear a one or more bits on a variable, or to
 * modify one or more bits on a variable. For the AVR, if you use something
 * like PORTA = preBitClr1(PORTA,3), it is compiled to an atomic instruction.
 * The method is used internally since it can also deal with the cfgUndefined
 * constant (which is ignored).
 *
 * Note: n0 .. n7 should be constants and not variables.
 */
#define preBitClr
#define preBitClr1(p,n0)                      ( (p) & (Tuint08)(0xFF & ~( ((Tuint16)1<<((n0) & 0xFF))) ) )
#define preBitClr2(p,n0,n1)                   ( (p) & (Tuint08)(0xFF & ~( ((Tuint16)1<<((n0) & 0xFF)) | ((Tuint16)1<<((n1) & 0xFF))) ) )
#define preBitClr3(p,n0,n1,n2)                ( (p) & (Tuint08)(0xFF & ~( ((Tuint16)1<<((n0) & 0xFF)) | ((Tuint16)1<<((n1) & 0xFF)) | ((Tuint16)1<<((n2) & 0xFF))) ) )
#define preBitClr4(p,n0,n1,n2,n3)             ( (p) & (Tuint08)(0xFF & ~( ((Tuint16)1<<((n0) & 0xFF)) | ((Tuint16)1<<((n1) & 0xFF)) | ((Tuint16)1<<((n2) & 0xFF)) | ((Tuint16)1<<((n3) & 0xFF))) ) )
#define preBitClr5(p,n0,n1,n2,n3,n4)          ( (p) & (Tuint08)(0xFF & ~( ((Tuint16)1<<((n0) & 0xFF)) | ((Tuint16)1<<((n1) & 0xFF)) | ((Tuint16)1<<((n2) & 0xFF)) | ((Tuint16)1<<((n3) & 0xFF)) | ((Tuint16)1<<((n4) & 0xFF))) ) )
#define preBitClr6(p,n0,n1,n2,n3,n4,n5)       ( (p) & (Tuint08)(0xFF & ~( ((Tuint16)1<<((n0) & 0xFF)) | ((Tuint16)1<<((n1) & 0xFF)) | ((Tuint16)1<<((n2) & 0xFF)) | ((Tuint16)1<<((n3) & 0xFF)) | ((Tuint16)1<<((n4) & 0xFF)) | ((Tuint16)1<<((n5) & 0xFF))) ) )
#define preBitClr7(p,n0,n1,n2,n3,n4,n5,n6)    ( (p) & (Tuint08)(0xFF & ~( ((Tuint16)1<<((n0) & 0xFF)) | ((Tuint16)1<<((n1) & 0xFF)) | ((Tuint16)1<<((n2) & 0xFF)) | ((Tuint16)1<<((n3) & 0xFF)) | ((Tuint16)1<<((n4) & 0xFF)) | ((Tuint16)1<<((n5) & 0xFF)) | ((Tuint16)1<<((n6) & 0xFF))) ) )
#define preBitClr8(p,n0,n1,n2,n3,n4,n5,n6,n7) ( (p) & (Tuint08)(0xFF & ~( ((Tuint16)1<<((n0) & 0xFF)) | ((Tuint16)1<<((n1) & 0xFF)) | ((Tuint16)1<<((n2) & 0xFF)) | ((Tuint16)1<<((n3) & 0xFF)) | ((Tuint16)1<<((n4) & 0xFF)) | ((Tuint16)1<<((n5) & 0xFF)) | ((Tuint16)1<<((n6) & 0xFF)) | ((Tuint16)1<<((n7) & 0xFF))) ) )

/** <!--0712-->
 * Test if a bit on a particular place is set.
 *
 * Use this to quickly test if a bit is set on a location (0..7). On the
 * AVR this is compiled to a atomic test if possible.
 */
#define preBitIsSet(p,q)                      ( ((p) & (1<<(q))) == (1<<(q)) )

/** <!--0713-->
 * Test if a bit on a particular place is cleared.
 *
 * Use this to quickly test if a bit is cleared on a location (0..7). On the
 * AVR this is compiled to a atomic test if possible.
 */
#define preBitIsClr(p,q)                      ( ((p) & (1<<(q))) == 0 )

/** <!--0720-->
 * Returns the task number of a task with the name "TaskName".
 *
 * Using this function it is possible to use static names for the tasks instead of
 * numbers. It returns the task number of a task with the compile time name "TaskName".
 * This does not equal the task
 * name that is embedded in the code, that may be different or even absent). This
 * function is implicitly called in all 'OnName' variants of the API calls. Make sure you
 * correctly spell the name, since that is not checked here (and will give rise to a compile
 * time warning only). Preferably uses names instead of numbers
 * in your code since the numbering depends an the inclusions and may differ at different compiles.
 * Substitution of the numbers is done at compile time and does not require code.
 * Cannot be used in preprocessor statements.
 */
#define preTaskNumberOf(TaskName) \
  ( (strcmp(DefToString(defCurrentTaskName),#TaskName)==0) ? defCurrentTaskNumber : (strcmp(DefToString(TN_00),#TaskName)==0) ? 0 : (strcmp(DefToString(TN_01),#TaskName)==0) ? 1 : (strcmp(DefToString(TN_02),#TaskName)==0) ? 2 : (strcmp(DefToString(TN_03),#TaskName)==0) ? 3 : (strcmp(DefToString(TN_04),#TaskName)==0) ? 4 : (strcmp(DefToString(TN_05),#TaskName)==0) ? 5 : (strcmp(DefToString(TN_06),#TaskName)==0) ? 6 : (strcmp(DefToString(TN_07),#TaskName)==0) ? 7 : (strcmp(DefToString(TN_08),#TaskName)==0) ? 8 : (strcmp(DefToString(TN_09),#TaskName)==0) ? 9 : (strcmp(DefToString(TN_10),#TaskName)==0) ? 10 : (strcmp(DefToString(TN_11),#TaskName)==0) ? 11 : (strcmp(DefToString(TN_12),#TaskName)==0) ? 12 : (strcmp(DefToString(TN_13),#TaskName)==0) ? 13 : (strcmp(DefToString(TN_14),#TaskName)==0) ? 14 : (strcmp(DefToString(TN_15),#TaskName)==0) ? 15 : -1 )

/** <!--0721-->
 * Returns the slot number of a slot with the name "SlotName".
 *
 * Using this function it is possible to use static names for the slots instead of
 * numbers. It returns the slot number of a slot with the compile time name "SlotName". This
 * function is implicitly called in all 'OnName' variants of the API calls. Make sure you
 * correctly spell the name, since that is not checked here (and will give rise to a compile
 * time warning only). Preferably uses names instead of numbers
 * in your code since the numbering depends an the inclusions and may differ at different compiles.
 * Substitution of the numbers is done at compile time and does not require code.
 * Cannot be used in preprocessor statements.
 */
#define preSlotNumberOf(SlotName) \
  ( (strcmp(DefToString(LN_01),#SlotName)==0) ? 1 : (strcmp(DefToString(LN_02),#SlotName)==0) ? 2 : (strcmp(DefToString(LN_03),#SlotName)==0) ? 3 : (strcmp(DefToString(LN_04),#SlotName)==0) ? 4 : (strcmp(DefToString(LN_05),#SlotName)==0) ? 5 : (strcmp(DefToString(LN_06),#SlotName)==0) ? 6 : (strcmp(DefToString(LN_07),#SlotName)==0) ? 7 : (strcmp(DefToString(LN_08),#SlotName)==0) ? 8 : (strcmp(DefToString(LN_09),#SlotName)==0) ? 9 : (strcmp(DefToString(LN_10),#SlotName)==0) ? 10 : (strcmp(DefToString(LN_11),#SlotName)==0) ? 11 : (strcmp(DefToString(LN_12),#SlotName)==0) ? 12 : (strcmp(DefToString(LN_13),#SlotName)==0) ? 13 : (strcmp(DefToString(LN_14),#SlotName)==0) ? 14 : (strcmp(DefToString(LN_15),#SlotName)==0) ? 15 : -1 )

/** <!--0722-->
 * Returns the file number of a file with the name "FileName".
 *
 * Using this function it is possible to use static names for the files instead of
 * numbers. It returns the file number of a file with the compile time name "FileName". This
 * function is implicitly called in all 'OnName' variants of the API calls. Make sure you
 * correctly spell the name, since that is not checked here (and will give rise to a compile
 * time warning only). Preferably uses names instead of numbers
 * in your code since the numbering depends an the inclusions and may differ at different compiles.
 * Substitution of the numbers is done at compile time and does not require code.
 * Cannot be used in preprocessor statements.
 */
#define preFileNumberOf(FileName) \
  ( (strcmp(DefToString(VN_00),#FileName)==0) ? 0 : (strcmp(DefToString(VN_01),#FileName)==0) ? 1 : (strcmp(DefToString(VN_02),#FileName)==0) ? 2 : (strcmp(DefToString(VN_03),#FileName)==0) ? 3 : (strcmp(DefToString(VN_04),#FileName)==0) ? 4 : (strcmp(DefToString(VN_05),#FileName)==0) ? 5 : (strcmp(DefToString(VN_06),#FileName)==0) ? 6 : (strcmp(DefToString(VN_07),#FileName)==0) ? 7 : (strcmp(DefToString(VN_08),#FileName)==0) ? 8 : (strcmp(DefToString(VN_09),#FileName)==0) ? 9 : (strcmp(DefToString(VN_10),#FileName)==0) ? 10 : (strcmp(DefToString(VN_11),#FileName)==0) ? 11 : (strcmp(DefToString(VN_12),#FileName)==0) ? 12 : (strcmp(DefToString(VN_13),#FileName)==0) ? 13 : (strcmp(DefToString(VN_14),#FileName)==0) ? 14 : (strcmp(DefToString(VN_15),#FileName)==0) ? 15 : -1 )

/** <!--0723-->
 * Returns the number of an event named "EventName".
 *
 * Using this function it is possible to use static names for the event instead of
 * numbers. It returns the event number of an event with the compile time name "EventName". This
 * function is implicitly called in all 'OnName' variants of the API calls. Make sure you
 * correctly spell the name, since that is not checked here (and will give rise to a compile
 * time warning only) Preferably uses names instead of numbers
 * in your code since the numbering depends an the inclusions and may differ at different compiles.
 * Substitution of the numbers is done at compile time and does not require code.
 * Cannot be used in preprocessor statements.
 */
#define preEventNumberOf(EventName) \
  ( (strcmp(DefToString(EN_00),#EventName)==0) ? 0 : (strcmp(DefToString(EN_01),#EventName)==0) ? 1 : (strcmp(DefToString(EN_02),#EventName)==0) ? 2 : (strcmp(DefToString(EN_03),#EventName)==0) ? 3 : (strcmp(DefToString(EN_04),#EventName)==0) ? 4 : (strcmp(DefToString(EN_05),#EventName)==0) ? 5 : (strcmp(DefToString(EN_06),#EventName)==0) ? 6 : (strcmp(DefToString(EN_07),#EventName)==0) ? 7 : -1 )

/**
 * Use this to directly transform the name of an Event to the correct bit location.
 * Equivalent with (1 << preEventNumberOf(EventName)) except for non existent names.
 */
#define preEventLocationOf(EventName) \
  ( (strcmp(DefToString(EN_00),#EventName)==0) ? 0x01 : (strcmp(DefToString(EN_01),#EventName)==0) ? 0x02 : (strcmp(DefToString(EN_02),#EventName)==0) ? 0x04 : (strcmp(DefToString(EN_03),#EventName)==0) ? 0x08 : (strcmp(DefToString(EN_04),#EventName)==0) ? 0x10 : (strcmp(DefToString(EN_05),#EventName)==0) ? 0x20 : (strcmp(DefToString(EN_06),#EventName)==0) ? 0x40 : (strcmp(DefToString(EN_07),#EventName)==0) ? 0x80 : -1 )

/**
 * Functions to check the use of the parameters inside macro's. It generates the
 * value -1 if the event number is illegal. If you this will issue a warning at
 * compile time (do not ignore it!) and an fatIllegalEvent at runtime.
 */
#if (cfgCheckMethodUse == cfgTrue) &&  (cfgSysSqueezeState == cfgFalse)
  #define CheckEventNumber(Event)            ( ((Event < defNumberEventBegin) || (Event >= defNumberEventEnd)) ? (-1) :  (defEqualAuxiliaryRegisters) ? (((Event) == devAuxSysRegBit) ? (-1) : (Event)) : (Event) )
#elif (cfgCheckMethodUse == cfgTrue)
  #define CheckEventNumber(Event)            ( ((Event < defNumberEventBegin) || (Event >= defNumberEventEnd)) ? (-1) : (Event) )
#else
  #define CheckEventNumber(Event)            (Event)
#endif

/**
 * Functions which check if a slot has been properly used.
 */
#if (cfgCheckMethodUse == cfgTrue)
  #define PassIfSlotIsWait(SlotNumber)  ( (((SlotNumber)>=defNumberWaitBegin) && ((SlotNumber)<defNumberWaitEnd)) ? (SlotNumber) : defCheckSlotTypeError )
  #define PassIfSlotIsMutex(SlotNumber) ( (((SlotNumber)>=defNumberMutexBegin) && ((SlotNumber)<defNumberMutexEnd)) ? (SlotNumber) : defCheckSlotTypeError )
  #define PassIfSlotIsQueu(SlotNumber)  ( (((SlotNumber)>=defNumberQueuBegin) && ((SlotNumber)<defNumberQueuEnd)) ? (SlotNumber) : defCheckSlotTypeError )
#else
  #define PassIfSlotIsWait(SlotNumber)  ( SlotNumber )
  #define PassIfSlotIsMutex(SlotNumber) ( SlotNumber )
  #define PassIfSlotIsQueu(SlotNumber)  ( SlotNumber )
#endif


#if defined(TN_00) && (TaskInclude(TN_00) != cfgExclude) && (StackIsShared(TN_00))
  #define defTNSC00  1
#else
  #define defTNSC00  0
#endif

#if defined(TN_01) && (TaskInclude(TN_01) != cfgExclude) && (StackIsShared(TN_01))
  #define defTNSC01  1
#else
  #define defTNSC01  0
#endif

#if defined(TN_02) && (TaskInclude(TN_02) != cfgExclude) && (StackIsShared(TN_02))
  #define defTNSC02  1
#else
  #define defTNSC02  0
#endif

#if defined(TN_03) && (TaskInclude(TN_03) != cfgExclude) && (StackIsShared(TN_03))
  #define defTNSC03  1
#else
  #define defTNSC03  0
#endif

#if defined(TN_04) && (TaskInclude(TN_04) != cfgExclude) && (StackIsShared(TN_04))
  #define defTNSC04  1
#else
  #define defTNSC04  0
#endif

#if defined(TN_05) && (TaskInclude(TN_05) != cfgExclude) && (StackIsShared(TN_05))
  #define defTNSC05  1
#else
  #define defTNSC05  0
#endif

#if defined(TN_06) && (TaskInclude(TN_06) != cfgExclude) && (StackIsShared(TN_06))
  #define defTNSC06  1
#else
  #define defTNSC06  0
#endif

#if defined(TN_07) && (TaskInclude(TN_07) != cfgExclude) && (StackIsShared(TN_07))
  #define defTNSC07  1
#else
  #define defTNSC07  0
#endif

#if defined(TN_08) && (TaskInclude(TN_08) != cfgExclude) && (StackIsShared(TN_08))
  #define defTNSC08  1
#else
  #define defTNSC08  0
#endif

#if defined(TN_09) && (TaskInclude(TN_09) != cfgExclude) && (StackIsShared(TN_09))
  #define defTNSC09  1
#else
  #define defTNSC09  0
#endif

#if defined(TN_10) && (TaskInclude(TN_10) != cfgExclude) && (StackIsShared(TN_10))
  #define defTNSC10  1
#else
  #define defTNSC10  0
#endif

#if defined(TN_11) && (TaskInclude(TN_11) != cfgExclude) && (StackIsShared(TN_11))
  #define defTNSC11  1
#else
  #define defTNSC11  0
#endif

#if defined(TN_12) && (TaskInclude(TN_12) != cfgExclude) && (StackIsShared(TN_12))
  #define defTNSC12  1
#else
  #define defTNSC12  0
#endif

#if defined(TN_13) && (TaskInclude(TN_13) != cfgExclude) && (StackIsShared(TN_13))
  #define defTNSC13  1
#else
  #define defTNSC13  0
#endif

#if defined(TN_14) && (TaskInclude(TN_14) != cfgExclude) && (StackIsShared(TN_14))
  #define defTNSC14  1
#else
  #define defTNSC14  0
#endif

#if defined(TN_15) && (TaskInclude(TN_15) != cfgExclude) && (StackIsShared(TN_15))
  #define defTNSC15  1
#else
  #define defTNSC15  0
#endif

#define defNumberOfSharedTasks (defTNSC00 + defTNSC01 + defTNSC02 + defTNSC03 + defTNSC04 + defTNSC05 + defTNSC06 + defTNSC07 + defTNSC08 + defTNSC09 + defTNSC10 + defTNSC11 + defTNSC12 + defTNSC13 + defTNSC14 + defTNSC15)


#if defined(TN_00) && (TaskInclude(TN_00) != cfgExclude) && (StackIsShared(TN_00))
  #define defTaskNumberSharedBegin  0
#elif defined(TN_01) && (TaskInclude(TN_01) != cfgExclude) && (StackIsShared(TN_01))
  #define defTaskNumberSharedBegin  1
#elif defined(TN_02) && (TaskInclude(TN_02) != cfgExclude) && (StackIsShared(TN_02))
  #define defTaskNumberSharedBegin  2
#elif defined(TN_03) && (TaskInclude(TN_03) != cfgExclude) && (StackIsShared(TN_03))
  #define defTaskNumberSharedBegin  3
#elif defined(TN_04) && (TaskInclude(TN_04) != cfgExclude) && (StackIsShared(TN_04))
  #define defTaskNumberSharedBegin  4
#elif defined(TN_05) && (TaskInclude(TN_05) != cfgExclude) && (StackIsShared(TN_05))
  #define defTaskNumberSharedBegin  5
#elif defined(TN_06) && (TaskInclude(TN_06) != cfgExclude) && (StackIsShared(TN_06))
  #define defTaskNumberSharedBegin  6
#elif defined(TN_07) && (TaskInclude(TN_07) != cfgExclude) && (StackIsShared(TN_07))
  #define defTaskNumberSharedBegin  7
#elif defined(TN_08) && (TaskInclude(TN_08) != cfgExclude) && (StackIsShared(TN_08))
  #define defTaskNumberSharedBegin  8
#elif defined(TN_09) && (TaskInclude(TN_09) != cfgExclude) && (StackIsShared(TN_09))
  #define defTaskNumberSharedBegin  9
#elif defined(TN_10) && (TaskInclude(TN_10) != cfgExclude) && (StackIsShared(TN_10))
  #define defTaskNumberSharedBegin 10
#elif defined(TN_11) && (TaskInclude(TN_11) != cfgExclude) && (StackIsShared(TN_11))
  #define defTaskNumberSharedBegin 11
#elif defined(TN_12) && (TaskInclude(TN_12) != cfgExclude) && (StackIsShared(TN_12))
  #define defTaskNumberSharedBegin 12
#elif defined(TN_13) && (TaskInclude(TN_13) != cfgExclude) && (StackIsShared(TN_13))
  #define defTaskNumberSharedBegin 13
#elif defined(TN_14) && (TaskInclude(TN_14) != cfgExclude) && (StackIsShared(TN_14))
  #define defTaskNumberSharedBegin 14
#elif defined(TN_15) && (TaskInclude(TN_15) != cfgExclude) && (StackIsShared(TN_15))
  #define defTaskNumberSharedBegin 15
#else
  #define defTaskNumberSharedBegin 16
#endif

#if defined(TN_15) && (TaskInclude(TN_15) != cfgExclude) && (StackIsShared(TN_15))
  #define defTaskNumberSharedEnd  16
#elif defined(TN_14) && (TaskInclude(TN_14) != cfgExclude) && (StackIsShared(TN_14))
  #define defTaskNumberSharedEnd  15
#elif defined(TN_13) && (TaskInclude(TN_13) != cfgExclude) && (StackIsShared(TN_13))
  #define defTaskNumberSharedEnd  14
#elif defined(TN_12) && (TaskInclude(TN_12) != cfgExclude) && (StackIsShared(TN_12))
  #define defTaskNumberSharedEnd  13
#elif defined(TN_11) && (TaskInclude(TN_11) != cfgExclude) && (StackIsShared(TN_11))
  #define defTaskNumberSharedEnd  12
#elif defined(TN_10) && (TaskInclude(TN_10) != cfgExclude) && (StackIsShared(TN_10))
  #define defTaskNumberSharedEnd  11
#elif defined(TN_09) && (TaskInclude(TN_09) != cfgExclude) && (StackIsShared(TN_09))
  #define defTaskNumberSharedEnd  10
#elif defined(TN_08) && (TaskInclude(TN_08) != cfgExclude) && (StackIsShared(TN_08))
  #define defTaskNumberSharedEnd   9
#elif defined(TN_07) && (TaskInclude(TN_07) != cfgExclude) && (StackIsShared(TN_07))
  #define defTaskNumberSharedEnd   8
#elif defined(TN_06) && (TaskInclude(TN_06) != cfgExclude) && (StackIsShared(TN_06))
  #define defTaskNumberSharedEnd   7
#elif defined(TN_05) && (TaskInclude(TN_05) != cfgExclude) && (StackIsShared(TN_05))
  #define defTaskNumberSharedEnd   6
#elif defined(TN_04) && (TaskInclude(TN_04) != cfgExclude) && (StackIsShared(TN_04))
  #define defTaskNumberSharedEnd   5
#elif defined(TN_03) && (TaskInclude(TN_03) != cfgExclude) && (StackIsShared(TN_03))
  #define defTaskNumberSharedEnd   4
#elif defined(TN_02) && (TaskInclude(TN_02) != cfgExclude) && (StackIsShared(TN_02))
  #define defTaskNumberSharedEnd   3
#elif defined(TN_01) && (TaskInclude(TN_01) != cfgExclude) && (StackIsShared(TN_01))
  #define defTaskNumberSharedEnd   2
#elif defined(TN_00) && (TaskInclude(TN_00) != cfgExclude) && (StackIsShared(TN_00))
  #define defTaskNumberSharedEnd   1
#else
  #define defTaskNumberSharedEnd   0
#endif

#if (defTaskNumberSharedEnd - defTaskNumberSharedBegin) > 0
 #define defUseSharedStack cfgTrue
#else
 #define defUseSharedStack cfgFalse
#endif

#if (defUseSharedStack == cfgTrue) && (defNumberOfSharedTasks == defNumberOfTasks)
  #define defAllSharedStack cfgTrue
#else
  #define defAllSharedStack cfgFalse
#endif

#if (defUseSharedStack == cfgTrue) && (defNumberOfSharedTasks == (defTaskNumberSharedEnd - defTaskNumberSharedBegin))
  #define defDenseSharedStack cfgTrue
#else
  #define defDenseSharedStack cfgFalse
#endif


#if (defUseSharedStack == cfgTrue) && (defAllSharedStack == cfgFalse)
  #define defSharesConstant cfgFalse
#else
  #define defSharesConstant cfgTrue
#endif

#if (defSharesConstant == cfgTrue)
  #if (defAllSharedStack == cfgTrue)
    #define defInitialSharedFixed defInitialSharePresent
  #else
    #define defInitialSharedFixed defInitialShareAbsent
  #endif
#endif


#if defNumberOfTasks == 0
 #define defRegisterUseCollect  ( 0x00 )
 #define defRegisterUseFixed    ( 0x00 )
 #define defRegisterUseTest     ( 1 == 1 )
 #define defPriorityFixed       ( 0x00 )
 #define defPriorityTest        ( 1 == 1 )
 #define defStackSizeFixed      ( 0x00 )
 #define defStackSizeTest       ( 1 == 1 )
 #define defStackSizeHuge       ( 1 == 0 )
 #define defStackSpaceTasks     ( 0 )
 #define defTaskIncludeFixed    ( 0x00 )
 #define defTaskIncludeTest     ( 1 == 1 )
 #define defCapabilitiesTest    ( 1 == 1 )
 #define defInterruptStartFixed ( 0x00 )
 #define defInterruptStartTest  ( 1 == 1 )
 #define defTimeSliceFixed      ( 0x00 )
 #define defTimeSliceTest       ( 1 == 1 )
#elif defNumberOfTasks == 1
 #define defRegisterUseCollect  ( (RegisterUse(TN_00)) )
 #define defRegisterUseFixed    ( (RegisterUse(TN_00)) )
 #define defRegisterUseTest     ( ( (defRegisterUseCollect) == (RegisterUse(TN_00)) ) )
 #define defPriorityFixed       ( Priority(TN_00) )
 #define defPriorityTest        ( 1 == 1 )
 #define defStackSizeFixed      ( StackSize(TN_00) )
 #define defStackSizeTest       ( 1 == 1 )
 #define defStackSizeHuge       ( ( (0xFF) < (StackSize(TN_00)) ) )
 #define defStackSpaceTasks     ( (StackSize(TN_00)) )
 #define defTaskIncludeFixed    ( TaskInclude(TN_00) )
 #define defTaskIncludeTest     ( 1 == 1 )
 #define defCapabilitiesTest    ( ( (cfgCapAll) == (Capabilities(TN_00)) ) )
 #define defInterruptStartFixed ( InterruptStart(TN_00) )
 #define defInterruptStartTest  ( 1 == 1 )
 #define defTimeSliceFixed      ( TimeSlice(TN_00) )
 #define defTimeSliceTest       ( 1 == 1 )
#elif defNumberOfTasks == 2
 #define defRegisterUseCollect  ( (RegisterUse(TN_00)) | (RegisterUse(TN_01)) )
 #define defRegisterUseFixed    ( (RegisterUse(TN_00)) & (RegisterUse(TN_01)) )
 #define defRegisterUseTest     ( ( (defRegisterUseCollect) == (RegisterUse(TN_00)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_01)) ) )
 #define defPriorityFixed       ( Priority(TN_00) )
 #define defPriorityTest        ( ( (defPriorityFixed) == (Priority(TN_01)) ) )
 #define defStackSizeFixed      ( StackSize(TN_00) )
 #define defStackSizeTest       ( ( (defStackSizeFixed) == (StackSize(TN_01)) ) )
 #define defStackSizeHuge       ( ( (0xFF) < (StackSize(TN_00)) ) || ( (0xFF) < (StackSize(TN_01)) ) )
 #define defStackSpaceTasks     ( (StackSize(TN_00)) + (StackSize(TN_01)) )
 #define defTaskIncludeFixed    ( TaskInclude(TN_00) )
 #define defTaskIncludeTest     ( ( (defTaskIncludeFixed) == (TaskInclude(TN_01)) ) )
 #define defCapabilitiesTest    ( ( (cfgCapAll) == (Capabilities(TN_00)) ) && ( (cfgCapAll) == (Capabilities(TN_01)) ) )
 #define defInterruptStartFixed ( InterruptStart(TN_00) )
 #define defInterruptStartTest  ( ( (defInterruptStartFixed) == (InterruptStart(TN_01)) ) )
 #define defTimeSliceFixed      ( TimeSlice(TN_00) )
 #define defTimeSliceTest       ( ( (defTimeSliceFixed) == (TimeSlice(TN_01)) ) )
#elif defNumberOfTasks == 3
 #define defRegisterUseCollect  ( (RegisterUse(TN_00)) | (RegisterUse(TN_01)) | (RegisterUse(TN_02)) )
 #define defRegisterUseFixed    ( (RegisterUse(TN_00)) & (RegisterUse(TN_01)) & (RegisterUse(TN_02)) )
 #define defRegisterUseTest     ( ( (defRegisterUseCollect) == (RegisterUse(TN_00)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_01)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_02)) ) )
 #define defPriorityFixed       ( Priority(TN_00) )
 #define defPriorityTest        ( ( (defPriorityFixed) == (Priority(TN_01)) ) &&  ( (defPriorityFixed) == (Priority(TN_02)) ) )
 #define defStackSizeFixed      ( StackSize(TN_00) )
 #define defStackSizeTest       ( ( (defStackSizeFixed) == (StackSize(TN_01)) ) &&  ( (defStackSizeFixed) == (StackSize(TN_02)) ) )
 #define defStackSizeHuge       ( ( (0xFF) < (StackSize(TN_00)) ) || ( (0xFF) < (StackSize(TN_01)) ) ||  ( (0xFF) < (StackSize(TN_02)) ) )
 #define defStackSpaceTasks     ( (StackSize(TN_00)) + (StackSize(TN_01)) +  (StackSize(TN_02)) )
 #define defTaskIncludeFixed    ( TaskInclude(TN_00) )
 #define defTaskIncludeTest     ( ( (defTaskIncludeFixed) == (TaskInclude(TN_01)) ) &&  ( (defTaskIncludeFixed) == (TaskInclude(TN_02)) ) )
 #define defCapabilitiesTest    ( ( (cfgCapAll) == (Capabilities(TN_00)) ) && ( (cfgCapAll) == (Capabilities(TN_01)) ) &&  ( (cfgCapAll) == (Capabilities(TN_02)) ) )
 #define defInterruptStartFixed ( InterruptStart(TN_00) )
 #define defInterruptStartTest  ( ( (defInterruptStartFixed) == (InterruptStart(TN_01)) ) &&  ( (defInterruptStartFixed) == (InterruptStart(TN_02)) ) )
 #define defTimeSliceFixed      ( TimeSlice(TN_00) )
 #define defTimeSliceTest       ( ( (defTimeSliceFixed) == (TimeSlice(TN_01)) ) &&  ( (defTimeSliceFixed) == (TimeSlice(TN_02)) ) )
#elif defNumberOfTasks == 4
 #define defRegisterUseCollect  ( (RegisterUse(TN_00)) | (RegisterUse(TN_01)) | (RegisterUse(TN_02)) | (RegisterUse(TN_03)) )
 #define defRegisterUseFixed    ( (RegisterUse(TN_00)) & (RegisterUse(TN_01)) & (RegisterUse(TN_02)) & (RegisterUse(TN_03)) )
 #define defRegisterUseTest     ( ( (defRegisterUseCollect) == (RegisterUse(TN_00)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_01)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_02)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_03)) ) )
 #define defPriorityFixed       ( Priority(TN_00) )
 #define defPriorityTest        ( ( (defPriorityFixed) == (Priority(TN_01)) ) &&  ( (defPriorityFixed) == (Priority(TN_02)) ) && ( (defPriorityFixed) == (Priority(TN_03)) ) )
 #define defStackSizeFixed      ( StackSize(TN_00) )
 #define defStackSizeTest       ( ( (defStackSizeFixed) == (StackSize(TN_01)) ) &&  ( (defStackSizeFixed) == (StackSize(TN_02)) ) && ( (defStackSizeFixed) == (StackSize(TN_03)) ) )
 #define defStackSizeHuge       ( ( (0xFF) < (StackSize(TN_00)) ) || ( (0xFF) < (StackSize(TN_01)) ) ||  ( (0xFF) < (StackSize(TN_02)) ) || ( (0xFF) < (StackSize(TN_03)) ) )
 #define defStackSpaceTasks     ( (StackSize(TN_00)) + (StackSize(TN_01)) +  (StackSize(TN_02)) + (StackSize(TN_03)) )
 #define defTaskIncludeFixed    ( TaskInclude(TN_00) )
 #define defTaskIncludeTest     ( ( (defTaskIncludeFixed) == (TaskInclude(TN_01)) ) &&  ( (defTaskIncludeFixed) == (TaskInclude(TN_02)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_03)) ) )
 #define defCapabilitiesTest    ( ( (cfgCapAll) == (Capabilities(TN_00)) ) && ( (cfgCapAll) == (Capabilities(TN_01)) ) &&  ( (cfgCapAll) == (Capabilities(TN_02)) ) && ( (cfgCapAll) == (Capabilities(TN_03)) ) )
 #define defInterruptStartFixed ( InterruptStart(TN_00) )
 #define defInterruptStartTest  ( ( (defInterruptStartFixed) == (InterruptStart(TN_01)) ) &&  ( (defInterruptStartFixed) == (InterruptStart(TN_02)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_03)) ) )
 #define defTimeSliceFixed      ( TimeSlice(TN_00) )
 #define defTimeSliceTest       ( ( (defTimeSliceFixed) == (TimeSlice(TN_01)) ) &&  ( (defTimeSliceFixed) == (TimeSlice(TN_02)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_03)) ) )
#elif defNumberOfTasks == 5
 #define defRegisterUseCollect  ( (RegisterUse(TN_00)) | (RegisterUse(TN_01)) | (RegisterUse(TN_02)) | (RegisterUse(TN_03)) | (RegisterUse(TN_04)) )
 #define defRegisterUseFixed    ( (RegisterUse(TN_00)) & (RegisterUse(TN_01)) & (RegisterUse(TN_02)) & (RegisterUse(TN_03)) & (RegisterUse(TN_04)) )
 #define defRegisterUseTest     ( ( (defRegisterUseCollect) == (RegisterUse(TN_00)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_01)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_02)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_03)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_04)) ) )
 #define defPriorityFixed       ( Priority(TN_00) )
 #define defPriorityTest        ( ( (defPriorityFixed) == (Priority(TN_01)) ) &&  ( (defPriorityFixed) == (Priority(TN_02)) ) && ( (defPriorityFixed) == (Priority(TN_03)) ) && ( (defPriorityFixed) == (Priority(TN_04)) ) )
 #define defStackSizeFixed      ( StackSize(TN_00) )
 #define defStackSizeTest       ( ( (defStackSizeFixed) == (StackSize(TN_01)) ) &&  ( (defStackSizeFixed) == (StackSize(TN_02)) ) && ( (defStackSizeFixed) == (StackSize(TN_03)) ) && ( (defStackSizeFixed) == (StackSize(TN_04)) ) )
 #define defStackSizeHuge       ( ( (0xFF) < (StackSize(TN_00)) ) || ( (0xFF) < (StackSize(TN_01)) ) ||  ( (0xFF) < (StackSize(TN_02)) ) || ( (0xFF) < (StackSize(TN_03)) ) || ( (0xFF) < (StackSize(TN_04)) ) )
 #define defStackSpaceTasks     ( (StackSize(TN_00)) + (StackSize(TN_01)) +  (StackSize(TN_02)) + (StackSize(TN_03)) + (StackSize(TN_04)) )
 #define defTaskIncludeFixed    ( TaskInclude(TN_00) )
 #define defTaskIncludeTest     ( ( (defTaskIncludeFixed) == (TaskInclude(TN_01)) ) &&  ( (defTaskIncludeFixed) == (TaskInclude(TN_02)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_03)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_04)) ) )
 #define defCapabilitiesTest    ( ( (cfgCapAll) == (Capabilities(TN_00)) ) && ( (cfgCapAll) == (Capabilities(TN_01)) ) &&  ( (cfgCapAll) == (Capabilities(TN_02)) ) && ( (cfgCapAll) == (Capabilities(TN_03)) ) && ( (cfgCapAll) == (Capabilities(TN_04)) ) )
 #define defInterruptStartFixed ( InterruptStart(TN_00) )
 #define defInterruptStartTest  ( ( (defInterruptStartFixed) == (InterruptStart(TN_01)) ) &&  ( (defInterruptStartFixed) == (InterruptStart(TN_02)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_03)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_04)) ) )
 #define defTimeSliceFixed      ( TimeSlice(TN_00) )
 #define defTimeSliceTest       ( ( (defTimeSliceFixed) == (TimeSlice(TN_01)) ) &&  ( (defTimeSliceFixed) == (TimeSlice(TN_02)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_03)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_04)) ) )
#elif defNumberOfTasks == 6
 #define defRegisterUseCollect  ( (RegisterUse(TN_00)) | (RegisterUse(TN_01)) | (RegisterUse(TN_02)) | (RegisterUse(TN_03)) | (RegisterUse(TN_04)) | (RegisterUse(TN_05)) )
 #define defRegisterUseFixed    ( (RegisterUse(TN_00)) & (RegisterUse(TN_01)) & (RegisterUse(TN_02)) & (RegisterUse(TN_03)) & (RegisterUse(TN_04)) & (RegisterUse(TN_05)) )
 #define defRegisterUseTest     ( ( (defRegisterUseCollect) == (RegisterUse(TN_00)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_01)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_02)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_03)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_04)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_05)) ) )
 #define defPriorityFixed       ( Priority(TN_00) )
 #define defPriorityTest        ( ( (defPriorityFixed) == (Priority(TN_01)) ) &&  ( (defPriorityFixed) == (Priority(TN_02)) ) && ( (defPriorityFixed) == (Priority(TN_03)) ) && ( (defPriorityFixed) == (Priority(TN_04)) ) && ( (defPriorityFixed) == (Priority(TN_05)) ) )
 #define defStackSizeFixed      ( StackSize(TN_00) )
 #define defStackSizeTest       ( ( (defStackSizeFixed) == (StackSize(TN_01)) ) &&  ( (defStackSizeFixed) == (StackSize(TN_02)) ) && ( (defStackSizeFixed) == (StackSize(TN_03)) ) && ( (defStackSizeFixed) == (StackSize(TN_04)) ) && ( (defStackSizeFixed) == (StackSize(TN_05)) ) )
 #define defStackSizeHuge       ( ( (0xFF) < (StackSize(TN_00)) ) || ( (0xFF) < (StackSize(TN_01)) ) ||  ( (0xFF) < (StackSize(TN_02)) ) || ( (0xFF) < (StackSize(TN_03)) ) || ( (0xFF) < (StackSize(TN_04)) ) || ( (0xFF) < (StackSize(TN_05)) ) )
 #define defStackSpaceTasks     ( (StackSize(TN_00)) + (StackSize(TN_01)) +  (StackSize(TN_02)) + (StackSize(TN_03)) + (StackSize(TN_04)) + (StackSize(TN_05)) )
 #define defTaskIncludeFixed    ( TaskInclude(TN_00) )
 #define defTaskIncludeTest     ( ( (defTaskIncludeFixed) == (TaskInclude(TN_01)) ) &&  ( (defTaskIncludeFixed) == (TaskInclude(TN_02)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_03)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_04)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_05)) ) )
 #define defCapabilitiesTest    ( ( (cfgCapAll) == (Capabilities(TN_00)) ) && ( (cfgCapAll) == (Capabilities(TN_01)) ) &&  ( (cfgCapAll) == (Capabilities(TN_02)) ) && ( (cfgCapAll) == (Capabilities(TN_03)) ) && ( (cfgCapAll) == (Capabilities(TN_04)) ) && ( (cfgCapAll) == (Capabilities(TN_05)) ) )
 #define defInterruptStartFixed ( InterruptStart(TN_00) )
 #define defInterruptStartTest  ( ( (defInterruptStartFixed) == (InterruptStart(TN_01)) ) &&  ( (defInterruptStartFixed) == (InterruptStart(TN_02)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_03)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_04)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_05)) ) )
 #define defTimeSliceFixed      ( TimeSlice(TN_00) )
 #define defTimeSliceTest       ( ( (defTimeSliceFixed) == (TimeSlice(TN_01)) ) &&  ( (defTimeSliceFixed) == (TimeSlice(TN_02)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_03)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_04)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_05)) ) )
#elif defNumberOfTasks == 7
 #define defRegisterUseCollect  ( (RegisterUse(TN_00)) | (RegisterUse(TN_01)) | (RegisterUse(TN_02)) | (RegisterUse(TN_03)) | (RegisterUse(TN_04)) | (RegisterUse(TN_05)) | (RegisterUse(TN_06)) )
 #define defRegisterUseFixed    ( (RegisterUse(TN_00)) & (RegisterUse(TN_01)) & (RegisterUse(TN_02)) & (RegisterUse(TN_03)) & (RegisterUse(TN_04)) & (RegisterUse(TN_05)) & (RegisterUse(TN_06)) )
 #define defRegisterUseTest     ( ( (defRegisterUseCollect) == (RegisterUse(TN_00)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_01)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_02)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_03)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_04)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_05)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_06)) ) )
 #define defPriorityFixed       ( Priority(TN_00) )
 #define defPriorityTest        ( ( (defPriorityFixed) == (Priority(TN_01)) ) &&  ( (defPriorityFixed) == (Priority(TN_02)) ) && ( (defPriorityFixed) == (Priority(TN_03)) ) && ( (defPriorityFixed) == (Priority(TN_04)) ) && ( (defPriorityFixed) == (Priority(TN_05)) ) && ( (defPriorityFixed) == (Priority(TN_06)) ) )
 #define defStackSizeFixed      ( StackSize(TN_00) )
 #define defStackSizeTest       ( ( (defStackSizeFixed) == (StackSize(TN_01)) ) &&  ( (defStackSizeFixed) == (StackSize(TN_02)) ) && ( (defStackSizeFixed) == (StackSize(TN_03)) ) && ( (defStackSizeFixed) == (StackSize(TN_04)) ) && ( (defStackSizeFixed) == (StackSize(TN_05)) ) && ( (defStackSizeFixed) == (StackSize(TN_06)) ) )
 #define defStackSizeHuge       ( ( (0xFF) < (StackSize(TN_00)) ) || ( (0xFF) < (StackSize(TN_01)) ) ||  ( (0xFF) < (StackSize(TN_02)) ) || ( (0xFF) < (StackSize(TN_03)) ) || ( (0xFF) < (StackSize(TN_04)) ) || ( (0xFF) < (StackSize(TN_05)) ) || ( (0xFF) < (StackSize(TN_06)) ) )
 #define defStackSpaceTasks     ( (StackSize(TN_00)) + (StackSize(TN_01)) +  (StackSize(TN_02)) + (StackSize(TN_03)) + (StackSize(TN_04)) + (StackSize(TN_05)) + (StackSize(TN_06)) )
 #define defTaskIncludeFixed    ( TaskInclude(TN_00) )
 #define defTaskIncludeTest     ( ( (defTaskIncludeFixed) == (TaskInclude(TN_01)) ) &&  ( (defTaskIncludeFixed) == (TaskInclude(TN_02)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_03)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_04)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_05)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_06)) ) )
 #define defCapabilitiesTest    ( ( (cfgCapAll) == (Capabilities(TN_00)) ) && ( (cfgCapAll) == (Capabilities(TN_01)) ) &&  ( (cfgCapAll) == (Capabilities(TN_02)) ) && ( (cfgCapAll) == (Capabilities(TN_03)) ) && ( (cfgCapAll) == (Capabilities(TN_04)) ) && ( (cfgCapAll) == (Capabilities(TN_05)) ) && ( (cfgCapAll) == (Capabilities(TN_06)) ) )
 #define defInterruptStartFixed ( InterruptStart(TN_00) )
 #define defInterruptStartTest  ( ( (defInterruptStartFixed) == (InterruptStart(TN_01)) ) &&  ( (defInterruptStartFixed) == (InterruptStart(TN_02)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_03)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_04)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_05)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_06)) ) )
 #define defTimeSliceFixed      ( TimeSlice(TN_00) )
 #define defTimeSliceTest       ( ( (defTimeSliceFixed) == (TimeSlice(TN_01)) ) &&  ( (defTimeSliceFixed) == (TimeSlice(TN_02)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_03)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_04)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_05)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_06)) ) )
#elif defNumberOfTasks == 8
 #define defRegisterUseCollect  ( (RegisterUse(TN_00)) | (RegisterUse(TN_01)) | (RegisterUse(TN_02)) | (RegisterUse(TN_03)) | (RegisterUse(TN_04)) | (RegisterUse(TN_05)) | (RegisterUse(TN_06)) | (RegisterUse(TN_07)) )
 #define defRegisterUseFixed    ( (RegisterUse(TN_00)) & (RegisterUse(TN_01)) & (RegisterUse(TN_02)) & (RegisterUse(TN_03)) & (RegisterUse(TN_04)) & (RegisterUse(TN_05)) & (RegisterUse(TN_06)) & (RegisterUse(TN_07)) )
 #define defRegisterUseTest     ( ( (defRegisterUseCollect) == (RegisterUse(TN_00)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_01)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_02)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_03)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_04)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_05)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_06)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_07)) ) )
 #define defPriorityFixed       ( Priority(TN_00) )
 #define defPriorityTest        ( ( (defPriorityFixed) == (Priority(TN_01)) ) &&  ( (defPriorityFixed) == (Priority(TN_02)) ) && ( (defPriorityFixed) == (Priority(TN_03)) ) && ( (defPriorityFixed) == (Priority(TN_04)) ) && ( (defPriorityFixed) == (Priority(TN_05)) ) && ( (defPriorityFixed) == (Priority(TN_06)) ) && ( (defPriorityFixed) == (Priority(TN_07)) ) )
 #define defStackSizeFixed      ( StackSize(TN_00) )
 #define defStackSizeTest       ( ( (defStackSizeFixed) == (StackSize(TN_01)) ) &&  ( (defStackSizeFixed) == (StackSize(TN_02)) ) && ( (defStackSizeFixed) == (StackSize(TN_03)) ) && ( (defStackSizeFixed) == (StackSize(TN_04)) ) && ( (defStackSizeFixed) == (StackSize(TN_05)) ) && ( (defStackSizeFixed) == (StackSize(TN_06)) ) && ( (defStackSizeFixed) == (StackSize(TN_07)) ) )
 #define defStackSizeHuge       ( ( (0xFF) < (StackSize(TN_00)) ) || ( (0xFF) < (StackSize(TN_01)) ) ||  ( (0xFF) < (StackSize(TN_02)) ) || ( (0xFF) < (StackSize(TN_03)) ) || ( (0xFF) < (StackSize(TN_04)) ) || ( (0xFF) < (StackSize(TN_05)) ) || ( (0xFF) < (StackSize(TN_06)) ) || ( (0xFF) < (StackSize(TN_07)) ) )
 #define defStackSpaceTasks     ( (StackSize(TN_00)) + (StackSize(TN_01)) +  (StackSize(TN_02)) + (StackSize(TN_03)) + (StackSize(TN_04)) + (StackSize(TN_05)) + (StackSize(TN_06)) + (StackSize(TN_07)) )
 #define defTaskIncludeFixed    ( TaskInclude(TN_00) )
 #define defTaskIncludeTest     ( ( (defTaskIncludeFixed) == (TaskInclude(TN_01)) ) &&  ( (defTaskIncludeFixed) == (TaskInclude(TN_02)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_03)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_04)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_05)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_06)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_07)) ) )
 #define defCapabilitiesTest    ( ( (cfgCapAll) == (Capabilities(TN_00)) ) && ( (cfgCapAll) == (Capabilities(TN_01)) ) &&  ( (cfgCapAll) == (Capabilities(TN_02)) ) && ( (cfgCapAll) == (Capabilities(TN_03)) ) && ( (cfgCapAll) == (Capabilities(TN_04)) ) && ( (cfgCapAll) == (Capabilities(TN_05)) ) && ( (cfgCapAll) == (Capabilities(TN_06)) ) && ( (cfgCapAll) == (Capabilities(TN_07)) ) )
 #define defInterruptStartFixed ( InterruptStart(TN_00) )
 #define defInterruptStartTest  ( ( (defInterruptStartFixed) == (InterruptStart(TN_01)) ) &&  ( (defInterruptStartFixed) == (InterruptStart(TN_02)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_03)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_04)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_05)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_06)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_07)) ) )
 #define defTimeSliceFixed      ( TimeSlice(TN_00) )
 #define defTimeSliceTest       ( ( (defTimeSliceFixed) == (TimeSlice(TN_01)) ) &&  ( (defTimeSliceFixed) == (TimeSlice(TN_02)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_03)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_04)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_05)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_06)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_07)) ) )
#elif defNumberOfTasks == 9
 #define defRegisterUseCollect  ( (RegisterUse(TN_00)) | (RegisterUse(TN_01)) | (RegisterUse(TN_02)) | (RegisterUse(TN_03)) | (RegisterUse(TN_04)) | (RegisterUse(TN_05)) | (RegisterUse(TN_06)) | (RegisterUse(TN_07)) | (RegisterUse(TN_08)) )
 #define defRegisterUseFixed    ( (RegisterUse(TN_00)) & (RegisterUse(TN_01)) & (RegisterUse(TN_02)) & (RegisterUse(TN_03)) & (RegisterUse(TN_04)) & (RegisterUse(TN_05)) & (RegisterUse(TN_06)) & (RegisterUse(TN_07)) & (RegisterUse(TN_08)) )
 #define defRegisterUseTest     ( ( (defRegisterUseCollect) == (RegisterUse(TN_00)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_01)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_02)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_03)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_04)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_05)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_06)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_07)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_08)) ) )
 #define defPriorityFixed       ( Priority(TN_00) )
 #define defPriorityTest        ( ( (defPriorityFixed) == (Priority(TN_01)) ) &&  ( (defPriorityFixed) == (Priority(TN_02)) ) && ( (defPriorityFixed) == (Priority(TN_03)) ) && ( (defPriorityFixed) == (Priority(TN_04)) ) && ( (defPriorityFixed) == (Priority(TN_05)) ) && ( (defPriorityFixed) == (Priority(TN_06)) ) && ( (defPriorityFixed) == (Priority(TN_07)) ) && ( (defPriorityFixed) == (Priority(TN_08)) ) )
 #define defStackSizeFixed      ( StackSize(TN_00) )
 #define defStackSizeTest       ( ( (defStackSizeFixed) == (StackSize(TN_01)) ) &&  ( (defStackSizeFixed) == (StackSize(TN_02)) ) && ( (defStackSizeFixed) == (StackSize(TN_03)) ) && ( (defStackSizeFixed) == (StackSize(TN_04)) ) && ( (defStackSizeFixed) == (StackSize(TN_05)) ) && ( (defStackSizeFixed) == (StackSize(TN_06)) ) && ( (defStackSizeFixed) == (StackSize(TN_07)) ) && ( (defStackSizeFixed) == (StackSize(TN_08)) ) )
 #define defStackSizeHuge       ( ( (0xFF) < (StackSize(TN_00)) ) || ( (0xFF) < (StackSize(TN_01)) ) ||  ( (0xFF) < (StackSize(TN_02)) ) || ( (0xFF) < (StackSize(TN_03)) ) || ( (0xFF) < (StackSize(TN_04)) ) || ( (0xFF) < (StackSize(TN_05)) ) || ( (0xFF) < (StackSize(TN_06)) ) || ( (0xFF) < (StackSize(TN_07)) ) || ( (0xFF) < (StackSize(TN_08)) ) )
 #define defStackSpaceTasks     ( (StackSize(TN_00)) + (StackSize(TN_01)) +  (StackSize(TN_02)) + (StackSize(TN_03)) + (StackSize(TN_04)) + (StackSize(TN_05)) + (StackSize(TN_06)) + (StackSize(TN_07)) + (StackSize(TN_08)) )
 #define defTaskIncludeFixed    ( TaskInclude(TN_00) )
 #define defTaskIncludeTest     ( ( (defTaskIncludeFixed) == (TaskInclude(TN_01)) ) &&  ( (defTaskIncludeFixed) == (TaskInclude(TN_02)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_03)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_04)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_05)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_06)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_07)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_08)) ) )
 #define defCapabilitiesTest    ( ( (cfgCapAll) == (Capabilities(TN_00)) ) && ( (cfgCapAll) == (Capabilities(TN_01)) ) &&  ( (cfgCapAll) == (Capabilities(TN_02)) ) && ( (cfgCapAll) == (Capabilities(TN_03)) ) && ( (cfgCapAll) == (Capabilities(TN_04)) ) && ( (cfgCapAll) == (Capabilities(TN_05)) ) && ( (cfgCapAll) == (Capabilities(TN_06)) ) && ( (cfgCapAll) == (Capabilities(TN_07)) ) && ( (cfgCapAll) == (Capabilities(TN_08)) ) )
 #define defInterruptStartFixed ( InterruptStart(TN_00) )
 #define defInterruptStartTest  ( ( (defInterruptStartFixed) == (InterruptStart(TN_01)) ) &&  ( (defInterruptStartFixed) == (InterruptStart(TN_02)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_03)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_04)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_05)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_06)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_07)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_08)) ) )
 #define defTimeSliceFixed      ( TimeSlice(TN_00) )
 #define defTimeSliceTest       ( ( (defTimeSliceFixed) == (TimeSlice(TN_01)) ) &&  ( (defTimeSliceFixed) == (TimeSlice(TN_02)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_03)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_04)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_05)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_06)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_07)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_08)) ) )
#elif defNumberOfTasks == 10
 #define defRegisterUseCollect  ( (RegisterUse(TN_00)) | (RegisterUse(TN_01)) | (RegisterUse(TN_02)) | (RegisterUse(TN_03)) | (RegisterUse(TN_04)) | (RegisterUse(TN_05)) | (RegisterUse(TN_06)) | (RegisterUse(TN_07)) | (RegisterUse(TN_08)) | (RegisterUse(TN_09)) )
 #define defRegisterUseFixed    ( (RegisterUse(TN_00)) & (RegisterUse(TN_01)) & (RegisterUse(TN_02)) & (RegisterUse(TN_03)) & (RegisterUse(TN_04)) & (RegisterUse(TN_05)) & (RegisterUse(TN_06)) & (RegisterUse(TN_07)) & (RegisterUse(TN_08)) & (RegisterUse(TN_09)) )
 #define defRegisterUseTest     ( ( (defRegisterUseCollect) == (RegisterUse(TN_00)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_01)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_02)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_03)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_04)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_05)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_06)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_07)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_08)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_09)) ) )
 #define defPriorityFixed       ( Priority(TN_00) )
 #define defPriorityTest        ( ( (defPriorityFixed) == (Priority(TN_01)) ) &&  ( (defPriorityFixed) == (Priority(TN_02)) ) && ( (defPriorityFixed) == (Priority(TN_03)) ) && ( (defPriorityFixed) == (Priority(TN_04)) ) && ( (defPriorityFixed) == (Priority(TN_05)) ) && ( (defPriorityFixed) == (Priority(TN_06)) ) && ( (defPriorityFixed) == (Priority(TN_07)) ) && ( (defPriorityFixed) == (Priority(TN_08)) ) && ( (defPriorityFixed) == (Priority(TN_09)) ) )
 #define defStackSizeFixed      ( StackSize(TN_00) )
 #define defStackSizeTest       ( ( (defStackSizeFixed) == (StackSize(TN_01)) ) &&  ( (defStackSizeFixed) == (StackSize(TN_02)) ) && ( (defStackSizeFixed) == (StackSize(TN_03)) ) && ( (defStackSizeFixed) == (StackSize(TN_04)) ) && ( (defStackSizeFixed) == (StackSize(TN_05)) ) && ( (defStackSizeFixed) == (StackSize(TN_06)) ) && ( (defStackSizeFixed) == (StackSize(TN_07)) ) && ( (defStackSizeFixed) == (StackSize(TN_08)) ) && ( (defStackSizeFixed) == (StackSize(TN_09)) ) )
 #define defStackSizeHuge       ( ( (0xFF) < (StackSize(TN_00)) ) || ( (0xFF) < (StackSize(TN_01)) ) ||  ( (0xFF) < (StackSize(TN_02)) ) || ( (0xFF) < (StackSize(TN_03)) ) || ( (0xFF) < (StackSize(TN_04)) ) || ( (0xFF) < (StackSize(TN_05)) ) || ( (0xFF) < (StackSize(TN_06)) ) || ( (0xFF) < (StackSize(TN_07)) ) || ( (0xFF) < (StackSize(TN_08)) ) || ( (0xFF) < (StackSize(TN_09)) ) )
 #define defStackSpaceTasks     ( (StackSize(TN_00)) + (StackSize(TN_01)) +  (StackSize(TN_02)) + (StackSize(TN_03)) + (StackSize(TN_04)) + (StackSize(TN_05)) + (StackSize(TN_06)) + (StackSize(TN_07)) + (StackSize(TN_08)) + (StackSize(TN_09)) )
 #define defTaskIncludeFixed    ( TaskInclude(TN_00) )
 #define defTaskIncludeTest     ( ( (defTaskIncludeFixed) == (TaskInclude(TN_01)) ) &&  ( (defTaskIncludeFixed) == (TaskInclude(TN_02)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_03)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_04)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_05)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_06)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_07)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_08)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_09)) ) )
 #define defCapabilitiesTest    ( ( (cfgCapAll) == (Capabilities(TN_00)) ) && ( (cfgCapAll) == (Capabilities(TN_01)) ) &&  ( (cfgCapAll) == (Capabilities(TN_02)) ) && ( (cfgCapAll) == (Capabilities(TN_03)) ) && ( (cfgCapAll) == (Capabilities(TN_04)) ) && ( (cfgCapAll) == (Capabilities(TN_05)) ) && ( (cfgCapAll) == (Capabilities(TN_06)) ) && ( (cfgCapAll) == (Capabilities(TN_07)) ) && ( (cfgCapAll) == (Capabilities(TN_08)) ) && ( (cfgCapAll) == (Capabilities(TN_09)) ) )
 #define defInterruptStartFixed ( InterruptStart(TN_00) )
 #define defInterruptStartTest  ( ( (defInterruptStartFixed) == (InterruptStart(TN_01)) ) &&  ( (defInterruptStartFixed) == (InterruptStart(TN_02)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_03)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_04)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_05)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_06)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_07)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_08)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_09)) ) )
 #define defTimeSliceFixed      ( TimeSlice(TN_00) )
 #define defTimeSliceTest       ( ( (defTimeSliceFixed) == (TimeSlice(TN_01)) ) &&  ( (defTimeSliceFixed) == (TimeSlice(TN_02)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_03)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_04)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_05)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_06)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_07)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_08)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_09)) ) )
#elif defNumberOfTasks == 11
 #define defRegisterUseCollect  ( (RegisterUse(TN_00)) | (RegisterUse(TN_01)) | (RegisterUse(TN_02)) | (RegisterUse(TN_03)) | (RegisterUse(TN_04)) | (RegisterUse(TN_05)) | (RegisterUse(TN_06)) | (RegisterUse(TN_07)) | (RegisterUse(TN_08)) | (RegisterUse(TN_09)) | (RegisterUse(TN_10)) )
 #define defRegisterUseFixed    ( (RegisterUse(TN_00)) & (RegisterUse(TN_01)) & (RegisterUse(TN_02)) & (RegisterUse(TN_03)) & (RegisterUse(TN_04)) & (RegisterUse(TN_05)) & (RegisterUse(TN_06)) & (RegisterUse(TN_07)) & (RegisterUse(TN_08)) & (RegisterUse(TN_09)) & (RegisterUse(TN_10)) )
 #define defRegisterUseTest     ( ( (defRegisterUseCollect) == (RegisterUse(TN_00)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_01)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_02)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_03)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_04)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_05)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_06)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_07)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_08)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_09)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_10)) ) )
 #define defPriorityFixed       ( Priority(TN_00) )
 #define defPriorityTest        ( ( (defPriorityFixed) == (Priority(TN_01)) ) &&  ( (defPriorityFixed) == (Priority(TN_02)) ) && ( (defPriorityFixed) == (Priority(TN_03)) ) && ( (defPriorityFixed) == (Priority(TN_04)) ) && ( (defPriorityFixed) == (Priority(TN_05)) ) && ( (defPriorityFixed) == (Priority(TN_06)) ) && ( (defPriorityFixed) == (Priority(TN_07)) ) && ( (defPriorityFixed) == (Priority(TN_08)) ) && ( (defPriorityFixed) == (Priority(TN_09)) ) && ( (defPriorityFixed) == (Priority(TN_10)) ) )
 #define defStackSizeFixed      ( StackSize(TN_00) )
 #define defStackSizeTest       ( ( (defStackSizeFixed) == (StackSize(TN_01)) ) &&  ( (defStackSizeFixed) == (StackSize(TN_02)) ) && ( (defStackSizeFixed) == (StackSize(TN_03)) ) && ( (defStackSizeFixed) == (StackSize(TN_04)) ) && ( (defStackSizeFixed) == (StackSize(TN_05)) ) && ( (defStackSizeFixed) == (StackSize(TN_06)) ) && ( (defStackSizeFixed) == (StackSize(TN_07)) ) && ( (defStackSizeFixed) == (StackSize(TN_08)) ) && ( (defStackSizeFixed) == (StackSize(TN_09)) ) && ( (defStackSizeFixed) == (StackSize(TN_10)) ) )
 #define defStackSizeHuge       ( ( (0xFF) < (StackSize(TN_00)) ) || ( (0xFF) < (StackSize(TN_01)) ) ||  ( (0xFF) < (StackSize(TN_02)) ) || ( (0xFF) < (StackSize(TN_03)) ) || ( (0xFF) < (StackSize(TN_04)) ) || ( (0xFF) < (StackSize(TN_05)) ) || ( (0xFF) < (StackSize(TN_06)) ) || ( (0xFF) < (StackSize(TN_07)) ) || ( (0xFF) < (StackSize(TN_08)) ) || ( (0xFF) < (StackSize(TN_09)) ) || ( (0xFF) < (StackSize(TN_10)) ) )
 #define defStackSpaceTasks     ( (StackSize(TN_00)) + (StackSize(TN_01)) +  (StackSize(TN_02)) + (StackSize(TN_03)) + (StackSize(TN_04)) + (StackSize(TN_05)) + (StackSize(TN_06)) + (StackSize(TN_07)) + (StackSize(TN_08)) + (StackSize(TN_09)) + (StackSize(TN_10)) )
 #define defTaskIncludeFixed    ( TaskInclude(TN_00) )
 #define defTaskIncludeTest     ( ( (defTaskIncludeFixed) == (TaskInclude(TN_01)) ) &&  ( (defTaskIncludeFixed) == (TaskInclude(TN_02)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_03)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_04)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_05)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_06)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_07)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_08)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_09)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_10)) ) )
 #define defCapabilitiesTest    ( ( (cfgCapAll) == (Capabilities(TN_00)) ) && ( (cfgCapAll) == (Capabilities(TN_01)) ) &&  ( (cfgCapAll) == (Capabilities(TN_02)) ) && ( (cfgCapAll) == (Capabilities(TN_03)) ) && ( (cfgCapAll) == (Capabilities(TN_04)) ) && ( (cfgCapAll) == (Capabilities(TN_05)) ) && ( (cfgCapAll) == (Capabilities(TN_06)) ) && ( (cfgCapAll) == (Capabilities(TN_07)) ) && ( (cfgCapAll) == (Capabilities(TN_08)) ) && ( (cfgCapAll) == (Capabilities(TN_09)) ) && ( (cfgCapAll) == (Capabilities(TN_10)) ) )
 #define defInterruptStartFixed ( InterruptStart(TN_00) )
 #define defInterruptStartTest  ( ( (defInterruptStartFixed) == (InterruptStart(TN_01)) ) &&  ( (defInterruptStartFixed) == (InterruptStart(TN_02)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_03)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_04)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_05)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_06)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_07)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_08)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_09)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_10)) ) )
 #define defTimeSliceFixed      ( TimeSlice(TN_00) )
 #define defTimeSliceTest       ( ( (defTimeSliceFixed) == (TimeSlice(TN_01)) ) &&  ( (defTimeSliceFixed) == (TimeSlice(TN_02)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_03)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_04)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_05)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_06)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_07)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_08)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_09)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_10)) ) )
#elif defNumberOfTasks == 12
 #define defRegisterUseCollect  ( (RegisterUse(TN_00)) | (RegisterUse(TN_01)) | (RegisterUse(TN_02)) | (RegisterUse(TN_03)) | (RegisterUse(TN_04)) | (RegisterUse(TN_05)) | (RegisterUse(TN_06)) | (RegisterUse(TN_07)) | (RegisterUse(TN_08)) | (RegisterUse(TN_09)) | (RegisterUse(TN_10)) | (RegisterUse(TN_11)) )
 #define defRegisterUseFixed    ( (RegisterUse(TN_00)) & (RegisterUse(TN_01)) & (RegisterUse(TN_02)) & (RegisterUse(TN_03)) & (RegisterUse(TN_04)) & (RegisterUse(TN_05)) & (RegisterUse(TN_06)) & (RegisterUse(TN_07)) & (RegisterUse(TN_08)) & (RegisterUse(TN_09)) & (RegisterUse(TN_10)) & (RegisterUse(TN_11)) )
 #define defRegisterUseTest     ( ( (defRegisterUseCollect) == (RegisterUse(TN_00)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_01)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_02)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_03)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_04)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_05)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_06)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_07)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_08)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_09)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_10)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_11)) ) )
 #define defPriorityFixed       ( Priority(TN_00) )
 #define defPriorityTest        ( ( (defPriorityFixed) == (Priority(TN_01)) ) &&  ( (defPriorityFixed) == (Priority(TN_02)) ) && ( (defPriorityFixed) == (Priority(TN_03)) ) && ( (defPriorityFixed) == (Priority(TN_04)) ) && ( (defPriorityFixed) == (Priority(TN_05)) ) && ( (defPriorityFixed) == (Priority(TN_06)) ) && ( (defPriorityFixed) == (Priority(TN_07)) ) && ( (defPriorityFixed) == (Priority(TN_08)) ) && ( (defPriorityFixed) == (Priority(TN_09)) ) && ( (defPriorityFixed) == (Priority(TN_10)) ) && ( (defPriorityFixed) == (Priority(TN_11)) ) )
 #define defStackSizeFixed      ( StackSize(TN_00) )
 #define defStackSizeTest       ( ( (defStackSizeFixed) == (StackSize(TN_01)) ) &&  ( (defStackSizeFixed) == (StackSize(TN_02)) ) && ( (defStackSizeFixed) == (StackSize(TN_03)) ) && ( (defStackSizeFixed) == (StackSize(TN_04)) ) && ( (defStackSizeFixed) == (StackSize(TN_05)) ) && ( (defStackSizeFixed) == (StackSize(TN_06)) ) && ( (defStackSizeFixed) == (StackSize(TN_07)) ) && ( (defStackSizeFixed) == (StackSize(TN_08)) ) && ( (defStackSizeFixed) == (StackSize(TN_09)) ) && ( (defStackSizeFixed) == (StackSize(TN_10)) ) && ( (defStackSizeFixed) == (StackSize(TN_11)) ) )
 #define defStackSizeHuge       ( ( (0xFF) < (StackSize(TN_00)) ) || ( (0xFF) < (StackSize(TN_01)) ) ||  ( (0xFF) < (StackSize(TN_02)) ) || ( (0xFF) < (StackSize(TN_03)) ) || ( (0xFF) < (StackSize(TN_04)) ) || ( (0xFF) < (StackSize(TN_05)) ) || ( (0xFF) < (StackSize(TN_06)) ) || ( (0xFF) < (StackSize(TN_07)) ) || ( (0xFF) < (StackSize(TN_08)) ) || ( (0xFF) < (StackSize(TN_09)) ) || ( (0xFF) < (StackSize(TN_10)) ) || ( (0xFF) < (StackSize(TN_11)) ) )
 #define defStackSpaceTasks     ( (StackSize(TN_00)) + (StackSize(TN_01)) +  (StackSize(TN_02)) + (StackSize(TN_03)) + (StackSize(TN_04)) + (StackSize(TN_05)) + (StackSize(TN_06)) + (StackSize(TN_07)) + (StackSize(TN_08)) + (StackSize(TN_09)) + (StackSize(TN_10)) + (StackSize(TN_11)) )
 #define defTaskIncludeFixed    ( TaskInclude(TN_00) )
 #define defTaskIncludeTest     ( ( (defTaskIncludeFixed) == (TaskInclude(TN_01)) ) &&  ( (defTaskIncludeFixed) == (TaskInclude(TN_02)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_03)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_04)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_05)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_06)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_07)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_08)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_09)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_10)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_11)) ) )
 #define defCapabilitiesTest    ( ( (cfgCapAll) == (Capabilities(TN_00)) ) && ( (cfgCapAll) == (Capabilities(TN_01)) ) &&  ( (cfgCapAll) == (Capabilities(TN_02)) ) && ( (cfgCapAll) == (Capabilities(TN_03)) ) && ( (cfgCapAll) == (Capabilities(TN_04)) ) && ( (cfgCapAll) == (Capabilities(TN_05)) ) && ( (cfgCapAll) == (Capabilities(TN_06)) ) && ( (cfgCapAll) == (Capabilities(TN_07)) ) && ( (cfgCapAll) == (Capabilities(TN_08)) ) && ( (cfgCapAll) == (Capabilities(TN_09)) ) && ( (cfgCapAll) == (Capabilities(TN_10)) ) && ( (cfgCapAll) == (Capabilities(TN_11)) ) )
 #define defInterruptStartFixed ( InterruptStart(TN_00) )
 #define defInterruptStartTest  ( ( (defInterruptStartFixed) == (InterruptStart(TN_01)) ) &&  ( (defInterruptStartFixed) == (InterruptStart(TN_02)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_03)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_04)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_05)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_06)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_07)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_08)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_09)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_10)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_11)) ) )
 #define defTimeSliceFixed      ( TimeSlice(TN_00) )
 #define defTimeSliceTest       ( ( (defTimeSliceFixed) == (TimeSlice(TN_01)) ) &&  ( (defTimeSliceFixed) == (TimeSlice(TN_02)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_03)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_04)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_05)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_06)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_07)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_08)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_09)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_10)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_11)) ) )
#elif defNumberOfTasks == 13
 #define defRegisterUseCollect  ( (RegisterUse(TN_00)) | (RegisterUse(TN_01)) | (RegisterUse(TN_02)) | (RegisterUse(TN_03)) | (RegisterUse(TN_04)) | (RegisterUse(TN_05)) | (RegisterUse(TN_06)) | (RegisterUse(TN_07)) | (RegisterUse(TN_08)) | (RegisterUse(TN_09)) | (RegisterUse(TN_10)) | (RegisterUse(TN_11)) | (RegisterUse(TN_12)) )
 #define defRegisterUseFixed    ( (RegisterUse(TN_00)) & (RegisterUse(TN_01)) & (RegisterUse(TN_02)) & (RegisterUse(TN_03)) & (RegisterUse(TN_04)) & (RegisterUse(TN_05)) & (RegisterUse(TN_06)) & (RegisterUse(TN_07)) & (RegisterUse(TN_08)) & (RegisterUse(TN_09)) & (RegisterUse(TN_10)) & (RegisterUse(TN_11)) & (RegisterUse(TN_12)) )
 #define defRegisterUseTest     ( ( (defRegisterUseCollect) == (RegisterUse(TN_00)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_01)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_02)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_03)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_04)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_05)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_06)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_07)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_08)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_09)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_10)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_11)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_12)) ) )
 #define defPriorityFixed       ( Priority(TN_00) )
 #define defPriorityTest        ( ( (defPriorityFixed) == (Priority(TN_01)) ) &&  ( (defPriorityFixed) == (Priority(TN_02)) ) && ( (defPriorityFixed) == (Priority(TN_03)) ) && ( (defPriorityFixed) == (Priority(TN_04)) ) && ( (defPriorityFixed) == (Priority(TN_05)) ) && ( (defPriorityFixed) == (Priority(TN_06)) ) && ( (defPriorityFixed) == (Priority(TN_07)) ) && ( (defPriorityFixed) == (Priority(TN_08)) ) && ( (defPriorityFixed) == (Priority(TN_09)) ) && ( (defPriorityFixed) == (Priority(TN_10)) ) && ( (defPriorityFixed) == (Priority(TN_11)) ) && ( (defPriorityFixed) == (Priority(TN_12)) ) )
 #define defStackSizeFixed      ( StackSize(TN_00) )
 #define defStackSizeTest       ( ( (defStackSizeFixed) == (StackSize(TN_01)) ) &&  ( (defStackSizeFixed) == (StackSize(TN_02)) ) && ( (defStackSizeFixed) == (StackSize(TN_03)) ) && ( (defStackSizeFixed) == (StackSize(TN_04)) ) && ( (defStackSizeFixed) == (StackSize(TN_05)) ) && ( (defStackSizeFixed) == (StackSize(TN_06)) ) && ( (defStackSizeFixed) == (StackSize(TN_07)) ) && ( (defStackSizeFixed) == (StackSize(TN_08)) ) && ( (defStackSizeFixed) == (StackSize(TN_09)) ) && ( (defStackSizeFixed) == (StackSize(TN_10)) ) && ( (defStackSizeFixed) == (StackSize(TN_11)) ) && ( (defStackSizeFixed) == (StackSize(TN_12)) ) )
 #define defStackSizeHuge       ( ( (0xFF) < (StackSize(TN_00)) ) || ( (0xFF) < (StackSize(TN_01)) ) ||  ( (0xFF) < (StackSize(TN_02)) ) || ( (0xFF) < (StackSize(TN_03)) ) || ( (0xFF) < (StackSize(TN_04)) ) || ( (0xFF) < (StackSize(TN_05)) ) || ( (0xFF) < (StackSize(TN_06)) ) || ( (0xFF) < (StackSize(TN_07)) ) || ( (0xFF) < (StackSize(TN_08)) ) || ( (0xFF) < (StackSize(TN_09)) ) || ( (0xFF) < (StackSize(TN_10)) ) || ( (0xFF) < (StackSize(TN_11)) ) || ( (0xFF) < (StackSize(TN_12)) ) )
 #define defStackSpaceTasks     ( (StackSize(TN_00)) + (StackSize(TN_01)) +  (StackSize(TN_02)) + (StackSize(TN_03)) + (StackSize(TN_04)) + (StackSize(TN_05)) + (StackSize(TN_06)) + (StackSize(TN_07)) + (StackSize(TN_08)) + (StackSize(TN_09)) + (StackSize(TN_10)) + (StackSize(TN_11)) + (StackSize(TN_12)) )
 #define defTaskIncludeFixed    ( TaskInclude(TN_00) )
 #define defTaskIncludeTest     ( ( (defTaskIncludeFixed) == (TaskInclude(TN_01)) ) &&  ( (defTaskIncludeFixed) == (TaskInclude(TN_02)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_03)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_04)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_05)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_06)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_07)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_08)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_09)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_10)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_11)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_12)) ) )
 #define defCapabilitiesTest    ( ( (cfgCapAll) == (Capabilities(TN_00)) ) && ( (cfgCapAll) == (Capabilities(TN_01)) ) &&  ( (cfgCapAll) == (Capabilities(TN_02)) ) && ( (cfgCapAll) == (Capabilities(TN_03)) ) && ( (cfgCapAll) == (Capabilities(TN_04)) ) && ( (cfgCapAll) == (Capabilities(TN_05)) ) && ( (cfgCapAll) == (Capabilities(TN_06)) ) && ( (cfgCapAll) == (Capabilities(TN_07)) ) && ( (cfgCapAll) == (Capabilities(TN_08)) ) && ( (cfgCapAll) == (Capabilities(TN_09)) ) && ( (cfgCapAll) == (Capabilities(TN_10)) ) && ( (cfgCapAll) == (Capabilities(TN_11)) ) && ( (cfgCapAll) == (Capabilities(TN_12)) ) )
 #define defInterruptStartFixed ( InterruptStart(TN_00) )
 #define defInterruptStartTest  ( ( (defInterruptStartFixed) == (InterruptStart(TN_01)) ) &&  ( (defInterruptStartFixed) == (InterruptStart(TN_02)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_03)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_04)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_05)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_06)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_07)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_08)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_09)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_10)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_11)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_12)) ) )
 #define defTimeSliceFixed      ( TimeSlice(TN_00) )
 #define defTimeSliceTest       ( ( (defTimeSliceFixed) == (TimeSlice(TN_01)) ) &&  ( (defTimeSliceFixed) == (TimeSlice(TN_02)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_03)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_04)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_05)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_06)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_07)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_08)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_09)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_10)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_11)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_12)) ) )
#elif defNumberOfTasks == 14
 #define defRegisterUseCollect  ( (RegisterUse(TN_00)) | (RegisterUse(TN_01)) | (RegisterUse(TN_02)) | (RegisterUse(TN_03)) | (RegisterUse(TN_04)) | (RegisterUse(TN_05)) | (RegisterUse(TN_06)) | (RegisterUse(TN_07)) | (RegisterUse(TN_08)) | (RegisterUse(TN_09)) | (RegisterUse(TN_10)) | (RegisterUse(TN_11)) | (RegisterUse(TN_12)) | (RegisterUse(TN_13)) )
 #define defRegisterUseFixed    ( (RegisterUse(TN_00)) & (RegisterUse(TN_01)) & (RegisterUse(TN_02)) & (RegisterUse(TN_03)) & (RegisterUse(TN_04)) & (RegisterUse(TN_05)) & (RegisterUse(TN_06)) & (RegisterUse(TN_07)) & (RegisterUse(TN_08)) & (RegisterUse(TN_09)) & (RegisterUse(TN_10)) & (RegisterUse(TN_11)) & (RegisterUse(TN_12)) & (RegisterUse(TN_13)) )
 #define defRegisterUseTest     ( ( (defRegisterUseCollect) == (RegisterUse(TN_00)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_01)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_02)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_03)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_04)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_05)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_06)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_07)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_08)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_09)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_10)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_11)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_12)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_13)) ) )
 #define defPriorityFixed       ( Priority(TN_00) )
 #define defPriorityTest        ( ( (defPriorityFixed) == (Priority(TN_01)) ) &&  ( (defPriorityFixed) == (Priority(TN_02)) ) && ( (defPriorityFixed) == (Priority(TN_03)) ) && ( (defPriorityFixed) == (Priority(TN_04)) ) && ( (defPriorityFixed) == (Priority(TN_05)) ) && ( (defPriorityFixed) == (Priority(TN_06)) ) && ( (defPriorityFixed) == (Priority(TN_07)) ) && ( (defPriorityFixed) == (Priority(TN_08)) ) && ( (defPriorityFixed) == (Priority(TN_09)) ) && ( (defPriorityFixed) == (Priority(TN_10)) ) && ( (defPriorityFixed) == (Priority(TN_11)) ) && ( (defPriorityFixed) == (Priority(TN_12)) ) && ( (defPriorityFixed) == (Priority(TN_13)) ) )
 #define defStackSizeFixed      ( StackSize(TN_00) )
 #define defStackSizeTest       ( ( (defStackSizeFixed) == (StackSize(TN_01)) ) &&  ( (defStackSizeFixed) == (StackSize(TN_02)) ) && ( (defStackSizeFixed) == (StackSize(TN_03)) ) && ( (defStackSizeFixed) == (StackSize(TN_04)) ) && ( (defStackSizeFixed) == (StackSize(TN_05)) ) && ( (defStackSizeFixed) == (StackSize(TN_06)) ) && ( (defStackSizeFixed) == (StackSize(TN_07)) ) && ( (defStackSizeFixed) == (StackSize(TN_08)) ) && ( (defStackSizeFixed) == (StackSize(TN_09)) ) && ( (defStackSizeFixed) == (StackSize(TN_10)) ) && ( (defStackSizeFixed) == (StackSize(TN_11)) ) && ( (defStackSizeFixed) == (StackSize(TN_12)) ) && ( (defStackSizeFixed) == (StackSize(TN_13)) ) )
 #define defStackSizeHuge       ( ( (0xFF) < (StackSize(TN_00)) ) || ( (0xFF) < (StackSize(TN_01)) ) ||  ( (0xFF) < (StackSize(TN_02)) ) || ( (0xFF) < (StackSize(TN_03)) ) || ( (0xFF) < (StackSize(TN_04)) ) || ( (0xFF) < (StackSize(TN_05)) ) || ( (0xFF) < (StackSize(TN_06)) ) || ( (0xFF) < (StackSize(TN_07)) ) || ( (0xFF) < (StackSize(TN_08)) ) || ( (0xFF) < (StackSize(TN_09)) ) || ( (0xFF) < (StackSize(TN_10)) ) || ( (0xFF) < (StackSize(TN_11)) ) || ( (0xFF) < (StackSize(TN_12)) ) || ( (0xFF) < (StackSize(TN_13)) ) )
 #define defStackSpaceTasks     ( (StackSize(TN_00)) + (StackSize(TN_01)) +  (StackSize(TN_02)) + (StackSize(TN_03)) + (StackSize(TN_04)) + (StackSize(TN_05)) + (StackSize(TN_06)) + (StackSize(TN_07)) + (StackSize(TN_08)) + (StackSize(TN_09)) + (StackSize(TN_10)) + (StackSize(TN_11)) + (StackSize(TN_12)) + (StackSize(TN_13)) )
 #define defTaskIncludeFixed    ( TaskInclude(TN_00) )
 #define defTaskIncludeTest     ( ( (defTaskIncludeFixed) == (TaskInclude(TN_01)) ) &&  ( (defTaskIncludeFixed) == (TaskInclude(TN_02)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_03)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_04)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_05)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_06)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_07)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_08)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_09)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_10)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_11)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_12)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_13)) ) )
 #define defCapabilitiesTest    ( ( (cfgCapAll) == (Capabilities(TN_00)) ) && ( (cfgCapAll) == (Capabilities(TN_01)) ) &&  ( (cfgCapAll) == (Capabilities(TN_02)) ) && ( (cfgCapAll) == (Capabilities(TN_03)) ) && ( (cfgCapAll) == (Capabilities(TN_04)) ) && ( (cfgCapAll) == (Capabilities(TN_05)) ) && ( (cfgCapAll) == (Capabilities(TN_06)) ) && ( (cfgCapAll) == (Capabilities(TN_07)) ) && ( (cfgCapAll) == (Capabilities(TN_08)) ) && ( (cfgCapAll) == (Capabilities(TN_09)) ) && ( (cfgCapAll) == (Capabilities(TN_10)) ) && ( (cfgCapAll) == (Capabilities(TN_11)) ) && ( (cfgCapAll) == (Capabilities(TN_12)) ) && ( (cfgCapAll) == (Capabilities(TN_13)) ) )
 #define defInterruptStartFixed ( InterruptStart(TN_00) )
 #define defInterruptStartTest  ( ( (defInterruptStartFixed) == (InterruptStart(TN_01)) ) &&  ( (defInterruptStartFixed) == (InterruptStart(TN_02)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_03)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_04)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_05)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_06)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_07)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_08)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_09)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_10)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_11)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_12)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_13)) ) )
 #define defTimeSliceFixed      ( TimeSlice(TN_00) )
 #define defTimeSliceTest       ( ( (defTimeSliceFixed) == (TimeSlice(TN_01)) ) &&  ( (defTimeSliceFixed) == (TimeSlice(TN_02)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_03)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_04)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_05)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_06)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_07)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_08)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_09)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_10)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_11)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_12)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_13)) ) )
#elif defNumberOfTasks == 15
 #define defRegisterUseCollect  ( (RegisterUse(TN_00)) | (RegisterUse(TN_01)) | (RegisterUse(TN_02)) | (RegisterUse(TN_03)) | (RegisterUse(TN_04)) | (RegisterUse(TN_05)) | (RegisterUse(TN_06)) | (RegisterUse(TN_07)) | (RegisterUse(TN_08)) | (RegisterUse(TN_09)) | (RegisterUse(TN_10)) | (RegisterUse(TN_11)) | (RegisterUse(TN_12)) | (RegisterUse(TN_13)) | (RegisterUse(TN_14)) )
 #define defRegisterUseFixed    ( (RegisterUse(TN_00)) & (RegisterUse(TN_01)) & (RegisterUse(TN_02)) & (RegisterUse(TN_03)) & (RegisterUse(TN_04)) & (RegisterUse(TN_05)) & (RegisterUse(TN_06)) & (RegisterUse(TN_07)) & (RegisterUse(TN_08)) & (RegisterUse(TN_09)) & (RegisterUse(TN_10)) & (RegisterUse(TN_11)) & (RegisterUse(TN_12)) & (RegisterUse(TN_13)) & (RegisterUse(TN_14)) )
 #define defRegisterUseTest     ( ( (defRegisterUseCollect) == (RegisterUse(TN_00)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_01)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_02)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_03)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_04)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_05)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_06)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_07)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_08)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_09)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_10)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_11)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_12)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_13)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_14)) ) )
 #define defPriorityFixed       ( Priority(TN_00) )
 #define defPriorityTest        ( ( (defPriorityFixed) == (Priority(TN_01)) ) &&  ( (defPriorityFixed) == (Priority(TN_02)) ) && ( (defPriorityFixed) == (Priority(TN_03)) ) && ( (defPriorityFixed) == (Priority(TN_04)) ) && ( (defPriorityFixed) == (Priority(TN_05)) ) && ( (defPriorityFixed) == (Priority(TN_06)) ) && ( (defPriorityFixed) == (Priority(TN_07)) ) && ( (defPriorityFixed) == (Priority(TN_08)) ) && ( (defPriorityFixed) == (Priority(TN_09)) ) && ( (defPriorityFixed) == (Priority(TN_10)) ) && ( (defPriorityFixed) == (Priority(TN_11)) ) && ( (defPriorityFixed) == (Priority(TN_12)) ) && ( (defPriorityFixed) == (Priority(TN_13)) ) && ( (defPriorityFixed) == (Priority(TN_14)) ) )
 #define defStackSizeFixed      ( StackSize(TN_00) )
 #define defStackSizeTest       ( ( (defStackSizeFixed) == (StackSize(TN_01)) ) &&  ( (defStackSizeFixed) == (StackSize(TN_02)) ) && ( (defStackSizeFixed) == (StackSize(TN_03)) ) && ( (defStackSizeFixed) == (StackSize(TN_04)) ) && ( (defStackSizeFixed) == (StackSize(TN_05)) ) && ( (defStackSizeFixed) == (StackSize(TN_06)) ) && ( (defStackSizeFixed) == (StackSize(TN_07)) ) && ( (defStackSizeFixed) == (StackSize(TN_08)) ) && ( (defStackSizeFixed) == (StackSize(TN_09)) ) && ( (defStackSizeFixed) == (StackSize(TN_10)) ) && ( (defStackSizeFixed) == (StackSize(TN_11)) ) && ( (defStackSizeFixed) == (StackSize(TN_12)) ) && ( (defStackSizeFixed) == (StackSize(TN_13)) ) && ( (defStackSizeFixed) == (StackSize(TN_14)) ) )
 #define defStackSizeHuge       ( ( (0xFF) < (StackSize(TN_00)) ) || ( (0xFF) < (StackSize(TN_01)) ) ||  ( (0xFF) < (StackSize(TN_02)) ) || ( (0xFF) < (StackSize(TN_03)) ) || ( (0xFF) < (StackSize(TN_04)) ) || ( (0xFF) < (StackSize(TN_05)) ) || ( (0xFF) < (StackSize(TN_06)) ) || ( (0xFF) < (StackSize(TN_07)) ) || ( (0xFF) < (StackSize(TN_08)) ) || ( (0xFF) < (StackSize(TN_09)) ) || ( (0xFF) < (StackSize(TN_10)) ) || ( (0xFF) < (StackSize(TN_11)) ) || ( (0xFF) < (StackSize(TN_12)) ) || ( (0xFF) < (StackSize(TN_13)) ) || ( (0xFF) < (StackSize(TN_14)) ) )
 #define defStackSpaceTasks     ( (StackSize(TN_00)) + (StackSize(TN_01)) +  (StackSize(TN_02)) + (StackSize(TN_03)) + (StackSize(TN_04)) + (StackSize(TN_05)) + (StackSize(TN_06)) + (StackSize(TN_07)) + (StackSize(TN_08)) + (StackSize(TN_09)) + (StackSize(TN_10)) + (StackSize(TN_11)) + (StackSize(TN_12)) + (StackSize(TN_13)) + (StackSize(TN_14)) )
 #define defTaskIncludeFixed    ( TaskInclude(TN_00) )
 #define defTaskIncludeTest     ( ( (defTaskIncludeFixed) == (TaskInclude(TN_01)) ) &&  ( (defTaskIncludeFixed) == (TaskInclude(TN_02)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_03)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_04)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_05)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_06)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_07)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_08)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_09)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_10)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_11)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_12)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_13)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_14)) ) )
 #define defCapabilitiesTest    ( ( (cfgCapAll) == (Capabilities(TN_00)) ) && ( (cfgCapAll) == (Capabilities(TN_01)) ) &&  ( (cfgCapAll) == (Capabilities(TN_02)) ) && ( (cfgCapAll) == (Capabilities(TN_03)) ) && ( (cfgCapAll) == (Capabilities(TN_04)) ) && ( (cfgCapAll) == (Capabilities(TN_05)) ) && ( (cfgCapAll) == (Capabilities(TN_06)) ) && ( (cfgCapAll) == (Capabilities(TN_07)) ) && ( (cfgCapAll) == (Capabilities(TN_08)) ) && ( (cfgCapAll) == (Capabilities(TN_09)) ) && ( (cfgCapAll) == (Capabilities(TN_10)) ) && ( (cfgCapAll) == (Capabilities(TN_11)) ) && ( (cfgCapAll) == (Capabilities(TN_12)) ) && ( (cfgCapAll) == (Capabilities(TN_13)) ) && ( (cfgCapAll) == (Capabilities(TN_14)) ) )
 #define defInterruptStartFixed ( InterruptStart(TN_00) )
 #define defInterruptStartTest  ( ( (defInterruptStartFixed) == (InterruptStart(TN_01)) ) &&  ( (defInterruptStartFixed) == (InterruptStart(TN_02)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_03)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_04)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_05)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_06)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_07)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_08)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_09)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_10)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_11)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_12)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_13)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_14)) ) )
 #define defTimeSliceFixed      ( TimeSlice(TN_00) )
 #define defTimeSliceTest       ( ( (defTimeSliceFixed) == (TimeSlice(TN_01)) ) &&  ( (defTimeSliceFixed) == (TimeSlice(TN_02)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_03)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_04)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_05)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_06)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_07)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_08)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_09)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_10)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_11)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_12)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_13)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_14)) ) )
#elif defNumberOfTasks == 16
 #define defRegisterUseCollect  ( (RegisterUse(TN_00)) | (RegisterUse(TN_01)) | (RegisterUse(TN_02)) | (RegisterUse(TN_03)) | (RegisterUse(TN_04)) | (RegisterUse(TN_05)) | (RegisterUse(TN_06)) | (RegisterUse(TN_07)) | (RegisterUse(TN_08)) | (RegisterUse(TN_09)) | (RegisterUse(TN_10)) | (RegisterUse(TN_11)) | (RegisterUse(TN_12)) | (RegisterUse(TN_13)) | (RegisterUse(TN_14)) | (RegisterUse(TN_15)) )
 #define defRegisterUseFixed    ( (RegisterUse(TN_00)) & (RegisterUse(TN_01)) & (RegisterUse(TN_02)) & (RegisterUse(TN_03)) & (RegisterUse(TN_04)) & (RegisterUse(TN_05)) & (RegisterUse(TN_06)) & (RegisterUse(TN_07)) & (RegisterUse(TN_08)) & (RegisterUse(TN_09)) & (RegisterUse(TN_10)) & (RegisterUse(TN_11)) & (RegisterUse(TN_12)) & (RegisterUse(TN_13)) & (RegisterUse(TN_14)) & (RegisterUse(TN_15)) )
 #define defRegisterUseTest     ( ( (defRegisterUseCollect) == (RegisterUse(TN_00)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_01)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_02)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_03)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_04)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_05)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_06)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_07)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_08)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_09)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_10)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_11)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_12)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_13)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_14)) ) && ( (defRegisterUseCollect) == (RegisterUse(TN_15)) ) )
 #define defPriorityFixed       ( Priority(TN_00) )
 #define defPriorityTest        ( ( (defPriorityFixed) == (Priority(TN_01)) ) &&  ( (defPriorityFixed) == (Priority(TN_02)) ) && ( (defPriorityFixed) == (Priority(TN_03)) ) && ( (defPriorityFixed) == (Priority(TN_04)) ) && ( (defPriorityFixed) == (Priority(TN_05)) ) && ( (defPriorityFixed) == (Priority(TN_06)) ) && ( (defPriorityFixed) == (Priority(TN_07)) ) && ( (defPriorityFixed) == (Priority(TN_08)) ) && ( (defPriorityFixed) == (Priority(TN_09)) ) && ( (defPriorityFixed) == (Priority(TN_10)) ) && ( (defPriorityFixed) == (Priority(TN_11)) ) && ( (defPriorityFixed) == (Priority(TN_12)) ) && ( (defPriorityFixed) == (Priority(TN_13)) ) && ( (defPriorityFixed) == (Priority(TN_14)) ) && ( (defPriorityFixed) == (Priority(TN_15)) ) )
 #define defStackSizeFixed      ( StackSize(TN_00) )
 #define defStackSizeTest       ( ( (defStackSizeFixed) == (StackSize(TN_01)) ) &&  ( (defStackSizeFixed) == (StackSize(TN_02)) ) && ( (defStackSizeFixed) == (StackSize(TN_03)) ) && ( (defStackSizeFixed) == (StackSize(TN_04)) ) && ( (defStackSizeFixed) == (StackSize(TN_05)) ) && ( (defStackSizeFixed) == (StackSize(TN_06)) ) && ( (defStackSizeFixed) == (StackSize(TN_07)) ) && ( (defStackSizeFixed) == (StackSize(TN_08)) ) && ( (defStackSizeFixed) == (StackSize(TN_09)) ) && ( (defStackSizeFixed) == (StackSize(TN_10)) ) && ( (defStackSizeFixed) == (StackSize(TN_11)) ) && ( (defStackSizeFixed) == (StackSize(TN_12)) ) && ( (defStackSizeFixed) == (StackSize(TN_13)) ) && ( (defStackSizeFixed) == (StackSize(TN_14)) ) && ( (defStackSizeFixed) == (StackSize(TN_15)) ) )
 #define defStackSizeHuge       ( ( (0xFF) < (StackSize(TN_00)) ) || ( (0xFF) < (StackSize(TN_01)) ) ||  ( (0xFF) < (StackSize(TN_02)) ) || ( (0xFF) < (StackSize(TN_03)) ) || ( (0xFF) < (StackSize(TN_04)) ) || ( (0xFF) < (StackSize(TN_05)) ) || ( (0xFF) < (StackSize(TN_06)) ) || ( (0xFF) < (StackSize(TN_07)) ) || ( (0xFF) < (StackSize(TN_08)) ) || ( (0xFF) < (StackSize(TN_09)) ) || ( (0xFF) < (StackSize(TN_10)) ) || ( (0xFF) < (StackSize(TN_11)) ) || ( (0xFF) < (StackSize(TN_12)) ) || ( (0xFF) < (StackSize(TN_13)) ) || ( (0xFF) < (StackSize(TN_14)) ) || ( (0xFF) < (StackSize(TN_15)) ) )
 #define defStackSpaceTasks     ( (StackSize(TN_00)) + (StackSize(TN_01)) +  (StackSize(TN_02)) + (StackSize(TN_03)) + (StackSize(TN_04)) + (StackSize(TN_05)) + (StackSize(TN_06)) + (StackSize(TN_07)) + (StackSize(TN_08)) + (StackSize(TN_09)) + (StackSize(TN_10)) + (StackSize(TN_11)) + (StackSize(TN_12)) + (StackSize(TN_13)) + (StackSize(TN_14)) + (StackSize(TN_15)) )
 #define defTaskIncludeFixed    ( TaskInclude(TN_00) )
 #define defTaskIncludeTest     ( ( (defTaskIncludeFixed) == (TaskInclude(TN_01)) ) &&  ( (defTaskIncludeFixed) == (TaskInclude(TN_02)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_03)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_04)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_05)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_06)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_07)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_08)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_09)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_10)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_11)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_12)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_13)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_14)) ) && ( (defTaskIncludeFixed) == (TaskInclude(TN_15)) ) )
 #define defCapabilitiesTest    ( ( (cfgCapAll) == (Capabilities(TN_00)) ) && ( (cfgCapAll) == (Capabilities(TN_01)) ) &&  ( (cfgCapAll) == (Capabilities(TN_02)) ) && ( (cfgCapAll) == (Capabilities(TN_03)) ) && ( (cfgCapAll) == (Capabilities(TN_04)) ) && ( (cfgCapAll) == (Capabilities(TN_05)) ) && ( (cfgCapAll) == (Capabilities(TN_06)) ) && ( (cfgCapAll) == (Capabilities(TN_07)) ) && ( (cfgCapAll) == (Capabilities(TN_08)) ) && ( (cfgCapAll) == (Capabilities(TN_09)) ) && ( (cfgCapAll) == (Capabilities(TN_10)) ) && ( (cfgCapAll) == (Capabilities(TN_11)) ) && ( (cfgCapAll) == (Capabilities(TN_12)) ) && ( (cfgCapAll) == (Capabilities(TN_13)) ) && ( (cfgCapAll) == (Capabilities(TN_14)) ) && ( (cfgCapAll) == (Capabilities(TN_15)) ) )
 #define defInterruptStartFixed ( InterruptStart(TN_00) )
 #define defInterruptStartTest  ( ( (defInterruptStartFixed) == (InterruptStart(TN_01)) ) &&  ( (defInterruptStartFixed) == (InterruptStart(TN_02)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_03)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_04)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_05)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_06)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_07)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_08)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_09)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_10)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_11)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_12)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_13)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_14)) ) && ( (defInterruptStartFixed) == (InterruptStart(TN_15)) ) )
 #define defTimeSliceFixed      ( TimeSlice(TN_00) )
 #define defTimeSliceTest       ( ( (defTimeSliceFixed) == (TimeSlice(TN_01)) ) &&  ( (defTimeSliceFixed) == (TimeSlice(TN_02)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_03)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_04)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_05)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_06)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_07)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_08)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_09)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_10)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_11)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_12)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_13)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_14)) ) && ( (defTimeSliceFixed) == (TimeSlice(TN_15)) ) )
#endif

#define defRegisterUseVariable (defRegisterUseCollect & (~defRegisterUseFixed))

#if (defUseIsrStack == cfgTrue) && (defUseSharedStack == cfgTrue)
  #define defStackSpaceTotal     ( (defStackSpaceTasks) + (StackSizeOS) + (StackSizeISR) + (StackSizeShared))
#elif (defUseIsrStack == cfgTrue) && (defUseSharedStack == cfgFalse)
  #define defStackSpaceTotal     ( (defStackSpaceTasks) + (StackSizeOS) + (StackSizeISR))
#elif (defUseIsrStack == cfgFalse) && (defUseSharedStack == cfgTrue)
  #define defStackSpaceTotal     ( (defStackSpaceTasks) + (StackSizeOS) + (StackSizeShared))
#else
  #define defStackSpaceTotal     ( (defStackSpaceTasks) + (StackSizeOS))
#endif

/* Test if all stacks fit on page zero */
#if (devRAMend <= 0xFF) || ((defSysZeroPageStack == cfgTrue) && \
    ( ((cfgSysReuseOsStack == cfgTrue)   && ((defStackSpaceTotal + devRAMbegin) <= 0xFF)) || \
      ((cfgSysReuseOsStack == cfgFalse)  && ((defStackSpaceTotal + defTaskSaveSize + devRAMbegin) <= 0xFF)) ))
  #define defStackSpaceExceedsOneByte cfgFalse
#else
  #define defStackSpaceExceedsOneByte cfgTrue
#endif

#if defRegisterUseTest
  #define defRegisterUseConstant cfgTrue
#else
  #define defRegisterUseConstant cfgFalse
#endif

#if (defRegisterUseConstant == cfgTrue)
  #define defRegisterCount (((defRegisterUseCollect & 0x80) >> 5) + ((defRegisterUseCollect & 0x40) >> 4) + ((defRegisterUseCollect & 0x20) >> 3) + ((defRegisterUseCollect & 0x10) >> 2) + ((defRegisterUseCollect & 0x08) >> 1) + ((defRegisterUseCollect & 0x04)) + ((defRegisterUseCollect & 0x02) << 1) + ((defRegisterUseCollect & 0x01) << 2))
#endif

#if defPriorityTest
  #define defPriorityConstant cfgTrue
#else
  #define defPriorityConstant cfgFalse
#endif

#if defStackSizeTest
  #define defStackSizeConstant cfgTrue
#else
  #define defStackSizeConstant cfgFalse
#endif

#if defTaskIncludeTest
  #define defTaskIncludeConstant cfgTrue
#else
  #define defTaskIncludeConstant cfgFalse
#endif

#if defInterruptStartTest
  #define defInterruptStartConstant cfgTrue
#else
  #define defInterruptStartConstant cfgFalse
#endif

#if (cfgUseEquidistantTicks == cfgTrue)
  #define defTimeSliceConstant cfgTrue
#else
  #if defTimeSliceTest
    #define defTimeSliceConstant cfgTrue
  #else
    #define defTimeSliceConstant cfgFalse
  #endif
#endif

/* Since the initial status and the task status are different, we need a
 * compile time mapping to keep the runtime small. This mapping takes
 * care of (I, ann S should have the bits on set on the proper place.
 * Other bits must be zero):
 *
 * if shared is off
 *  00  ==>  000   (suspended)
 *  01  ==>  010   (sleeping)
 *  11  ==>  110   (running)
 *
 * if shared is on
 *  00  ==>  001   (suspended)
 *  01  ==>  011   (sleeping)
 *  11  ==>  101   (shared)
 */
#define InitialStartModifier(I,S)  ( ( ((I)|(S)) == (defRestartRunning | defInitialSharePresent)) ? (defRestartShared | defInitialSharePresent) : ((I)|(S)) )

#define InitialShared(X) ((StackIsShared(X)) ? (defInitialSharePresent) : (defInitialShareAbsent))

#if (defPriorityConstant == cfgTrue) && (defInterruptStartConstant == cfgTrue) && (defTaskIncludeConstant == cfgTrue) && (defSharesConstant == cfgTrue)
  #define defInitialStatusConstant cfgTrue
  #define defInitialStatusFixed ( InitialStartModifier(((defTaskIncludeFixed) & (defInitialStartGetMask)),defInitialSharedFixed) | ((defInterruptStartFixed) & (defInitialInterruptGetMask)) | (((defPriorityFixed) << defBasePrioShift) & defInitialPriorityGetMask) )
#else
  #define defInitialStatusConstant cfgFalse
#endif

#if defCapabilitiesTest
  #define defCapabilitiesFull cfgTrue
#else
  #define defCapabilitiesFull cfgFalse
#endif

#if defStackSizeHuge
  #define Tstack Tuint16
  #define defStackSizeExceedsOneByte cfgTrue
#else
  #define Tstack Tuint08
  #define defStackSizeExceedsOneByte cfgFalse
#endif


#if (cfgCheckRegisters == cfgTrue)

/* TODO: implement this? */
/* It is not needed to check registers which are marked 'in use'.
 * This could be activated with
 *   #define RegisterCheckReduced(X)  ( (RegisterCheck(X)) &(~(RegisterUse(X))))
 * but that changes the registers in use in the logging. Maybe we should
 * find a different solution. For now we stick with the old solution */
#define RegisterCheckReduced(X)    (RegisterCheck(X))

#if defNumberOfTasks == 0
 #define defRegisterCheckCollect  ( 0x00 )
 #define defRegisterCheckTest     ( 1 == 1 )
#elif defNumberOfTasks == 1
 #define defRegisterCheckCollect  ( (RegisterCheckReduced(TN_00) ) )
 #define defRegisterCheckTest      ( ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_00)) ) )
#elif defNumberOfTasks == 2
 #define defRegisterCheckCollect  ( (RegisterCheckReduced(TN_00)) | (RegisterCheckReduced(TN_01)) )
 #define defRegisterCheckTest      ( ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_00)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_01)) ) )
#elif defNumberOfTasks == 3
 #define defRegisterCheckCollect  ( (RegisterCheckReduced(TN_00)) | (RegisterCheckReduced(TN_01)) | (RegisterCheckReduced(TN_02)) )
 #define defRegisterCheckTest      ( ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_00)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_01)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_02)) ) )
#elif defNumberOfTasks == 4
 #define defRegisterCheckCollect  ( (RegisterCheckReduced(TN_00)) | (RegisterCheckReduced(TN_01)) | (RegisterCheckReduced(TN_02)) | (RegisterCheckReduced(TN_03)) )
 #define defRegisterCheckTest      ( ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_00)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_01)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_02)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_03)) ) )
#elif defNumberOfTasks == 5
 #define defRegisterCheckCollect  ( (RegisterCheckReduced(TN_00)) | (RegisterCheckReduced(TN_01)) | (RegisterCheckReduced(TN_02)) | (RegisterCheckReduced(TN_03)) | (RegisterCheckReduced(TN_04)) )
 #define defRegisterCheckTest      ( ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_00)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_01)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_02)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_03)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_04)) ) )
#elif defNumberOfTasks == 6
 #define defRegisterCheckCollect  ( (RegisterCheckReduced(TN_00)) | (RegisterCheckReduced(TN_01)) | (RegisterCheckReduced(TN_02)) | (RegisterCheckReduced(TN_03)) | (RegisterCheckReduced(TN_04)) | (RegisterCheckReduced(TN_05)) )
 #define defRegisterCheckTest      ( ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_00)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_01)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_02)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_03)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_04)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_05)) ) )
#elif defNumberOfTasks == 7
 #define defRegisterCheckCollect  ( (RegisterCheckReduced(TN_00)) | (RegisterCheckReduced(TN_01)) | (RegisterCheckReduced(TN_02)) | (RegisterCheckReduced(TN_03)) | (RegisterCheckReduced(TN_04)) | (RegisterCheckReduced(TN_05)) | (RegisterCheckReduced(TN_06)) )
 #define defRegisterCheckTest      ( ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_00)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_01)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_02)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_03)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_04)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_05)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_06)) ) )
#elif defNumberOfTasks == 8
 #define defRegisterCheckCollect  ( (RegisterCheckReduced(TN_00)) | (RegisterCheckReduced(TN_01)) | (RegisterCheckReduced(TN_02)) | (RegisterCheckReduced(TN_03)) | (RegisterCheckReduced(TN_04)) | (RegisterCheckReduced(TN_05)) | (RegisterCheckReduced(TN_06)) | (RegisterCheckReduced(TN_07)) )
 #define defRegisterCheckTest      ( ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_00)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_01)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_02)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_03)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_04)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_05)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_06)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_07)) ) )
#elif defNumberOfTasks == 9
 #define defRegisterCheckCollect  ( (RegisterCheckReduced(TN_00)) | (RegisterCheckReduced(TN_01)) | (RegisterCheckReduced(TN_02)) | (RegisterCheckReduced(TN_03)) | (RegisterCheckReduced(TN_04)) | (RegisterCheckReduced(TN_05)) | (RegisterCheckReduced(TN_06)) | (RegisterCheckReduced(TN_07)) | (RegisterCheckReduced(TN_08)) )
 #define defRegisterCheckTest      ( ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_00)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_01)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_02)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_03)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_04)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_05)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_06)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_07)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_08)) ) )
#elif defNumberOfTasks == 10
 #define defRegisterCheckCollect  ( (RegisterCheckReduced(TN_00)) | (RegisterCheckReduced(TN_01)) | (RegisterCheckReduced(TN_02)) | (RegisterCheckReduced(TN_03)) | (RegisterCheckReduced(TN_04)) | (RegisterCheckReduced(TN_05)) | (RegisterCheckReduced(TN_06)) | (RegisterCheckReduced(TN_07)) | (RegisterCheckReduced(TN_08)) | (RegisterCheckReduced(TN_09)) )
 #define defRegisterCheckTest      ( ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_00)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_01)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_02)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_03)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_04)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_05)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_06)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_07)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_08)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_09)) ) )
#elif defNumberOfTasks == 11
 #define defRegisterCheckCollect  ( (RegisterCheckReduced(TN_00)) | (RegisterCheckReduced(TN_01)) | (RegisterCheckReduced(TN_02)) | (RegisterCheckReduced(TN_03)) | (RegisterCheckReduced(TN_04)) | (RegisterCheckReduced(TN_05)) | (RegisterCheckReduced(TN_06)) | (RegisterCheckReduced(TN_07)) | (RegisterCheckReduced(TN_08)) | (RegisterCheckReduced(TN_09)) | (RegisterCheckReduced(TN_10)) )
 #define defRegisterCheckTest      ( ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_00)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_01)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_02)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_03)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_04)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_05)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_06)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_07)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_08)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_09)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_10)) ) )
#elif defNumberOfTasks == 12
 #define defRegisterCheckCollect  ( (RegisterCheckReduced(TN_00)) | (RegisterCheckReduced(TN_01)) | (RegisterCheckReduced(TN_02)) | (RegisterCheckReduced(TN_03)) | (RegisterCheckReduced(TN_04)) | (RegisterCheckReduced(TN_05)) | (RegisterCheckReduced(TN_06)) | (RegisterCheckReduced(TN_07)) | (RegisterCheckReduced(TN_08)) | (RegisterCheckReduced(TN_09)) | (RegisterCheckReduced(TN_10)) | (RegisterCheckReduced(TN_11)) )
 #define defRegisterCheckTest      ( ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_00)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_01)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_02)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_03)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_04)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_05)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_06)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_07)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_08)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_09)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_10)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_11)) ) )
#elif defNumberOfTasks == 13
 #define defRegisterCheckCollect  ( (RegisterCheckReduced(TN_00)) | (RegisterCheckReduced(TN_01)) | (RegisterCheckReduced(TN_02)) | (RegisterCheckReduced(TN_03)) | (RegisterCheckReduced(TN_04)) | (RegisterCheckReduced(TN_05)) | (RegisterCheckReduced(TN_06)) | (RegisterCheckReduced(TN_07)) | (RegisterCheckReduced(TN_08)) | (RegisterCheckReduced(TN_09)) | (RegisterCheckReduced(TN_10)) | (RegisterCheckReduced(TN_11)) | (RegisterCheckReduced(TN_12)) )
 #define defRegisterCheckTest      ( ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_00)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_01)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_02)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_03)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_04)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_05)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_06)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_07)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_08)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_09)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_10)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_11)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_12)) ) )
#elif defNumberOfTasks == 14
 #define defRegisterCheckCollect  ( (RegisterCheckReduced(TN_00)) | (RegisterCheckReduced(TN_01)) | (RegisterCheckReduced(TN_02)) | (RegisterCheckReduced(TN_03)) | (RegisterCheckReduced(TN_04)) | (RegisterCheckReduced(TN_05)) | (RegisterCheckReduced(TN_06)) | (RegisterCheckReduced(TN_07)) | (RegisterCheckReduced(TN_08)) | (RegisterCheckReduced(TN_09)) | (RegisterCheckReduced(TN_10)) | (RegisterCheckReduced(TN_11)) | (RegisterCheckReduced(TN_12)) | (RegisterCheckReduced(TN_13)) )
 #define defRegisterCheckTest      ( ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_00)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_01)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_02)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_03)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_04)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_05)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_06)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_07)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_08)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_09)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_10)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_11)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_12)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_13)) ) )
#elif defNumberOfTasks == 15
 #define defRegisterCheckCollect  ( (RegisterCheckReduced(TN_00)) | (RegisterCheckReduced(TN_01)) | (RegisterCheckReduced(TN_02)) | (RegisterCheckReduced(TN_03)) | (RegisterCheckReduced(TN_04)) | (RegisterCheckReduced(TN_05)) | (RegisterCheckReduced(TN_06)) | (RegisterCheckReduced(TN_07)) | (RegisterCheckReduced(TN_08)) | (RegisterCheckReduced(TN_09)) | (RegisterCheckReduced(TN_10)) | (RegisterCheckReduced(TN_11)) | (RegisterCheckReduced(TN_12)) | (RegisterCheckReduced(TN_13)) | (RegisterCheckReduced(TN_14)) )
 #define defRegisterCheckTest      ( ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_00)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_01)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_02)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_03)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_04)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_05)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_06)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_07)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_08)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_09)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_10)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_11)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_12)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_13)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_14)) ) )
#elif defNumberOfTasks == 16
 #define defRegisterCheckCollect  ( (RegisterCheckReduced(TN_00)) | (RegisterCheckReduced(TN_01)) | (RegisterCheckReduced(TN_02)) | (RegisterCheckReduced(TN_03)) | (RegisterCheckReduced(TN_04)) | (RegisterCheckReduced(TN_05)) | (RegisterCheckReduced(TN_06)) | (RegisterCheckReduced(TN_07)) | (RegisterCheckReduced(TN_08)) | (RegisterCheckReduced(TN_09)) | (RegisterCheckReduced(TN_10)) | (RegisterCheckReduced(TN_11)) | (RegisterCheckReduced(TN_12)) | (RegisterCheckReduced(TN_13)) | (RegisterCheckReduced(TN_14)) | (RegisterCheckReduced(TN_15)) )
 #define defRegisterCheckTest      ( ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_00)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_01)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_02)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_03)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_04)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_05)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_06)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_07)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_08)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_09)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_10)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_11)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_12)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_13)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_14)) ) && ( (defRegisterCheckCollect) == (RegisterCheckReduced(TN_15)) ) )
#endif

#if defRegisterCheckTest
  #define defRegisterCheckConstant cfgTrue
#else
  #define defRegisterCheckConstant cfgFalse
#endif

#elif (cfgCheckWatermarks == cfgTrue)
  #define defRegisterCheckCollect  (registersAll)
  #define defRegisterCheckConstant cfgTrue

#elif (cfgCheckTaskStack == cfgTrue)
  #define defRegisterCheckCollect  (registersNon)
  #define defRegisterCheckConstant cfgTrue

#else
  #define defRegisterCheckCollect  (registersNon)
  #define defRegisterCheckConstant cfgTrue

#endif


#if (cfgUseSynchronization != cfgSyncNon)

#define SlotDepth(X)           ((SlotSize(X)+1)>>1)

#if defNumberOfTasksWithSlot == 0
 #define defSlotSizeTest1      ( 1 == 0 )
 #define defSlotSizeTest2      ( 1 == 0 )
 #define defSlotDepthCollect   ( 0x00 )
 #define defSlotDepthTest      ( 1 == 1 )
#elif defNumberOfTasksWithSlot == 1
 #define defSlotSizeTest1      ( ((SlotSize(TN_00))<2) )
 #define defSlotSizeTest2      ( ((SlotSize(TN_00))<3) )
 #define defSlotDepthCollect   ( (SlotDepth(TN_00)) )
 #define defSlotDepthTest      ( ((defSlotDepthCollect) == (SlotDepth(TN_00))) )
#elif defNumberOfTasksWithSlot == 2
 #define defSlotSizeTest1      ( ((SlotSize(TN_00))<2) && ((SlotSize(TN_01))<2) )
 #define defSlotSizeTest2      ( ((SlotSize(TN_00))<3) && ((SlotSize(TN_01))<3) )
 #define defSlotDepthCollect   ( (SlotDepth(TN_00)) | (SlotDepth(TN_01)) )
 #define defSlotDepthTest      ( ((defSlotDepthCollect) == (SlotDepth(TN_00))) && ((defSlotDepthCollect) == (SlotDepth(TN_01))) )
#elif defNumberOfTasksWithSlot == 3
 #define defSlotSizeTest1      ( ((SlotSize(TN_00))<2) && ((SlotSize(TN_01))<2) && ((SlotSize(TN_02))<2) )
 #define defSlotSizeTest2      ( ((SlotSize(TN_00))<3) && ((SlotSize(TN_01))<3) && ((SlotSize(TN_02))<3) )
 #define defSlotDepthCollect   ( (SlotDepth(TN_00)) | (SlotDepth(TN_01)) | (SlotDepth(TN_02)) )
 #define defSlotDepthTest      ( ((defSlotDepthCollect) == (SlotDepth(TN_00))) && ((defSlotDepthCollect) == (SlotDepth(TN_01))) && ((defSlotDepthCollect) == (SlotDepth(TN_02))) )
#elif defNumberOfTasksWithSlot == 4
 #define defSlotSizeTest1      ( ((SlotSize(TN_00))<2) && ((SlotSize(TN_01))<2) && ((SlotSize(TN_02))<2) && ((SlotSize(TN_03))<2) )
 #define defSlotSizeTest2      ( ((SlotSize(TN_00))<3) && ((SlotSize(TN_01))<3) && ((SlotSize(TN_02))<3) && ((SlotSize(TN_03))<3) )
 #define defSlotDepthCollect   ( (SlotDepth(TN_00)) | (SlotDepth(TN_01)) | (SlotDepth(TN_02)) | (SlotDepth(TN_03)) )
 #define defSlotDepthTest      ( ((defSlotDepthCollect) == (SlotDepth(TN_00))) && ((defSlotDepthCollect) == (SlotDepth(TN_01))) && ((defSlotDepthCollect) == (SlotDepth(TN_02))) && ((defSlotDepthCollect) == (SlotDepth(TN_03))) )
#elif defNumberOfTasksWithSlot == 5
 #define defSlotSizeTest1      ( ((SlotSize(TN_00))<2) && ((SlotSize(TN_01))<2) && ((SlotSize(TN_02))<2) && ((SlotSize(TN_03))<2) && ((SlotSize(TN_04))<2) )
 #define defSlotSizeTest2      ( ((SlotSize(TN_00))<3) && ((SlotSize(TN_01))<3) && ((SlotSize(TN_02))<3) && ((SlotSize(TN_03))<3) && ((SlotSize(TN_04))<3) )
 #define defSlotDepthCollect   ( (SlotDepth(TN_00)) | (SlotDepth(TN_01)) | (SlotDepth(TN_02)) | (SlotDepth(TN_03)) | (SlotDepth(TN_04)) )
 #define defSlotDepthTest      ( ((defSlotDepthCollect) == (SlotDepth(TN_00))) && ((defSlotDepthCollect) == (SlotDepth(TN_01))) && ((defSlotDepthCollect) == (SlotDepth(TN_02))) && ((defSlotDepthCollect) == (SlotDepth(TN_03))) && ((defSlotDepthCollect) == (SlotDepth(TN_04))) )
#elif defNumberOfTasksWithSlot == 6
 #define defSlotSizeTest1      ( ((SlotSize(TN_00))<2) && ((SlotSize(TN_01))<2) && ((SlotSize(TN_02))<2) && ((SlotSize(TN_03))<2) && ((SlotSize(TN_04))<2) && ((SlotSize(TN_05))<2) )
 #define defSlotSizeTest2      ( ((SlotSize(TN_00))<3) && ((SlotSize(TN_01))<3) && ((SlotSize(TN_02))<3) && ((SlotSize(TN_03))<3) && ((SlotSize(TN_04))<3) && ((SlotSize(TN_05))<3) )
 #define defSlotDepthCollect   ( (SlotDepth(TN_00)) | (SlotDepth(TN_01)) | (SlotDepth(TN_02)) | (SlotDepth(TN_03)) | (SlotDepth(TN_04)) | (SlotDepth(TN_05)) )
 #define defSlotDepthTest      ( ((defSlotDepthCollect) == (SlotDepth(TN_00))) && ((defSlotDepthCollect) == (SlotDepth(TN_01))) && ((defSlotDepthCollect) == (SlotDepth(TN_02))) && ((defSlotDepthCollect) == (SlotDepth(TN_03))) && ((defSlotDepthCollect) == (SlotDepth(TN_04))) && ((defSlotDepthCollect) == (SlotDepth(TN_05))) )
#elif defNumberOfTasksWithSlot == 7
 #define defSlotSizeTest1      ( ((SlotSize(TN_00))<2) && ((SlotSize(TN_01))<2) && ((SlotSize(TN_02))<2) && ((SlotSize(TN_03))<2) && ((SlotSize(TN_04))<2) && ((SlotSize(TN_05))<2) && ((SlotSize(TN_06))<2) )
 #define defSlotSizeTest2      ( ((SlotSize(TN_00))<3) && ((SlotSize(TN_01))<3) && ((SlotSize(TN_02))<3) && ((SlotSize(TN_03))<3) && ((SlotSize(TN_04))<3) && ((SlotSize(TN_05))<3) && ((SlotSize(TN_06))<3) )
 #define defSlotDepthCollect   ( (SlotDepth(TN_00)) | (SlotDepth(TN_01)) | (SlotDepth(TN_02)) | (SlotDepth(TN_03)) | (SlotDepth(TN_04)) | (SlotDepth(TN_05)) | (SlotDepth(TN_06)) )
 #define defSlotDepthTest      ( ((defSlotDepthCollect) == (SlotDepth(TN_00))) && ((defSlotDepthCollect) == (SlotDepth(TN_01))) && ((defSlotDepthCollect) == (SlotDepth(TN_02))) && ((defSlotDepthCollect) == (SlotDepth(TN_03))) && ((defSlotDepthCollect) == (SlotDepth(TN_04))) && ((defSlotDepthCollect) == (SlotDepth(TN_05))) && ((defSlotDepthCollect) == (SlotDepth(TN_06))) )
#elif defNumberOfTasksWithSlot == 8
 #define defSlotSizeTest1      ( ((SlotSize(TN_00))<2) && ((SlotSize(TN_01))<2) && ((SlotSize(TN_02))<2) && ((SlotSize(TN_03))<2) && ((SlotSize(TN_04))<2) && ((SlotSize(TN_05))<2) && ((SlotSize(TN_06))<2) && ((SlotSize(TN_07))<2) )
 #define defSlotSizeTest2      ( ((SlotSize(TN_00))<3) && ((SlotSize(TN_01))<3) && ((SlotSize(TN_02))<3) && ((SlotSize(TN_03))<3) && ((SlotSize(TN_04))<3) && ((SlotSize(TN_05))<3) && ((SlotSize(TN_06))<3) && ((SlotSize(TN_07))<3) )
 #define defSlotDepthCollect   ( (SlotDepth(TN_00)) | (SlotDepth(TN_01)) | (SlotDepth(TN_02)) | (SlotDepth(TN_03)) | (SlotDepth(TN_04)) | (SlotDepth(TN_05)) | (SlotDepth(TN_06)) | (SlotDepth(TN_07)) )
 #define defSlotDepthTest      ( ((defSlotDepthCollect) == (SlotDepth(TN_00))) && ((defSlotDepthCollect) == (SlotDepth(TN_01))) && ((defSlotDepthCollect) == (SlotDepth(TN_02))) && ((defSlotDepthCollect) == (SlotDepth(TN_03))) && ((defSlotDepthCollect) == (SlotDepth(TN_04))) && ((defSlotDepthCollect) == (SlotDepth(TN_05))) && ((defSlotDepthCollect) == (SlotDepth(TN_06))) && ((defSlotDepthCollect) == (SlotDepth(TN_07))) )
#elif defNumberOfTasksWithSlot == 9
 #define defSlotSizeTest1      ( ((SlotSize(TN_00))<2) && ((SlotSize(TN_01))<2) && ((SlotSize(TN_02))<2) && ((SlotSize(TN_03))<2) && ((SlotSize(TN_04))<2) && ((SlotSize(TN_05))<2) && ((SlotSize(TN_06))<2) && ((SlotSize(TN_07))<2) && ((SlotSize(TN_08))<2) )
 #define defSlotSizeTest2      ( ((SlotSize(TN_00))<3) && ((SlotSize(TN_01))<3) && ((SlotSize(TN_02))<3) && ((SlotSize(TN_03))<3) && ((SlotSize(TN_04))<3) && ((SlotSize(TN_05))<3) && ((SlotSize(TN_06))<3) && ((SlotSize(TN_07))<3) && ((SlotSize(TN_08))<3) )
 #define defSlotDepthCollect   ( (SlotDepth(TN_00)) | (SlotDepth(TN_01)) | (SlotDepth(TN_02)) | (SlotDepth(TN_03)) | (SlotDepth(TN_04)) | (SlotDepth(TN_05)) | (SlotDepth(TN_06)) | (SlotDepth(TN_07)) | (SlotDepth(TN_08)) )
 #define defSlotDepthTest      ( ((defSlotDepthCollect) == (SlotDepth(TN_00))) && ((defSlotDepthCollect) == (SlotDepth(TN_01))) && ((defSlotDepthCollect) == (SlotDepth(TN_02))) && ((defSlotDepthCollect) == (SlotDepth(TN_03))) && ((defSlotDepthCollect) == (SlotDepth(TN_04))) && ((defSlotDepthCollect) == (SlotDepth(TN_05))) && ((defSlotDepthCollect) == (SlotDepth(TN_06))) && ((defSlotDepthCollect) == (SlotDepth(TN_07))) && ((defSlotDepthCollect) == (SlotDepth(TN_08))) )
#elif defNumberOfTasksWithSlot == 10
 #define defSlotSizeTest1      ( ((SlotSize(TN_00))<2) && ((SlotSize(TN_01))<2) && ((SlotSize(TN_02))<2) && ((SlotSize(TN_03))<2) && ((SlotSize(TN_04))<2) && ((SlotSize(TN_05))<2) && ((SlotSize(TN_06))<2) && ((SlotSize(TN_07))<2) && ((SlotSize(TN_08))<2) && ((SlotSize(TN_09))<2) )
 #define defSlotSizeTest2      ( ((SlotSize(TN_00))<3) && ((SlotSize(TN_01))<3) && ((SlotSize(TN_02))<3) && ((SlotSize(TN_03))<3) && ((SlotSize(TN_04))<3) && ((SlotSize(TN_05))<3) && ((SlotSize(TN_06))<3) && ((SlotSize(TN_07))<3) && ((SlotSize(TN_08))<3) && ((SlotSize(TN_09))<3) )
 #define defSlotDepthCollect   ( (SlotDepth(TN_00)) | (SlotDepth(TN_01)) | (SlotDepth(TN_02)) | (SlotDepth(TN_03)) | (SlotDepth(TN_04)) | (SlotDepth(TN_05)) | (SlotDepth(TN_06)) | (SlotDepth(TN_07)) | (SlotDepth(TN_08)) | (SlotDepth(TN_09)) )
 #define defSlotDepthTest      ( ((defSlotDepthCollect) == (SlotDepth(TN_00))) && ((defSlotDepthCollect) == (SlotDepth(TN_01))) && ((defSlotDepthCollect) == (SlotDepth(TN_02))) && ((defSlotDepthCollect) == (SlotDepth(TN_03))) && ((defSlotDepthCollect) == (SlotDepth(TN_04))) && ((defSlotDepthCollect) == (SlotDepth(TN_05))) && ((defSlotDepthCollect) == (SlotDepth(TN_06))) && ((defSlotDepthCollect) == (SlotDepth(TN_07))) && ((defSlotDepthCollect) == (SlotDepth(TN_08))) && ((defSlotDepthCollect) == (SlotDepth(TN_09))) )
#elif defNumberOfTasksWithSlot == 11
 #define defSlotSizeTest1      ( ((SlotSize(TN_00))<2) && ((SlotSize(TN_01))<2) && ((SlotSize(TN_02))<2) && ((SlotSize(TN_03))<2) && ((SlotSize(TN_04))<2) && ((SlotSize(TN_05))<2) && ((SlotSize(TN_06))<2) && ((SlotSize(TN_07))<2) && ((SlotSize(TN_08))<2) && ((SlotSize(TN_09))<2) && ((SlotSize(TN_10))<2) )
 #define defSlotSizeTest2      ( ((SlotSize(TN_00))<3) && ((SlotSize(TN_01))<3) && ((SlotSize(TN_02))<3) && ((SlotSize(TN_03))<3) && ((SlotSize(TN_04))<3) && ((SlotSize(TN_05))<3) && ((SlotSize(TN_06))<3) && ((SlotSize(TN_07))<3) && ((SlotSize(TN_08))<3) && ((SlotSize(TN_09))<3) && ((SlotSize(TN_10))<3) )
 #define defSlotDepthCollect   ( (SlotDepth(TN_00)) | (SlotDepth(TN_01)) | (SlotDepth(TN_02)) | (SlotDepth(TN_03)) | (SlotDepth(TN_04)) | (SlotDepth(TN_05)) | (SlotDepth(TN_06)) | (SlotDepth(TN_07)) | (SlotDepth(TN_08)) | (SlotDepth(TN_09)) | (SlotDepth(TN_10)) )
 #define defSlotDepthTest      ( ((defSlotDepthCollect) == (SlotDepth(TN_00))) && ((defSlotDepthCollect) == (SlotDepth(TN_01))) && ((defSlotDepthCollect) == (SlotDepth(TN_02))) && ((defSlotDepthCollect) == (SlotDepth(TN_03))) && ((defSlotDepthCollect) == (SlotDepth(TN_04))) && ((defSlotDepthCollect) == (SlotDepth(TN_05))) && ((defSlotDepthCollect) == (SlotDepth(TN_06))) && ((defSlotDepthCollect) == (SlotDepth(TN_07))) && ((defSlotDepthCollect) == (SlotDepth(TN_08))) && ((defSlotDepthCollect) == (SlotDepth(TN_09))) && ((defSlotDepthCollect) == (SlotDepth(TN_10))) )
#elif defNumberOfTasksWithSlot == 12
 #define defSlotSizeTest1      ( ((SlotSize(TN_00))<2) && ((SlotSize(TN_01))<2) && ((SlotSize(TN_02))<2) && ((SlotSize(TN_03))<2) && ((SlotSize(TN_04))<2) && ((SlotSize(TN_05))<2) && ((SlotSize(TN_06))<2) && ((SlotSize(TN_07))<2) && ((SlotSize(TN_08))<2) && ((SlotSize(TN_09))<2) && ((SlotSize(TN_10))<2) && ((SlotSize(TN_11))<2) )
 #define defSlotSizeTest2      ( ((SlotSize(TN_00))<3) && ((SlotSize(TN_01))<3) && ((SlotSize(TN_02))<3) && ((SlotSize(TN_03))<3) && ((SlotSize(TN_04))<3) && ((SlotSize(TN_05))<3) && ((SlotSize(TN_06))<3) && ((SlotSize(TN_07))<3) && ((SlotSize(TN_08))<3) && ((SlotSize(TN_09))<3) && ((SlotSize(TN_10))<3) && ((SlotSize(TN_11))<3) )
 #define defSlotDepthCollect   ( (SlotDepth(TN_00)) | (SlotDepth(TN_01)) | (SlotDepth(TN_02)) | (SlotDepth(TN_03)) | (SlotDepth(TN_04)) | (SlotDepth(TN_05)) | (SlotDepth(TN_06)) | (SlotDepth(TN_07)) | (SlotDepth(TN_08)) | (SlotDepth(TN_09)) | (SlotDepth(TN_10)) | (SlotDepth(TN_11)) )
 #define defSlotDepthTest      ( ((defSlotDepthCollect) == (SlotDepth(TN_00))) && ((defSlotDepthCollect) == (SlotDepth(TN_01))) && ((defSlotDepthCollect) == (SlotDepth(TN_02))) && ((defSlotDepthCollect) == (SlotDepth(TN_03))) && ((defSlotDepthCollect) == (SlotDepth(TN_04))) && ((defSlotDepthCollect) == (SlotDepth(TN_05))) && ((defSlotDepthCollect) == (SlotDepth(TN_06))) && ((defSlotDepthCollect) == (SlotDepth(TN_07))) && ((defSlotDepthCollect) == (SlotDepth(TN_08))) && ((defSlotDepthCollect) == (SlotDepth(TN_09))) && ((defSlotDepthCollect) == (SlotDepth(TN_10))) && ((defSlotDepthCollect) == (SlotDepth(TN_11))) )
#elif defNumberOfTasksWithSlot == 13
 #define defSlotSizeTest1      ( ((SlotSize(TN_00))<2) && ((SlotSize(TN_01))<2) && ((SlotSize(TN_02))<2) && ((SlotSize(TN_03))<2) && ((SlotSize(TN_04))<2) && ((SlotSize(TN_05))<2) && ((SlotSize(TN_06))<2) && ((SlotSize(TN_07))<2) && ((SlotSize(TN_08))<2) && ((SlotSize(TN_09))<2) && ((SlotSize(TN_10))<2) && ((SlotSize(TN_11))<2) && ((SlotSize(TN_12))<2) )
 #define defSlotSizeTest2      ( ((SlotSize(TN_00))<3) && ((SlotSize(TN_01))<3) && ((SlotSize(TN_02))<3) && ((SlotSize(TN_03))<3) && ((SlotSize(TN_04))<3) && ((SlotSize(TN_05))<3) && ((SlotSize(TN_06))<3) && ((SlotSize(TN_07))<3) && ((SlotSize(TN_08))<3) && ((SlotSize(TN_09))<3) && ((SlotSize(TN_10))<3) && ((SlotSize(TN_11))<3) && ((SlotSize(TN_12))<3) )
 #define defSlotDepthCollect   ( (SlotDepth(TN_00)) | (SlotDepth(TN_01)) | (SlotDepth(TN_02)) | (SlotDepth(TN_03)) | (SlotDepth(TN_04)) | (SlotDepth(TN_05)) | (SlotDepth(TN_06)) | (SlotDepth(TN_07)) | (SlotDepth(TN_08)) | (SlotDepth(TN_09)) | (SlotDepth(TN_10)) | (SlotDepth(TN_11)) | (SlotDepth(TN_12)) )
 #define defSlotDepthTest      ( ((defSlotDepthCollect) == (SlotDepth(TN_00))) && ((defSlotDepthCollect) == (SlotDepth(TN_01))) && ((defSlotDepthCollect) == (SlotDepth(TN_02))) && ((defSlotDepthCollect) == (SlotDepth(TN_03))) && ((defSlotDepthCollect) == (SlotDepth(TN_04))) && ((defSlotDepthCollect) == (SlotDepth(TN_05))) && ((defSlotDepthCollect) == (SlotDepth(TN_06))) && ((defSlotDepthCollect) == (SlotDepth(TN_07))) && ((defSlotDepthCollect) == (SlotDepth(TN_08))) && ((defSlotDepthCollect) == (SlotDepth(TN_09))) && ((defSlotDepthCollect) == (SlotDepth(TN_10))) && ((defSlotDepthCollect) == (SlotDepth(TN_11))) && ((defSlotDepthCollect) == (SlotDepth(TN_12))) )
#elif defNumberOfTasksWithSlot == 14
 #define defSlotSizeTest1      ( ((SlotSize(TN_00))<2) && ((SlotSize(TN_01))<2) && ((SlotSize(TN_02))<2) && ((SlotSize(TN_03))<2) && ((SlotSize(TN_04))<2) && ((SlotSize(TN_05))<2) && ((SlotSize(TN_06))<2) && ((SlotSize(TN_07))<2) && ((SlotSize(TN_08))<2) && ((SlotSize(TN_09))<2) && ((SlotSize(TN_10))<2) && ((SlotSize(TN_11))<2) && ((SlotSize(TN_12))<2) && ((SlotSize(TN_13))<2) )
 #define defSlotSizeTest2      ( ((SlotSize(TN_00))<3) && ((SlotSize(TN_01))<3) && ((SlotSize(TN_02))<3) && ((SlotSize(TN_03))<3) && ((SlotSize(TN_04))<3) && ((SlotSize(TN_05))<3) && ((SlotSize(TN_06))<3) && ((SlotSize(TN_07))<3) && ((SlotSize(TN_08))<3) && ((SlotSize(TN_09))<3) && ((SlotSize(TN_10))<3) && ((SlotSize(TN_11))<3) && ((SlotSize(TN_12))<3) && ((SlotSize(TN_13))<3) )
 #define defSlotDepthCollect   ( (SlotDepth(TN_00)) | (SlotDepth(TN_01)) | (SlotDepth(TN_02)) | (SlotDepth(TN_03)) | (SlotDepth(TN_04)) | (SlotDepth(TN_05)) | (SlotDepth(TN_06)) | (SlotDepth(TN_07)) | (SlotDepth(TN_08)) | (SlotDepth(TN_09)) | (SlotDepth(TN_10)) | (SlotDepth(TN_11)) | (SlotDepth(TN_12)) | (SlotDepth(TN_13)) )
 #define defSlotDepthTest      ( ((defSlotDepthCollect) == (SlotDepth(TN_00))) && ((defSlotDepthCollect) == (SlotDepth(TN_01))) && ((defSlotDepthCollect) == (SlotDepth(TN_02))) && ((defSlotDepthCollect) == (SlotDepth(TN_03))) && ((defSlotDepthCollect) == (SlotDepth(TN_04))) && ((defSlotDepthCollect) == (SlotDepth(TN_05))) && ((defSlotDepthCollect) == (SlotDepth(TN_06))) && ((defSlotDepthCollect) == (SlotDepth(TN_07))) && ((defSlotDepthCollect) == (SlotDepth(TN_08))) && ((defSlotDepthCollect) == (SlotDepth(TN_09))) && ((defSlotDepthCollect) == (SlotDepth(TN_10))) && ((defSlotDepthCollect) == (SlotDepth(TN_11))) && ((defSlotDepthCollect) == (SlotDepth(TN_12))) && ((defSlotDepthCollect) == (SlotDepth(TN_13))) )
#elif defNumberOfTasksWithSlot == 15
 #define defSlotSizeTest1      ( ((SlotSize(TN_00))<2) && ((SlotSize(TN_01))<2) && ((SlotSize(TN_02))<2) && ((SlotSize(TN_03))<2) && ((SlotSize(TN_04))<2) && ((SlotSize(TN_05))<2) && ((SlotSize(TN_06))<2) && ((SlotSize(TN_07))<2) && ((SlotSize(TN_08))<2) && ((SlotSize(TN_09))<2) && ((SlotSize(TN_10))<2) && ((SlotSize(TN_11))<2) && ((SlotSize(TN_12))<2) && ((SlotSize(TN_13))<2) && ((SlotSize(TN_14))<2) )
 #define defSlotSizeTest2      ( ((SlotSize(TN_00))<3) && ((SlotSize(TN_01))<3) && ((SlotSize(TN_02))<3) && ((SlotSize(TN_03))<3) && ((SlotSize(TN_04))<3) && ((SlotSize(TN_05))<3) && ((SlotSize(TN_06))<3) && ((SlotSize(TN_07))<3) && ((SlotSize(TN_08))<3) && ((SlotSize(TN_09))<3) && ((SlotSize(TN_10))<3) && ((SlotSize(TN_11))<3) && ((SlotSize(TN_12))<3) && ((SlotSize(TN_13))<3) && ((SlotSize(TN_14))<3) )
 #define defSlotDepthCollect   ( (SlotDepth(TN_00)) | (SlotDepth(TN_01)) | (SlotDepth(TN_02)) | (SlotDepth(TN_03)) | (SlotDepth(TN_04)) | (SlotDepth(TN_05)) | (SlotDepth(TN_06)) | (SlotDepth(TN_07)) | (SlotDepth(TN_08)) | (SlotDepth(TN_09)) | (SlotDepth(TN_10)) | (SlotDepth(TN_11)) | (SlotDepth(TN_12)) | (SlotDepth(TN_13)) | (SlotDepth(TN_14)) )
 #define defSlotDepthTest      ( ((defSlotDepthCollect) == (SlotDepth(TN_00))) && ((defSlotDepthCollect) == (SlotDepth(TN_01))) && ((defSlotDepthCollect) == (SlotDepth(TN_02))) && ((defSlotDepthCollect) == (SlotDepth(TN_03))) && ((defSlotDepthCollect) == (SlotDepth(TN_04))) && ((defSlotDepthCollect) == (SlotDepth(TN_05))) && ((defSlotDepthCollect) == (SlotDepth(TN_06))) && ((defSlotDepthCollect) == (SlotDepth(TN_07))) && ((defSlotDepthCollect) == (SlotDepth(TN_08))) && ((defSlotDepthCollect) == (SlotDepth(TN_09))) && ((defSlotDepthCollect) == (SlotDepth(TN_10))) && ((defSlotDepthCollect) == (SlotDepth(TN_11))) && ((defSlotDepthCollect) == (SlotDepth(TN_12))) && ((defSlotDepthCollect) == (SlotDepth(TN_13))) && ((defSlotDepthCollect) == (SlotDepth(TN_14))) )
#elif defNumberOfTasksWithSlot == 16
 #define defSlotSizeTest1      ( ((SlotSize(TN_00))<2) && ((SlotSize(TN_01))<2) && ((SlotSize(TN_02))<2) && ((SlotSize(TN_03))<2) && ((SlotSize(TN_04))<2) && ((SlotSize(TN_05))<2) && ((SlotSize(TN_06))<2) && ((SlotSize(TN_07))<2) && ((SlotSize(TN_08))<2) && ((SlotSize(TN_09))<2) && ((SlotSize(TN_10))<2) && ((SlotSize(TN_11))<2) && ((SlotSize(TN_12))<2) && ((SlotSize(TN_13))<2) && ((SlotSize(TN_14))<2) && ((SlotSize(TN_15))<2) )
 #define defSlotSizeTest2      ( ((SlotSize(TN_00))<3) && ((SlotSize(TN_01))<3) && ((SlotSize(TN_02))<3) && ((SlotSize(TN_03))<3) && ((SlotSize(TN_04))<3) && ((SlotSize(TN_05))<3) && ((SlotSize(TN_06))<3) && ((SlotSize(TN_07))<3) && ((SlotSize(TN_08))<3) && ((SlotSize(TN_09))<3) && ((SlotSize(TN_10))<3) && ((SlotSize(TN_11))<3) && ((SlotSize(TN_12))<3) && ((SlotSize(TN_13))<3) && ((SlotSize(TN_14))<3) && ((SlotSize(TN_15))<3) )
#define defSlotDepthCollect    ( (SlotDepth(TN_00)) | (SlotDepth(TN_01)) | (SlotDepth(TN_02)) | (SlotDepth(TN_03)) | (SlotDepth(TN_04)) | (SlotDepth(TN_05)) | (SlotDepth(TN_06)) | (SlotDepth(TN_07)) | (SlotDepth(TN_08)) | (SlotDepth(TN_09)) | (SlotDepth(TN_10)) | (SlotDepth(TN_11)) | (SlotDepth(TN_12)) | (SlotDepth(TN_13)) | (SlotDepth(TN_14)) | (SlotDepth(TN_15)) )
 #define defSlotDepthTest      ( ((defSlotDepthCollect) == (SlotDepth(TN_00))) && ((defSlotDepthCollect) == (SlotDepth(TN_01))) && ((defSlotDepthCollect) == (SlotDepth(TN_02))) && ((defSlotDepthCollect) == (SlotDepth(TN_03))) && ((defSlotDepthCollect) == (SlotDepth(TN_04))) && ((defSlotDepthCollect) == (SlotDepth(TN_05))) && ((defSlotDepthCollect) == (SlotDepth(TN_06))) && ((defSlotDepthCollect) == (SlotDepth(TN_07))) && ((defSlotDepthCollect) == (SlotDepth(TN_08))) && ((defSlotDepthCollect) == (SlotDepth(TN_09))) && ((defSlotDepthCollect) == (SlotDepth(TN_10))) && ((defSlotDepthCollect) == (SlotDepth(TN_11))) && ((defSlotDepthCollect) == (SlotDepth(TN_12))) && ((defSlotDepthCollect) == (SlotDepth(TN_13))) && ((defSlotDepthCollect) == (SlotDepth(TN_14))) && ((defSlotDepthCollect) == (SlotDepth(TN_15))) )
#endif


#if defSlotSizeTest1
  #define defSlotOneOnly       cfgTrue
  #define defSlotTwoOnly       cfgFalse
  #define defSlotsMore         cfgFalse
#elif defSlotSizeTest2
  #define defSlotOneOnly       cfgFalse
  #define defSlotTwoOnly       cfgTrue
  #define defSlotsMore         cfgFalse
#else
  #define defSlotOneOnly       cfgFalse
  #define defSlotTwoOnly       cfgFalse
  #define defSlotsMore         cfgTrue
#endif

#if defSlotDepthTest
  #define defSlotDepthConstant cfgTrue
#else
  #define defSlotDepthConstant cfgFalse
#endif

#else
 #define defSlotDepthCollect   ( 0x00 )
 #define defSlotDepthConstant  cfgTrue
 #define defSlotOneOnly        cfgFalse
 #define defSlotTwoOnly        cfgFalse
 #define defSlotsMore          cfgFalse

#endif


#if (cfgUseSynchronization != cfgSyncNon)

#if defNumberOfQueus == 0
 #define defQueuSizeFixed ( 0x00 )
 #define defQueuSizeTest  ( 1 == 1 )
#elif defNumberOfQueus == 1
 #define defQueuSizeFixed ( QueuSize(LN_01) )
 #define defQueuSizeTest  ( 1 == 1 )
#elif defNumberOfQueus == 2
 #define defQueuSizeFixed ( QueuSize(LN_01) )
 #define defQueuSizeTest  ( ( (defQueuSizeFixed) == (QueuSize(LN_02)) ) )
#elif defNumberOfQueus == 3
 #define defQueuSizeFixed ( QueuSize(LN_01) )
 #define defQueuSizeTest  ( ( (defQueuSizeFixed) == (QueuSize(LN_02)) ) &&  ( (defQueuSizeFixed) == (QueuSize(LN_03)) ) )
#elif defNumberOfQueus == 4
 #define defQueuSizeFixed ( QueuSize(LN_01) )
 #define defQueuSizeTest  ( ( (defQueuSizeFixed) == (QueuSize(LN_02)) ) &&  ( (defQueuSizeFixed) == (QueuSize(LN_03)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_04)) ) )
#elif defNumberOfQueus == 5
 #define defQueuSizeFixed ( QueuSize(LN_01) )
 #define defQueuSizeTest  ( ( (defQueuSizeFixed) == (QueuSize(LN_02)) ) &&  ( (defQueuSizeFixed) == (QueuSize(LN_03)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_04)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_05)) ) )
#elif defNumberOfQueus == 6
 #define defQueuSizeFixed ( QueuSize(LN_01) )
 #define defQueuSizeTest  ( ( (defQueuSizeFixed) == (QueuSize(LN_02)) ) &&  ( (defQueuSizeFixed) == (QueuSize(LN_03)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_04)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_05)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_06)) ) )
#elif defNumberOfQueus == 7
 #define defQueuSizeFixed ( QueuSize(LN_01) )
 #define defQueuSizeTest  ( ( (defQueuSizeFixed) == (QueuSize(LN_02)) ) &&  ( (defQueuSizeFixed) == (QueuSize(LN_03)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_04)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_05)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_06)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_07)) ) )
#elif defNumberOfQueus == 8
 #define defQueuSizeFixed ( QueuSize(LN_01) )
 #define defQueuSizeTest  ( ( (defQueuSizeFixed) == (QueuSize(LN_02)) ) &&  ( (defQueuSizeFixed) == (QueuSize(LN_03)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_04)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_05)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_06)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_07)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_08)) ) )
#elif defNumberOfQueus == 9
 #define defQueuSizeFixed ( QueuSize(LN_01) )
 #define defQueuSizeTest  ( ( (defQueuSizeFixed) == (QueuSize(LN_02)) ) &&  ( (defQueuSizeFixed) == (QueuSize(LN_03)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_04)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_05)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_06)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_07)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_08)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_09)) ) )
#elif defNumberOfQueus == 10
 #define defQueuSizeFixed ( QueuSize(LN_01) )
 #define defQueuSizeTest  ( ( (defQueuSizeFixed) == (QueuSize(LN_02)) ) &&  ( (defQueuSizeFixed) == (QueuSize(LN_03)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_04)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_05)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_06)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_07)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_08)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_09)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_10)) ) )
#elif defNumberOfQueus == 11
 #define defQueuSizeFixed ( QueuSize(LN_01) )
 #define defQueuSizeTest  ( ( (defQueuSizeFixed) == (QueuSize(LN_02)) ) &&  ( (defQueuSizeFixed) == (QueuSize(LN_03)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_04)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_05)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_06)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_07)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_08)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_09)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_10)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_11)) ) )
#elif defNumberOfQueus == 12
 #define defQueuSizeFixed ( QueuSize(LN_01) )
 #define defQueuSizeTest  ( ( (defQueuSizeFixed) == (QueuSize(LN_02)) ) &&  ( (defQueuSizeFixed) == (QueuSize(LN_03)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_04)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_05)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_06)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_07)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_08)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_09)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_10)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_11)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_12)) ) )
#elif defNumberOfQueus == 13
 #define defQueuSizeFixed ( QueuSize(LN_01) )
 #define defQueuSizeTest  ( ( (defQueuSizeFixed) == (QueuSize(LN_02)) ) &&  ( (defQueuSizeFixed) == (QueuSize(LN_03)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_04)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_05)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_06)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_07)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_08)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_09)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_10)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_11)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_12)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_13)) ) )
#elif defNumberOfQueus == 14
 #define defQueuSizeFixed ( QueuSize(LN_01) )
 #define defQueuSizeTest  ( ( (defQueuSizeFixed) == (QueuSize(LN_02)) ) &&  ( (defQueuSizeFixed) == (QueuSize(LN_03)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_04)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_05)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_06)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_07)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_08)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_09)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_10)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_11)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_12)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_13)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_14)) ) )
#elif defNumberOfQueus == 15
 #define defQueuSizeFixed ( QueuSize(LN_01) )
 #define defQueuSizeTest  ( ( (defQueuSizeFixed) == (QueuSize(LN_02)) ) &&  ( (defQueuSizeFixed) == (QueuSize(LN_03)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_04)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_05)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_06)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_07)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_08)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_09)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_10)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_11)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_12)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_13)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_14)) ) && ( (defQueuSizeFixed) == (QueuSize(LN_15)) ) )
#endif

#if defQueuSizeTest
  #define defQueuSizeConstant cfgTrue
#else
  #define defQueuSizeConstant cfgFalse
#endif

#endif


#if (cfgUseFileSystem  ==  cfgTrue)

#if defined(FN_00) && (FileSpace(FN_00) > 0)
  #define VN_00 FN_00
#endif

#if defined(FN_01) && (FileSpace(FN_01) > 0)
  #if !defined(VN_00)
    #define VN_00 FN_01
  #else
    #define VN_01 FN_01
  #endif
#endif

#if defined(FN_02) && (FileSpace(FN_02) > 0)
  #if !defined(VN_00)
    #define VN_00 FN_02
  #elif !defined(VN_01)
    #define VN_01 FN_02
  #else
    #define VN_02 FN_02
  #endif
#endif

#if defined(FN_03) && (FileSpace(FN_03) > 0)
  #if !defined(VN_00)
    #define VN_00 FN_03
  #elif !defined(VN_01)
    #define VN_01 FN_03
  #elif !defined(VN_02)
    #define VN_02 FN_03
  #else
    #define VN_03 FN_03
  #endif
#endif

#if defined(FN_04) && (FileSpace(FN_04) > 0)
  #if !defined(VN_00)
    #define VN_00 FN_04
  #elif !defined(VN_01)
    #define VN_01 FN_04
  #elif !defined(VN_02)
    #define VN_02 FN_04
  #elif !defined(VN_03)
    #define VN_03 FN_04
  #else
    #define VN_04 FN_04
  #endif
#endif


#if defined(FN_05) && (FileSpace(FN_05) > 0)
  #if !defined(VN_00)
    #define VN_00 FN_05
  #elif !defined(VN_01)
    #define VN_01 FN_05
  #elif !defined(VN_02)
    #define VN_02 FN_05
  #elif !defined(VN_03)
    #define VN_03 FN_05
  #elif !defined(VN_04)
    #define VN_04 FN_05
  #else
    #define VN_05 FN_05
  #endif
#endif

#if defined(FN_06) && (FileSpace(FN_06) > 0)
  #if !defined(VN_00)
    #define VN_00 FN_06
  #elif !defined(VN_01)
    #define VN_01 FN_06
  #elif !defined(VN_02)
    #define VN_02 FN_06
  #elif !defined(VN_03)
    #define VN_03 FN_06
  #elif !defined(VN_04)
    #define VN_04 FN_06
  #elif !defined(VN_05)
    #define VN_05 FN_06
  #else
    #define VN_06 FN_06
  #endif
#endif

#if defined(FN_07) && (FileSpace(FN_07) > 0)
  #if !defined(VN_00)
    #define VN_00 FN_07
  #elif !defined(VN_01)
    #define VN_01 FN_07
  #elif !defined(VN_02)
    #define VN_02 FN_07
  #elif !defined(VN_03)
    #define VN_03 FN_07
  #elif !defined(VN_04)
    #define VN_04 FN_07
  #elif !defined(VN_05)
    #define VN_05 FN_07
  #elif !defined(VN_06)
    #define VN_06 FN_07
  #else
    #define VN_07 FN_07
  #endif
#endif

#if defined(FN_08) && (FileSpace(FN_08) > 0)
  #if !defined(VN_00)
    #define VN_00 FN_08
  #elif !defined(VN_01)
    #define VN_01 FN_08
  #elif !defined(VN_02)
    #define VN_02 FN_08
  #elif !defined(VN_03)
    #define VN_03 FN_08
  #elif !defined(VN_04)
    #define VN_04 FN_08
  #elif !defined(VN_05)
    #define VN_05 FN_08
  #elif !defined(VN_06)
    #define VN_06 FN_08
  #elif !defined(VN_07)
    #define VN_07 FN_08
  #else
    #define VN_08 FN_08
  #endif
#endif

#if defined(FN_09) && (FileSpace(FN_09) > 0)
  #if !defined(VN_00)
    #define VN_00 FN_09
  #elif !defined(VN_01)
    #define VN_01 FN_09
  #elif !defined(VN_02)
    #define VN_02 FN_09
  #elif !defined(VN_03)
    #define VN_03 FN_09
  #elif !defined(VN_04)
    #define VN_04 FN_09
  #elif !defined(VN_05)
    #define VN_05 FN_09
  #elif !defined(VN_06)
    #define VN_06 FN_09
  #elif !defined(VN_07)
    #define VN_07 FN_09
  #elif !defined(VN_08)
    #define VN_08 FN_09
  #else
    #define VN_09 FN_09
  #endif
#endif

#if defined(FN_10) && (FileSpace(FN_10) > 0)
  #if !defined(VN_00)
    #define VN_00 FN_10
  #elif !defined(VN_01)
    #define VN_01 FN_10
  #elif !defined(VN_02)
    #define VN_02 FN_10
  #elif !defined(VN_03)
    #define VN_03 FN_10
  #elif !defined(VN_04)
    #define VN_04 FN_10
  #elif !defined(VN_05)
    #define VN_05 FN_10
  #elif !defined(VN_06)
    #define VN_06 FN_10
  #elif !defined(VN_07)
    #define VN_07 FN_10
  #elif !defined(VN_08)
    #define VN_08 FN_10
  #elif !defined(VN_09)
    #define VN_09 FN_10
  #else
    #define VN_10 FN_10
  #endif
#endif

#if defined(FN_11) && (FileSpace(FN_11) > 0)
  #if !defined(VN_00)
    #define VN_00 FN_11
  #elif !defined(VN_01)
    #define VN_01 FN_11
  #elif !defined(VN_02)
    #define VN_02 FN_11
  #elif !defined(VN_03)
    #define VN_03 FN_11
  #elif !defined(VN_04)
    #define VN_04 FN_11
  #elif !defined(VN_05)
    #define VN_05 FN_11
  #elif !defined(VN_06)
    #define VN_06 FN_11
  #elif !defined(VN_07)
    #define VN_07 FN_11
  #elif !defined(VN_08)
    #define VN_08 FN_11
  #elif !defined(VN_09)
    #define VN_09 FN_11
  #elif !defined(VN_10)
    #define VN_10 FN_11
  #else
    #define VN_11 FN_11
  #endif
#endif

#if defined(FN_12) && (FileSpace(FN_12) > 0)
  #if !defined(VN_00)
    #define VN_00 FN_12
  #elif !defined(VN_01)
    #define VN_01 FN_12
  #elif !defined(VN_02)
    #define VN_02 FN_12
  #elif !defined(VN_03)
    #define VN_03 FN_12
  #elif !defined(VN_04)
    #define VN_04 FN_12
  #elif !defined(VN_05)
    #define VN_05 FN_12
  #elif !defined(VN_06)
    #define VN_06 FN_12
  #elif !defined(VN_07)
    #define VN_07 FN_12
  #elif !defined(VN_08)
    #define VN_08 FN_12
  #elif !defined(VN_09)
    #define VN_09 FN_12
  #elif !defined(VN_10)
    #define VN_10 FN_12
  #elif !defined(VN_11)
    #define VN_11 FN_12
  #else
    #define VN_12 FN_12
  #endif
#endif

#if defined(FN_13) && (FileSpace(FN_13) > 0)
  #if !defined(VN_00)
    #define VN_00 FN_13
  #elif !defined(VN_01)
    #define VN_01 FN_13
  #elif !defined(VN_02)
    #define VN_02 FN_13
  #elif !defined(VN_03)
    #define VN_03 FN_13
  #elif !defined(VN_04)
    #define VN_04 FN_13
  #elif !defined(VN_05)
    #define VN_05 FN_13
  #elif !defined(VN_06)
    #define VN_06 FN_13
  #elif !defined(VN_07)
    #define VN_07 FN_13
  #elif !defined(VN_08)
    #define VN_08 FN_13
  #elif !defined(VN_09)
    #define VN_09 FN_13
  #elif !defined(VN_10)
    #define VN_10 FN_13
  #elif !defined(VN_11)
    #define VN_11 FN_13
  #elif !defined(VN_12)
    #define VN_12 FN_13
  #else
    #define VN_13 FN_13
  #endif
#endif

#if defined(FN_14) && (FileSpace(FN_14) > 0)
  #if !defined(VN_00)
    #define VN_00 FN_14
  #elif !defined(VN_01)
    #define VN_01 FN_14
  #elif !defined(VN_02)
    #define VN_02 FN_14
  #elif !defined(VN_03)
    #define VN_03 FN_14
  #elif !defined(VN_04)
    #define VN_04 FN_14
  #elif !defined(VN_05)
    #define VN_05 FN_14
  #elif !defined(VN_06)
    #define VN_06 FN_14
  #elif !defined(VN_07)
    #define VN_07 FN_14
  #elif !defined(VN_08)
    #define VN_08 FN_14
  #elif !defined(VN_09)
    #define VN_09 FN_14
  #elif !defined(VN_10)
    #define VN_10 FN_14
  #elif !defined(VN_11)
    #define VN_11 FN_14
  #elif !defined(VN_12)
    #define VN_12 FN_14
  #elif !defined(VN_13)
    #define VN_13 FN_14
  #else
    #define VN_14 FN_14
  #endif
#endif

#if defined(FN_15) && (FileSpace(FN_15) > 0)
  #if !defined(VN_00)
    #define VN_00 FN_15
  #elif !defined(VN_01)
    #define VN_01 FN_15
  #elif !defined(VN_02)
    #define VN_02 FN_15
  #elif !defined(VN_03)
    #define VN_03 FN_15
  #elif !defined(VN_04)
    #define VN_04 FN_15
  #elif !defined(VN_05)
    #define VN_05 FN_15
  #elif !defined(VN_06)
    #define VN_06 FN_15
  #elif !defined(VN_07)
    #define VN_07 FN_15
  #elif !defined(VN_08)
    #define VN_08 FN_15
  #elif !defined(VN_09)
    #define VN_09 FN_15
  #elif !defined(VN_10)
    #define VN_10 FN_15
  #elif !defined(VN_11)
    #define VN_11 FN_15
  #elif !defined(VN_12)
    #define VN_12 FN_15
  #elif !defined(VN_13)
    #define VN_13 FN_15
  #elif !defined(VN_14)
    #define VN_14 FN_15
  #else
    #define VN_15 FN_15
  #endif
#endif


#if !defined(VN_00)
  #define defFsNumberOfPreFiles 0
#elif !defined(VN_01)
  #define defFsNumberOfPreFiles 1
#elif !defined(VN_02)
  #define defFsNumberOfPreFiles 2
#elif !defined(VN_03)
  #define defFsNumberOfPreFiles 3
#elif !defined(VN_04)
  #define defFsNumberOfPreFiles 4
#elif !defined(VN_05)
  #define defFsNumberOfPreFiles 5
#elif !defined(VN_06)
  #define defFsNumberOfPreFiles 6
#elif !defined(VN_07)
  #define defFsNumberOfPreFiles 7
#elif !defined(VN_08)
  #define defFsNumberOfPreFiles 8
#elif !defined(VN_09)
  #define defFsNumberOfPreFiles 9
#elif !defined(VN_10)
  #define defFsNumberOfPreFiles 10
#elif !defined(VN_11)
  #define defFsNumberOfPreFiles 11
#elif !defined(VN_12)
  #define defFsNumberOfPreFiles 12
#elif !defined(VN_13)
  #define defFsNumberOfPreFiles 13
#elif !defined(VN_14)
  #define defFsNumberOfPreFiles 14
#elif !defined(VN_15)
  #define defFsNumberOfPreFiles 15
#else
  #define defFsNumberOfPreFiles 16
#endif

#if defFsNumberOfPreFiles == 0
  #define defTotalPreFileSpace  ( 0 )
  #define defFilePreSpaceFixed  ( 0x00 )
  #define defFilePreSpaceTest   ( 1 == 1 )
#elif defFsNumberOfPreFiles == 1
  #define defTotalPreFileSpace  ( (Tuint16) FileSpace(VN_00) )
  #define defFilePreSpaceFixed  ( FileSpace(VN_00) )
  #define defFilePreSpaceTest   ( 1 == 1 )
#elif defFsNumberOfPreFiles == 2
  #define defTotalPreFileSpace  ( (Tuint16) FileSpace(VN_00) + FileSpace(VN_01) )
  #define defFilePreSpaceFixed  ( FileSpace(VN_00) )
  #define defFilePreSpaceTest   ( ( (defFilePreSpaceFixed) == (FileSpace(VN_01)) ) )
#elif defFsNumberOfPreFiles == 3
  #define defTotalPreFileSpace  ( (Tuint16) FileSpace(VN_00) + FileSpace(VN_01) + FileSpace(VN_02) )
  #define defFilePreSpaceFixed  ( FileSpace(VN_00) )
  #define defFilePreSpaceTest   ( ( (defFilePreSpaceFixed) == (FileSpace(VN_01)) ) &&  ( (defFilePreSpaceFixed) == (FileSpace(VN_02)) ) )
#elif defFsNumberOfPreFiles == 4
  #define defTotalPreFileSpace  ( (Tuint16) FileSpace(VN_00) + FileSpace(VN_01) + FileSpace(VN_02) + FileSpace(VN_03) )
  #define defFilePreSpaceFixed  ( FileSpace(VN_00) )
  #define defFilePreSpaceTest   ( ( (defFilePreSpaceFixed) == (FileSpace(VN_01)) ) &&  ( (defFilePreSpaceFixed) == (FileSpace(VN_02)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_03)) ) )
#elif defFsNumberOfPreFiles == 5
  #define defTotalPreFileSpace  ( (Tuint16) FileSpace(VN_00) + FileSpace(VN_01) + FileSpace(VN_02) + FileSpace(VN_03) + FileSpace(VN_04) )
  #define defFilePreSpaceFixed  ( FileSpace(VN_00) )
  #define defFilePreSpaceTest   ( ( (defFilePreSpaceFixed) == (FileSpace(VN_01)) ) &&  ( (defFilePreSpaceFixed) == (FileSpace(VN_02)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_03)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_04)) ) )
#elif defFsNumberOfPreFiles == 6
  #define defTotalPreFileSpace  ( (Tuint16) FileSpace(VN_00) + FileSpace(VN_01) + FileSpace(VN_02) + FileSpace(VN_03) + FileSpace(VN_04) + FileSpace(VN_05) )
  #define defFilePreSpaceFixed  ( FileSpace(VN_00) )
  #define defFilePreSpaceTest   ( ( (defFilePreSpaceFixed) == (FileSpace(VN_01)) ) &&  ( (defFilePreSpaceFixed) == (FileSpace(VN_02)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_03)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_04)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_05)) ) )
#elif defFsNumberOfPreFiles == 7
  #define defTotalPreFileSpace  ( (Tuint16) FileSpace(VN_00) + FileSpace(VN_01) + FileSpace(VN_02) + FileSpace(VN_03) + FileSpace(VN_04) + FileSpace(VN_05) + FileSpace(VN_06) )
  #define defFilePreSpaceFixed  ( FileSpace(VN_00) )
  #define defFilePreSpaceTest   ( ( (defFilePreSpaceFixed) == (FileSpace(VN_01)) ) &&  ( (defFilePreSpaceFixed) == (FileSpace(VN_02)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_03)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_04)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_05)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_06)) ) )
#elif defFsNumberOfPreFiles == 8
  #define defTotalPreFileSpace  ( (Tuint16) FileSpace(VN_00) + FileSpace(VN_01) + FileSpace(VN_02) + FileSpace(VN_03) + FileSpace(VN_04) + FileSpace(VN_05) + FileSpace(VN_06) + FileSpace(VN_07) )
  #define defFilePreSpaceFixed  ( FileSpace(VN_00) )
  #define defFilePreSpaceTest   ( ( (defFilePreSpaceFixed) == (FileSpace(VN_01)) ) &&  ( (defFilePreSpaceFixed) == (FileSpace(VN_02)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_03)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_04)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_05)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_06)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_07)) ) )
#elif defFsNumberOfPreFiles == 9
  #define defTotalPreFileSpace  ( (Tuint16) FileSpace(VN_00) + FileSpace(VN_01) + FileSpace(VN_02) + FileSpace(VN_03) + FileSpace(VN_04) + FileSpace(VN_05) + FileSpace(VN_06) + FileSpace(VN_07) + FileSpace(VN_08) )
  #define defFilePreSpaceFixed  ( FileSpace(VN_00) )
  #define defFilePreSpaceTest   ( ( (defFilePreSpaceFixed) == (FileSpace(VN_01)) ) &&  ( (defFilePreSpaceFixed) == (FileSpace(VN_02)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_03)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_04)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_05)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_06)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_07)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_08)) ) )
#elif defFsNumberOfPreFiles == 10
  #define defTotalPreFileSpace  ( (Tuint16) FileSpace(VN_00) + FileSpace(VN_01) + FileSpace(VN_02) + FileSpace(VN_03) + FileSpace(VN_04) + FileSpace(VN_05) + FileSpace(VN_06) + FileSpace(VN_07) + FileSpace(VN_08) + FileSpace(VN_09) )
  #define defFilePreSpaceFixed  ( FileSpace(VN_00) )
  #define defFilePreSpaceTest   ( ( (defFilePreSpaceFixed) == (FileSpace(VN_01)) ) &&  ( (defFilePreSpaceFixed) == (FileSpace(VN_02)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_03)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_04)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_05)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_06)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_07)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_08)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_09)) ) )
#elif defFsNumberOfPreFiles == 11
  #define defTotalPreFileSpace  ( (Tuint16) FileSpace(VN_00) + FileSpace(VN_01) + FileSpace(VN_02) + FileSpace(VN_03) + FileSpace(VN_04) + FileSpace(VN_05) + FileSpace(VN_06) + FileSpace(VN_07) + FileSpace(VN_08) + FileSpace(VN_09) + FileSpace(VN_10) )
  #define defFilePreSpaceFixed  ( FileSpace(VN_00) )
  #define defFilePreSpaceTest   ( ( (defFilePreSpaceFixed) == (FileSpace(VN_01)) ) &&  ( (defFilePreSpaceFixed) == (FileSpace(VN_02)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_03)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_04)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_05)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_06)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_07)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_08)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_09)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_10)) ) )
#elif defFsNumberOfPreFiles == 12
  #define defTotalPreFileSpace  ( (Tuint16) FileSpace(VN_00) + FileSpace(VN_01) + FileSpace(VN_02) + FileSpace(VN_03) + FileSpace(VN_04) + FileSpace(VN_05) + FileSpace(VN_06) + FileSpace(VN_07) + FileSpace(VN_08) + FileSpace(VN_09) + FileSpace(VN_10) + FileSpace(VN_11) )
  #define defFilePreSpaceFixed  ( FileSpace(VN_00) )
  #define defFilePreSpaceTest   ( ( (defFilePreSpaceFixed) == (FileSpace(VN_01)) ) &&  ( (defFilePreSpaceFixed) == (FileSpace(VN_02)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_03)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_04)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_05)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_06)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_07)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_08)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_09)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_10)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_11)) ) )
#elif defFsNumberOfPreFiles == 13
  #define defTotalPreFileSpace  ( (Tuint16) FileSpace(VN_00) + FileSpace(VN_01) + FileSpace(VN_02) + FileSpace(VN_03) + FileSpace(VN_04) + FileSpace(VN_05) + FileSpace(VN_06) + FileSpace(VN_07) + FileSpace(VN_08) + FileSpace(VN_09) + FileSpace(VN_10) + FileSpace(VN_11) + FileSpace(VN_12) )
  #define defFilePreSpaceFixed  ( FileSpace(VN_00) )
  #define defFilePreSpaceTest   ( ( (defFilePreSpaceFixed) == (FileSpace(VN_01)) ) &&  ( (defFilePreSpaceFixed) == (FileSpace(VN_02)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_03)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_04)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_05)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_06)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_07)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_08)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_09)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_10)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_11)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_12)) ) )
#elif defFsNumberOfPreFiles == 14
  #define defTotalPreFileSpace  ( (Tuint16) FileSpace(VN_00) + FileSpace(VN_01) + FileSpace(VN_02) + FileSpace(VN_03) + FileSpace(VN_04) + FileSpace(VN_05) + FileSpace(VN_06) + FileSpace(VN_07) + FileSpace(VN_08) + FileSpace(VN_09) + FileSpace(VN_10) + FileSpace(VN_11) + FileSpace(VN_12) + FileSpace(VN_13) )
  #define defFilePreSpaceFixed  ( FileSpace(VN_00) )
  #define defFilePreSpaceTest   ( ( (defFilePreSpaceFixed) == (FileSpace(VN_01)) ) &&  ( (defFilePreSpaceFixed) == (FileSpace(VN_02)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_03)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_04)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_05)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_06)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_07)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_08)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_09)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_10)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_11)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_12)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_13)) ) )
#elif defFsNumberOfPreFiles == 15
  #define defTotalPreFileSpace  ( (Tuint16) FileSpace(VN_00) + FileSpace(VN_01) + FileSpace(VN_02) + FileSpace(VN_03) + FileSpace(VN_04) + FileSpace(VN_05) + FileSpace(VN_06) + FileSpace(VN_07) + FileSpace(VN_08) + FileSpace(VN_09) + FileSpace(VN_10) + FileSpace(VN_11) + FileSpace(VN_12) + FileSpace(VN_13) + FileSpace(VN_14) )
  #define defFilePreSpaceFixed  ( FileSpace(VN_00) )
  #define defFilePreSpaceTest   ( ( (defFilePreSpaceFixed) == (FileSpace(VN_01)) ) &&  ( (defFilePreSpaceFixed) == (FileSpace(VN_02)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_03)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_04)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_05)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_06)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_07)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_08)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_09)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_10)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_11)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_12)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_13)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_14)) ) )
#elif defFsNumberOfPreFiles == 16
  #define defTotalPreFileSpace  ( (Tuint16) FileSpace(VN_00) + FileSpace(VN_01) + FileSpace(VN_02) + FileSpace(VN_03) + FileSpace(VN_04) + FileSpace(VN_05) + FileSpace(VN_06) + FileSpace(VN_07) + FileSpace(VN_08) + FileSpace(VN_09) + FileSpace(VN_10) + FileSpace(VN_11) + FileSpace(VN_12) + FileSpace(VN_13) + FileSpace(VN_14) + FileSpace(VN_15) )
  #define defFilePreSpaceFixed  ( FileSpace(VN_00) )
  #define defFilePreSpaceTest   ( ( (defFilePreSpaceFixed) == (FileSpace(VN_01)) ) &&  ( (defFilePreSpaceFixed) == (FileSpace(VN_02)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_03)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_04)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_05)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_06)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_07)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_08)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_09)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_10)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_11)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_12)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_13)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_14)) ) && ( (defFilePreSpaceFixed) == (FileSpace(VN_15)) ) )
#endif


#if (FileSpaceStandard > 0)
  #if (cfgUseFileSystemMaintainFAT == cfgTrue)
    #define defFsNumberOfAllFiles   ( ((Tuint16) cfgSysFsStorageSpace - defTotalPreFileSpace + ((Tuint16) defFsNumberOfPreFiles * FileSpaceStandard)) / ((Tuint16) 1+FileSpaceStandard) )
    #define defTotalFatFileSpace    ( defFsNumberOfAllFiles )
  #else
    #define defFsNumberOfAllFiles   ( ((Tuint16) cfgSysFsStorageSpace - defTotalPreFileSpace + ((Tuint16) defFsNumberOfPreFiles * FileSpaceStandard)) / ((Tuint16) FileSpaceStandard) )
    #define defTotalFatFileSpace    0
  #endif
  #define defFsNumberOfDefFiles     ( (defFsNumberOfAllFiles) - (defFsNumberOfPreFiles) )
  #define defTotalDefFileSpace      ( (defFsNumberOfDefFiles) * (FileSpaceStandard) )
  #define defTotalAllFileSpace      ( (defTotalFatFileSpace) + (defTotalPreFileSpace) + (defTotalDefFileSpace) )
#else
  #define defFsNumberOfAllFiles     ( defFsNumberOfPreFiles )
  #define defFsNumberOfDefFiles     0
  #define defTotalFatFileSpace      0
  #define defTotalDefFileSpace      0
  #define defTotalAllFileSpace      ( defTotalPreFileSpace )
#endif

#define defFileSpaceFreeTotal     ( cfgSysFsStorageSpace - defTotalAllFileSpace)
#define defFileSpaceFreeStart     ( defTotalAllFileSpace )

#if defFilePreSpaceTest
   #define defFilePreSpaceConstant cfgTrue
#else
   #define defFilePreSpaceConstant cfgFalse
#endif

#else

#define defFsNumberOfPreFiles       0
#define defFsNumberOfDefFiles       0
#define defFsNumberOfAllFiles       0
#define defFileSpaceFreeTotal       ( cfgSysFsStorageSpace )
#define defFileSpaceFreeStart       0

#endif


#if (cfgUseTaskWatchdog == cfgTrue)

  #if defined(TN_00) && (TaskInclude(TN_00) != cfgExclude) && (((Capabilities(TN_00)) & cfgCapWatchdog) == cfgCapWatchdog)
    #define defTaskNumberWatchdogBegin  0
    #define BN_00 TN_00
    #define BN_01 TN_01
    #define BN_02 TN_02
    #define BN_03 TN_03
    #define BN_04 TN_04
    #define BN_05 TN_05
    #define BN_06 TN_06
    #define BN_07 TN_07
    #define BN_08 TN_08
    #define BN_09 TN_09
    #define BN_10 TN_10
    #define BN_11 TN_11
    #define BN_12 TN_12
    #define BN_13 TN_13
    #define BN_14 TN_14
    #define BN_15 TN_15
  #elif defined(TN_01) && (TaskInclude(TN_01) != cfgExclude) && (((Capabilities(TN_01)) & cfgCapWatchdog) == cfgCapWatchdog)
    #define defTaskNumberWatchdogBegin  1
    #define BN_00 TN_01
    #define BN_01 TN_02
    #define BN_02 TN_03
    #define BN_03 TN_04
    #define BN_04 TN_05
    #define BN_05 TN_06
    #define BN_06 TN_07
    #define BN_07 TN_08
    #define BN_08 TN_09
    #define BN_09 TN_10
    #define BN_10 TN_11
    #define BN_11 TN_12
    #define BN_12 TN_13
    #define BN_13 TN_14
    #define BN_14 TN_15
  #elif defined(TN_02) && (TaskInclude(TN_02) != cfgExclude) && (((Capabilities(TN_02)) & cfgCapWatchdog) == cfgCapWatchdog)
    #define defTaskNumberWatchdogBegin  2
    #define BN_00 TN_02
    #define BN_01 TN_03
    #define BN_02 TN_04
    #define BN_03 TN_05
    #define BN_04 TN_06
    #define BN_05 TN_07
    #define BN_06 TN_08
    #define BN_07 TN_09
    #define BN_08 TN_10
    #define BN_09 TN_11
    #define BN_10 TN_12
    #define BN_11 TN_13
    #define BN_12 TN_14
    #define BN_13 TN_15
  #elif defined(TN_03) && (TaskInclude(TN_03) != cfgExclude) && (((Capabilities(TN_03)) & cfgCapWatchdog) == cfgCapWatchdog)
    #define defTaskNumberWatchdogBegin  3
    #define BN_00 TN_03
    #define BN_01 TN_04
    #define BN_02 TN_05
    #define BN_03 TN_06
    #define BN_04 TN_07
    #define BN_05 TN_08
    #define BN_06 TN_09
    #define BN_07 TN_10
    #define BN_08 TN_11
    #define BN_09 TN_12
    #define BN_10 TN_13
    #define BN_11 TN_14
    #define BN_12 TN_15
  #elif defined(TN_04) && (TaskInclude(TN_04) != cfgExclude) && (((Capabilities(TN_04)) & cfgCapWatchdog) == cfgCapWatchdog)
    #define defTaskNumberWatchdogBegin  4
    #define BN_00 TN_04
    #define BN_01 TN_05
    #define BN_02 TN_06
    #define BN_03 TN_07
    #define BN_04 TN_08
    #define BN_05 TN_09
    #define BN_06 TN_10
    #define BN_07 TN_11
    #define BN_08 TN_12
    #define BN_09 TN_13
    #define BN_10 TN_14
    #define BN_11 TN_15
  #elif defined(TN_05) && (TaskInclude(TN_05) != cfgExclude) && (((Capabilities(TN_05)) & cfgCapWatchdog) == cfgCapWatchdog)
    #define defTaskNumberWatchdogBegin  5
    #define BN_00 TN_05
    #define BN_01 TN_06
    #define BN_02 TN_07
    #define BN_03 TN_08
    #define BN_04 TN_09
    #define BN_05 TN_10
    #define BN_06 TN_11
    #define BN_07 TN_12
    #define BN_08 TN_13
    #define BN_09 TN_14
    #define BN_10 TN_15
  #elif defined(TN_06) && (TaskInclude(TN_06) != cfgExclude) && (((Capabilities(TN_06)) & cfgCapWatchdog) == cfgCapWatchdog)
    #define defTaskNumberWatchdogBegin  6
    #define BN_00 TN_06
    #define BN_01 TN_07
    #define BN_02 TN_08
    #define BN_03 TN_09
    #define BN_04 TN_10
    #define BN_05 TN_11
    #define BN_06 TN_12
    #define BN_07 TN_13
    #define BN_08 TN_14
    #define BN_09 TN_15
  #elif defined(TN_07) && (TaskInclude(TN_07) != cfgExclude) && (((Capabilities(TN_07)) & cfgCapWatchdog) == cfgCapWatchdog)
    #define defTaskNumberWatchdogBegin  7
    #define BN_00 TN_07
    #define BN_01 TN_08
    #define BN_02 TN_09
    #define BN_03 TN_10
    #define BN_04 TN_11
    #define BN_05 TN_12
    #define BN_06 TN_13
    #define BN_07 TN_14
    #define BN_08 TN_15
  #elif defined(TN_08) && (TaskInclude(TN_08) != cfgExclude) && (((Capabilities(TN_08)) & cfgCapWatchdog) == cfgCapWatchdog)
    #define defTaskNumberWatchdogBegin  8
    #define BN_00 TN_08
    #define BN_01 TN_09
    #define BN_02 TN_10
    #define BN_03 TN_11
    #define BN_04 TN_12
    #define BN_05 TN_13
    #define BN_06 TN_14
    #define BN_07 TN_15
  #elif defined(TN_09) && (TaskInclude(TN_09) != cfgExclude) && (((Capabilities(TN_09)) & cfgCapWatchdog) == cfgCapWatchdog)
    #define defTaskNumberWatchdogBegin  9
    #define BN_00 TN_09
    #define BN_01 TN_10
    #define BN_02 TN_11
    #define BN_03 TN_12
    #define BN_04 TN_13
    #define BN_05 TN_14
    #define BN_06 TN_15
  #elif defined(TN_10) && (TaskInclude(TN_10) != cfgExclude) && (((Capabilities(TN_10)) & cfgCapWatchdog) == cfgCapWatchdog)
    #define defTaskNumberWatchdogBegin 10
    #define BN_00 TN_10
    #define BN_01 TN_11
    #define BN_02 TN_12
    #define BN_03 TN_13
    #define BN_04 TN_14
    #define BN_05 TN_15
  #elif defined(TN_11) && (TaskInclude(TN_11) != cfgExclude) && (((Capabilities(TN_11)) & cfgCapWatchdog) == cfgCapWatchdog)
    #define defTaskNumberWatchdogBegin 11
    #define BN_00 TN_11
    #define BN_01 TN_12
    #define BN_02 TN_13
    #define BN_03 TN_14
    #define BN_04 TN_15
  #elif defined(TN_12) && (TaskInclude(TN_12) != cfgExclude) && (((Capabilities(TN_12)) & cfgCapWatchdog) == cfgCapWatchdog)
    #define defTaskNumberWatchdogBegin 12
    #define BN_00 TN_12
    #define BN_01 TN_13
    #define BN_02 TN_14
    #define BN_03 TN_15
  #elif defined(TN_13) && (TaskInclude(TN_13) != cfgExclude) && (((Capabilities(TN_13)) & cfgCapWatchdog) == cfgCapWatchdog)
    #define defTaskNumberWatchdogBegin 13
    #define BN_00 TN_13
    #define BN_01 TN_14
    #define BN_02 TN_15
  #elif defined(TN_14) && (TaskInclude(TN_14) != cfgExclude) && (((Capabilities(TN_14)) & cfgCapWatchdog) == cfgCapWatchdog)
    #define defTaskNumberWatchdogBegin 14
    #define BN_00 TN_14
    #define BN_01 TN_15
  #elif defined(TN_15) && (TaskInclude(TN_15) != cfgExclude) && (((Capabilities(TN_15)) & cfgCapWatchdog) == cfgCapWatchdog)
    #define defTaskNumberWatchdogBegin 15
    #define BN_00 TN_15
  #else
    #define defTaskNumberWatchdogBegin 16
  #endif

  #if defined(TN_15) && (TaskInclude(TN_15) != cfgExclude) && (((Capabilities(TN_15)) & cfgCapWatchdog) == cfgCapWatchdog)
    #define defTaskNumberWatchdogEnd  16
  #elif defined(TN_14) && (TaskInclude(TN_14) != cfgExclude) && (((Capabilities(TN_14)) & cfgCapWatchdog) == cfgCapWatchdog)
    #define defTaskNumberWatchdogEnd  15
  #elif defined(TN_13) && (TaskInclude(TN_13) != cfgExclude) && (((Capabilities(TN_13)) & cfgCapWatchdog) == cfgCapWatchdog)
    #define defTaskNumberWatchdogEnd  14
  #elif defined(TN_12) && (TaskInclude(TN_12) != cfgExclude) && (((Capabilities(TN_12)) & cfgCapWatchdog) == cfgCapWatchdog)
    #define defTaskNumberWatchdogEnd  13
  #elif defined(TN_11) && (TaskInclude(TN_11) != cfgExclude) && (((Capabilities(TN_11)) & cfgCapWatchdog) == cfgCapWatchdog)
    #define defTaskNumberWatchdogEnd  12
  #elif defined(TN_10) && (TaskInclude(TN_10) != cfgExclude) && (((Capabilities(TN_10)) & cfgCapWatchdog) == cfgCapWatchdog)
    #define defTaskNumberWatchdogEnd  11
  #elif defined(TN_09) && (TaskInclude(TN_09) != cfgExclude) && (((Capabilities(TN_09)) & cfgCapWatchdog) == cfgCapWatchdog)
    #define defTaskNumberWatchdogEnd  10
  #elif defined(TN_08) && (TaskInclude(TN_08) != cfgExclude) && (((Capabilities(TN_08)) & cfgCapWatchdog) == cfgCapWatchdog)
    #define defTaskNumberWatchdogEnd   9
  #elif defined(TN_07) && (TaskInclude(TN_07) != cfgExclude) && (((Capabilities(TN_07)) & cfgCapWatchdog) == cfgCapWatchdog)
    #define defTaskNumberWatchdogEnd   8
  #elif defined(TN_06) && (TaskInclude(TN_06) != cfgExclude) && (((Capabilities(TN_06)) & cfgCapWatchdog) == cfgCapWatchdog)
    #define defTaskNumberWatchdogEnd   7
  #elif defined(TN_05) && (TaskInclude(TN_05) != cfgExclude) && (((Capabilities(TN_05)) & cfgCapWatchdog) == cfgCapWatchdog)
    #define defTaskNumberWatchdogEnd   6
  #elif defined(TN_04) && (TaskInclude(TN_04) != cfgExclude) && (((Capabilities(TN_04)) & cfgCapWatchdog) == cfgCapWatchdog)
    #define defTaskNumberWatchdogEnd   5
  #elif defined(TN_03) && (TaskInclude(TN_03) != cfgExclude) && (((Capabilities(TN_03)) & cfgCapWatchdog) == cfgCapWatchdog)
    #define defTaskNumberWatchdogEnd   4
  #elif defined(TN_02) && (TaskInclude(TN_02) != cfgExclude) && (((Capabilities(TN_02)) & cfgCapWatchdog) == cfgCapWatchdog)
    #define defTaskNumberWatchdogEnd   3
  #elif defined(TN_01) && (TaskInclude(TN_01) != cfgExclude) && (((Capabilities(TN_01)) & cfgCapWatchdog) == cfgCapWatchdog)
    #define defTaskNumberWatchdogEnd   2
  #elif defined(TN_00) && (TaskInclude(TN_00) != cfgExclude) && (((Capabilities(TN_00)) & cfgCapWatchdog) == cfgCapWatchdog)
    #define defTaskNumberWatchdogEnd   1
  #else
    #define defTaskNumberWatchdogEnd   0
  #endif

  #if (defTaskNumberWatchdogEnd - defTaskNumberWatchdogBegin) > 0
   #define defNumberOfWatchdogTasks (defTaskNumberWatchdogEnd - defTaskNumberWatchdogBegin)
  #else
   #define defNumberOfWatchdogTasks 0
  #endif

#else

  #define defNumberOfWatchdogTasks 0

#endif


#if (cfgUseDelay == cfgTrue)

  #if defined(TN_00) && (TaskInclude(TN_00) != cfgExclude) && (((Capabilities(TN_00)) & cfgCapDelay) == cfgCapDelay)
    #define defTaskNumberDelayBegin  0
  #elif defined(TN_01) && (TaskInclude(TN_01) != cfgExclude) && (((Capabilities(TN_01)) & cfgCapDelay) == cfgCapDelay)
    #define defTaskNumberDelayBegin  1
  #elif defined(TN_02) && (TaskInclude(TN_02) != cfgExclude) && (((Capabilities(TN_02)) & cfgCapDelay) == cfgCapDelay)
    #define defTaskNumberDelayBegin  2
  #elif defined(TN_03) && (TaskInclude(TN_03) != cfgExclude) && (((Capabilities(TN_03)) & cfgCapDelay) == cfgCapDelay)
    #define defTaskNumberDelayBegin  3
  #elif defined(TN_04) && (TaskInclude(TN_04) != cfgExclude) && (((Capabilities(TN_04)) & cfgCapDelay) == cfgCapDelay)
    #define defTaskNumberDelayBegin  4
  #elif defined(TN_05) && (TaskInclude(TN_05) != cfgExclude) && (((Capabilities(TN_05)) & cfgCapDelay) == cfgCapDelay)
    #define defTaskNumberDelayBegin  5
  #elif defined(TN_06) && (TaskInclude(TN_06) != cfgExclude) && (((Capabilities(TN_06)) & cfgCapDelay) == cfgCapDelay)
    #define defTaskNumberDelayBegin  6
  #elif defined(TN_07) && (TaskInclude(TN_07) != cfgExclude) && (((Capabilities(TN_07)) & cfgCapDelay) == cfgCapDelay)
    #define defTaskNumberDelayBegin  7
  #elif defined(TN_08) && (TaskInclude(TN_08) != cfgExclude) && (((Capabilities(TN_08)) & cfgCapDelay) == cfgCapDelay)
    #define defTaskNumberDelayBegin  8
  #elif defined(TN_09) && (TaskInclude(TN_09) != cfgExclude) && (((Capabilities(TN_09)) & cfgCapDelay) == cfgCapDelay)
    #define defTaskNumberDelayBegin  9
  #elif defined(TN_10) && (TaskInclude(TN_10) != cfgExclude) && (((Capabilities(TN_10)) & cfgCapDelay) == cfgCapDelay)
    #define defTaskNumberDelayBegin 10
  #elif defined(TN_11) && (TaskInclude(TN_11) != cfgExclude) && (((Capabilities(TN_11)) & cfgCapDelay) == cfgCapDelay)
    #define defTaskNumberDelayBegin 11
  #elif defined(TN_12) && (TaskInclude(TN_12) != cfgExclude) && (((Capabilities(TN_12)) & cfgCapDelay) == cfgCapDelay)
    #define defTaskNumberDelayBegin 12
  #elif defined(TN_13) && (TaskInclude(TN_13) != cfgExclude) && (((Capabilities(TN_13)) & cfgCapDelay) == cfgCapDelay)
    #define defTaskNumberDelayBegin 13
  #elif defined(TN_14) && (TaskInclude(TN_14) != cfgExclude) && (((Capabilities(TN_14)) & cfgCapDelay) == cfgCapDelay)
    #define defTaskNumberDelayBegin 14
  #elif defined(TN_15) && (TaskInclude(TN_15) != cfgExclude) && (((Capabilities(TN_15)) & cfgCapDelay) == cfgCapDelay)
    #define defTaskNumberDelayBegin 15
  #else
    #define defTaskNumberDelayBegin 16
  #endif

  #if defined(TN_15) && (TaskInclude(TN_15) != cfgExclude) && (((Capabilities(TN_15)) & cfgCapDelay) == cfgCapDelay)
    #define defTaskNumberDelayEnd  16
  #elif defined(TN_14) && (TaskInclude(TN_14) != cfgExclude) && (((Capabilities(TN_14)) & cfgCapDelay) == cfgCapDelay)
    #define defTaskNumberDelayEnd  15
  #elif defined(TN_13) && (TaskInclude(TN_13) != cfgExclude) && (((Capabilities(TN_13)) & cfgCapDelay) == cfgCapDelay)
    #define defTaskNumberDelayEnd  14
  #elif defined(TN_12) && (TaskInclude(TN_12) != cfgExclude) && (((Capabilities(TN_12)) & cfgCapDelay) == cfgCapDelay)
    #define defTaskNumberDelayEnd  13
  #elif defined(TN_11) && (TaskInclude(TN_11) != cfgExclude) && (((Capabilities(TN_11)) & cfgCapDelay) == cfgCapDelay)
    #define defTaskNumberDelayEnd  12
  #elif defined(TN_10) && (TaskInclude(TN_10) != cfgExclude) && (((Capabilities(TN_10)) & cfgCapDelay) == cfgCapDelay)
    #define defTaskNumberDelayEnd  11
  #elif defined(TN_09) && (TaskInclude(TN_09) != cfgExclude) && (((Capabilities(TN_09)) & cfgCapDelay) == cfgCapDelay)
    #define defTaskNumberDelayEnd  10
  #elif defined(TN_08) && (TaskInclude(TN_08) != cfgExclude) && (((Capabilities(TN_08)) & cfgCapDelay) == cfgCapDelay)
    #define defTaskNumberDelayEnd   9
  #elif defined(TN_07) && (TaskInclude(TN_07) != cfgExclude) && (((Capabilities(TN_07)) & cfgCapDelay) == cfgCapDelay)
    #define defTaskNumberDelayEnd   8
  #elif defined(TN_06) && (TaskInclude(TN_06) != cfgExclude) && (((Capabilities(TN_06)) & cfgCapDelay) == cfgCapDelay)
    #define defTaskNumberDelayEnd   7
  #elif defined(TN_05) && (TaskInclude(TN_05) != cfgExclude) && (((Capabilities(TN_05)) & cfgCapDelay) == cfgCapDelay)
    #define defTaskNumberDelayEnd   6
  #elif defined(TN_04) && (TaskInclude(TN_04) != cfgExclude) && (((Capabilities(TN_04)) & cfgCapDelay) == cfgCapDelay)
    #define defTaskNumberDelayEnd   5
  #elif defined(TN_03) && (TaskInclude(TN_03) != cfgExclude) && (((Capabilities(TN_03)) & cfgCapDelay) == cfgCapDelay)
    #define defTaskNumberDelayEnd   4
  #elif defined(TN_02) && (TaskInclude(TN_02) != cfgExclude) && (((Capabilities(TN_02)) & cfgCapDelay) == cfgCapDelay)
    #define defTaskNumberDelayEnd   3
  #elif defined(TN_01) && (TaskInclude(TN_01) != cfgExclude) && (((Capabilities(TN_01)) & cfgCapDelay) == cfgCapDelay)
    #define defTaskNumberDelayEnd   2
  #elif defined(TN_00) && (TaskInclude(TN_00) != cfgExclude) && (((Capabilities(TN_00)) & cfgCapDelay) == cfgCapDelay)
    #define defTaskNumberDelayEnd   1
  #else
    #define defTaskNumberDelayEnd   0
  #endif

  #if (defTaskNumberDelayEnd - defTaskNumberDelayBegin) > 0
   #define defNumberOfDelayTasks (defTaskNumberDelayEnd - defTaskNumberDelayBegin)
  #else
   #define defNumberOfDelayTasks 0
  #endif

#else

  #define defNumberOfDelayTasks 0

#endif


#if (cfgUseSynchronization == cfgTrue)

   #define defTaskNumberSynchronizationBegin  0
   #define defTaskNumberSynchronizationEnd    defNumberOfTasksWithSlot
   #define defNumberOfSynchronizationTasks    defNumberOfTasksWithSlot

#endif


#if (cfgUseFileSystem == cfgTrue)

  #if defined(TN_00) && (TaskInclude(TN_00) != cfgExclude) && (((Capabilities(TN_00)) & cfgCapFileSystem) == cfgCapFileSystem)
    #define defTaskNumberFileSystemBegin  0
  #elif defined(TN_01) && (TaskInclude(TN_01) != cfgExclude) && (((Capabilities(TN_01)) & cfgCapFileSystem) == cfgCapFileSystem)
    #define defTaskNumberFileSystemBegin  1
  #elif defined(TN_02) && (TaskInclude(TN_02) != cfgExclude) && (((Capabilities(TN_02)) & cfgCapFileSystem) == cfgCapFileSystem)
    #define defTaskNumberFileSystemBegin  2
  #elif defined(TN_03) && (TaskInclude(TN_03) != cfgExclude) && (((Capabilities(TN_03)) & cfgCapFileSystem) == cfgCapFileSystem)
    #define defTaskNumberFileSystemBegin  3
  #elif defined(TN_04) && (TaskInclude(TN_04) != cfgExclude) && (((Capabilities(TN_04)) & cfgCapFileSystem) == cfgCapFileSystem)
    #define defTaskNumberFileSystemBegin  4
  #elif defined(TN_05) && (TaskInclude(TN_05) != cfgExclude) && (((Capabilities(TN_05)) & cfgCapFileSystem) == cfgCapFileSystem)
    #define defTaskNumberFileSystemBegin  5
  #elif defined(TN_06) && (TaskInclude(TN_06) != cfgExclude) && (((Capabilities(TN_06)) & cfgCapFileSystem) == cfgCapFileSystem)
    #define defTaskNumberFileSystemBegin  6
  #elif defined(TN_07) && (TaskInclude(TN_07) != cfgExclude) && (((Capabilities(TN_07)) & cfgCapFileSystem) == cfgCapFileSystem)
    #define defTaskNumberFileSystemBegin  7
  #elif defined(TN_08) && (TaskInclude(TN_08) != cfgExclude) && (((Capabilities(TN_08)) & cfgCapFileSystem) == cfgCapFileSystem)
    #define defTaskNumberFileSystemBegin  8
  #elif defined(TN_09) && (TaskInclude(TN_09) != cfgExclude) && (((Capabilities(TN_09)) & cfgCapFileSystem) == cfgCapFileSystem)
    #define defTaskNumberFileSystemBegin  9
  #elif defined(TN_10) && (TaskInclude(TN_10) != cfgExclude) && (((Capabilities(TN_10)) & cfgCapFileSystem) == cfgCapFileSystem)
    #define defTaskNumberFileSystemBegin 10
  #elif defined(TN_11) && (TaskInclude(TN_11) != cfgExclude) && (((Capabilities(TN_11)) & cfgCapFileSystem) == cfgCapFileSystem)
    #define defTaskNumberFileSystemBegin 11
  #elif defined(TN_12) && (TaskInclude(TN_12) != cfgExclude) && (((Capabilities(TN_12)) & cfgCapFileSystem) == cfgCapFileSystem)
    #define defTaskNumberFileSystemBegin 12
  #elif defined(TN_13) && (TaskInclude(TN_13) != cfgExclude) && (((Capabilities(TN_13)) & cfgCapFileSystem) == cfgCapFileSystem)
    #define defTaskNumberFileSystemBegin 13
  #elif defined(TN_14) && (TaskInclude(TN_14) != cfgExclude) && (((Capabilities(TN_14)) & cfgCapFileSystem) == cfgCapFileSystem)
    #define defTaskNumberFileSystemBegin 14
  #elif defined(TN_15) && (TaskInclude(TN_15) != cfgExclude) && (((Capabilities(TN_15)) & cfgCapFileSystem) == cfgCapFileSystem)
    #define defTaskNumberFileSystemBegin 15
  #else
    #define defTaskNumberFileSystemBegin 16
  #endif

  #if defined(TN_15) && (TaskInclude(TN_15) != cfgExclude) && (((Capabilities(TN_15)) & cfgCapFileSystem) == cfgCapFileSystem)
    #define defTaskNumberFileSystemEnd  16
  #elif defined(TN_14) && (TaskInclude(TN_14) != cfgExclude) && (((Capabilities(TN_14)) & cfgCapFileSystem) == cfgCapFileSystem)
    #define defTaskNumberFileSystemEnd  15
  #elif defined(TN_13) && (TaskInclude(TN_13) != cfgExclude) && (((Capabilities(TN_13)) & cfgCapFileSystem) == cfgCapFileSystem)
    #define defTaskNumberFileSystemEnd  14
  #elif defined(TN_12) && (TaskInclude(TN_12) != cfgExclude) && (((Capabilities(TN_12)) & cfgCapFileSystem) == cfgCapFileSystem)
    #define defTaskNumberFileSystemEnd  13
  #elif defined(TN_11) && (TaskInclude(TN_11) != cfgExclude) && (((Capabilities(TN_11)) & cfgCapFileSystem) == cfgCapFileSystem)
    #define defTaskNumberFileSystemEnd  12
  #elif defined(TN_10) && (TaskInclude(TN_10) != cfgExclude) && (((Capabilities(TN_10)) & cfgCapFileSystem) == cfgCapFileSystem)
    #define defTaskNumberFileSystemEnd  11
  #elif defined(TN_09) && (TaskInclude(TN_09) != cfgExclude) && (((Capabilities(TN_09)) & cfgCapFileSystem) == cfgCapFileSystem)
    #define defTaskNumberFileSystemEnd  10
  #elif defined(TN_08) && (TaskInclude(TN_08) != cfgExclude) && (((Capabilities(TN_08)) & cfgCapFileSystem) == cfgCapFileSystem)
    #define defTaskNumberFileSystemEnd   9
  #elif defined(TN_07) && (TaskInclude(TN_07) != cfgExclude) && (((Capabilities(TN_07)) & cfgCapFileSystem) == cfgCapFileSystem)
    #define defTaskNumberFileSystemEnd   8
  #elif defined(TN_06) && (TaskInclude(TN_06) != cfgExclude) && (((Capabilities(TN_06)) & cfgCapFileSystem) == cfgCapFileSystem)
    #define defTaskNumberFileSystemEnd   7
  #elif defined(TN_05) && (TaskInclude(TN_05) != cfgExclude) && (((Capabilities(TN_05)) & cfgCapFileSystem) == cfgCapFileSystem)
    #define defTaskNumberFileSystemEnd   6
  #elif defined(TN_04) && (TaskInclude(TN_04) != cfgExclude) && (((Capabilities(TN_04)) & cfgCapFileSystem) == cfgCapFileSystem)
    #define defTaskNumberFileSystemEnd   5
  #elif defined(TN_03) && (TaskInclude(TN_03) != cfgExclude) && (((Capabilities(TN_03)) & cfgCapFileSystem) == cfgCapFileSystem)
    #define defTaskNumberFileSystemEnd   4
  #elif defined(TN_02) && (TaskInclude(TN_02) != cfgExclude) && (((Capabilities(TN_02)) & cfgCapFileSystem) == cfgCapFileSystem)
    #define defTaskNumberFileSystemEnd   3
  #elif defined(TN_01) && (TaskInclude(TN_01) != cfgExclude) && (((Capabilities(TN_01)) & cfgCapFileSystem) == cfgCapFileSystem)
    #define defTaskNumberFileSystemEnd   2
  #elif defined(TN_00) && (TaskInclude(TN_00) != cfgExclude) && (((Capabilities(TN_00)) & cfgCapFileSystem) == cfgCapFileSystem)
    #define defTaskNumberFileSystemEnd   1
  #else
    #define defTaskNumberFileSystemEnd   0
  #endif

  #if (defTaskNumberFileSystemEnd - defTaskNumberFileSystemBegin) > 0
   #define defNumberOfFileSystemTasks (defTaskNumberFileSystemEnd - defTaskNumberFileSystemBegin)
  #else
   #define defNumberOfFileSystemTasks 0
  #endif

  #if (defTaskNumberFileSystemEnd - defTaskNumberFileSystemBegin) == 1
    #define defUseFsOnMultipleTasks                cfgFalse
    #define defUseFsSingleTaskNumber               defTaskNumberFileSystemBegin
  #else
    #define defUseFsOnMultipleTasks                cfgTrue
  #endif

#else

  #define defNumberOfFileSystemTasks 0

#endif


#if (cfgUseEvents == cfgTrue)

  #if defined(TN_00) && (TaskInclude(TN_00) != cfgExclude) && (((Capabilities(TN_00)) & cfgCapEvent) == cfgCapEvent)
    #define defTaskNumberEventBegin  0
  #elif defined(TN_01) && (TaskInclude(TN_01) != cfgExclude) && (((Capabilities(TN_01)) & cfgCapEvent) == cfgCapEvent)
    #define defTaskNumberEventBegin  1
  #elif defined(TN_02) && (TaskInclude(TN_02) != cfgExclude) && (((Capabilities(TN_02)) & cfgCapEvent) == cfgCapEvent)
    #define defTaskNumberEventBegin  2
  #elif defined(TN_03) && (TaskInclude(TN_03) != cfgExclude) && (((Capabilities(TN_03)) & cfgCapEvent) == cfgCapEvent)
    #define defTaskNumberEventBegin  3
  #elif defined(TN_04) && (TaskInclude(TN_04) != cfgExclude) && (((Capabilities(TN_04)) & cfgCapEvent) == cfgCapEvent)
    #define defTaskNumberEventBegin  4
  #elif defined(TN_05) && (TaskInclude(TN_05) != cfgExclude) && (((Capabilities(TN_05)) & cfgCapEvent) == cfgCapEvent)
    #define defTaskNumberEventBegin  5
  #elif defined(TN_06) && (TaskInclude(TN_06) != cfgExclude) && (((Capabilities(TN_06)) & cfgCapEvent) == cfgCapEvent)
    #define defTaskNumberEventBegin  6
  #elif defined(TN_07) && (TaskInclude(TN_07) != cfgExclude) && (((Capabilities(TN_07)) & cfgCapEvent) == cfgCapEvent)
    #define defTaskNumberEventBegin  7
  #elif defined(TN_08) && (TaskInclude(TN_08) != cfgExclude) && (((Capabilities(TN_08)) & cfgCapEvent) == cfgCapEvent)
    #define defTaskNumberEventBegin  8
  #elif defined(TN_09) && (TaskInclude(TN_09) != cfgExclude) && (((Capabilities(TN_09)) & cfgCapEvent) == cfgCapEvent)
    #define defTaskNumberEventBegin  9
  #elif defined(TN_10) && (TaskInclude(TN_10) != cfgExclude) && (((Capabilities(TN_10)) & cfgCapEvent) == cfgCapEvent)
    #define defTaskNumberEventBegin 10
  #elif defined(TN_11) && (TaskInclude(TN_11) != cfgExclude) && (((Capabilities(TN_11)) & cfgCapEvent) == cfgCapEvent)
    #define defTaskNumberEventBegin 11
  #elif defined(TN_12) && (TaskInclude(TN_12) != cfgExclude) && (((Capabilities(TN_12)) & cfgCapEvent) == cfgCapEvent)
    #define defTaskNumberEventBegin 12
  #elif defined(TN_13) && (TaskInclude(TN_13) != cfgExclude) && (((Capabilities(TN_13)) & cfgCapEvent) == cfgCapEvent)
    #define defTaskNumberEventBegin 13
  #elif defined(TN_14) && (TaskInclude(TN_14) != cfgExclude) && (((Capabilities(TN_14)) & cfgCapEvent) == cfgCapEvent)
    #define defTaskNumberEventBegin 14
  #elif defined(TN_15) && (TaskInclude(TN_15) != cfgExclude) && (((Capabilities(TN_15)) & cfgCapEvent) == cfgCapEvent)
    #define defTaskNumberEventBegin 15
  #else
    #define defTaskNumberEventBegin 16
  #endif

  #if defined(TN_15) && (TaskInclude(TN_15) != cfgExclude) && (((Capabilities(TN_15)) & cfgCapEvent) == cfgCapEvent)
    #define defTaskNumberEventEnd  16
  #elif defined(TN_14) && (TaskInclude(TN_14) != cfgExclude) && (((Capabilities(TN_14)) & cfgCapEvent) == cfgCapEvent)
    #define defTaskNumberEventEnd  15
  #elif defined(TN_13) && (TaskInclude(TN_13) != cfgExclude) && (((Capabilities(TN_13)) & cfgCapEvent) == cfgCapEvent)
    #define defTaskNumberEventEnd  14
  #elif defined(TN_12) && (TaskInclude(TN_12) != cfgExclude) && (((Capabilities(TN_12)) & cfgCapEvent) == cfgCapEvent)
    #define defTaskNumberEventEnd  13
  #elif defined(TN_11) && (TaskInclude(TN_11) != cfgExclude) && (((Capabilities(TN_11)) & cfgCapEvent) == cfgCapEvent)
    #define defTaskNumberEventEnd  12
  #elif defined(TN_10) && (TaskInclude(TN_10) != cfgExclude) && (((Capabilities(TN_10)) & cfgCapEvent) == cfgCapEvent)
    #define defTaskNumberEventEnd  11
  #elif defined(TN_09) && (TaskInclude(TN_09) != cfgExclude) && (((Capabilities(TN_09)) & cfgCapEvent) == cfgCapEvent)
    #define defTaskNumberEventEnd  10
  #elif defined(TN_08) && (TaskInclude(TN_08) != cfgExclude) && (((Capabilities(TN_08)) & cfgCapEvent) == cfgCapEvent)
    #define defTaskNumberEventEnd   9
  #elif defined(TN_07) && (TaskInclude(TN_07) != cfgExclude) && (((Capabilities(TN_07)) & cfgCapEvent) == cfgCapEvent)
    #define defTaskNumberEventEnd   8
  #elif defined(TN_06) && (TaskInclude(TN_06) != cfgExclude) && (((Capabilities(TN_06)) & cfgCapEvent) == cfgCapEvent)
    #define defTaskNumberEventEnd   7
  #elif defined(TN_05) && (TaskInclude(TN_05) != cfgExclude) && (((Capabilities(TN_05)) & cfgCapEvent) == cfgCapEvent)
    #define defTaskNumberEventEnd   6
  #elif defined(TN_04) && (TaskInclude(TN_04) != cfgExclude) && (((Capabilities(TN_04)) & cfgCapEvent) == cfgCapEvent)
    #define defTaskNumberEventEnd   5
  #elif defined(TN_03) && (TaskInclude(TN_03) != cfgExclude) && (((Capabilities(TN_03)) & cfgCapEvent) == cfgCapEvent)
    #define defTaskNumberEventEnd   4
  #elif defined(TN_02) && (TaskInclude(TN_02) != cfgExclude) && (((Capabilities(TN_02)) & cfgCapEvent) == cfgCapEvent)
    #define defTaskNumberEventEnd   3
  #elif defined(TN_01) && (TaskInclude(TN_01) != cfgExclude) && (((Capabilities(TN_01)) & cfgCapEvent) == cfgCapEvent)
    #define defTaskNumberEventEnd   2
  #elif defined(TN_00) && (TaskInclude(TN_00) != cfgExclude) && (((Capabilities(TN_00)) & cfgCapEvent) == cfgCapEvent)
    #define defTaskNumberEventEnd   1
  #else
    #define defTaskNumberEventEnd   0
  #endif

  #if (defTaskNumberEventEnd - defTaskNumberEventBegin) > 0
   #define defNumberOfEventTasks (defTaskNumberEventEnd - defTaskNumberEventBegin)
  #else
   #define defNumberOfEventTasks 0
  #endif

  #define defNumberEventBegin 0
  #define defNumberEventEnd   8

#else

  #define defNumberOfEventTasks 0

#endif


#endif /* FEMTOOS_ORDER_H */
