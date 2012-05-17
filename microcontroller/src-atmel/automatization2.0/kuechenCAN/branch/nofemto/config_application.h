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

#ifndef CONFIG_FEMTOOS_H_
#define CONFIG_FEMTOOS_H_

#define TWI_BITRATE 100000
#define TWI_MTHREAD 
#define TWI_SLAVE (15)

#define MC_MOSI    PB3
#define MC_CLK     PB5

#define MCP_CS        PB2
#define MCP_CMD_PORT  PORTB

#define SPI_REG_PIN_MCP_INT  PIND
#define SPI_PIN_MCP_INT      PD2

#define F_MCP F_CPU

#define CAN_THREAD 

#define MEGA8

#ifdef MEGA8
#	define SPI_PORT PORTB
#	define SPI_DDR DDRB
#	define SPI_PIN_SCK PB5
#	define SPI_PIN_MOSI PB3
#else
#	define SPI_PORT PORTB
#	define SPI_DDR DDRB
#	define SPI_PIN_SCK PB7
#	define SPI_PIN_MOSI PB5
#endif


#if CANADDR <=0x03
#error "you have to define a can-address which will not interfere with critical LABOR-Hardware"
#endif

/* Comments are in the config default file */

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
#define  cfgSysFsStorageSpace                    0


/* ========================================================================= */
/* INTERRUPT HANDLING ====================================================== */
/* ========================================================================= */

#define  cfgIntGlobalOnly                        cfgTrue
#define  cfgIntUserDefined                       cfgFalse
#define  cfgIntSwitchUsesOSstack                 cfgFalse
#define  cfgIntOsProtected                       cfgTrue
#define  cfgIntTickTrack                         cfgFalse
#define  cfgIntManualTicks                       cfgFalse
//#define  smallISR                                cfgFalse

/* ========================================================================= */
/* QUALITY ASSURANCE ======================================================= */
/* ========================================================================= */

#define  cfgCheckSystem                          cfgTrue
#define  cfgCheckReset                           cfgTrue
#define  cfgCheckTrace                           cfgFalse
#define  cfgCheckAlwaysFatal                     cfgFalse
#define  cfgCheckWatermarks                      cfgFalse
#define  cfgCheckOsStack                         cfgTrue
#define  cfgCheckTaskStack                       cfgTrue
#define  cfgCheckIsrStack                        cfgFalse
#define  cfgCheckRegisters                       cfgTrue
#define  cfgCheckTiming                          cfgTrue
#define  cfgCheckQueuFilling                     cfgFalse
#define  cfgCheckMethodUse                       cfgTrue
#define  cfgCheckApplication                     cfgFalse


/* ========================================================================= */
/* COMMON CAPABILITIES ===================================================== */
/* ========================================================================= */

#define  cfgUseEquidistantTicks                  cfgTrue
#define  cfgUseDelay                             cfgTrue
#define  cfgUseSynchronization                   cfgSyncNon
//#define  cfgUseSynchronization                   cfgSyncSingleSlot
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
#define  callAppBoot                             cfgTrue
#define  callAppInit                             cfgFalse
#define  callAppBark                             cfgFalse


/* ========================================================================= */
/* API INCLUSIONS ========================================================== */
/* ========================================================================= */

/* Standard functions ------------------------------------------------------ */
#define  includeTaskYield                        cfgFalse
#define  includeTaskDelayFromNow                 cfgTrue
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
#define  includeTaskProtectGlobalCritical        cfgTrue
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

/* Quality assurance functions --------------------------------------------- */
#define  includeTaskStackCheck                   cfgFalse
#define  includeIsrStackCheck                    cfgFalse


/* ========================================================================= */
/* TASK NAMES ============================================================== */
/* ========================================================================= */




#define  CN_00                                   mainthread
#define  CN_01                                   leds
#define  CN_02                                   alarm
#define  CN_03                                   alarmt



#ifdef CAN_THREAD
#define  CN_14                                   xcan
#define  TaskInclude_xcan                     cfgStartRunning
#define  Capabilities_xcan                    cfgCapAll
#define  RegisterUse_xcan                      registersAll

#endif
#ifdef TWI_MTHREAD
#define  CN_15                                   twim
#define  TaskInclude_twim                     cfgStartRunning
#define  Capabilities_twim                    cfgCapAll
#define  RegisterUse_twim                      registersAll
#endif




/* ========================================================================= */
/* INCLUDE TASKS =========================================================== */
/* ========================================================================= */

//#define  TaskIncludeOverride                     cfgStartRunning

#define  TaskInclude_mainthread                     cfgStartRunning
#define  TaskInclude_alarm                     cfgStartRunning
#define  TaskInclude_alarmt                     cfgStartRunning
#define  TaskInclude_leds                     cfgStartRunning


/* ========================================================================= */
/* TASK CAPABILITIES ======================================================= */
/* ========================================================================= */

#define  Capabilities_leds                    cfgCapAll
#define  Capabilities_alarm                    cfgCapAll
#define  Capabilities_mainthread                    cfgCapAll
#define  Capabilities_alarmt                    cfgCapAll

/* ========================================================================= */
/* STACK SIZES ============================================================= */
/* ========================================================================= */

#define  StackSafety                             4
#define  StackSizeOS                             24
#define  StackSizeISR                            24
#define  StackSizeShared                         0
#define  StackSizeOverride                       100


/* ========================================================================= */
/* TIMES SLICES ============================================================ */
/* ========================================================================= */

#define  TimeSliceIdleTime                       cfgSysSubTicksPerFullTick


/* ========================================================================= */
/* RUNTIME TASK NAMES ====================================================== */
/* ========================================================================= */


/* ========================================================================= */
/* TASK PRIORITIES ========================================================= */
/* ========================================================================= */

#define  PriorityOverride                        0


/* ========================================================================= */
/* REGISTER USE ============================================================ */
/* ========================================================================= */

//#define  RegisterUseOverride                      registersAll

#define  RegisterUse_xcan                      registersAll
#define  RegisterUse_leds                      registersAll
#define  RegisterUse_alarm                      registersAll
#define  RegisterUse_alarmt                      registersAll
#define  RegisterUse_mainthread                     registersAll


/* ========================================================================= */
/* REGISTER CHECK ========================================================== */
/* ========================================================================= */

#define  RegisterCheckOverride                   registersAll


/* ========================================================================= */
/* INTERRUPT SETTING ======================================================= */
/* ========================================================================= */

//#define  InterruptStartOverride                  cfgGlobSet | cfgTickSet
#define  InterruptStartOverride                  cfgGlobClear | cfgTickSet


/* ========================================================================= */
/* EVENT NAMES ============================================================= */
/* ========================================================================= */

//#define eventCANBUSrecv                          0
//#define eventCANBUSsend                          1

/* ========================================================================= */
/* SLOT NAMES ============================================================== */
/* ========================================================================= */
//#define  SN_01                                   Slot1
//#define  SN_02                                   Slot2


/* ========================================================================= */
/* SLOT SIZES ============================================================== */
/* ========================================================================= */
//#define  SlotSize_Slot1                       4
//#define  SlotSize_Slot2                       4


/* ========================================================================= */
/* SLOT USAGE ============================================================== */
/* ========================================================================= */
//#define  SlotUse_Slot1                           cfgUseAsMutex
//#define  SlotUse_Slot2                           cfgUseAsMutex


/* ========================================================================= */
/* QUEU SIZES ============================================================== */
/* ========================================================================= */


/* ========================================================================= */
/* FILE NAMES ============================================================== */
/* ========================================================================= */


/* ========================================================================= */
/* FILE SIZES ============================================================== */
/* ========================================================================= */

#define  FileSpaceStandard                       0


#endif /*CONFIG_TESTHELLOWORLD_H_*/
