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

#ifndef FEMTOOS_CHECK_H
#define FEMTOOS_CHECK_H

/* This file checks the data in you config file as far as checking is
 * reasonably possible by preprocessor means. It prevents that typos
 * in the configuration lead to unexpected behavior for example.
 * If you get an compile error in this file you most certainly made
 * a mistake in your configuration file.
 */

#define  cfgMaxSysRegisterCheckByte                 0xFF
#define  cfgMaxSysClockDivider                      0xFFFF
#define  cfgMaxSysSubTicksPerFullTick               0xFFFF
#define  cfgMaxSysSubTickDivider                    0xFFFF
#define  cfgMaxSysFsStorageSpace                    0xFFFF
#define  cfgMaxNumberOfTasks                        16
#define  cfgMaxNumberOfQueus                        15
#define  cfgMaxNumWatchdogDiv                       8
#define  cfgMaxNumMonitorDiv                        8
#define  cfgMaxNumSleepThreshold                    100000
#define  cfgMaxNumSleepPeriod                       100000
#define  cfgMaxNumGlobWidth                         8
#define  cfgMaxNumSwitchWidth                       8
#define  cfgMaxNumTickWidth                         8
#define  MaxCapabilities_Task                       cfgCapAll
#define  MaxStackSafety                             0xFF
#define  MaxStackSizeOS                             0xFF
#define  MaxStackSizeISR                            0x7FFF
#define  MaxStackSize_Task                          0x7FFF
#define  MaxTimeSliceIdleTime                       0xFF
#define  MaxTimeSlice_Task                          0xFF
#define  MaxPriority_Task                           7
#define  MaxRegisterUse_Task                        registersAll
#define  MaxRegisterCheck_Task                      registersAll
#define  MaxQueuSize                                127
#define  MaxFileSpace_Task                          (cfgSysFsStorageSpace)
#define  MaxLevelsWidth                             8

#define  cfgMinSysRegisterCheckByte                 0x00
#define  cfgMinSysClockDivider                      1
#define  cfgMinSysSubTicksPerFullTick               1
#define  cfgMinSysSubTickDivider                    1
#define  cfgMinSysFsStorageSpace                    0
#define  cfgMinNumberOfTasks                        0
#define  cfgMinNumberOfQueus                        0
#define  cfgMinNumWatchdogDiv                       0
#define  cfgMinNumMonitorDiv                        0
#define  cfgMinNumSleepThreshold                    0
#define  cfgMinNumSleepPeriod                       0
#define  cfgMinNumGlobWidth                         0
#define  cfgMinNumSwitchWidth                       0
#define  cfgMinNumTickWidth                         0
#define  MinCapabilities_Task                       cfgCapNon
#define  MinStackSafety                             0
#define  MinStackSizeOS                             (3*(defAddressSize))
#define  MinStackSizeISR                            0
#define  MinTimeSliceIdleTime                       1
#define  MinStackSize_Task                          (2*(defAddressSize))
#define  MinTimeSlice_Task                          1
#define  MinPriority_Task                           0
#define  MinRegisterUse_Task                        registersNon
#define  MinRegisterCheck_Task                      registersNon
#define  MinQueuSize                                0
#define  MinFileSpace_Task                          0

#define  NullSlotSize_Task                          0

#if   (cfgUseSynchronization == cfgSyncSingleSlot)
  #define  MinSlotSize_Task                         1
  #define  MaxSlotSize_Task                         1
#elif (cfgUseSynchronization == cfgSyncSingleBlock)
  #define  MinSlotSize_Task                         2
  #define  MaxSlotSize_Task                         14
#elif (cfgUseSynchronization == cfgSyncDoubleBlock)
  #define  MinSlotSize_Task                         4
  #define  MaxSlotSize_Task                         14
#elif (cfgUseSynchronization == cfgSyncNon)
  #define  MinSlotSize_Task                         1
  #define  MaxSlotSize_Task                         14
#else
  /* You should not come here, if you do cfgUseSynchronization is misspelled. */
  #define  MinSlotSize_Task                         1
  #define  MaxSlotSize_Task                         14
#endif

#if !defined(PriorityOverride) || ((PriorityOverride) == cfgOverrideNon)
  #define ConcatPriority(X)        (Priority_ ## X)
  #define Priority(X)              ConcatPriority(X)
#else
  #define Priority(X)              (PriorityOverride)
#endif

#if !defined(RegisterUseOverride) || ((RegisterUseOverride) == cfgOverrideNon)
  #define ConcatRegisterUse(X)     (RegisterUse_ ## X)
  #define RegisterUse(X)           ConcatRegisterUse(X)
#else
  #define RegisterUse(X)           (RegisterUseOverride)
#endif

#if !defined(RegisterCheckOverride) || ((RegisterCheckOverride) == cfgOverrideNon)
  #define ConcatRegisterCheck(X)   (RegisterCheck_ ## X)
  #define RegisterCheck(X)         ConcatRegisterCheck(X)
#else
  #define RegisterCheck(X)         (RegisterCheckOverride)
#endif

#if !defined(InterruptStartOverride) || ((InterruptStartOverride) == cfgOverrideNon)
  #define ConcatInterruptStart(X)  (InterruptStart_ ## X)
  #define InterruptStart(X)        ConcatInterruptStart(X)
#else
  #define InterruptStart(X)        (InterruptStartOverride)
#endif

#if !defined(StackSizeOverride) || ((StackSizeOverride) == cfgOverrideNon)
  #define ConcatStackSizeDef(X)    (StackSize_ ## X)
  #define StackSizeDef(X)          ConcatStackSizeDef(X)
#else
  #define StackSizeDef(X)          (StackSizeOverride)
#endif

#define StackIsShared(X)           ( (StackSizeDef(X) == cfgShared) )
#define StackSize(X)               ( (StackIsShared(X)) ? (StackSizeShared) : (StackSizeDef(X)) )
#define StackSizeReal(X)           ( (StackIsShared(X)) ? (cfgShared & MaxStackSize_Task) : (StackSizeDef(X)) )

#if !defined(TimeSliceOverride) || ((TimeSliceOverride) == cfgOverrideNon)
  #define ConcatTimeSlice(X)       (TimeSlice_ ## X)
  #define TimeSlice(X)             ConcatTimeSlice(X)
#else
  #define TimeSlice(X)             (TimeSliceOverride)
#endif

#if !defined(SlotSizeOverride) || ((SlotSizeOverride) == cfgOverrideNon)
  #define ConcatSlotSize(X)        (SlotSize_ ## X)
  #define SlotSize(X)              ConcatSlotSize(X)
#else
  #define SlotSize(X)              (SlotSizeOverride)
#endif

#if !defined(FileSpaceOverride) || ((FileSpaceOverride) == cfgOverrideNon)
  #define ConcatFileSpace(X)       (FileSpace_ ## X)
  #define FileSpace(X)             ConcatFileSpace(X)
#else
  #define FileSpace(X)             (FileSpaceOverride)
#endif

#if !defined(QueuSizeOverride) || ((QueuSizeOverride) == cfgOverrideNon)
  #define ConcatQueuSize(X)        (QueuSize_ ## X)
  #define QueuSize(X)              ConcatQueuSize(X)
#else
  #define QueuSize(X)              (QueuSizeOverride)
#endif

#if !defined(TaskIncludeOverride) || ((TaskIncludeOverride) == cfgOverrideNon)
  #define ConcatTaskInclude(X)     (TaskInclude_ ## X)
  #define TaskInclude(X)           ConcatTaskInclude(X)
#else
  #define TaskInclude(X)           (TaskIncludeOverride)
#endif

#if !defined(SlotUseOverride) || ((SlotUseOverride) == cfgOverrideNon)
  #define ConcatSlotUse(X)         (SlotUse_ ## X)
  #define SlotUse(X)               ConcatSlotUse(X)
#else
  #define SlotUse(X)               (SlotUseOverride)
#endif

#if !defined(CapabilitiesOverride) || ((CapabilitiesOverride) == cfgOverrideNon)
  #define ConcatCapabilities(X)    (Capabilities_ ## X)
  #define Capabilities(X)          ConcatCapabilities(X)
#else
  #define Capabilities(X)          (CapabilitiesOverride)
#endif


/**
 * Defined if we make use of some facility that reports an error.
 */
#if (cfgCheckSystem == cfgTrue) || (cfgCheckOsStack == cfgTrue) || (cfgCheckTaskStack == cfgTrue) || (cfgCheckIsrStack == cfgTrue) || (cfgCheckRegisters == cfgTrue) || (cfgCheckTiming == cfgTrue) || (cfgCheckQueuFilling == cfgTrue) || (cfgCheckMethodUse == cfgTrue) || (cfgCheckApplication == cfgTrue)
  #define defCheckReportingError       cfgTrue
#else
  #define defCheckReportingError       cfgFalse
#endif


#define ConcatCouplingTask(X)      defInfoCouplingTask_ ## X
#define defInfoCouplingTask(X)     ConcatCouplingTask(X)
#define ConcatCouplingSlot(X)      defInfoCouplingSlot_ ## X
#define defInfoCouplingSlot(X)     ConcatCouplingSlot(X)
#define ConcatCouplingFile(X)      defInfoCouplingFile_ ## X
#define defInfoCouplingFile(X)     ConcatCouplingFile(X)

#define ConcatTaskName(X)        (TaskName_ ## X)
#define TaskName(X)              ConcatTaskName(X)
#define ConcatInit(X)            (appInit_ ## X)
#define AppInit(X)               ConcatInit(X)
#define ConcatLoop(X)            (appLoop_ ## X)
#define AppLoop(X)               ConcatLoop(X)
#define ConcatBark(X)            (appBark_ ## X)
#define AppBark(X)               ConcatBark(X)
#define ConcatPrescaler(X)       (devPrescale_ ## X)
#define DefPrescaler(X)          ConcatPrescaler(X)
#define DefToString(X)           DefExpandString(X)
#define DefExpandString(X)       #X

#define RegisterUseSize(X)       ((((X)<<2) & 0x04) + (((X)<<1) & 0x04) + (((X)) & 0x04) + (((X)>>1) & 0x04) + (((X)>>2) & 0x04) + (((X)>>3) & 0x04) + (((X)>>4) & 0x04) + (((X)>>5) & 0x04))

#if ( defined(defExtOptimized) && ((defExtOptimized != cfgTrue) && (defExtOptimized != cfgFalse)) )
  #error "External parameter 'defExtOptimized' is misspelled or incorrect."
#endif

#if ( defined(defExtGCCstartup) && ((defExtGCCstartup != cfgKeep) && (defExtGCCstartup != cfgOverride) && (defExtGCCstartup != cfgReplace)) )
  #error "External parameter 'defExtGCCstartup' is misspelled or incorrect."
#endif

#if ( !defined(cfgSysOptimized) || ((cfgSysOptimized != cfgTrue) && (cfgSysOptimized != cfgFalse)) )
  #error "Parameter 'cfgSysOptimized' is misspelled or incorrect."
#endif

#if ( !defined(cfgSysDebug) || ((cfgSysDebug != cfgTrue) && (cfgSysDebug != cfgFalse)) )
  #error "Parameter 'cfgSysDebug' is misspelled or incorrect."
#endif

#if ( !defined(cfgSysInline) || ((cfgSysInline != cfgTrue) && (cfgSysInline != cfgFalse)) )
  #error "Parameter 'cfgSysInline' is misspelled or incorrect."
#endif

#if ( !defined(cfgSysFramePointerCounterMeasures) || ((cfgSysFramePointerCounterMeasures != cfgTrue) && (cfgSysFramePointerCounterMeasures != cfgFalse)) )
  #error "Parameter 'cfgSysFramePointerCounterMeasures' is misspelled or incorrect."
#endif

#if ( !defined(cfgSysReduceProEpilogue) || ((cfgSysReduceProEpilogue != cfgNaked) && (cfgSysReduceProEpilogue != cfgBikini) && (cfgSysReduceProEpilogue != cfgComplete)))
  #error "Parameter 'cfgSysReduceProEpilogue' is misspelled or incorrect."
#endif

#if ( !defined(cfgSysRegisterCheckByte) || (cfgSysRegisterCheckByte < cfgMinSysRegisterCheckByte) || (cfgSysRegisterCheckByte > cfgMaxSysRegisterCheckByte) )
  #error "Parameter 'cfgSysRegisterCheckByte' is misspelled or incorrect."
#endif

#if ( !defined(cfgSysStackGrowthUp) || ((cfgSysStackGrowthUp != cfgTrue) && (cfgSysStackGrowthUp != cfgFalse)) )
  #error "Parameter 'cfgSysStackGrowthUp' is misspelled or incorrect."
#endif

#if ( !defined(cfgSysZeroPageStack) || ((cfgSysZeroPageStack != cfgTrue) && (cfgSysZeroPageStack != cfgFalse)) )
  #error "Parameter 'cfgSysZeroPageStack' is misspelled or incorrect."
#endif

#if ( !defined(cfgSysClearUnusedR1) || ((cfgSysClearUnusedR1 != cfgTrue) && (cfgSysClearUnusedR1 != cfgFalse)) )
  #error "Parameter 'cfgSysClearUnusedR1' is misspelled or incorrect."
#endif

#if ( !defined(cfgSysSqueezeState) || ((cfgSysSqueezeState != cfgTrue) && (cfgSysSqueezeState != cfgFalse)) )
  #error "Parameter 'cfgSysSqueezeState' is misspelled or incorrect."
#endif

#if ( !defined(cfgSysReuseOsStack) || ((cfgSysReuseOsStack != cfgTrue) && (cfgSysReuseOsStack != cfgFalse)) )
  #error "Parameter 'cfgSysReuseOsStack' is misspelled or incorrect."
#endif

#if ( !defined(cfgSysClockDivider) || (cfgSysClockDivider < cfgMinSysClockDivider) || (cfgSysClockDivider > cfgMaxSysClockDivider) )
  #error "Parameter 'cfgSysClockDivider' is misspelled or incorrect."
#endif

#if ( !defined(cfgSysSubTicksPerFullTick) || (cfgSysSubTicksPerFullTick < cfgMinSysSubTicksPerFullTick) || (cfgSysSubTicksPerFullTick > cfgMaxSysSubTicksPerFullTick) )
  #error "Parameter 'cfgSysSubTicksPerFullTick' is misspelled or incorrect."
#endif

#if ( !defined(cfgSysSubTickDivider) || (cfgSysSubTickDivider < cfgMinSysSubTickDivider) || (cfgSysSubTickDivider > cfgMaxSysSubTickDivider) )
  #error "Parameter 'cfgSysSubTickDivider' is misspelled or incorrect."
#endif

#if ( !defined(cfgSysGCCstartup) || ((cfgSysGCCstartup != cfgKeep) && (cfgSysGCCstartup != cfgOverride) && (cfgSysGCCstartup != cfgReplace)) )
  #error "Parameter 'cfgSysGCCstartup' is misspelled or incorrect."
#endif

#if ( !defined(cfgSysFsStorageSpace) || (cfgSysFsStorageSpace < cfgMinSysFsStorageSpace) || (cfgSysFsStorageSpace > cfgMaxSysFsStorageSpace) )
  #error "Parameter 'cfgSysFsStorageSpace' is misspelled or incorrect."
#endif

#if ( !defined(cfgIntGlobalOnly) || ((cfgIntGlobalOnly != cfgTrue) && (cfgIntGlobalOnly != cfgFalse)) )
  #error "Parameter 'cfgIntGlobalOnly' is misspelled or incorrect."
#endif

#if ( !defined(cfgIntUserDefined) || ((cfgIntUserDefined != cfgTrue) && (cfgIntUserDefined != cfgFalse)) )
  #error "Parameter 'cfgIntUserDefined' is misspelled or incorrect."
#endif

#if ( !defined(cfgIntSwitchUsesOSstack) || ((cfgIntSwitchUsesOSstack != cfgTrue) && (cfgIntSwitchUsesOSstack != cfgFalse)) )
  #error "Parameter 'cfgIntSwitchUsesOSstack' is misspelled or incorrect."
#endif

#if ( !defined(cfgIntOsProtected) || ((cfgIntOsProtected != cfgTrue) && (cfgIntOsProtected != cfgFalse)) )
  #error "Parameter 'cfgIntOsProtected' is misspelled or incorrect."
#endif

#if ( !defined(cfgIntTickTrack) || ((cfgIntTickTrack != cfgTrue) && (cfgIntTickTrack != cfgFalse)) )
  #error "Parameter 'cfgIntTickTrack' is misspelled or incorrect."
#endif

#if ( !defined(cfgIntManualTicks) || ((cfgIntManualTicks != cfgTrue) && (cfgIntManualTicks != cfgFalse)) )
  #error "Parameter 'cfgIntManualTicks' is misspelled or incorrect."
#endif

#if ( !defined(cfgCheckSystem) || ((cfgCheckSystem != cfgTrue) && (cfgCheckSystem != cfgFalse)) )
  #error "Parameter 'cfgCheckSystem' is misspelled or incorrect."
#endif

#if ( !defined(cfgCheckReset) || ((cfgCheckReset != cfgTrue) && (cfgCheckReset != cfgFalse)) )
  #error "Parameter 'cfgCheckReset' is misspelled or incorrect."
#endif

#if ( !defined(cfgCheckWatermarks) || ((cfgCheckWatermarks != cfgTrue) && (cfgCheckWatermarks != cfgFalse)) )
  #error "Parameter 'cfgCheckWatermarks' is misspelled or incorrect."
#endif

#if ( !defined(cfgCheckTrace) || ((cfgCheckTrace  != cfgTrue) && (cfgCheckTrace != cfgFalse)) )
  #error "Parameter 'cfgCheckTrace ' is misspelled or incorrect."
#endif

#if ( !defined(cfgCheckAlwaysFatal) || ((cfgCheckAlwaysFatal  != cfgTrue) && (cfgCheckAlwaysFatal != cfgFalse)) )
  #error "Parameter 'cfgCheckAlwaysFatal ' is misspelled or incorrect."
#endif

#if ( !defined(cfgCheckOsStack) || ((cfgCheckOsStack != cfgTrue) && (cfgCheckOsStack != cfgFalse)) )
  #error "Parameter 'cfgCheckOsStack' is misspelled or incorrect."
#endif

#if ( !defined(cfgCheckTaskStack) || ((cfgCheckTaskStack != cfgTrue) && (cfgCheckTaskStack != cfgFalse)) )
  #error "Parameter 'cfgCheckTaskStack' is misspelled or incorrect."
#endif

#if ( !defined(cfgCheckIsrStack) || ((cfgCheckIsrStack != cfgTrue) && (cfgCheckIsrStack != cfgFalse)) )
  #error "Parameter 'cfgCheckIsrStack' is misspelled or incorrect."
#endif

#if ( !defined(cfgCheckRegisters) || ((cfgCheckRegisters != cfgTrue) && (cfgCheckRegisters != cfgFalse)) )
  #error "Parameter 'cfgCheckRegisters' is misspelled or incorrect."
#endif

#if ( !defined(cfgCheckTiming) || ((cfgCheckTiming != cfgTrue) && (cfgCheckTiming != cfgFalse)) )
  #error "Parameter 'cfgCheckTiming' is misspelled or incorrect."
#endif

#if ( !defined(cfgCheckQueuFilling) || ((cfgCheckQueuFilling != cfgTrue) && (cfgCheckQueuFilling != cfgFalse)) )
  #error "Parameter 'cfgCheckQueuFilling' is misspelled or incorrect."
#endif

#if ( !defined(cfgCheckMethodUse) || ((cfgCheckMethodUse != cfgTrue) && (cfgCheckMethodUse != cfgFalse)) )
  #error "Parameter 'cfgCheckMethodUse' is misspelled or incorrect."
#endif

#if ( !defined(cfgCheckApplication) || ((cfgCheckApplication != cfgTrue) && (cfgCheckApplication != cfgFalse)) )
  #error "Parameter 'cfgCheckApplication' is misspelled or incorrect."
#endif

#if ( !defined(cfgUseEquidistantTicks) || ((cfgUseEquidistantTicks != cfgTrue) && (cfgUseEquidistantTicks != cfgFalse)) )
  #error "Parameter 'cfgUseEquidistantTicks' is misspelled or incorrect."
#endif

#if ( !defined(cfgUseDelay) || ((cfgUseDelay != cfgTrue) && (cfgUseDelay != cfgFalse)) )
  #error "Parameter 'cfgUseDelay' is misspelled or incorrect."
#endif

#if ( !defined(cfgUseSuspendOnDelay) || ((cfgUseSuspendOnDelay != cfgTrue) && (cfgUseSuspendOnDelay != cfgFalse)) )
  #error "Parameter 'cfgUseSuspendOnDelay' is misspelled or incorrect."
#endif

#if ( !defined(cfgUseSynchronization) || ((cfgUseSynchronization != cfgSyncSingleSlot) && (cfgUseSynchronization != cfgSyncSingleBlock) && (cfgUseSynchronization != cfgSyncDoubleBlock) && (cfgUseSynchronization != cfgSyncNon)) )
  #error "Parameter 'cfgUseSynchronization' is misspelled or incorrect."
#endif

#if ( !defined(cfgUseHierarchicalRoundRobin) || ((cfgUseHierarchicalRoundRobin != cfgTrue) && (cfgUseHierarchicalRoundRobin != cfgFalse)) )
  #error "Parameter 'cfgUseHierarchicalRoundRobin' is misspelled or incorrect."
#endif

#if ( !defined(cfgUseNestedCriticals) || ((cfgUseNestedCriticals != cfgTrue) && (cfgUseNestedCriticals != cfgFalse)) )
  #error "Parameter 'cfgUseNestedCriticals' is misspelled or incorrect."
#endif

#if ( !defined(cfgUsePrioritizedRelease) || ((cfgUsePrioritizedRelease != cfgTrue) && (cfgUsePrioritizedRelease != cfgFalse)) )
  #error "Parameter 'cfgUsePrioritizedRelease' is misspelled or incorrect."
#endif

#if ( !defined(cfgUsePriorityLifting) || ((cfgUsePriorityLifting != cfgTrue) && (cfgUsePriorityLifting != cfgFalse)) )
  #error "Parameter 'cfgUsePriorityLifting' is misspelled or incorrect."
#endif

#if ( !defined(cfgUseTaskWatchdog) || ((cfgUseTaskWatchdog != cfgTrue) && (cfgUseTaskWatchdog != cfgFalse)) )
  #error "Parameter 'cfgUseTaskWatchdog' is misspelled or incorrect."
#endif

#if ( !defined(cfgUseCorrectWakeupTimes) || ((cfgUseCorrectWakeupTimes != cfgTrue) && (cfgUseCorrectWakeupTimes != cfgFalse)) )
  #error "Parameter 'cfgUseCorrectWakeupTimes' is misspelled or incorrect."
#endif

#if ( !defined(cfgUseTimeout) || ((cfgUseTimeout != cfgTrue) && (cfgUseTimeout != cfgFalse)) )
  #error "Parameter 'cfgUseTimeout' is misspelled or incorrect."
#endif

#if ( !defined(cfgUseTasknames) || ((cfgUseTasknames != cfgTrue) && (cfgUseTasknames != cfgFalse)) )
  #error "Parameter 'cfgUseTasknames' is misspelled or incorrect."
#endif

#if ( !defined(cfgUseLowPowerSleep) || ((cfgUseLowPowerSleep != cfgTrue) && (cfgUseLowPowerSleep != cfgFalse)) )
  #error "Parameter 'cfgUseLowPowerSleep' is misspelled or incorrect."
#endif

#if ( !defined(cfgUseLowPowerOnDelay) || ((cfgUseLowPowerOnDelay != cfgTrue) && (cfgUseLowPowerOnDelay != cfgFalse)) )
  #error "Parameter 'cfgUseLowPowerOnDelay' is misspelled or incorrect."
#endif

#if ( !defined(cfgUseLowPowerDelayRelease) || ((cfgUseLowPowerDelayRelease != cfgTrue) && (cfgUseLowPowerDelayRelease != cfgFalse)) )
  #error "Parameter 'cfgUseLowPowerDelayRelease' is misspelled or incorrect."
#endif

#if ( !defined(cfgUseLoadMonitor) || ((cfgUseLoadMonitor != cfgTrue) && (cfgUseLoadMonitor != cfgFalse)) )
  #error "Parameter 'cfgUseLoadMonitor' is misspelled or incorrect."
#endif

#if ( !defined(cfgUseFileSystem) || ((cfgUseFileSystem != cfgTrue) && (cfgUseFileSystem != cfgFalse)) )
  #error "Parameter 'cfgUseFileSystem' is misspelled or incorrect."
#endif

#if ( !defined(cfgUseFileSystemConcurrentRead) || ((cfgUseFileSystemConcurrentRead != cfgTrue) && (cfgUseFileSystemConcurrentRead != cfgFalse)) )
  #error "Parameter 'cfgUseFileSystemConcurrentRead' is misspelled or incorrect."
#endif

#if ( !defined(cfgUseFileSystemEconomyMode) || ((cfgUseFileSystemEconomyMode != cfgTrue) && (cfgUseFileSystemEconomyMode != cfgFalse)) )
  #error "Parameter 'cfgUseFileSystemEconomyMode' is misspelled or incorrect."
#endif

#if ( !defined(cfgUseFileSystemMaintainFAT) || ((cfgUseFileSystemMaintainFAT != cfgTrue) && (cfgUseFileSystemMaintainFAT != cfgFalse)) )
  #error "Parameter 'cfgUseFileSystemMaintainFAT' is misspelled or incorrect."
#endif

#if ( !defined(cfgUseEvents) || ((cfgUseEvents != cfgTrue) && (cfgUseEvents != cfgFalse)) )
  #error "Parameter 'cfgUseEvents' is misspelled or incorrect."
#endif

#if ( !defined(cfgUseEventsOnVariables) || ((cfgUseEventsOnVariables != cfgTrue) && (cfgUseEventsOnVariables != cfgFalse)) )
  #error "Parameter 'cfgUseEventsOnVariables' is misspelled or incorrect."
#endif

#if ( !defined(cfgUseEventsOnVariables) || ((cfgUseEventsOnVariables != cfgTrue) && (cfgUseEventsOnVariables != cfgFalse)) )
  #error "Parameter 'cfgUseEventsOnVariables' is misspelled or incorrect."
#endif

#if ( !defined(cfgNumWatchdogDiv) || (cfgNumWatchdogDiv < cfgMinNumWatchdogDiv) || (cfgNumWatchdogDiv > cfgMaxNumWatchdogDiv) )
  #error "Parameter 'cfgNumWatchdogDiv' is misspelled or incorrect."
#endif

#if ( !defined(cfgNumMonitorDiv) || (cfgNumMonitorDiv < cfgMinNumMonitorDiv) || (cfgNumMonitorDiv > cfgMaxNumMonitorDiv) )
  #error "Parameter 'cfgNumMonitorDiv' is misspelled or incorrect."
#endif

#if ( !defined(cfgNumSleepThreshold) || (cfgNumSleepThreshold < cfgMinNumSleepThreshold) || (cfgNumSleepThreshold > cfgMaxNumSleepThreshold) )
  #error "Parameter 'cfgNumSleepThreshold' is misspelled or incorrect."
#endif

#if ( !defined(cfgNumSleepPeriod) || (cfgNumSleepPeriod < cfgMinNumSleepPeriod) || (cfgNumSleepPeriod > cfgMaxNumSleepPeriod) )
  #error "Parameter 'cfgNumSleepPeriod' is misspelled or incorrect."
#endif

#if ( !defined(cfgNumGlobWidth) || (cfgNumGlobWidth < cfgMinNumGlobWidth) || (cfgNumGlobWidth > cfgMaxNumGlobWidth) )
  #error "Parameter 'cfgNumGlobWidth' is misspelled or incorrect."
#endif

#if ( !defined(cfgNumSwitchWidth) || (cfgNumSwitchWidth < cfgMinNumSwitchWidth) || (cfgNumSwitchWidth > cfgMaxNumSwitchWidth) )
  #error "Parameter 'cfgNumSwitchWidth' is misspelled or incorrect."
#endif

#if ( !defined(cfgNumTickWidth) || (cfgNumTickWidth < cfgMinNumTickWidth) || (cfgNumTickWidth > cfgMaxNumTickWidth) )
  #error "Parameter 'cfgNumTickWidth' is misspelled or incorrect."
#endif

#if ( !defined(callAppTick00) || ((callAppTick00 != cfgTrue) && (callAppTick00 != cfgFalse)) )
  #error "Parameter 'callAppTick00' is misspelled or incorrect."
#endif

#if ( !defined(callAppTick08) || ((callAppTick08 != cfgTrue) && (callAppTick08 != cfgFalse)) )
  #error "Parameter 'callAppTick08' is misspelled or incorrect."
#endif

#if ( !defined(callAppTick16) || ((callAppTick16 != cfgTrue) && (callAppTick16 != cfgFalse)) )
  #error "Parameter 'callAppTick16' is misspelled or incorrect."
#endif

#if ( !defined(callAppTickSleep) || ((callAppTickSleep != cfgTrue) && (callAppTickSleep != cfgFalse)) )
  #error "Parameter 'callAppTickSleep' is misspelled or incorrect."
#endif

#if ( !defined(callAppEnterIdle) || ((callAppEnterIdle != cfgTrue) && (callAppEnterIdle != cfgFalse)) )
  #error "Parameter 'callAppEnterIdle' is misspelled or incorrect."
#endif

#if ( !defined(callAppEnterSleep) || ((callAppEnterSleep != cfgTrue) && (callAppEnterSleep != cfgFalse)) )
  #error "Parameter 'callAppEnterSleep' is misspelled or incorrect."
#endif

#if ( !defined(callAppExitSleep) || ((callAppExitSleep != cfgTrue) && (callAppExitSleep != cfgFalse)) )
  #error "Parameter 'callAppExitSleep' is misspelled or incorrect."
#endif

#if ( !defined(callAppBoot) || ((callAppBoot != cfgTrue) && (callAppBoot != cfgFalse)) )
  #error "Parameter 'callAppBoot' is misspelled or incorrect."
#endif

#if ( !defined(callAppInit) || ((callAppInit != cfgTrue) && (callAppInit != cfgFalse)) )
  #error "Parameter 'callAppInit' is misspelled or incorrect."
#endif

#if ( !defined(callAppBark) || ((callAppBark != cfgTrue) && (callAppBark != cfgFalse)) )
  #error "Parameter 'callAppBark' is misspelled or incorrect."
#endif

#if ( !defined(includeTaskYield) || ((includeTaskYield != cfgTrue) && (includeTaskYield != cfgFalse)) )
  #error "Parameter 'includeTaskYield' is misspelled or incorrect."
#endif

#if ( !defined(includeTaskDelayFromNow) || ((includeTaskDelayFromNow != cfgTrue) && (includeTaskDelayFromNow != cfgFalse)) )
  #error "Parameter 'includeTaskDelayFromNow' is misspelled or incorrect."
#endif

#if ( !defined(includeTaskDelayFromWake) || ((includeTaskDelayFromWake != cfgTrue) && (includeTaskDelayFromWake != cfgFalse)) )
  #error "Parameter 'includeTaskDelayFromWake' is misspelled or incorrect."
#endif

#if ( !defined(includeGenSuspend) || ((includeGenSuspend != cfgTrue) && (includeGenSuspend != cfgFalse)) )
  #error "Parameter 'includeGenSuspend' is misspelled or incorrect."
#endif

#if ( !defined(includeTaskSuspend) || ((includeTaskSuspend != cfgTrue) && (includeTaskSuspend != cfgFalse)) )
  #error "Parameter 'includeTaskSuspend' is misspelled or incorrect."
#endif

#if ( !defined(includeGenResume) || ((includeGenResume != cfgTrue) && (includeGenResume != cfgFalse)) )
  #error "Parameter 'includeGenResume' is misspelled or incorrect."
#endif

#if ( !defined(includeTaskSleep) || ((includeTaskSleep != cfgTrue) && (includeTaskSleep != cfgFalse)) )
  #error "Parameter 'includeTaskSleep' is misspelled or incorrect."
#endif

#if ( !defined(includeTaskSleepAll) || ((includeTaskSleepAll != cfgTrue) && (includeTaskSleepAll != cfgFalse)) )
  #error "Parameter 'includeTaskSleepAll' is misspelled or incorrect."
#endif

#if ( !defined(includeGenSetPriority) || ((includeGenSetPriority != cfgTrue) && (includeGenSetPriority != cfgFalse)) )
  #error "Parameter 'includeGenSetPriority' is misspelled or incorrect."
#endif

#if ( !defined(includeGenGetPriority) || ((includeGenGetPriority != cfgTrue) && (includeGenGetPriority != cfgFalse)) )
  #error "Parameter 'includeGenGetPriority' is misspelled or incorrect."
#endif

#if ( !defined(includeGenGetTickCount) || ((includeGenGetTickCount != cfgTrue) && (includeGenGetTickCount != cfgFalse)) )
  #error "Parameter 'includeGenGetTickCount' is misspelled or incorrect."
#endif

#if ( !defined(includeGenAddtoTickCount) || ((includeGenAddtoTickCount != cfgTrue) && (includeGenAddtoTickCount != cfgFalse)) )
  #error "Parameter 'includeGenAddtoTickCount' is misspelled or incorrect."
#endif

#if ( !defined(includeGenGetLastWakeTime) || ((includeGenGetLastWakeTime != cfgTrue) && (includeGenGetLastWakeTime != cfgFalse)) )
  #error "Parameter 'includeGenGetLastWakeTime' is misspelled or incorrect."
#endif

#if ( !defined(includeTaskFeedWatchdog) || ((includeTaskFeedWatchdog != cfgTrue) && (includeTaskFeedWatchdog != cfgFalse)) )
  #error "Parameter 'includeTaskFeedWatchdog' is misspelled or incorrect."
#endif

#if ( !defined(includeTaskKillWatchdog) || ((includeTaskKillWatchdog != cfgTrue) && (includeTaskKillWatchdog != cfgFalse)) )
  #error "Parameter 'includeTaskKillWatchdog' is misspelled or incorrect."
#endif

#if ( !defined(includeGenGetTaskname) || ((includeGenGetTaskname != cfgTrue) && (includeGenGetTaskname != cfgFalse)) )
  #error "Parameter 'includeGenGetTaskname' is misspelled or incorrect."
#endif

#if ( !defined(includeTaskTerminate) || ((includeTaskTerminate != cfgTrue) && (includeTaskTerminate != cfgFalse)) )
  #error "Parameter 'includeTaskTerminate' is misspelled or incorrect."
#endif

#if ( !defined(includeGenReboot) || ((includeGenReboot != cfgTrue) && (includeGenReboot != cfgFalse)) )
  #error "Parameter 'includeGenReboot' is misspelled or incorrect."
#endif

#if ( !defined(includeTaskRestart) || ((includeTaskRestart != cfgTrue) && (includeTaskRestart != cfgFalse)) )
  #error "Parameter 'includeTaskRestart' is misspelled or incorrect."
#endif

#if ( !defined(includeTaskRecreate) || ((includeTaskRecreate != cfgTrue) && (includeTaskRecreate != cfgFalse)) )
  #error "Parameter 'includeTaskRecreate' is misspelled or incorrect."
#endif

#if ( !defined(includeGenLogTask) || ((includeGenLogTask != cfgTrue) && (includeGenLogTask != cfgFalse)) )
  #error "Parameter 'includeGenLogTask' is misspelled or incorrect."
#endif

#if ( !defined(includeGenLogOs) || ((includeGenLogOs != cfgTrue) && (includeGenLogOs != cfgFalse)) )
  #error "Parameter 'includeGenLogOs' is misspelled or incorrect."
#endif

#if ( !defined(includeGenTrace) || ((includeGenTrace != cfgTrue) && (includeGenTrace != cfgFalse)) )
  #error "Parameter 'includeGenTrace' is misspelled or incorrect."
#endif

#if ( !defined(includeGenPipeInt16) || ((includeGenPipeInt16 != cfgTrue) && (includeGenPipeInt16 != cfgFalse)) )
  #error "Parameter 'includeGenPipeInt16' is misspelled or incorrect."
#endif

#if ( !defined(includeGenPassFlashString) || ((includeGenPassFlashString != cfgTrue) && (includeGenPassFlashString != cfgFalse)) )
  #error "Parameter 'includeGenPassFlashString' is misspelled or incorrect."
#endif

#if ( !defined(includeTaskWaitForEvents) || ((includeTaskWaitForEvents != cfgTrue) && (includeTaskWaitForEvents != cfgFalse)) )
  #error "Parameter 'includeTaskWaitForEvents' is misspelled or incorrect."
#endif

#if ( !defined(includeIsrFireEvent) || ((includeIsrFireEvent != cfgTrue) && (includeIsrFireEvent != cfgFalse)) )
  #error "Parameter 'includeIsrFireEvent' is misspelled or incorrect."
#endif

#if ( !defined(includeGenFireEvent) || ((includeGenFireEvent != cfgTrue) && (includeGenFireEvent != cfgFalse)) )
  #error "Parameter 'includeGenFireEvent' is misspelled or incorrect."
#endif

#if ( !defined(includeGenFireEventSet) || ((includeGenFireEventSet != cfgTrue) && (includeGenFireEventSet != cfgFalse)) )
  #error "Parameter 'includeGenFireEventSet' is misspelled or incorrect."
#endif

#if ( !defined(includeTaskWaitForTasks) || ((includeTaskWaitForTasks != cfgTrue) && (includeTaskWaitForTasks != cfgFalse)) )
  #error "Parameter 'includeTaskWaitForTasks' is misspelled or incorrect."
#endif

#if ( !defined(includeGenWaitRelease) || ((includeGenWaitRelease != cfgTrue) && (includeGenWaitRelease != cfgFalse)) )
  #error "Parameter 'includeGenWaitRelease' is misspelled or incorrect."
#endif

#if ( !defined(includeTaskMutex) || ((includeTaskMutex != cfgTrue) && (includeTaskMutex != cfgFalse)) )
  #error "Parameter 'includeTaskMutex' is misspelled or incorrect."
#endif

#if ( !defined(includeTaskQueu) || ((includeTaskQueu != cfgTrue) && (includeTaskQueu != cfgFalse)) )
  #error "Parameter 'includeTaskQueu' is misspelled or incorrect."
#endif

#if ( !defined(includeGenQueuWrite) || ((includeGenQueuWrite != cfgTrue) && (includeGenQueuWrite != cfgFalse)) )
  #error "Parameter 'includeGenQueuWrite' is misspelled or incorrect."
#endif

#if ( !defined(includeGenQueuRead) || ((includeGenQueuRead != cfgTrue) && (includeGenQueuRead != cfgFalse)) )
  #error "Parameter 'includeGenQueuRead' is misspelled or incorrect."
#endif

#if ( !defined(includeGenQueuUnwrite) || ((includeGenQueuUnwrite != cfgTrue) && (includeGenQueuUnwrite != cfgFalse)) )
  #error "Parameter 'includeGenQueuUnwrite' is misspelled or incorrect."
#endif

#if ( !defined(includeGenQueuUnread) || ((includeGenQueuUnread != cfgTrue) && (includeGenQueuUnread != cfgFalse)) )
  #error "Parameter 'includeGenQueuUnread' is misspelled or incorrect."
#endif

#if ( !defined(includeGenQueuClear) || ((includeGenQueuClear != cfgTrue) && (includeGenQueuClear != cfgFalse)) )
  #error "Parameter 'includeGenQueuClear' is misspelled or incorrect."
#endif

#if ( !defined(includeGenQueuPeek) || ((includeGenQueuPeek != cfgTrue) && (includeGenQueuPeek != cfgFalse)) )
  #error "Parameter 'includeGenQueuPeek' is misspelled or incorrect."
#endif

#if ( !defined(includeGenQueuReadable) || ((includeGenQueuReadable != cfgTrue) && (includeGenQueuReadable != cfgFalse)) )
  #error "Parameter 'includeGenQueuReadable' is misspelled or incorrect."
#endif

#if ( !defined(includeGenQueuWriteable) || ((includeGenQueuWriteable != cfgTrue) && (includeGenQueuWriteable != cfgFalse)) )
  #error "Parameter 'includeGenQueuWriteable' is misspelled or incorrect."
#endif

#if ( !defined(includeGenQueuFull) || ((includeGenQueuFull != cfgTrue) && (includeGenQueuFull != cfgFalse)) )
  #error "Parameter 'includeGenQueuFull' is misspelled or incorrect."
#endif

#if ( !defined(includeGenQueuEmpty) || ((includeGenQueuEmpty != cfgTrue) && (includeGenQueuEmpty != cfgFalse)) )
  #error "Parameter 'includeGenQueuEmpty' is misspelled or incorrect."
#endif

#if ( !defined(includeTaskFileAccess) || ((includeTaskFileAccess != cfgTrue) && (includeTaskFileAccess != cfgFalse)) )
  #error "Parameter 'includeTaskFileAccess' is misspelled or incorrect."
#endif

#if ( !defined(includeTaskFileFormat) || ((includeTaskFileFormat != cfgTrue) && (includeTaskFileFormat != cfgFalse)) )
  #error "Parameter 'includeTaskFileFormat' is misspelled or incorrect."
#endif

#if ( !defined(includeTaskFileGetSize) || ((includeTaskFileGetSize != cfgTrue) && (includeTaskFileGetSize != cfgFalse)) )
  #error "Parameter 'includeTaskFileGetSize' is misspelled or incorrect."
#endif

#if ( !defined(includeTaskFileSetSize) || ((includeTaskFileSetSize != cfgTrue) && (includeTaskFileSetSize != cfgFalse)) )
  #error "Parameter 'includeTaskFileSetSize' is misspelled or incorrect."
#endif

#if ( !defined(includeTaskFileReadByte) || ((includeTaskFileReadByte != cfgTrue) && (includeTaskFileReadByte != cfgFalse)) )
  #error "Parameter 'includeTaskFileReadByte' is misspelled or incorrect."
#endif

#if ( !defined(includeTaskFileWriteByte) || ((includeTaskFileWriteByte != cfgTrue) && (includeTaskFileWriteByte != cfgFalse)) )
  #error "Parameter 'includeTaskFileWriteByte' is misspelled or incorrect."
#endif

#if ( !defined(includeTaskFileAppendByte) || ((includeTaskFileAppendByte != cfgTrue) && (includeTaskFileAppendByte != cfgFalse)) )
  #error "Parameter 'includeTaskFileAppendByte' is misspelled or incorrect."
#endif

#if ( !defined(includeTaskFileReadBuffer) || ((includeTaskFileReadBuffer != cfgTrue) && (includeTaskFileReadBuffer != cfgFalse)) )
  #error "Parameter 'includeTaskFileReadBuffer' is misspelled or incorrect."
#endif

#if ( !defined(includeTaskFileWriteBuffer) || ((includeTaskFileWriteBuffer != cfgTrue) && (includeTaskFileWriteBuffer != cfgFalse)) )
  #error "Parameter 'includeTaskFileWriteBuffer' is misspelled or incorrect."
#endif

#if ( !defined(includeTaskFileReadPipe) || ((includeTaskFileReadPipe != cfgTrue) && (includeTaskFileReadPipe != cfgFalse)) )
  #error "Parameter 'includeTaskFileReadPipe' is misspelled or incorrect."
#endif

#if ( !defined(includeTaskFileWritePipe) || ((includeTaskFileWritePipe != cfgTrue) && (includeTaskFileWritePipe != cfgFalse)) )
  #error "Parameter 'includeTaskFileWritePipe' is misspelled or incorrect."
#endif

#if ( !defined(includeTaskProtectGlobalInterrupts) || ((includeTaskProtectGlobalInterrupts != cfgTrue) && (includeTaskProtectGlobalInterrupts != cfgFalse)) )
  #error "Parameter 'includeTaskProtectGlobalInterrupts' is misspelled or incorrect."
#endif

#if ( !defined(includeTaskProtectSwitchTasks) || ((includeTaskProtectSwitchTasks != cfgTrue) && (includeTaskProtectSwitchTasks != cfgFalse)) )
  #error "Parameter 'includeTaskProtectSwitchTasks' is misspelled or incorrect."
#endif

#if ( !defined(includeTaskProtectTickInterrupts) || ((includeTaskProtectTickInterrupts != cfgTrue) && (includeTaskProtectTickInterrupts != cfgFalse)) )
  #error "Parameter 'includeTaskProtectTickInterrupts' is misspelled or incorrect."
#endif

#if ( !defined(includeTaskProtectGlobalCritical) || ((includeTaskProtectGlobalCritical != cfgTrue) && (includeTaskProtectGlobalCritical != cfgFalse)) )
  #error "Parameter 'includeTaskProtectGlobalCritical' is misspelled or incorrect."
#endif

#if ( !defined(includeTaskProtectSwitchCritical) || ((includeTaskProtectSwitchCritical != cfgTrue) && (includeTaskProtectSwitchCritical != cfgFalse)) )
  #error "Parameter 'includeTaskProtectSwitchCritical' is misspelled or incorrect."
#endif

#if ( !defined(includeTaskProtectTickCritical) || ((includeTaskProtectTickCritical != cfgTrue) && (includeTaskProtectTickCritical != cfgFalse)) )
  #error "Parameter 'includeTaskProtectTickCritical' is misspelled or incorrect."
#endif

#if ( !defined(includeIsrEnter) || ((includeIsrEnter != cfgTrue) && (includeIsrEnter != cfgFalse)) )
  #error "Parameter 'includeIsrEnter' is misspelled or incorrect."
#endif

#if ( !defined(includeIsrExit) || ((includeIsrExit != cfgTrue) && (includeIsrExit != cfgFalse)) )
  #error "Parameter 'includeIsrExit' is misspelled or incorrect."
#endif

#if ( !defined(includeIsrBegin) || ((includeIsrBegin != cfgTrue) && (includeIsrBegin != cfgFalse)) )
  #error "Parameter 'includeIsrBegin' is misspelled or incorrect."
#endif

#if ( !defined(includeIsrEndReturn) || ((includeIsrEndReturn != cfgTrue) && (includeIsrEndReturn != cfgFalse)) )
  #error "Parameter 'includeIsrEndReturn' is misspelled or incorrect."
#endif

#if ( !defined(includeIsrEndYield) || ((includeIsrEndYield != cfgTrue) && (includeIsrEndYield != cfgFalse)) )
  #error "Parameter 'includeIsrEndYield' is misspelled or incorrect."
#endif

#if ( !defined(includeIsrStartLoad) || ((includeIsrStartLoad != cfgTrue) && (includeIsrStartLoad != cfgFalse)) )
  #error "Parameter 'includeIsrStartLoad' is misspelled or incorrect."
#endif

#if ( !defined(includeIsrStopLoad) || ((includeIsrStopLoad != cfgTrue) && (includeIsrStopLoad != cfgFalse)) )
  #error "Parameter 'includeIsrStopLoad' is misspelled or incorrect."
#endif

#if ( !defined(includeTaskStackCheck) || ((includeTaskStackCheck != cfgTrue) && (includeTaskStackCheck != cfgFalse)) )
  #error "Parameter 'includeTaskStackCheck' is misspelled or incorrect."
#endif

#if ( !defined(includeIsrStackCheck) || ((includeIsrStackCheck != cfgTrue) && (includeIsrStackCheck != cfgFalse)) )
  #error "Parameter 'includeIsrStackCheck' is misspelled or incorrect."
#endif

#if ( !defined(StackSafety) || ((StackSafety) < MinStackSafety) || ((StackSafety) > MaxStackSafety) )
  #error "Parameter 'StackSafety' is misspelled or incorrect."
#endif

#if ( !defined(StackSizeOS) || ((StackSizeOS) < MinStackSizeOS) || ((StackSizeOS) > MaxStackSizeOS) )
  #error "Parameter 'StackSizeOS' is misspelled or incorrect."
#endif

#if ( !defined(StackSizeISR) || ((StackSizeISR) < MinStackSizeISR) || ((StackSizeISR) > MaxStackSizeISR) )
  #error "Parameter 'StackSizeISR' is misspelled or incorrect."
#endif

#if ( !defined(StackSizeShared) || (((StackSizeShared) != 0) && ((StackSizeShared) < MinStackSize_Task) || ((StackSizeShared) > MaxStackSize_Task)) )
  #error "Parameter 'StackSizeShared' is misspelled or incorrect."
#endif

#if ( !defined(TimeSliceIdleTime) || (TimeSliceIdleTime < MinTimeSliceIdleTime) || (TimeSliceIdleTime > MaxTimeSliceIdleTime) )
  #error "Parameter 'TimeSliceIdleTime' is misspelled or incorrect."
#endif

#if ( !defined(FileSpaceStandard) || (FileSpaceStandard < MinFileSpace_Task) || (FileSpaceStandard > MaxFileSpace_Task) )
  #error "Parameter 'FileSpaceStandard' is misspelled or incorrect (cannot exceed cfgSysFsStorageSpace)."
#endif


#if !defined(IncludeOverride) || ((IncludeOverride) == cfgOverrideNon)

#if defined(CN_00) && (TaskInclude(CN_00) != cfgExclude) && (TaskInclude(CN_00) != cfgStartSleeping) && (TaskInclude(CN_00) != cfgStartSuspended) && (TaskInclude(CN_00) != cfgStartRunning)
  #error "Parameter 'TaskInclude on task CN_00' is misspelled or incorrect."
#endif

#if defined(CN_01) && (TaskInclude(CN_01) != cfgExclude) && (TaskInclude(CN_01) != cfgStartSleeping) && (TaskInclude(CN_01) != cfgStartSuspended) && (TaskInclude(CN_01) != cfgStartRunning)
  #error "Parameter 'TaskInclude on task CN_01' is misspelled or incorrect."
#endif

#if defined(CN_02) && (TaskInclude(CN_02) != cfgExclude) && (TaskInclude(CN_02) != cfgStartSleeping) && (TaskInclude(CN_02) != cfgStartSuspended) && (TaskInclude(CN_02) != cfgStartRunning)
  #error "Parameter 'TaskInclude on task CN_02' is misspelled or incorrect."
#endif

#if defined(CN_03) && (TaskInclude(CN_03) != cfgExclude) && (TaskInclude(CN_03) != cfgStartSleeping) && (TaskInclude(CN_03) != cfgStartSuspended) && (TaskInclude(CN_03) != cfgStartRunning)
  #error "Parameter 'TaskInclude on task CN_03' is misspelled or incorrect."
#endif

#if defined(CN_04) && (TaskInclude(CN_04) != cfgExclude) && (TaskInclude(CN_04) != cfgStartSleeping) && (TaskInclude(CN_04) != cfgStartSuspended) && (TaskInclude(CN_04) != cfgStartRunning)
  #error "Parameter 'TaskInclude on task CN_04' is misspelled or incorrect."
#endif

#if defined(CN_05) && (TaskInclude(CN_05) != cfgExclude) && (TaskInclude(CN_05) != cfgStartSleeping) && (TaskInclude(CN_05) != cfgStartSuspended) && (TaskInclude(CN_05) != cfgStartRunning)
  #error "Parameter 'TaskInclude on task CN_05' is misspelled or incorrect."
#endif

#if defined(CN_06) && (TaskInclude(CN_06) != cfgExclude) && (TaskInclude(CN_06) != cfgStartSleeping) && (TaskInclude(CN_06) != cfgStartSuspended) && (TaskInclude(CN_06) != cfgStartRunning)
  #error "Parameter 'TaskInclude on task CN_06' is misspelled or incorrect."
#endif

#if defined(CN_07) && (TaskInclude(CN_07) != cfgExclude) && (TaskInclude(CN_07) != cfgStartSleeping) && (TaskInclude(CN_07) != cfgStartSuspended) && (TaskInclude(CN_07) != cfgStartRunning)
  #error "Parameter 'TaskInclude on task CN_07' is misspelled or incorrect."
#endif

#if defined(CN_08) && (TaskInclude(CN_08) != cfgExclude) && (TaskInclude(CN_08) != cfgStartSleeping) && (TaskInclude(CN_08) != cfgStartSuspended) && (TaskInclude(CN_08) != cfgStartRunning)
  #error "Parameter 'TaskInclude on task CN_08' is misspelled or incorrect."
#endif

#if defined(CN_09) && (TaskInclude(CN_09) != cfgExclude) && (TaskInclude(CN_09) != cfgStartSleeping) && (TaskInclude(CN_09) != cfgStartSuspended) && (TaskInclude(CN_09) != cfgStartRunning)
  #error "Parameter 'TaskInclude on task CN_09' is misspelled or incorrect."
#endif

#if defined(CN_10) && (TaskInclude(CN_10) != cfgExclude) && (TaskInclude(CN_10) != cfgStartSleeping) && (TaskInclude(CN_10) != cfgStartSuspended) && (TaskInclude(CN_10) != cfgStartRunning)
  #error "Parameter 'TaskInclude on task CN_10' is misspelled or incorrect."
#endif

#if defined(CN_11) && (TaskInclude(CN_11) != cfgExclude) && (TaskInclude(CN_11) != cfgStartSleeping) && (TaskInclude(CN_11) != cfgStartSuspended) && (TaskInclude(CN_11) != cfgStartRunning)
  #error "Parameter 'TaskInclude on task CN_11' is misspelled or incorrect."
#endif

#if defined(CN_12) && (TaskInclude(CN_12) != cfgExclude) && (TaskInclude(CN_12) != cfgStartSleeping) && (TaskInclude(CN_12) != cfgStartSuspended) && (TaskInclude(CN_12) != cfgStartRunning)
  #error "Parameter 'TaskInclude on task CN_12' is misspelled or incorrect."
#endif

#if defined(CN_13) && (TaskInclude(CN_13) != cfgExclude) && (TaskInclude(CN_13) != cfgStartSleeping) && (TaskInclude(CN_13) != cfgStartSuspended) && (TaskInclude(CN_13) != cfgStartRunning)
  #error "Parameter 'TaskInclude on task CN_13' is misspelled or incorrect."
#endif

#if defined(CN_14) && (TaskInclude(CN_14) != cfgExclude) && (TaskInclude(CN_14) != cfgStartSleeping) && (TaskInclude(CN_14) != cfgStartSuspended) && (TaskInclude(CN_14) != cfgStartRunning)
  #error "Parameter 'TaskInclude on task CN_14' is misspelled or incorrect."
#endif

#if defined(CN_15) && (TaskInclude(CN_15) != cfgExclude) && (TaskInclude(CN_15) != cfgStartSleeping) && (TaskInclude(CN_15) != cfgStartSuspended) && (TaskInclude(CN_15) != cfgStartRunning)
  #error "Parameter 'TaskInclude on task CN_15' is misspelled or incorrect."
#endif

#else

#if (IncludeOverride != cfgExclude) && (IncludeOverride != cfgStartSleeping) && (IncludeOverride != cfgStartSuspended) && (IncludeOverride != cfgStartRunning)
  #error "Parameter 'IncludeOverride' is misspelled or incorrect."
#endif

#endif


#if !defined(StackSizeOverride) || ((StackSizeOverride) == cfgOverrideNon)

#if defined(CN_00) && (TaskInclude(CN_00) != cfgExclude) && ((StackSizeDef(CN_00) < MinStackSize_Task) || (StackSizeDef(CN_00) > MaxStackSize_Task)) && (StackSizeDef(CN_00) != cfgShared)
  #error "Parameter 'StackSize on task CN_00' is misspelled or incorrect."
#endif

#if defined(CN_01) && (TaskInclude(CN_01) != cfgExclude) && ((StackSizeDef(CN_01) < MinStackSize_Task) || (StackSizeDef(CN_01) > MaxStackSize_Task)) && (StackSizeDef(CN_01) != cfgShared)
  #error "Parameter 'StackSize on task CN_01' is misspelled or incorrect."
#endif

#if defined(CN_02) && (TaskInclude(CN_02) != cfgExclude) && ((StackSizeDef(CN_02) < MinStackSize_Task) || (StackSizeDef(CN_02) > MaxStackSize_Task)) && (StackSizeDef(CN_02) != cfgShared)
  #error "Parameter 'StackSize on task CN_02' is misspelled or incorrect."
#endif

#if defined(CN_03) && (TaskInclude(CN_03) != cfgExclude) && ((StackSizeDef(CN_03) < MinStackSize_Task) || (StackSizeDef(CN_03) > MaxStackSize_Task)) && (StackSizeDef(CN_03) != cfgShared)
  #error "Parameter 'StackSize on task CN_03' is misspelled or incorrect."
#endif

#if defined(CN_04) && (TaskInclude(CN_04) != cfgExclude) && ((StackSizeDef(CN_04) < MinStackSize_Task) || (StackSizeDef(CN_04) > MaxStackSize_Task)) && (StackSizeDef(CN_04) != cfgShared)
  #error "Parameter 'StackSize on task CN_04' is misspelled or incorrect."
#endif

#if defined(CN_05) && (TaskInclude(CN_05) != cfgExclude) && ((StackSizeDef(CN_05) < MinStackSize_Task) || (StackSizeDef(CN_05) > MaxStackSize_Task)) && (StackSizeDef(CN_05) != cfgShared)
  #error "Parameter 'StackSize on task CN_05' is misspelled or incorrect."
#endif

#if defined(CN_06) && (TaskInclude(CN_06) != cfgExclude) && ((StackSizeDef(CN_06) < MinStackSize_Task) || (StackSizeDef(CN_06) > MaxStackSize_Task)) && (StackSizeDef(CN_06) != cfgShared)
  #error "Parameter 'StackSize on task CN_06' is misspelled or incorrect."
#endif

#if defined(CN_07) && (TaskInclude(CN_07) != cfgExclude) && ((StackSizeDef(CN_07) < MinStackSize_Task) || (StackSizeDef(CN_07) > MaxStackSize_Task)) && (StackSizeDef(CN_07) != cfgShared)
  #error "Parameter 'StackSize on task CN_07' is misspelled or incorrect."
#endif

#if defined(CN_08) && (TaskInclude(CN_08) != cfgExclude) && ((StackSizeDef(CN_08) < MinStackSize_Task) || (StackSizeDef(CN_08) > MaxStackSize_Task)) && (StackSizeDef(CN_08) != cfgShared)
  #error "Parameter 'StackSize on task CN_08' is misspelled or incorrect."
#endif

#if defined(CN_09) && (TaskInclude(CN_09) != cfgExclude) && ((StackSizeDef(CN_09) < MinStackSize_Task) || (StackSizeDef(CN_09) > MaxStackSize_Task)) && (StackSizeDef(CN_09) != cfgShared)
  #error "Parameter 'StackSize on task CN_09' is misspelled or incorrect."
#endif

#if defined(CN_10) && (TaskInclude(CN_10) != cfgExclude) && ((StackSizeDef(CN_10) < MinStackSize_Task) || (StackSizeDef(CN_10) > MaxStackSize_Task)) && (StackSizeDef(CN_10) != cfgShared)
  #error "Parameter 'StackSize on task CN_10' is misspelled or incorrect."
#endif

#if defined(CN_11) && (TaskInclude(CN_11) != cfgExclude) && ((StackSizeDef(CN_11) < MinStackSize_Task) || (StackSizeDef(CN_11) > MaxStackSize_Task)) && (StackSizeDef(CN_11) != cfgShared)
  #error "Parameter 'StackSize on task CN_11' is misspelled or incorrect."
#endif

#if defined(CN_12) && (TaskInclude(CN_12) != cfgExclude) && ((StackSizeDef(CN_12) < MinStackSize_Task) || (StackSizeDef(CN_12) > MaxStackSize_Task)) && (StackSizeDef(CN_12) != cfgShared)
  #error "Parameter 'StackSize on task CN_12' is misspelled or incorrect."
#endif

#if defined(CN_13) && (TaskInclude(CN_13) != cfgExclude) && ((StackSizeDef(CN_13) < MinStackSize_Task) || (StackSizeDef(CN_13) > MaxStackSize_Task)) && (StackSizeDef(CN_13) != cfgShared)
  #error "Parameter 'StackSize on task CN_13' is misspelled or incorrect."
#endif

#if defined(CN_14) && (TaskInclude(CN_14) != cfgExclude) && ((StackSizeDef(CN_14) < MinStackSize_Task) || (StackSizeDef(CN_14) > MaxStackSize_Task)) && (StackSizeDef(CN_14) != cfgShared)
  #error "Parameter 'StackSize on task CN_14' is misspelled or incorrect."
#endif

#if defined(CN_15) && (TaskInclude(CN_15) != cfgExclude) && ((StackSizeDef(CN_15) < MinStackSize_Task) || (StackSizeDef(CN_15) > MaxStackSize_Task)) && (StackSizeDef(CN_15) != cfgShared)
  #error "Parameter 'StackSize on task CN_15' is misspelled or incorrect."
#endif

#else

#if (((StackSizeOverride) < MinStackSize_Task) || ((StackSizeOverride) > MaxStackSize_Task))  && (StackSizeOverride != cfgShared)
  #error "Parameter 'StackSizeOverride' is misspelled or incorrect."
#endif

#endif


#if defined(CN_00) && (TaskInclude(CN_00) != cfgExclude) && ((StackSize(CN_00) < (defTaskStackMinimum)) || (StackSize(CN_00) < (RegisterUseSize(RegisterUse(CN_00)) + defTaskStackGround + StackSafety)))
  #error "Parameter 'StackSize on task CN_00' is too small."
#endif

#if defined(CN_01) && (TaskInclude(CN_01) != cfgExclude) && ((StackSize(CN_01) < (defTaskStackMinimum)) || (StackSize(CN_01) < (RegisterUseSize(RegisterUse(CN_01)) + defTaskStackGround + StackSafety)))
  #error "Parameter 'StackSize on task CN_01' is too small."
#endif

#if defined(CN_02) && (TaskInclude(CN_02) != cfgExclude) && ((StackSize(CN_02) < (defTaskStackMinimum)) || (StackSize(CN_02) < (RegisterUseSize(RegisterUse(CN_02)) + defTaskStackGround + StackSafety)))
  #error "Parameter 'StackSize on task CN_02' is too small."
#endif

#if defined(CN_03) && (TaskInclude(CN_03) != cfgExclude) && ((StackSize(CN_03) < (defTaskStackMinimum)) || (StackSize(CN_03) < (RegisterUseSize(RegisterUse(CN_03)) + defTaskStackGround + StackSafety)))
  #error "Parameter 'StackSize on task CN_03' is too small."
#endif

#if defined(CN_04) && (TaskInclude(CN_04) != cfgExclude) && ((StackSize(CN_04) < (defTaskStackMinimum)) || (StackSize(CN_04) < (RegisterUseSize(RegisterUse(CN_04)) + defTaskStackGround + StackSafety)))
  #error "Parameter 'StackSize on task CN_04' is too small."
#endif

#if defined(CN_05) && (TaskInclude(CN_05) != cfgExclude) && ((StackSize(CN_05) < (defTaskStackMinimum)) || (StackSize(CN_05) < (RegisterUseSize(RegisterUse(CN_05)) + defTaskStackGround + StackSafety)))
  #error "Parameter 'StackSize on task CN_05' is too small."
#endif

#if defined(CN_06) && (TaskInclude(CN_06) != cfgExclude) && ((StackSize(CN_06) < (defTaskStackMinimum)) || (StackSize(CN_06) < (RegisterUseSize(RegisterUse(CN_06)) + defTaskStackGround + StackSafety)))
  #error "Parameter 'StackSize on task CN_06' is too small."
#endif

#if defined(CN_07) && (TaskInclude(CN_07) != cfgExclude) && ((StackSize(CN_07) < (defTaskStackMinimum)) || (StackSize(CN_07) < (RegisterUseSize(RegisterUse(CN_07)) + defTaskStackGround + StackSafety)))
  #error "Parameter 'StackSize on task CN_07' is too small."
#endif

#if defined(CN_08) && (TaskInclude(CN_08) != cfgExclude) && ((StackSize(CN_08) < (defTaskStackMinimum)) || (StackSize(CN_08) < (RegisterUseSize(RegisterUse(CN_08)) + defTaskStackGround + StackSafety)))
  #error "Parameter 'StackSize on task CN_08' is too small."
#endif

#if defined(CN_09) && (TaskInclude(CN_09) != cfgExclude) && ((StackSize(CN_09) < (defTaskStackMinimum)) || (StackSize(CN_09) < (RegisterUseSize(RegisterUse(CN_09)) + defTaskStackGround + StackSafety)))
  #error "Parameter 'StackSize on task CN_09' is too small."
#endif

#if defined(CN_10) && (TaskInclude(CN_10) != cfgExclude) && ((StackSize(CN_10) < (defTaskStackMinimum)) || (StackSize(CN_10) < (RegisterUseSize(RegisterUse(CN_10)) + defTaskStackGround + StackSafety)))
  #error "Parameter 'StackSize on task CN_10' is too small."
#endif

#if defined(CN_11) && (TaskInclude(CN_11) != cfgExclude) && ((StackSize(CN_11) < (defTaskStackMinimum)) || (StackSize(CN_11) < (RegisterUseSize(RegisterUse(CN_11)) + defTaskStackGround + StackSafety)))
  #error "Parameter 'StackSize on task CN_11' is too small."
#endif

#if defined(CN_12) && (TaskInclude(CN_12) != cfgExclude) && ((StackSize(CN_12) < (defTaskStackMinimum)) || (StackSize(CN_12) < (RegisterUseSize(RegisterUse(CN_12)) + defTaskStackGround + StackSafety)))
  #error "Parameter 'StackSize on task CN_12' is too small."
#endif

#if defined(CN_13) && (TaskInclude(CN_13) != cfgExclude) && ((StackSize(CN_13) < (defTaskStackMinimum)) || (StackSize(CN_13) < (RegisterUseSize(RegisterUse(CN_13)) + defTaskStackGround + StackSafety)))
  #error "Parameter 'StackSize on task CN_13' is too small."
#endif

#if defined(CN_14) && (TaskInclude(CN_14) != cfgExclude) && ((StackSize(CN_14) < (defTaskStackMinimum)) || (StackSize(CN_14) < (RegisterUseSize(RegisterUse(CN_14)) + defTaskStackGround + StackSafety)))
  #error "Parameter 'StackSize on task CN_14' is too small."
#endif

#if defined(CN_15) && (TaskInclude(CN_15) != cfgExclude) && ((StackSize(CN_15) < (defTaskStackMinimum)) || (StackSize(CN_15) < (RegisterUseSize(RegisterUse(CN_15)) + defTaskStackGround + StackSafety)))
  #error "Parameter 'StackSize on task CN_14' is too small."
#endif


#if (( defined(CN_00) && (TaskInclude(CN_00) != cfgExclude) && (StackIsShared(CN_00)) ) || \
     ( defined(CN_01) && (TaskInclude(CN_01) != cfgExclude) && (StackIsShared(CN_01)) ) || \
     ( defined(CN_02) && (TaskInclude(CN_02) != cfgExclude) && (StackIsShared(CN_02)) ) || \
     ( defined(CN_03) && (TaskInclude(CN_03) != cfgExclude) && (StackIsShared(CN_03)) ) || \
     ( defined(CN_04) && (TaskInclude(CN_04) != cfgExclude) && (StackIsShared(CN_04)) ) || \
     ( defined(CN_05) && (TaskInclude(CN_05) != cfgExclude) && (StackIsShared(CN_05)) ) || \
     ( defined(CN_06) && (TaskInclude(CN_06) != cfgExclude) && (StackIsShared(CN_06)) ) || \
     ( defined(CN_07) && (TaskInclude(CN_07) != cfgExclude) && (StackIsShared(CN_07)) ) || \
     ( defined(CN_08) && (TaskInclude(CN_08) != cfgExclude) && (StackIsShared(CN_08)) ) || \
     ( defined(CN_09) && (TaskInclude(CN_09) != cfgExclude) && (StackIsShared(CN_09)) ) || \
     ( defined(CN_10) && (TaskInclude(CN_10) != cfgExclude) && (StackIsShared(CN_10)) ) || \
     ( defined(CN_11) && (TaskInclude(CN_11) != cfgExclude) && (StackIsShared(CN_11)) ) || \
     ( defined(CN_12) && (TaskInclude(CN_12) != cfgExclude) && (StackIsShared(CN_12)) ) || \
     ( defined(CN_13) && (TaskInclude(CN_13) != cfgExclude) && (StackIsShared(CN_13)) ) || \
     ( defined(CN_14) && (TaskInclude(CN_14) != cfgExclude) && (StackIsShared(CN_14)) ) || \
     ( defined(CN_15) && (TaskInclude(CN_15) != cfgExclude) && (StackIsShared(CN_15)) )) && \
     ( (StackSizeShared) == 0 )
  #error Use of the Shared Stack requires a stack of suitable size.
#endif



#if (cfgUseEquidistantTicks == cfgFalse )

#if !defined(TimeSliceOverride) || ((TimeSliceOverride) == cfgOverrideNon)

#if defined(CN_00) && (TaskInclude(CN_00) != cfgExclude) && ((TimeSlice(CN_00) < MinTimeSlice_Task) || (TimeSlice(CN_00) > MaxTimeSlice_Task))
  #error "Parameter 'TimeSlice on task CN_00' is misspelled or incorrect."
#endif

#if defined(CN_01) && (TaskInclude(CN_01) != cfgExclude) && ((TimeSlice(CN_01) < MinTimeSlice_Task) || (TimeSlice(CN_01) > MaxTimeSlice_Task))
  #error "Parameter 'TimeSlice on task CN_01' is misspelled or incorrect."
#endif

#if defined(CN_02) && (TaskInclude(CN_02) != cfgExclude) && ((TimeSlice(CN_02) < MinTimeSlice_Task) || (TimeSlice(CN_02) > MaxTimeSlice_Task))
  #error "Parameter 'TimeSlice on task CN_02' is misspelled or incorrect."
#endif

#if defined(CN_03) && (TaskInclude(CN_03) != cfgExclude) && ((TimeSlice(CN_03) < MinTimeSlice_Task) || (TimeSlice(CN_03) > MaxTimeSlice_Task))
  #error "Parameter 'TimeSlice on task CN_03' is misspelled or incorrect."
#endif

#if defined(CN_04) && (TaskInclude(CN_04) != cfgExclude) && ((TimeSlice(CN_04) < MinTimeSlice_Task) || (TimeSlice(CN_04) > MaxTimeSlice_Task))
  #error "Parameter 'TimeSlice on task CN_04' is misspelled or incorrect."
#endif

#if defined(CN_05) && (TaskInclude(CN_05) != cfgExclude) && ((TimeSlice(CN_05) < MinTimeSlice_Task) || (TimeSlice(CN_05) > MaxTimeSlice_Task))
  #error "Parameter 'TimeSlice on task CN_05' is misspelled or incorrect."
#endif

#if defined(CN_06) && (TaskInclude(CN_06) != cfgExclude) && ((TimeSlice(CN_06) < MinTimeSlice_Task) || (TimeSlice(CN_06) > MaxTimeSlice_Task))
  #error "Parameter 'TimeSlice on task CN_06' is misspelled or incorrect."
#endif

#if defined(CN_07) && (TaskInclude(CN_07) != cfgExclude) && ((TimeSlice(CN_07) < MinTimeSlice_Task) || (TimeSlice(CN_07) > MaxTimeSlice_Task))
  #error "Parameter 'TimeSlice on task CN_07' is misspelled or incorrect."
#endif

#if defined(CN_08) && (TaskInclude(CN_08) != cfgExclude) && ((TimeSlice(CN_08) < MinTimeSlice_Task) || (TimeSlice(CN_08) > MaxTimeSlice_Task))
  #error "Parameter 'TimeSlice on task CN_08' is misspelled or incorrect."
#endif

#if defined(CN_09) && (TaskInclude(CN_09) != cfgExclude) && ((TimeSlice(CN_09) < MinTimeSlice_Task) || (TimeSlice(CN_09) > MaxTimeSlice_Task))
  #error "Parameter 'TimeSlice on task CN_09' is misspelled or incorrect."
#endif

#if defined(CN_10) && (TaskInclude(CN_10) != cfgExclude) && ((TimeSlice(CN_10) < MinTimeSlice_Task) || (TimeSlice(CN_10) > MaxTimeSlice_Task))
  #error "Parameter 'TimeSlice on task CN_10' is misspelled or incorrect."
#endif

#if defined(CN_11) && (TaskInclude(CN_11) != cfgExclude) && ((TimeSlice(CN_11) < MinTimeSlice_Task) || (TimeSlice(CN_11) > MaxTimeSlice_Task))
  #error "Parameter 'TimeSlice on task CN_11' is misspelled or incorrect."
#endif

#if defined(CN_12) && (TaskInclude(CN_12) != cfgExclude) && ((TimeSlice(CN_12) < MinTimeSlice_Task) || (TimeSlice(CN_12) > MaxTimeSlice_Task))
  #error "Parameter 'TimeSlice on task CN_12' is misspelled or incorrect."
#endif

#if defined(CN_13) && (TaskInclude(CN_13) != cfgExclude) && ((TimeSlice(CN_13) < MinTimeSlice_Task) || (TimeSlice(CN_13) > MaxTimeSlice_Task))
  #error "Parameter 'TimeSlice on task CN_13' is misspelled or incorrect."
#endif

#if defined(CN_14) && (TaskInclude(CN_14) != cfgExclude) && ((TimeSlice(CN_14) < MinTimeSlice_Task) || (TimeSlice(CN_14) > MaxTimeSlice_Task))
  #error "Parameter 'TimeSlice on task CN_14' is misspelled or incorrect."
#endif

#if defined(CN_15) && (TaskInclude(CN_15) != cfgExclude) && ((TimeSlice(CN_15) < MinTimeSlice_Task) || (TimeSlice(CN_15) > MaxTimeSlice_Task))
  #error "Parameter 'TimeSlice on task CN_15' is misspelled or incorrect."
#endif

#else

#if ((TimeSliceOverride) < MinTimeSlice_Task) || ((TimeSliceOverride) > MaxTimeSlice_Task)
  #error "Parameter 'TimeSliceOverride' is misspelled or incorrect."
#endif

#endif

#endif


#if !defined(CapabilitiesOverride) || ((CapabilitiesOverride) == cfgOverrideNon)

#if defined(CN_00) && (TaskInclude(CN_00) != cfgExclude) && (((Capabilities(CN_00)) < MinCapabilities_Task) || ((Capabilities(CN_00)) > MaxCapabilities_Task))
  #error "Parameter 'Capabilities on task CN_00' is misspelled or incorrect."
#endif

#if defined(CN_01) && (TaskInclude(CN_01) != cfgExclude) && (((Capabilities(CN_01)) < MinCapabilities_Task) || ((Capabilities(CN_01)) > MaxCapabilities_Task))
  #error "Parameter 'Capabilities on task CN_01' is misspelled or incorrect."
#endif

#if defined(CN_02) && (TaskInclude(CN_02) != cfgExclude) && (((Capabilities(CN_02)) < MinCapabilities_Task) || ((Capabilities(CN_02)) > MaxCapabilities_Task))
  #error "Parameter 'Capabilities on task CN_02' is misspelled or incorrect."
#endif

#if defined(CN_03) && (TaskInclude(CN_03) != cfgExclude) && (((Capabilities(CN_03)) < MinCapabilities_Task) || ((Capabilities(CN_03)) > MaxCapabilities_Task))
  #error "Parameter 'Capabilities on task CN_03' is misspelled or incorrect."
#endif

#if defined(CN_04) && (TaskInclude(CN_04) != cfgExclude) && (((Capabilities(CN_04)) < MinCapabilities_Task) || ((Capabilities(CN_04)) > MaxCapabilities_Task))
  #error "Parameter 'Capabilities on task CN_04' is misspelled or incorrect."
#endif

#if defined(CN_05) && (TaskInclude(CN_05) != cfgExclude) && (((Capabilities(CN_05)) < MinCapabilities_Task) || ((Capabilities(CN_05)) > MaxCapabilities_Task))
  #error "Parameter 'Capabilities on task CN_05' is misspelled or incorrect."
#endif

#if defined(CN_06) && (TaskInclude(CN_06) != cfgExclude) && (((Capabilities(CN_06)) < MinCapabilities_Task) || ((Capabilities(CN_06)) > MaxCapabilities_Task))
  #error "Parameter 'Capabilities on task CN_06' is misspelled or incorrect."
#endif

#if defined(CN_07) && (TaskInclude(CN_07) != cfgExclude) && (((Capabilities(CN_07)) < MinCapabilities_Task) || ((Capabilities(CN_07)) > MaxCapabilities_Task))
  #error "Parameter 'Capabilities on task CN_07' is misspelled or incorrect."
#endif

#if defined(CN_08) && (TaskInclude(CN_08) != cfgExclude) && (((Capabilities(CN_08)) < MinCapabilities_Task) || ((Capabilities(CN_08)) > MaxCapabilities_Task))
  #error "Parameter 'Capabilities on task CN_08' is misspelled or incorrect."
#endif

#if defined(CN_09) && (TaskInclude(CN_09) != cfgExclude) && (((Capabilities(CN_09)) < MinCapabilities_Task) || ((Capabilities(CN_09)) > MaxCapabilities_Task))
  #error "Parameter 'Capabilities on task CN_09' is misspelled or incorrect."
#endif

#if defined(CN_10) && (TaskInclude(CN_10) != cfgExclude) && (((Capabilities(CN_10)) < MinCapabilities_Task) || ((Capabilities(CN_10)) > MaxCapabilities_Task))
  #error "Parameter 'Capabilities on task CN_10' is misspelled or incorrect."
#endif

#if defined(CN_11) && (TaskInclude(CN_11) != cfgExclude) && (((Capabilities(CN_11)) < MinCapabilities_Task) || ((Capabilities(CN_11)) > MaxCapabilities_Task))
  #error "Parameter 'Capabilities on task CN_11' is misspelled or incorrect."
#endif

#if defined(CN_12) && (TaskInclude(CN_12) != cfgExclude) && (((Capabilities(CN_12)) < MinCapabilities_Task) || ((Capabilities(CN_12)) > MaxCapabilities_Task))
  #error "Parameter 'Capabilities on task CN_12' is misspelled or incorrect."
#endif

#if defined(CN_13) && (TaskInclude(CN_13) != cfgExclude) && (((Capabilities(CN_13)) < MinCapabilities_Task) || ((Capabilities(CN_13)) > MaxCapabilities_Task))
  #error "Parameter 'Capabilities on task CN_13' is misspelled or incorrect."
#endif

#if defined(CN_14) && (TaskInclude(CN_14) != cfgExclude) && (((Capabilities(CN_14)) < MinCapabilities_Task) || ((Capabilities(CN_14)) > MaxCapabilities_Task))
  #error "Parameter 'Capabilities on task CN_14' is misspelled or incorrect."
#endif

#if defined(CN_15) && (TaskInclude(CN_15) != cfgExclude) && (((Capabilities(CN_15)) < MinCapabilities_Task) || ((Capabilities(CN_15)) > MaxCapabilities_Task))
  #error "Parameter 'Capabilities on task CN_15' is misspelled or incorrect."
#endif

#else

#if ((CapabilitiesOverride) < MinCapabilities_Task) || ((CapabilitiesOverride) > MaxCapabilities_Task)
  #error "Parameter 'CapabilitiesOverride' is misspelled or incorrect."
#endif

#endif


#if !defined(PriorityOverride) || ((PriorityOverride) == cfgOverrideNon)

#if defined(CN_00) && (TaskInclude(CN_00) != cfgExclude) && ((Priority(CN_00) < MinPriority_Task) || (Priority(CN_00) > MaxPriority_Task))
  #error "Parameter 'Priority on task CN_00' is misspelled or incorrect."
#endif

#if defined(CN_01) && (TaskInclude(CN_01) != cfgExclude) && ((Priority(CN_01) < MinPriority_Task) || (Priority(CN_01) > MaxPriority_Task))
  #error "Parameter 'Priority on task CN_01' is misspelled or incorrect."
#endif

#if defined(CN_02) && (TaskInclude(CN_02) != cfgExclude) && ((Priority(CN_02) < MinPriority_Task) || (Priority(CN_02) > MaxPriority_Task))
  #error "Parameter 'Priority on task CN_02' is misspelled or incorrect."
#endif

#if defined(CN_03) && (TaskInclude(CN_03) != cfgExclude) && ((Priority(CN_03) < MinPriority_Task) || (Priority(CN_03) > MaxPriority_Task))
  #error "Parameter 'Priority on task CN_03' is misspelled or incorrect."
#endif

#if defined(CN_04) && (TaskInclude(CN_04) != cfgExclude) && ((Priority(CN_04) < MinPriority_Task) || (Priority(CN_04) > MaxPriority_Task))
  #error "Parameter 'Priority on task CN_04' is misspelled or incorrect."
#endif

#if defined(CN_05) && (TaskInclude(CN_05) != cfgExclude) && ((Priority(CN_05) < MinPriority_Task) || (Priority(CN_05) > MaxPriority_Task))
  #error "Parameter 'Priority on task CN_05' is misspelled or incorrect."
#endif

#if defined(CN_06) && (TaskInclude(CN_06) != cfgExclude) && ((Priority(CN_06) < MinPriority_Task) || (Priority(CN_06) > MaxPriority_Task))
  #error "Parameter 'Priority on task CN_06' is misspelled or incorrect."
#endif

#if defined(CN_07) && (TaskInclude(CN_07) != cfgExclude) && ((Priority(CN_07) < MinPriority_Task) || (Priority(CN_07) > MaxPriority_Task))
  #error "Parameter 'Priority on task CN_07' is misspelled or incorrect."
#endif

#if defined(CN_08) && (TaskInclude(CN_08) != cfgExclude) && ((Priority(CN_08) < MinPriority_Task) || (Priority(CN_08) > MaxPriority_Task))
  #error "Parameter 'Priority on task CN_08' is misspelled or incorrect."
#endif

#if defined(CN_09) && (TaskInclude(CN_09) != cfgExclude) && ((Priority(CN_09) < MinPriority_Task) || (Priority(CN_09) > MaxPriority_Task))
  #error "Parameter 'Priority on task CN_09' is misspelled or incorrect."
#endif

#if defined(CN_10) && (TaskInclude(CN_10) != cfgExclude) && ((Priority(CN_10) < MinPriority_Task) || (Priority(CN_10) > MaxPriority_Task))
  #error "Parameter 'Priority on task CN_10' is misspelled or incorrect."
#endif

#if defined(CN_11) && (TaskInclude(CN_11) != cfgExclude) && ((Priority(CN_11) < MinPriority_Task) || (Priority(CN_11) > MaxPriority_Task))
  #error "Parameter 'Priority on task CN_11' is misspelled or incorrect."
#endif

#if defined(CN_12) && (TaskInclude(CN_12) != cfgExclude) && ((Priority(CN_12) < MinPriority_Task) || (Priority(CN_12) > MaxPriority_Task))
  #error "Parameter 'Priority on task CN_12' is misspelled or incorrect."
#endif

#if defined(CN_13) && (TaskInclude(CN_13) != cfgExclude) && ((Priority(CN_13) < MinPriority_Task) || (Priority(CN_13) > MaxPriority_Task))
  #error "Parameter 'Priority on task CN_13' is misspelled or incorrect."
#endif

#if defined(CN_14) && (TaskInclude(CN_14) != cfgExclude) && ((Priority(CN_14) < MinPriority_Task) || (Priority(CN_14) > MaxPriority_Task))
  #error "Parameter 'Priority on task CN_14' is misspelled or incorrect."
#endif

#if defined(CN_15) && (TaskInclude(CN_15) != cfgExclude) && ((Priority(CN_15) < MinPriority_Task) || (Priority(CN_15) > MaxPriority_Task))
  #error "Parameter 'Priority on task CN_15' is misspelled or incorrect."
#endif

#else

#if ((PriorityOverride) < MinPriority_Task) || ((PriorityOverride) > MaxPriority_Task)
  #error "Parameter 'PriorityOverride' is misspelled or incorrect."
#endif

#endif


#if !defined(RegisterUseOverride) || ((RegisterUseOverride) == cfgOverrideNon)

#if defined(CN_00) && (TaskInclude(CN_00) != cfgExclude) && (((RegisterUse(CN_00)) < MinRegisterUse_Task) || ((RegisterUse(CN_00)) > MaxRegisterUse_Task))
  #error "Parameter 'RegisterUse on task CN_00' is misspelled or incorrect."
#endif

#if defined(CN_01) && (TaskInclude(CN_01) != cfgExclude) && (((RegisterUse(CN_01)) < MinRegisterUse_Task) || ((RegisterUse(CN_01)) > MaxRegisterUse_Task))
  #error "Parameter 'RegisterUse on task CN_01' is misspelled or incorrect."
#endif

#if defined(CN_02) && (TaskInclude(CN_02) != cfgExclude) && (((RegisterUse(CN_02)) < MinRegisterUse_Task) || ((RegisterUse(CN_02)) > MaxRegisterUse_Task))
  #error "Parameter 'RegisterUse on task CN_02' is misspelled or incorrect."
#endif

#if defined(CN_03) && (TaskInclude(CN_03) != cfgExclude) && (((RegisterUse(CN_03)) < MinRegisterUse_Task) || ((RegisterUse(CN_03)) > MaxRegisterUse_Task))
  #error "Parameter 'RegisterUse on task CN_03' is misspelled or incorrect."
#endif

#if defined(CN_04) && (TaskInclude(CN_04) != cfgExclude) && (((RegisterUse(CN_04)) < MinRegisterUse_Task) || ((RegisterUse(CN_04)) > MaxRegisterUse_Task))
  #error "Parameter 'RegisterUse on task CN_04' is misspelled or incorrect."
#endif

#if defined(CN_05) && (TaskInclude(CN_05) != cfgExclude) && (((RegisterUse(CN_05)) < MinRegisterUse_Task) || ((RegisterUse(CN_05)) > MaxRegisterUse_Task))
  #error "Parameter 'RegisterUse on task CN_05' is misspelled or incorrect."
#endif

#if defined(CN_06) && (TaskInclude(CN_06) != cfgExclude) && (((RegisterUse(CN_06)) < MinRegisterUse_Task) || ((RegisterUse(CN_06)) > MaxRegisterUse_Task))
  #error "Parameter 'RegisterUse on task CN_06' is misspelled or incorrect."
#endif

#if defined(CN_07) && (TaskInclude(CN_07) != cfgExclude) && (((RegisterUse(CN_07)) < MinRegisterUse_Task) || ((RegisterUse(CN_07)) > MaxRegisterUse_Task))
  #error "Parameter 'RegisterUse on task CN_07' is misspelled or incorrect."
#endif

#if defined(CN_08) && (TaskInclude(CN_08) != cfgExclude) && (((RegisterUse(CN_08)) < MinRegisterUse_Task) || ((RegisterUse(CN_08)) > MaxRegisterUse_Task))
  #error "Parameter 'RegisterUse on task CN_08' is misspelled or incorrect."
#endif

#if defined(CN_09) && (TaskInclude(CN_09) != cfgExclude) && (((RegisterUse(CN_09)) < MinRegisterUse_Task) || ((RegisterUse(CN_09)) > MaxRegisterUse_Task))
  #error "Parameter 'RegisterUse on task CN_09' is misspelled or incorrect."
#endif

#if defined(CN_10) && (TaskInclude(CN_10) != cfgExclude) && (((RegisterUse(CN_10)) < MinRegisterUse_Task) || ((RegisterUse(CN_10)) > MaxRegisterUse_Task))
  #error "Parameter 'RegisterUse on task CN_10' is misspelled or incorrect."
#endif

#if defined(CN_11) && (TaskInclude(CN_11) != cfgExclude) && (((RegisterUse(CN_11)) < MinRegisterUse_Task) || ((RegisterUse(CN_11)) > MaxRegisterUse_Task))
  #error "Parameter 'RegisterUse on task CN_11' is misspelled or incorrect."
#endif

#if defined(CN_12) && (TaskInclude(CN_12) != cfgExclude) && (((RegisterUse(CN_12)) < MinRegisterUse_Task) || ((RegisterUse(CN_12)) > MaxRegisterUse_Task))
  #error "Parameter 'RegisterUse on task CN_12' is misspelled or incorrect."
#endif

#if defined(CN_13) && (TaskInclude(CN_13) != cfgExclude) && (((RegisterUse(CN_13)) < MinRegisterUse_Task) || ((RegisterUse(CN_13)) > MaxRegisterUse_Task))
  #error "Parameter 'RegisterUse on task CN_13' is misspelled or incorrect."
#endif

#if defined(CN_14) && (TaskInclude(CN_14) != cfgExclude) && (((RegisterUse(CN_14)) < MinRegisterUse_Task) || ((RegisterUse(CN_14)) > MaxRegisterUse_Task))
  #error "Parameter 'RegisterUse on task CN_14' is misspelled or incorrect."
#endif

#if defined(CN_15) && (TaskInclude(CN_15) != cfgExclude) && (((RegisterUse(CN_15)) < MinRegisterUse_Task) || ((RegisterUse(CN_15)) > MaxRegisterUse_Task))
  #error "Parameter 'RegisterUse on task CN_15' is misspelled or incorrect."
#endif

#else

#if ((RegisterUseOverride) < MinRegisterUse_Task) || ((RegisterUseOverride) > MaxRegisterUse_Task)
  #error "Parameter 'RegisterUseOverride' is misspelled or incorrect."
#endif

#endif


#if (cfgCheckRegisters == cfgTrue )

#if !defined(RegisterCheckOverride) || ((RegisterCheckOverride) == cfgOverrideNon)

#if defined(CN_00) && (TaskInclude(CN_00) != cfgExclude) && (((RegisterCheck(CN_00)) < MinRegisterCheck_Task) || ((RegisterCheck(CN_00)) > MaxRegisterCheck_Task))
  #error "Parameter 'RegisterCheck on task CN_00' is misspelled or incorrect."
#endif

#if defined(CN_01) && (TaskInclude(CN_01) != cfgExclude) && (((RegisterCheck(CN_01)) < MinRegisterCheck_Task) || ((RegisterCheck(CN_01)) > MaxRegisterCheck_Task))
  #error "Parameter 'RegisterCheck on task CN_01' is misspelled or incorrect."
#endif

#if defined(CN_02) && (TaskInclude(CN_02) != cfgExclude) && (((RegisterCheck(CN_02)) < MinRegisterCheck_Task) || ((RegisterCheck(CN_02)) > MaxRegisterCheck_Task))
  #error "Parameter 'RegisterCheck on task CN_02' is misspelled or incorrect."
#endif

#if defined(CN_03) && (TaskInclude(CN_03) != cfgExclude) && (((RegisterCheck(CN_03)) < MinRegisterCheck_Task) || ((RegisterCheck(CN_03)) > MaxRegisterCheck_Task))
  #error "Parameter 'RegisterCheck on task CN_03' is misspelled or incorrect."
#endif

#if defined(CN_04) && (TaskInclude(CN_04) != cfgExclude) && (((RegisterCheck(CN_04)) < MinRegisterCheck_Task) || ((RegisterCheck(CN_04)) > MaxRegisterCheck_Task))
  #error "Parameter 'RegisterCheck on task CN_04' is misspelled or incorrect."
#endif

#if defined(CN_05) && (TaskInclude(CN_05) != cfgExclude) && (((RegisterCheck(CN_05)) < MinRegisterCheck_Task) || ((RegisterCheck(CN_05)) > MaxRegisterCheck_Task))
  #error "Parameter 'RegisterCheck on task CN_05' is misspelled or incorrect."
#endif

#if defined(CN_06) && (TaskInclude(CN_06) != cfgExclude) && (((RegisterCheck(CN_06)) < MinRegisterCheck_Task) || ((RegisterCheck(CN_06)) > MaxRegisterCheck_Task))
  #error "Parameter 'RegisterCheck on task CN_06' is misspelled or incorrect."
#endif

#if defined(CN_07) && (TaskInclude(CN_07) != cfgExclude) && (((RegisterCheck(CN_07)) < MinRegisterCheck_Task) || ((RegisterCheck(CN_07)) > MaxRegisterCheck_Task))
  #error "Parameter 'RegisterCheck on task CN_07' is misspelled or incorrect."
#endif

#if defined(CN_08) && (TaskInclude(CN_08) != cfgExclude) && (((RegisterCheck(CN_08)) < MinRegisterCheck_Task) || ((RegisterCheck(CN_08)) > MaxRegisterCheck_Task))
  #error "Parameter 'RegisterCheck on task CN_08' is misspelled or incorrect."
#endif

#if defined(CN_09) && (TaskInclude(CN_09) != cfgExclude) && (((RegisterCheck(CN_09)) < MinRegisterCheck_Task) || ((RegisterCheck(CN_09)) > MaxRegisterCheck_Task))
  #error "Parameter 'RegisterCheck on task CN_09' is misspelled or incorrect."
#endif

#if defined(CN_10) && (TaskInclude(CN_10) != cfgExclude) && (((RegisterCheck(CN_10)) < MinRegisterCheck_Task) || ((RegisterCheck(CN_10)) > MaxRegisterCheck_Task))
  #error "Parameter 'RegisterCheck on task CN_10' is misspelled or incorrect."
#endif

#if defined(CN_11) && (TaskInclude(CN_11) != cfgExclude) && (((RegisterCheck(CN_11)) < MinRegisterCheck_Task) || ((RegisterCheck(CN_11)) > MaxRegisterCheck_Task))
  #error "Parameter 'RegisterCheck on task CN_11' is misspelled or incorrect."
#endif

#if defined(CN_12) && (TaskInclude(CN_12) != cfgExclude) && (((RegisterCheck(CN_12)) < MinRegisterCheck_Task) || ((RegisterCheck(CN_12)) > MaxRegisterCheck_Task))
  #error "Parameter 'RegisterCheck on task CN_12' is misspelled or incorrect."
#endif

#if defined(CN_13) && (TaskInclude(CN_13) != cfgExclude) && (((RegisterCheck(CN_13)) < MinRegisterCheck_Task) || ((RegisterCheck(CN_13)) > MaxRegisterCheck_Task))
  #error "Parameter 'RegisterCheck on task CN_13' is misspelled or incorrect."
#endif

#if defined(CN_14) && (TaskInclude(CN_14) != cfgExclude) && (((RegisterCheck(CN_14)) < MinRegisterCheck_Task) || ((RegisterCheck(CN_14)) > MaxRegisterCheck_Task))
  #error "Parameter 'RegisterCheck on task CN_14' is misspelled or incorrect."
#endif

#if defined(CN_15) && (TaskInclude(CN_15) != cfgExclude) && (((RegisterCheck(CN_15)) < MinRegisterCheck_Task) || ((RegisterCheck(CN_15)) > MaxRegisterCheck_Task))
  #error "Parameter 'RegisterCheck on task CN_15' is misspelled or incorrect."
#endif

#else

#if ((RegisterCheckOverride) < MinRegisterCheck_Task) || ((RegisterCheckOverride) > MaxRegisterCheck_Task)
  #error "Parameter 'RegisterCheckOverride' is misspelled or incorrect."
#endif

#endif

#endif


#if !defined(InterruptStartOverride) || ((InterruptStartOverride) == cfgOverrideNon)

#if defined(CN_00) && (TaskInclude(CN_00) != cfgExclude) && (((InterruptStart(CN_00)) & (cfgGlobSet | cfgGlobClear | cfgTickSet | cfgTickClear) ) == 0)
  #error "Parameter 'InterruptStart on task CN_00' is misspelled or incorrect."
#endif

#if defined(CN_01) && (TaskInclude(CN_01) != cfgExclude) && (((InterruptStart(CN_01)) & (cfgGlobSet | cfgGlobClear | cfgTickSet | cfgTickClear) ) == 0)
  #error "Parameter 'InterruptStart on task CN_01' is misspelled or incorrect."
#endif

#if defined(CN_02) && (TaskInclude(CN_02) != cfgExclude) && (((InterruptStart(CN_02)) & (cfgGlobSet | cfgGlobClear | cfgTickSet | cfgTickClear) ) == 0)
  #error "Parameter 'InterruptStart on task CN_02' is misspelled or incorrect."
#endif

#if defined(CN_03) && (TaskInclude(CN_03) != cfgExclude) && (((InterruptStart(CN_03)) & (cfgGlobSet | cfgGlobClear | cfgTickSet | cfgTickClear) ) == 0)
  #error "Parameter 'InterruptStart on task CN_03' is misspelled or incorrect."
#endif

#if defined(CN_04) && (TaskInclude(CN_04) != cfgExclude) && (((InterruptStart(CN_04)) & (cfgGlobSet | cfgGlobClear | cfgTickSet | cfgTickClear) ) == 0)
  #error "Parameter 'InterruptStart on task CN_04' is misspelled or incorrect."
#endif

#if defined(CN_05) && (TaskInclude(CN_05) != cfgExclude) && (((InterruptStart(CN_05)) & (cfgGlobSet | cfgGlobClear | cfgTickSet | cfgTickClear) ) == 0)
  #error "Parameter 'InterruptStart on task CN_05' is misspelled or incorrect."
#endif

#if defined(CN_06) && (TaskInclude(CN_06) != cfgExclude) && (((InterruptStart(CN_06)) & (cfgGlobSet | cfgGlobClear | cfgTickSet | cfgTickClear) ) == 0)
  #error "Parameter 'InterruptStart on task CN_06' is misspelled or incorrect."
#endif

#if defined(CN_07) && (TaskInclude(CN_07) != cfgExclude) && (((InterruptStart(CN_07)) & (cfgGlobSet | cfgGlobClear | cfgTickSet | cfgTickClear) ) == 0)
  #error "Parameter 'InterruptStart on task CN_07' is misspelled or incorrect."
#endif

#if defined(CN_08) && (TaskInclude(CN_08) != cfgExclude) && (((InterruptStart(CN_08)) & (cfgGlobSet | cfgGlobClear | cfgTickSet | cfgTickClear) ) == 0)
  #error "Parameter 'InterruptStart on task CN_08' is misspelled or incorrect."
#endif

#if defined(CN_09) && (TaskInclude(CN_09) != cfgExclude) && (((InterruptStart(CN_09)) & (cfgGlobSet | cfgGlobClear | cfgTickSet | cfgTickClear) ) == 0)
  #error "Parameter 'InterruptStart on task CN_09' is misspelled or incorrect."
#endif

#if defined(CN_10) && (TaskInclude(CN_10) != cfgExclude) && (((InterruptStart(CN_10)) & (cfgGlobSet | cfgGlobClear | cfgTickSet | cfgTickClear) ) == 0)
  #error "Parameter 'InterruptStart on task CN_10' is misspelled or incorrect."
#endif

#if defined(CN_11) && (TaskInclude(CN_11) != cfgExclude) && (((InterruptStart(CN_11)) & (cfgGlobSet | cfgGlobClear | cfgTickSet | cfgTickClear) ) == 0)
  #error "Parameter 'InterruptStart on task CN_11' is misspelled or incorrect."
#endif

#if defined(CN_12) && (TaskInclude(CN_12) != cfgExclude) && (((InterruptStart(CN_12)) & (cfgGlobSet | cfgGlobClear | cfgTickSet | cfgTickClear) ) == 0)
  #error "Parameter 'InterruptStart on task CN_12' is misspelled or incorrect."
#endif

#if defined(CN_13) && (TaskInclude(CN_13) != cfgExclude) && (((InterruptStart(CN_13)) & (cfgGlobSet | cfgGlobClear | cfgTickSet | cfgTickClear) ) == 0)
  #error "Parameter 'InterruptStart on task CN_13' is misspelled or incorrect."
#endif

#if defined(CN_14) && (TaskInclude(CN_14) != cfgExclude) && (((InterruptStart(CN_14)) & (cfgGlobSet | cfgGlobClear | cfgTickSet | cfgTickClear) ) == 0)
  #error "Parameter 'InterruptStart on task CN_14' is misspelled or incorrect."
#endif

#if defined(CN_15) && (TaskInclude(CN_15) != cfgExclude) && (((InterruptStart(CN_15)) & (cfgGlobSet | cfgGlobClear | cfgTickSet | cfgTickClear) ) == 0)
  #error "Parameter 'InterruptStart on task CN_15' is misspelled or incorrect."
#endif

#else

#if ((InterruptStartOverride & (cfgGlobSet | cfgGlobClear | cfgTickSet | cfgTickClear) ) == 0)
  #error "Parameter 'InterruptStartOverride' is misspelled or incorrect."
#endif

#endif


#if (cfgUseSynchronization != cfgSyncNon)

#if !defined(SlotSizeOverride) || ((SlotSizeOverride) == cfgOverrideNon)

#if defined(CN_00) && (TaskInclude(CN_00) != cfgExclude) && (SlotSize(CN_00) != NullSlotSize_Task) && ((SlotSize(CN_00) < MinSlotSize_Task) || (SlotSize(CN_00) > MaxSlotSize_Task))
  #error "Parameter 'SlotSize on task CN_00' is misspelled or incorrect."
#endif

#if defined(CN_01) && (TaskInclude(CN_01) != cfgExclude) && (SlotSize(CN_01) != NullSlotSize_Task) && ((SlotSize(CN_01) < MinSlotSize_Task) || (SlotSize(CN_01) > MaxSlotSize_Task))
  #error "Parameter 'SlotSize on task CN_01' is misspelled or incorrect."
#endif

#if defined(CN_02) && (TaskInclude(CN_02) != cfgExclude) && (SlotSize(CN_02) != NullSlotSize_Task) && ((SlotSize(CN_02) < MinSlotSize_Task) || (SlotSize(CN_02) > MaxSlotSize_Task))
  #error "Parameter 'SlotSize on task CN_02' is misspelled or incorrect."
#endif

#if defined(CN_03) && (TaskInclude(CN_03) != cfgExclude) && (SlotSize(CN_03) != NullSlotSize_Task) && ((SlotSize(CN_03) < MinSlotSize_Task) || (SlotSize(CN_03) > MaxSlotSize_Task))
  #error "Parameter 'SlotSize on task CN_03' is misspelled or incorrect."
#endif

#if defined(CN_04) && (TaskInclude(CN_04) != cfgExclude) && (SlotSize(CN_04) != NullSlotSize_Task) && ((SlotSize(CN_04) < MinSlotSize_Task) || (SlotSize(CN_04) > MaxSlotSize_Task))
  #error "Parameter 'SlotSize on task CN_04' is misspelled or incorrect."
#endif

#if defined(CN_05) && (TaskInclude(CN_05) != cfgExclude) && (SlotSize(CN_05) != NullSlotSize_Task) && ((SlotSize(CN_05) < MinSlotSize_Task) || (SlotSize(CN_05) > MaxSlotSize_Task))
  #error "Parameter 'SlotSize on task CN_05' is misspelled or incorrect."
#endif

#if defined(CN_06) && (TaskInclude(CN_06) != cfgExclude) && (SlotSize(CN_06) != NullSlotSize_Task) && ((SlotSize(CN_06) < MinSlotSize_Task) || (SlotSize(CN_06) > MaxSlotSize_Task))
  #error "Parameter 'SlotSize on task CN_06' is misspelled or incorrect."
#endif

#if defined(CN_07) && (TaskInclude(CN_07) != cfgExclude) && (SlotSize(CN_07) != NullSlotSize_Task) && ((SlotSize(CN_07) < MinSlotSize_Task) || (SlotSize(CN_07) > MaxSlotSize_Task))
  #error "Parameter 'SlotSize on task CN_07' is misspelled or incorrect."
#endif

#if defined(CN_08) && (TaskInclude(CN_08) != cfgExclude) && (SlotSize(CN_08) != NullSlotSize_Task) && ((SlotSize(CN_08) < MinSlotSize_Task) || (SlotSize(CN_08) > MaxSlotSize_Task))
  #error "Parameter 'SlotSize on task CN_08' is misspelled or incorrect."
#endif

#if defined(CN_09) && (TaskInclude(CN_09) != cfgExclude) && (SlotSize(CN_09) != NullSlotSize_Task) && ((SlotSize(CN_09) < MinSlotSize_Task) || (SlotSize(CN_09) > MaxSlotSize_Task))
  #error "Parameter 'SlotSize on task CN_09' is misspelled or incorrect."
#endif

#if defined(CN_10) && (TaskInclude(CN_10) != cfgExclude) && (SlotSize(CN_10) != NullSlotSize_Task) && ((SlotSize(CN_10) < MinSlotSize_Task) || (SlotSize(CN_10) > MaxSlotSize_Task))
  #error "Parameter 'SlotSize on task CN_10' is misspelled or incorrect."
#endif

#if defined(CN_11) && (TaskInclude(CN_11) != cfgExclude) && (SlotSize(CN_11) != NullSlotSize_Task) && ((SlotSize(CN_11) < MinSlotSize_Task) || (SlotSize(CN_11) > MaxSlotSize_Task))
  #error "Parameter 'SlotSize on task CN_11' is misspelled or incorrect."
#endif

#if defined(CN_12) && (TaskInclude(CN_12) != cfgExclude) && (SlotSize(CN_12) != NullSlotSize_Task) && ((SlotSize(CN_12) < MinSlotSize_Task) || (SlotSize(CN_12) > MaxSlotSize_Task))
  #error "Parameter 'SlotSize on task CN_12' is misspelled or incorrect."
#endif

#if defined(CN_13) && (TaskInclude(CN_13) != cfgExclude) && (SlotSize(CN_13) != NullSlotSize_Task) && ((SlotSize(CN_13) < MinSlotSize_Task) || (SlotSize(CN_13) > MaxSlotSize_Task))
  #error "Parameter 'SlotSize on task CN_13' is misspelled or incorrect."
#endif

#if defined(CN_14) && (TaskInclude(CN_14) != cfgExclude) && (SlotSize(CN_14) != NullSlotSize_Task) && ((SlotSize(CN_14) < MinSlotSize_Task) || (SlotSize(CN_14) > MaxSlotSize_Task))
  #error "Parameter 'SlotSize on task CN_14' is misspelled or incorrect."
#endif

#if defined(CN_15) && (TaskInclude(CN_15) != cfgExclude) && (SlotSize(CN_15) != NullSlotSize_Task) && ((SlotSize(CN_15) < MinSlotSize_Task) || (SlotSize(CN_15) > MaxSlotSize_Task))
  #error "Parameter 'SlotSize on task CN_15' is misspelled or incorrect."
#endif

#else

#if ((SlotSizeOverride) != NullSlotSize_Task) && (((SlotSizeOverride) < MinSlotSize_Task) || ((SlotSizeOverride) > MaxSlotSize_Task))
  #error "Parameter 'SlotSizeOverride' is misspelled or incorrect."
#endif

#endif

#endif


#if (cfgUseSynchronization != cfgSyncNon)

#if !defined(SlotUseOverride) || ((SlotUseOverride) == cfgOverrideNon)

#if defined(SN_01) && (SlotUse(SN_01) == cfgTrue) && (SlotUse(SN_01) != cfgUseAsWait) && (SlotUse(SN_01) != cfgUseAsMutex) && (SlotUse(SN_01) != cfgUseAsQueu) && (SlotUse(SN_01) != cfgUseAsNon)
  #error "Parameter 'SlotUse on slot SN_01' is misspelled or incorrect."
#endif

#if defined(SN_02) && (SlotUse(SN_02) == cfgTrue) && (SlotUse(SN_02) != cfgUseAsWait) && (SlotUse(SN_02) != cfgUseAsMutex) && (SlotUse(SN_02) != cfgUseAsQueu) && (SlotUse(SN_02) != cfgUseAsNon)
  #error "Parameter 'SlotUse on slot SN_02' is misspelled or incorrect."
#endif

#if defined(SN_03) && (SlotUse(SN_03) == cfgTrue) && (SlotUse(SN_03) != cfgUseAsWait) && (SlotUse(SN_03) != cfgUseAsMutex) && (SlotUse(SN_03) != cfgUseAsQueu) && (SlotUse(SN_03) != cfgUseAsNon)
  #error "Parameter 'SlotUse on slot SN_03' is misspelled or incorrect."
#endif

#if defined(SN_04) && (SlotUse(SN_04) == cfgTrue) && (SlotUse(SN_04) != cfgUseAsWait) && (SlotUse(SN_04) != cfgUseAsMutex) && (SlotUse(SN_04) != cfgUseAsQueu) && (SlotUse(SN_04) != cfgUseAsNon)
  #error "Parameter 'SlotUse on slot SN_04' is misspelled or incorrect."
#endif

#if defined(SN_05) && (SlotUse(SN_05) == cfgTrue) && (SlotUse(SN_05) != cfgUseAsWait) && (SlotUse(SN_05) != cfgUseAsMutex) && (SlotUse(SN_05) != cfgUseAsQueu) && (SlotUse(SN_05) != cfgUseAsNon)
  #error "Parameter 'SlotUse on slot SN_05' is misspelled or incorrect."
#endif

#if defined(SN_06) && (SlotUse(SN_06) == cfgTrue) && (SlotUse(SN_06) != cfgUseAsWait) && (SlotUse(SN_06) != cfgUseAsMutex) && (SlotUse(SN_06) != cfgUseAsQueu) && (SlotUse(SN_06) != cfgUseAsNon)
  #error "Parameter 'SlotUse on slot SN_06' is misspelled or incorrect."
#endif

#if defined(SN_07) && (SlotUse(SN_07) == cfgTrue) && (SlotUse(SN_07) != cfgUseAsWait) && (SlotUse(SN_07) != cfgUseAsMutex) && (SlotUse(SN_07) != cfgUseAsQueu) && (SlotUse(SN_07) != cfgUseAsNon)
  #error "Parameter 'SlotUse on slot SN_07' is misspelled or incorrect."
#endif

#if defined(SN_08) && (SlotUse(SN_08) == cfgTrue) && (SlotUse(SN_08) != cfgUseAsWait) && (SlotUse(SN_08) != cfgUseAsMutex) && (SlotUse(SN_08) != cfgUseAsQueu) && (SlotUse(SN_08) != cfgUseAsNon)
  #error "Parameter 'SlotUse on slot SN_08' is misspelled or incorrect."
#endif

#if defined(SN_09) && (SlotUse(SN_09) == cfgTrue) && (SlotUse(SN_09) != cfgUseAsWait) && (SlotUse(SN_09) != cfgUseAsMutex) && (SlotUse(SN_09) != cfgUseAsQueu) && (SlotUse(SN_09) != cfgUseAsNon)
  #error "Parameter 'SlotUse on slot SN_09' is misspelled or incorrect."
#endif

#if defined(SN_10) && (SlotUse(SN_10) == cfgTrue) && (SlotUse(SN_10) != cfgUseAsWait) && (SlotUse(SN_10) != cfgUseAsMutex) && (SlotUse(SN_10) != cfgUseAsQueu) && (SlotUse(SN_10) != cfgUseAsNon)
  #error "Parameter 'SlotUse on slot SN_10' is misspelled or incorrect."
#endif

#if defined(SN_11) && (SlotUse(SN_11) == cfgTrue) && (SlotUse(SN_11) != cfgUseAsWait) && (SlotUse(SN_11) != cfgUseAsMutex) && (SlotUse(SN_11) != cfgUseAsQueu) && (SlotUse(SN_11) != cfgUseAsNon)
  #error "Parameter 'SlotUse on slot SN_11' is misspelled or incorrect."
#endif

#if defined(SN_12) && (SlotUse(SN_12) == cfgTrue) && (SlotUse(SN_12) != cfgUseAsWait) && (SlotUse(SN_12) != cfgUseAsMutex) && (SlotUse(SN_12) != cfgUseAsQueu) && (SlotUse(SN_12) != cfgUseAsNon)
  #error "Parameter 'SlotUse on slot SN_12' is misspelled or incorrect."
#endif

#if defined(SN_13) && (SlotUse(SN_13) == cfgTrue) && (SlotUse(SN_13) != cfgUseAsWait) && (SlotUse(SN_13) != cfgUseAsMutex) && (SlotUse(SN_13) != cfgUseAsQueu) && (SlotUse(SN_13) != cfgUseAsNon)
  #error "Parameter 'SlotUse on slot SN_13' is misspelled or incorrect."
#endif

#if defined(SN_14) && (SlotUse(SN_14) == cfgTrue) && (SlotUse(SN_14) != cfgUseAsWait) && (SlotUse(SN_14) != cfgUseAsMutex) && (SlotUse(SN_14) != cfgUseAsQueu) && (SlotUse(SN_14) != cfgUseAsNon)
  #error "Parameter 'SlotUse on slot SN_14' is misspelled or incorrect."
#endif

#if defined(SN_15) && (SlotUse(SN_15) == cfgTrue) && (SlotUse(SN_15) != cfgUseAsWait) && (SlotUse(SN_15) != cfgUseAsMutex) && (SlotUse(SN_15) != cfgUseAsQueu) && (SlotUse(SN_15) != cfgUseAsNon)
  #error "Parameter 'SlotUse on slot SN_15' is misspelled or incorrect."
#endif

#else

#if ((SlotUseOverride) == cfgTrue) && (SlotUseOverride != cfgUseAsWait) && (SlotUseOverride != cfgUseAsMutex) && (SlotUseOverride != cfgUseAsQueu) && (SlotUseOverride != cfgUseAsNon)
  #error "Parameter 'SlotUseOverride' is misspelled or incorrect."
#endif

#endif

#endif


#if (cfgUseSynchronization != cfgSyncNon)

#if !defined(QueuSizeOverride) || ((QueuSizeOverride) == cfgOverrideNon)

#if defined(SN_01) && (SlotUse(SN_01) == cfgUseAsQueu) && ((QueuSize(SN_01) < MinQueuSize) || (QueuSize(SN_01) > MaxQueuSize))
  #error "Parameter 'QueuSize on slot SN_01' is misspelled or incorrect."
#endif

#if defined(SN_02) && (SlotUse(SN_02) == cfgUseAsQueu) && ((QueuSize(SN_02) < MinQueuSize) || (QueuSize(SN_02) > MaxQueuSize))
  #error "Parameter 'QueuSize on slot SN_02' is misspelled or incorrect."
#endif

#if defined(SN_03) && (SlotUse(SN_03) == cfgUseAsQueu) && ((QueuSize(SN_03) < MinQueuSize) || (QueuSize(SN_03) > MaxQueuSize))
  #error "Parameter 'QueuSize on slot SN_03' is misspelled or incorrect."
#endif

#if defined(SN_04) && (SlotUse(SN_04) == cfgUseAsQueu) && ((QueuSize(SN_04) < MinQueuSize) || (QueuSize(SN_04) > MaxQueuSize))
  #error "Parameter 'QueuSize on slot SN_04' is misspelled or incorrect."
#endif

#if defined(SN_05) && (SlotUse(SN_05) == cfgUseAsQueu) && ((QueuSize(SN_05) < MinQueuSize) || (QueuSize(SN_05) > MaxQueuSize))
  #error "Parameter 'QueuSize on slot SN_05' is misspelled or incorrect."
#endif

#if defined(SN_06) && (SlotUse(SN_06) == cfgUseAsQueu) && ((QueuSize(SN_06) < MinQueuSize) || (QueuSize(SN_06) > MaxQueuSize))
  #error "Parameter 'QueuSize on slot SN_06' is misspelled or incorrect."
#endif

#if defined(SN_07) && (SlotUse(SN_07) == cfgUseAsQueu) && ((QueuSize(SN_07) < MinQueuSize) || (QueuSize(SN_07) > MaxQueuSize))
  #error "Parameter 'QueuSize on slot SN_07' is misspelled or incorrect."
#endif

#if defined(SN_08) && (SlotUse(SN_08) == cfgUseAsQueu) && ((QueuSize(SN_08) < MinQueuSize) || (QueuSize(SN_08) > MaxQueuSize))
  #error "Parameter 'QueuSize on slot SN_08' is misspelled or incorrect."
#endif

#if defined(SN_09) && (SlotUse(SN_09) == cfgUseAsQueu) && ((QueuSize(SN_09) < MinQueuSize) || (QueuSize(SN_09) > MaxQueuSize))
  #error "Parameter 'QueuSize on slot SN_09' is misspelled or incorrect."
#endif

#if defined(SN_10) && (SlotUse(SN_10) == cfgUseAsQueu) && ((QueuSize(SN_10) < MinQueuSize) || (QueuSize(SN_10) > MaxQueuSize))
  #error "Parameter 'QueuSize on slot SN_10' is misspelled or incorrect."
#endif

#if defined(SN_11) && (SlotUse(SN_11) == cfgUseAsQueu) && ((QueuSize(SN_11) < MinQueuSize) || (QueuSize(SN_11) > MaxQueuSize))
  #error "Parameter 'QueuSize on slot SN_11' is misspelled or incorrect."
#endif

#if defined(SN_12) && (SlotUse(SN_12) == cfgUseAsQueu) && ((QueuSize(SN_12) < MinQueuSize) || (QueuSize(SN_12) > MaxQueuSize))
  #error "Parameter 'QueuSize on slot SN_12' is misspelled or incorrect."
#endif

#if defined(SN_13) && (SlotUse(SN_13) == cfgUseAsQueu) && ((QueuSize(SN_13) < MinQueuSize) || (QueuSize(SN_13) > MaxQueuSize))
  #error "Parameter 'QueuSize on slot SN_13' is misspelled or incorrect."
#endif

#if defined(SN_14) && (SlotUse(SN_14) == cfgUseAsQueu) && ((QueuSize(SN_14) < MinQueuSize) || (QueuSize(SN_14) > MaxQueuSize))
  #error "Parameter 'QueuSize on slot SN_14' is misspelled or incorrect."
#endif

#if defined(SN_15) && (SlotUse(SN_15) == cfgUseAsQueu) && ((QueuSize(SN_15) < MinQueuSize) || (QueuSize(SN_15) > MaxQueuSize))
  #error "Parameter 'QueuSize on slot SN_15' is misspelled or incorrect."
#endif

#else

#if ((QueuSizeOverride) == cfgUseAsQueu) && (((QueuSizeOverride) < MinQueuSize) || ((QueuSizeOverride) > MaxQueuSize))
  #error "Parameter 'QueuSizeOverride' is misspelled or incorrect."
#endif

#endif

#endif


#if (cfgUseFileSystem == cfgTrue)

#if !defined(FileSpaceOverride) || ((FileSpaceOverride) == cfgOverrideNon)

#if defined(FN_00) && ((FileSpace(FN_00) < MinFileSpace_Task) || (FileSpace(FN_00) > MaxFileSpace_Task))
  #error "Parameter 'FileSpace on file FN_00' is misspelled or incorrect."
#endif

#if defined(FN_01) && ((FileSpace(FN_01) < MinFileSpace_Task) || (FileSpace(FN_01) > MaxFileSpace_Task))
  #error "Parameter 'FileSpace on file FN_01' is misspelled or incorrect."
#endif

#if defined(FN_02) && ((FileSpace(FN_02) < MinFileSpace_Task) || (FileSpace(FN_02) > MaxFileSpace_Task))
  #error "Parameter 'FileSpace on file FN_02' is misspelled or incorrect."
#endif

#if defined(FN_03) && ((FileSpace(FN_03) < MinFileSpace_Task) || (FileSpace(FN_03) > MaxFileSpace_Task))
  #error "Parameter 'FileSpace on file FN_03' is misspelled or incorrect."
#endif

#if defined(FN_04) && ((FileSpace(FN_04) < MinFileSpace_Task) || (FileSpace(FN_04) > MaxFileSpace_Task))
  #error "Parameter 'FileSpace on file FN_04' is misspelled or incorrect."
#endif

#if defined(FN_05) && ((FileSpace(FN_05) < MinFileSpace_Task) || (FileSpace(FN_05) > MaxFileSpace_Task))
  #error "Parameter 'FileSpace on file FN_05' is misspelled or incorrect."
#endif

#if defined(FN_06) && ((FileSpace(FN_06) < MinFileSpace_Task) || (FileSpace(FN_06) > MaxFileSpace_Task))
  #error "Parameter 'FileSpace on file FN_06' is misspelled or incorrect."
#endif

#if defined(FN_07) && ((FileSpace(FN_07) < MinFileSpace_Task) || (FileSpace(FN_07) > MaxFileSpace_Task))
  #error "Parameter 'FileSpace on file FN_07' is misspelled or incorrect."
#endif

#if defined(FN_08) && ((FileSpace(FN_08) < MinFileSpace_Task) || (FileSpace(FN_08) > MaxFileSpace_Task))
  #error "Parameter 'FileSpace on file FN_08' is misspelled or incorrect."
#endif

#if defined(FN_09) && ((FileSpace(FN_09) < MinFileSpace_Task) || (FileSpace(FN_09) > MaxFileSpace_Task))
  #error "Parameter 'FileSpace on file FN_09' is misspelled or incorrect."
#endif

#if defined(FN_10) && ((FileSpace(FN_10) < MinFileSpace_Task) || (FileSpace(FN_10) > MaxFileSpace_Task))
  #error "Parameter 'FileSpace on file FN_10' is misspelled or incorrect."
#endif

#if defined(FN_11) && ((FileSpace(FN_11) < MinFileSpace_Task) || (FileSpace(FN_11) > MaxFileSpace_Task))
  #error "Parameter 'FileSpace on file FN_11' is misspelled or incorrect."
#endif

#if defined(FN_12) && ((FileSpace(FN_12) < MinFileSpace_Task) || (FileSpace(FN_12) > MaxFileSpace_Task))
  #error "Parameter 'FileSpace on file FN_12' is misspelled or incorrect."
#endif

#if defined(FN_13) && ((FileSpace(FN_13) < MinFileSpace_Task) || (FileSpace(FN_13) > MaxFileSpace_Task))
  #error "Parameter 'FileSpace on file FN_13' is misspelled or incorrect."
#endif

#if defined(FN_14) && ((FileSpace(FN_14) < MinFileSpace_Task) || (FileSpace(FN_14) > MaxFileSpace_Task))
  #error "Parameter 'FileSpace on file FN_14' is misspelled or incorrect."
#endif

#if defined(FN_15) && ((FileSpace(FN_15) < MinFileSpace_Task) || (FileSpace(FN_15) > MaxFileSpace_Task))
  #error "Parameter 'FileSpace on file FN_15' is misspelled or incorrect."
#endif

#else

#if ((FileSpaceOverride) < MinFileSpace_Task) || ((FileSpaceOverride) > MaxFileSpace_Task))
  #error "Parameter 'FileSpaceOverride' is misspelled or incorrect."
#endif

#endif

#endif


#if (cfgUseFileSystem == cfgTrue)

#if (cfgUseFileSystemMaintainFAT == cfgTrue)
  #define FileSpace_FN_00 -1
  #define FileSpace_FN_01 -1
  #define FileSpace_FN_02 -1
  #define FileSpace_FN_03 -1
  #define FileSpace_FN_04 -1
  #define FileSpace_FN_05 -1
  #define FileSpace_FN_06 -1
  #define FileSpace_FN_07 -1
  #define FileSpace_FN_08 -1
  #define FileSpace_FN_09 -1
  #define FileSpace_FN_10 -1
  #define FileSpace_FN_11 -1
  #define FileSpace_FN_12 -1
  #define FileSpace_FN_13 -1
  #define FileSpace_FN_14 -1
  #define FileSpace_FN_15 -1
  #if (  ((FileSpace(FN_00))+1) + ((FileSpace(FN_01))+1) + ((FileSpace(FN_02))+1) + ((FileSpace(FN_03))+1) + ((FileSpace(FN_04))+1) + ((FileSpace(FN_05))+1) + ((FileSpace(FN_06))+1) + ((FileSpace(FN_07))+1) + ((FileSpace(FN_08))+1) + ((FileSpace(FN_09))+1) + ((FileSpace(FN_10))+1) + ((FileSpace(FN_11))+1) + ((FileSpace(FN_12))+1) + ((FileSpace(FN_13))+1) + ((FileSpace(FN_14))+1) + ((FileSpace(FN_15))+1) ) > (cfgSysFsStorageSpace)
    #error Total space of all predefined files exceeds cfgSysFsStorageSpace.
  #endif
#else
  #define FileSpace_FN_00 0
  #define FileSpace_FN_01 0
  #define FileSpace_FN_02 0
  #define FileSpace_FN_03 0
  #define FileSpace_FN_04 0
  #define FileSpace_FN_05 0
  #define FileSpace_FN_06 0
  #define FileSpace_FN_07 0
  #define FileSpace_FN_08 0
  #define FileSpace_FN_09 0
  #define FileSpace_FN_10 0
  #define FileSpace_FN_11 0
  #define FileSpace_FN_12 0
  #define FileSpace_FN_13 0
  #define FileSpace_FN_14 0
  #define FileSpace_FN_15 0
  #if ( (FileSpace(FN_00)) + (FileSpace(FN_01)) + (FileSpace(FN_02)) + (FileSpace(FN_03)) + (FileSpace(FN_04)) + (FileSpace(FN_05)) + (FileSpace(FN_06)) + (FileSpace(FN_07)) + (FileSpace(FN_08)) + (FileSpace(FN_09)) + (FileSpace(FN_10)) + (FileSpace(FN_11)) + (FileSpace(FN_12)) + (FileSpace(FN_13)) + (FileSpace(FN_14)) + (FileSpace(FN_15)) ) > (cfgSysFsStorageSpace)
    #error Total space of all predefined files exceeds cfgSysFsStorageSpace.
  #endif
#endif

#endif

#if ( (defSysGCCstartup == cfgReplace) && ((cfgIntUserDefined == cfgTrue) || (cfgCheckTrace == cfgTrue) || (cfgCheckTiming == cfgTrue)) )
  #error "(cfgSysGCCstartup == cfgReplace) not possible for this configuration setting."
#endif

#if ( defined(cfgNumGlobWidth) && defined(cfgNumSwitchWidth) && defined(cfgNumTickWidth) && ((cfgNumGlobWidth) + (cfgNumSwitchWidth) + (cfgNumTickWidth) > MaxLevelsWidth) )
  #error "Sum of widths for critical nesting exceeds 8 bits."
#endif


#if (cfgSysReuseOsStack == cfgTrue) && ((StackSizeOS) < defTaskSaveSize)
  #error "Increase the size of the OS stack to match defTaskSaveSize (7, 8, 13, or 14 bytes) depending on other settings."
#endif


/* Warnings */

#if (includeTaskDelayFromNow == cfgTrue) && (cfgUseDelay == cfgFalse)
  #warning "Use of 'includeTaskDelayFromNow' requires activation of cfgUseDelay."
#endif

#if (includeTaskDelayFromWake == cfgTrue) && (cfgUseDelay == cfgFalse)
  #warning "Use of 'includeTaskDelayFromWake' requires activation of cfgUseDelay."
#endif

#if (includeTaskRestart == cfgTrue) && (cfgUseDelay == cfgFalse)
  #warning "Use of 'includeTaskRestart' requires activation of cfgUseDelay."
#endif

#if (includeTaskSleep == cfgTrue) && (cfgUseLowPowerSleep == cfgFalse)
  #warning "Use of 'includeTaskSleep' requires activation of cfgUseLowPowerSleep."
#endif

#if (includeTaskSleepAll == cfgTrue) && (cfgUseLowPowerSleep == cfgFalse)
  #warning "Use of 'includeTaskSleepAll' requires activation of cfgUseLowPowerSleep."
#endif

#if (includeGenGetLastWakeTime == cfgTrue) && (cfgUseDelay == cfgFalse)
  #warning "Use of 'includeGenGetLastWakeTime' requires activation of cfgUseDelay."
#endif

#if (includeTaskFeedWatchdog == cfgTrue) && (cfgUseTaskWatchdog == cfgFalse)
  #warning "Use of 'includeTaskFeedWatchdog' requires activation of cfgUseTaskWatchdog."
#endif

#if (includeTaskKillWatchdog == cfgTrue) && (cfgUseTaskWatchdog == cfgFalse)
  #warning "Use of 'includeTaskKillWatchdog' requires activation of cfgUseTaskWatchdog."
#endif

#if (includeGenGetTaskname == cfgTrue) && (cfgUseTasknames == cfgFalse)
  #warning "Use of 'includeGenGetTaskname' requires activation of cfgUseTasknames."
#endif

#if (includeTaskWaitForTasks == cfgTrue) && (cfgUseSynchronization == cfgSyncNon)
  #warning "Use of 'includeTaskWaitForTasks' requires activation of cfgUseSynchronization."
#endif

#if (includeGenWaitRelease == cfgTrue) && (cfgUseSynchronization == cfgSyncNon)
  #warning "Use of 'includeGenWaitRelease' requires activation of cfgUseSynchronization."
#endif

#if (includeTaskMutex == cfgTrue) && (cfgUseSynchronization == cfgSyncNon)
  #warning "Use of 'includeTaskMutex' requires activation of cfgUseSynchronization."
#endif

#if (includeTaskQueu == cfgTrue) && (cfgUseSynchronization == cfgSyncNon)
  #warning "Use of 'includeTaskQueu' requires activation of cfgUseSynchronization."
#endif

#if (includeGenQueuWrite == cfgTrue) && (cfgUseSynchronization == cfgSyncNon)
  #warning "Use of 'includeGenQueuWrite' requires activation of cfgUseSynchronization."
#endif

#if (includeGenQueuRead == cfgTrue) && (cfgUseSynchronization == cfgSyncNon)
  #warning "Use of 'includeGenQueuRead' requires activation of cfgUseSynchronization."
#endif

#if (includeGenQueuUnwrite == cfgTrue) && (cfgUseSynchronization == cfgSyncNon)
  #warning "Use of 'includeGenQueuUnwrite' requires activation of cfgUseSynchronization."
#endif

#if (includeGenQueuUnread == cfgTrue) && (cfgUseSynchronization == cfgSyncNon)
  #warning "Use of 'includeGenQueuUnread' requires activation of cfgUseSynchronization."
#endif

#if (includeGenQueuClear == cfgTrue) && (cfgUseSynchronization == cfgSyncNon)
  #warning "Use of 'includeGenQueuClear' requires activation of cfgUseSynchronization."
#endif

#if (includeGenQueuPeek == cfgTrue) && (cfgUseSynchronization == cfgSyncNon)
  #warning "Use of 'includeGenQueuPeek' requires activation of cfgUseSynchronization."
#endif

#if (includeGenQueuReadable == cfgTrue) && (cfgUseSynchronization == cfgSyncNon)
  #warning "Use of 'includeGenQueuReadable' requires activation of cfgUseSynchronization."
#endif

#if (includeGenQueuWriteable == cfgTrue) && (cfgUseSynchronization == cfgSyncNon)
  #warning "Use of 'includeGenQueuWriteable' requires activation of cfgUseSynchronization."
#endif

#if (includeGenQueuFull == cfgTrue) && (cfgUseSynchronization == cfgSyncNon)
  #warning "Use of 'includeGenQueuFull' requires activation of cfgUseSynchronization."
#endif

#if (includeGenQueuEmpty == cfgTrue) && (cfgUseSynchronization == cfgSyncNon)
  #warning "Use of 'includeGenQueuEmpty' requires activation of cfgUseSynchronization."
#endif

#if (includeTaskFileAccess == cfgTrue) && (cfgUseFileSystem == cfgFalse)
  #warning "Use of 'includeTaskFileAccess' requires activation of cfgUseFileSystem."
#endif

#if (includeTaskFileFormat == cfgTrue) && (cfgUseFileSystem == cfgFalse)
  #warning "Use of 'includeTaskFileFormat' requires activation of cfgUseFileSystem."
#endif

#if (includeTaskFileGetSize == cfgTrue) && (cfgUseFileSystem == cfgFalse)
  #warning "Use of 'includeTaskFileGetSize' requires activation of cfgUseFileSystem."
#endif

#if (includeTaskFileSetSize == cfgTrue) && (cfgUseFileSystem == cfgFalse)
  #warning "Use of 'includeTaskFileSize' requires activation of cfgUseFileSystem."
#endif

#if (includeTaskFileReadByte == cfgTrue) && (cfgUseFileSystem == cfgFalse)
  #warning "Use of 'includeTaskFileReadByte' requires activation of cfgUseFileSystem."
#endif

#if (includeTaskFileWriteByte == cfgTrue) && (cfgUseFileSystem == cfgFalse)
  #warning "Use of 'includeTaskFileWriteByte' requires activation of cfgUseFileSystem."
#endif

#if (includeTaskFileAppendByte == cfgTrue) && (cfgUseFileSystem == cfgFalse)
  #warning "Use of 'includeTaskFileAppendByte' requires activation of cfgUseFileSystem."
#endif

#if (includeTaskFileReadPipe == cfgTrue) && (cfgUseFileSystem == cfgFalse)
  #warning "Use of 'includeTaskFileReadPipe' requires activation of cfgUseFileSystem."
#endif

#if (includeTaskFileWritePipe == cfgTrue) && (cfgUseFileSystem == cfgFalse)
  #warning "Use of 'includeTaskFileWritePipe' requires activation of cfgUseFileSystem."
#endif

#if (includeTaskFileReadBuffer == cfgTrue) && (cfgUseFileSystem == cfgFalse)
  #warning "Use of 'includeTaskFileReadBuffer' requires activation of cfgUseFileSystem."
#endif

#if (includeTaskFileWriteBuffer == cfgTrue) && (cfgUseFileSystem == cfgFalse)
  #warning "Use of 'includeTaskFileWriteBuffer' requires activation of cfgUseFileSystem."
#endif

#if (includeIsrEnter == cfgTrue) && (cfgIntUserDefined == cfgFalse)
  #warning "Use of 'includeIsrEnter' requires activation of cfgIntUserDefined."
#endif

#if (includeIsrExit == cfgTrue) && (cfgIntUserDefined == cfgFalse)
  #warning "Use of 'includeIsrExit' requires activation of cfgIntUserDefined."
#endif

#if (includeIsrBegin == cfgTrue) && (cfgIntUserDefined == cfgFalse)
  #warning "Use of 'includeIsrBegin' requires activation of cfgIntUserDefined."
#endif

#if (includeIsrEndReturn == cfgTrue) && (cfgIntUserDefined == cfgFalse)
  #warning "Use of 'includeIsrEndReturn' requires activation of cfgIntUserDefined."
#endif

#if (includeIsrEndYield == cfgTrue) && (cfgIntUserDefined == cfgFalse)
  #warning "Use of 'includeIsrEndYield' requires activation of cfgIntUserDefined."
#endif

#if (includeIsrEnter == cfgTrue) && (cfgUseLowPowerSleep == cfgTrue)
  #warning "Use of 'isrEnter()' is not allowed if device may fall into low power sleep."
#endif

#if (includeIsrExit == cfgTrue) && (cfgUseLowPowerSleep == cfgTrue)
  #warning "Use of 'isrExit()' is not allowed if device may fall into low power sleep."
#endif

#if (includeIsrEndYield == cfgTrue) && (cfgUseLowPowerSleep == cfgTrue)
  #warning "Use of 'isrEndYield()' is not allowed if device may fall into low power sleep."
#endif

#if (includeIsrEnter == cfgTrue) && (cfgIntSwitchUsesOSstack == cfgFalse) && ((StackSizeISR) == 0)
  #warning "Use of 'includeIsrEnter()'  requires an non empty ISR stack when it does not utilize the OS stack."
#endif

#if (includeIsrExit == cfgTrue) && (cfgIntSwitchUsesOSstack == cfgFalse) && ((StackSizeISR) == 0)
  #warning "Use of 'includeIsrExit()'  requires an non empty ISR stack when it does not utilize the OS stack."
#endif

#if (includeIsrEndYield == cfgTrue) && (cfgIntSwitchUsesOSstack == cfgFalse) && ((StackSizeISR) == 0)
  #warning "Use of 'includeIsrEndYield()'  requires an non empty ISR stack when it does not utilize the OS stack."
#endif

#if (includeIsrBegin == cfgTrue) && ((StackSizeISR) == 0)
  #warning "Use of 'isrBegin()'  requires an non empty ISR stack."
#endif

#if (includeIsrEndReturn == cfgTrue) && ((StackSizeISR) == 0)
  #warning "Use of 'isrEndReturn()' requires an non empty ISR stack."
#endif

#if (includeIsrEndYield == cfgTrue) && ((StackSizeISR) == 0)
  #warning "Use of 'isrEndYield()' requires an non empty ISR stack."
#endif

#if (includeIsrEnter == cfgTrue) && (cfgIntTickTrack == cfgTrue)
  #warning "Use of 'isrEnter()' is not allowed if tick interrupt tracking is activated."
#endif

#if (includeIsrExit == cfgTrue) && (cfgIntTickTrack == cfgTrue)
  #warning "Use of 'isrExit()' is not allowed if tick interrupt tracking is activated."
#endif

#if (includeIsrEndYield == cfgTrue) && (cfgIntTickTrack == cfgTrue)
  #warning "Use of 'isrEndYield()' is not allowed if tick interrupt tracking is activated."
#endif

#if (includeIsrEndYield == cfgTrue) && (cfgIntOsProtected == cfgFalse)
  #warning "Use of 'includeIsrEndYield' requires activation of cfgIntOsProtected."
#endif

#if (includeIsrEndYield == cfgTrue) && (includeTaskYield == cfgFalse)
  #warning "Use of 'includeIsrEndYield' requires activation of includeTaskYield."
#endif

#if (includeIsrStartLoad == cfgTrue) && (cfgIntUserDefined == cfgFalse)
  #warning "Use of 'includeIsrStartLoad' requires activation of cfgIntUserDefined."
#endif

#if (includeIsrStopLoad == cfgTrue) && (cfgIntUserDefined == cfgFalse)
  #warning "Use of 'includeIsrStopLoad' requires activation of cfgIntUserDefined."
#endif

#if (callAppTickSleep == cfgTrue) && (cfgUseLowPowerSleep == cfgFalse)
  #warning "Parameter 'callAppTickSleep' requires activation of cfgUseLowPowerSleep."
#endif

#if (callAppEnterSleep == cfgTrue) && (cfgUseLowPowerSleep == cfgFalse)
  #warning "Parameter 'callAppEnterSleep' requires activation of cfgUseLowPowerSleep."
#endif

#if (callAppExitSleep == cfgTrue) && (cfgUseLowPowerSleep == cfgFalse)
  #warning "Parameter 'callAppExitSleep' requires activation of cfgUseLowPowerSleep."
#endif

#if (callAppBark == cfgTrue) && (cfgUseTaskWatchdog == cfgFalse)
  #warning "Use of 'callAppBark' requires activation of cfgUseTaskWatchdog."
#endif

#if (cfgUseDelay == cfgTrue) && (includeTaskDelayFromNow == cfgFalse) && (includeTaskDelayFromWake == cfgFalse) && (includeTaskRestart == cfgFalse) && ((cfgUseSynchronization != cfgSyncNon) && (cfgUseTimeout == cfgFalse))
  #warning "'cfgUseDelay' seems to include dead code, you may deactivate this."
#endif

#if (cfgUseLowPowerSleep == cfgTrue) && (includeTaskSleep == cfgFalse) && (includeTaskSleepAll == cfgFalse) && (cfgUseLowPowerOnDelay == cfgFalse)
  #warning "'cfgUseLowPowerSleep' seems to include dead code, you may deactivate this."
#endif

#if (cfgUseTaskWatchdog == cfgTrue) && (includeTaskFeedWatchdog == cfgFalse) && (includeTaskKillWatchdog == cfgFalse)
  #warning "'cfgUseTaskWatchdog' seems to include dead code, you may deactivate this."
#endif

#if (cfgUseSynchronization != cfgSyncNon) && (includeTaskWaitForTasks == cfgFalse) && (includeGenWaitRelease == cfgFalse) && (includeTaskMutex == cfgFalse) && (includeTaskQueu == cfgFalse)
  #warning "'cfgUseSynchronization' seems to include dead code, you may deactivate this."
#endif

#if (cfgUseFileSystem == cfgTrue) && (includeTaskFileFormat == cfgFalse) && (includeTaskFileAccess == cfgFalse) && (includeTaskFileSetSize == cfgFalse) && (includeTaskFileGetSize == cfgFalse) && (includeTaskFileReadByte == cfgFalse) && (includeTaskFileWriteByte == cfgFalse) && (includeTaskFileAppendByte == cfgFalse) && (includeTaskFileReadPipe == cfgFalse) && (includeTaskFileWritePipe == cfgFalse) && (includeTaskFileReadBuffer == cfgFalse) && (includeTaskFileWriteBuffer == cfgFalse)
  #warning "'cfgUseFileSystem' seems to include dead code, you may deactivate this."
#endif

#if (cfgUseEvents == cfgTrue) && ((includeTaskWaitForEvents == cfgFalse) || ((includeIsrFireEvent == cfgFalse) && (includeGenFireEvent == cfgFalse) && (includeGenFireEventSet == cfgFalse) ))
  #warning "'cfgUseEvents' seems to include dead code, you may deactivate this."
#endif

#if (cfgUseNestedCriticals ==cfgTrue) && (includeTaskProtectGlobalCritical == cfgTrue) && (cfgNumGlobWidth == 0)
  #warning "'cfgNumGlobWidth' is to low to keep any critical level."
#endif

#if (cfgUseNestedCriticals ==cfgTrue) && (includeTaskProtectSwitchCritical == cfgTrue) && (cfgNumSwitchWidth == 0)
  #warning "'cfgNumSwitchWidth' is to low to keep any critical level."
#endif

#if (cfgUseNestedCriticals ==cfgTrue) && (includeTaskProtectTickCritical == cfgTrue) && (cfgNumTickWidth == 0)
  #warning "'cfgNumTickWidth' is to low to keep any critical level."
#endif

#if (cfgIntGlobalOnly ==  cfgTrue) && (cfgIntTickTrack == cfgTrue)
  #warning "Use of 'cfgIntGlobalOnly' renders tick tracking useless and dangerous."
#endif

#if (cfgIntOsProtected == cfgFalse) &&              \
    ( (includeGenSuspend == cfgTrue) ||             \
      (includeGenResume == cfgTrue) ||              \
      (includeGenSetPriority == cfgTrue) ||         \
      (includeGenGetPriority == cfgTrue) ||         \
      (includeGenGetTickCount == cfgTrue) ||        \
      (includeGenAddtoTickCount == cfgTrue) ||      \
      (includeGenGetLastWakeTime == cfgTrue) ||     \
      (includeGenGetTaskname == cfgTrue) ||         \
      (includeGenReboot == cfgTrue) ||              \
      (includeGenLogTask == cfgTrue) ||             \
      (includeGenLogOs == cfgTrue) ||               \
      (includeGenTrace == cfgTrue) ||               \
      (includeGenWaitRelease == cfgTrue) ||         \
      (includeGenQueuWrite == cfgTrue) ||           \
      (includeGenQueuRead == cfgTrue) ||            \
      (includeGenQueuUnwrite == cfgTrue) ||         \
      (includeGenQueuUnread == cfgTrue) ||          \
      (includeGenQueuClear == cfgTrue) ||           \
      (includeGenQueuPeek == cfgTrue) ||            \
      (includeGenQueuReadable == cfgTrue) ||        \
      (includeGenQueuWriteable == cfgTrue) ||       \
      (includeGenQueuFull == cfgTrue) ||            \
      (includeGenQueuEmpty == cfgTrue) )
  #warning "Possibly unprotected calls to the OS, activate cfgIntOsProtected."
  /* You get this warning if an interrupt may occur at execution of the OS and
   * you make use of genXXX calls in your code at the same time. In general you
   * may not make use genXXX in this situation. However, if you need the OS to
   * be interruptible for fast response and you make sure you do not use genXXX
   * calls in your ISR, you can ignore the warning.  */
#endif

#if ( ( defined(CN_00) && (TaskInclude(CN_00) != cfgExclude) && (StackIsShared(CN_00)) ) || \
      ( defined(CN_01) && (TaskInclude(CN_01) != cfgExclude) && (StackIsShared(CN_01)) ) || \
      ( defined(CN_02) && (TaskInclude(CN_02) != cfgExclude) && (StackIsShared(CN_02)) ) || \
      ( defined(CN_03) && (TaskInclude(CN_03) != cfgExclude) && (StackIsShared(CN_03)) ) || \
      ( defined(CN_04) && (TaskInclude(CN_04) != cfgExclude) && (StackIsShared(CN_04)) ) || \
      ( defined(CN_05) && (TaskInclude(CN_05) != cfgExclude) && (StackIsShared(CN_05)) ) || \
      ( defined(CN_06) && (TaskInclude(CN_06) != cfgExclude) && (StackIsShared(CN_06)) ) || \
      ( defined(CN_07) && (TaskInclude(CN_07) != cfgExclude) && (StackIsShared(CN_07)) ) || \
      ( defined(CN_08) && (TaskInclude(CN_08) != cfgExclude) && (StackIsShared(CN_08)) ) || \
      ( defined(CN_09) && (TaskInclude(CN_09) != cfgExclude) && (StackIsShared(CN_09)) ) || \
      ( defined(CN_10) && (TaskInclude(CN_10) != cfgExclude) && (StackIsShared(CN_10)) ) || \
      ( defined(CN_11) && (TaskInclude(CN_11) != cfgExclude) && (StackIsShared(CN_11)) ) || \
      ( defined(CN_12) && (TaskInclude(CN_12) != cfgExclude) && (StackIsShared(CN_12)) ) || \
      ( defined(CN_13) && (TaskInclude(CN_13) != cfgExclude) && (StackIsShared(CN_13)) ) || \
      ( defined(CN_14) && (TaskInclude(CN_14) != cfgExclude) && (StackIsShared(CN_14)) ) || \
      ( defined(CN_15) && (TaskInclude(CN_15) != cfgExclude) && (StackIsShared(CN_15)) ) ) && \
      ( (includeTaskRestart == cfgFalse) )
  #warning "Tasks with shared stack must be released with taskRestart()."
#endif

#if ( !defined(CN_00) || (TaskInclude(CN_00) == cfgExclude) || (StackIsShared(CN_00)) ) && \
    ( !defined(CN_01) || (TaskInclude(CN_01) == cfgExclude) || (StackIsShared(CN_01)) ) && \
    ( !defined(CN_02) || (TaskInclude(CN_02) == cfgExclude) || (StackIsShared(CN_02)) ) && \
    ( !defined(CN_03) || (TaskInclude(CN_03) == cfgExclude) || (StackIsShared(CN_03)) ) && \
    ( !defined(CN_04) || (TaskInclude(CN_04) == cfgExclude) || (StackIsShared(CN_04)) ) && \
    ( !defined(CN_05) || (TaskInclude(CN_05) == cfgExclude) || (StackIsShared(CN_05)) ) && \
    ( !defined(CN_06) || (TaskInclude(CN_06) == cfgExclude) || (StackIsShared(CN_06)) ) && \
    ( !defined(CN_07) || (TaskInclude(CN_07) == cfgExclude) || (StackIsShared(CN_07)) ) && \
    ( !defined(CN_08) || (TaskInclude(CN_08) == cfgExclude) || (StackIsShared(CN_08)) ) && \
    ( !defined(CN_09) || (TaskInclude(CN_09) == cfgExclude) || (StackIsShared(CN_09)) ) && \
    ( !defined(CN_10) || (TaskInclude(CN_10) == cfgExclude) || (StackIsShared(CN_10)) ) && \
    ( !defined(CN_11) || (TaskInclude(CN_11) == cfgExclude) || (StackIsShared(CN_11)) ) && \
    ( !defined(CN_12) || (TaskInclude(CN_12) == cfgExclude) || (StackIsShared(CN_12)) ) && \
    ( !defined(CN_13) || (TaskInclude(CN_13) == cfgExclude) || (StackIsShared(CN_13)) ) && \
    ( !defined(CN_14) || (TaskInclude(CN_14) == cfgExclude) || (StackIsShared(CN_14)) ) && \
    ( !defined(CN_15) || (TaskInclude(CN_15) == cfgExclude) || (StackIsShared(CN_15)) ) && \
    ( (includeTaskProtectSwitchTasks == cfgTrue) || (includeTaskProtectSwitchCritical == cfgTrue) )
  #warning "Do not use the dominant state when all tasks have a shared stack."
#endif



/* Device specific checks */
#ifndef __ASSEMBLER__

#if (cfgSysSubTickDivider!=1) && (cfgSysSubTickDivider!=8) && (cfgSysSubTickDivider!=32) && (cfgSysSubTickDivider!=64) && (cfgSysSubTickDivider!=128) && (cfgSysSubTickDivider!=256) && (cfgSysSubTickDivider!=1024)
  #error "No cfgSysSubTickDivider value must one of 1,8,32,64,128,256,1024."
#endif

#if (DefPrescaler(cfgSysSubTickDivider) == cfgUndefined)
  #error "The current value of cfgSysSubTickDivider is not defined on this device"
#endif

#if (devEEPROMsize < cfgSysFsStorageSpace)
  #error "File system is too large for this device. "
#endif

#if (cfgUseLowPowerSleep == cfgTrue) && (cfgUseLowPowerOnDelay == cfgTrue) && !defined(devSigWatchdogTimeout)
  /* We come here if we go to sleep on a device which has no possibilities
   * to periodically wake the device to see how much time has passed. Thus
   * it is dangerous to have cfgUseLowPowerOnDelay activated. */
  #error "Low power sleep on delay is not permitted on this device."
#endif

#if (cfgUseFileSystem == cfgTrue) && (cfgUseFileSystemEconomyMode == cfgTrue)
  /* This isonly allowed if the device has featues for split-byte programming, if not,
   *  signal an error. */
  #if (devEEPM0 == cfgUndefined) || (devEEPM1 == cfgUndefined)
    #error "No support for split byte programming, set cfgUseFileSystemEconomyMode to cfgFalse."
  #endif
#endif

#if (devAuxSysRegBit<0) || ((devAuxSysRegBit>7) && (devAuxSysRegBit != cfgUndefined))
  #error "Value of devAuxSysRegBit is unrecognized."
#endif

#if (cfgSysSqueezeState == cfgFalse) && (devAuxSysRegBit == cfgUndefined)
  #error "Femto OS needs one bit of auxilary storage. Activate cfgSysSqueezeState or define devAuxSysReg and devAuxSysRegBit"
#endif

#if (cfgSysSqueezeState == cfgFalse) && (defAuxSysRegLowIO == cfgFalse)
  #error "devAuxSysReg must be defined in the lower I/O region < 0x20"
#endif



#endif


#endif /* FEMTOOS_CHECK_H */

