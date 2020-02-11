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

#ifndef FEMTOOS_SHARED_H
#define FEMTOOS_SHARED_H

#include "femtoos_types.h"


/* This file contains the variables and declarations shared between the implementation
 * files of the different part of the Femto OS. It should not be included in application
 * code.
 */

extern Ttickcount  uxTickCount;
extern Tuint08     uiOsStatus;
extern TosData     xOS;
extern Tuint08     uiFsStatus;

extern Tuint08     uiOsTimeAverage;
extern Tuint16     uiOsLoadCollect;
extern Tuint16     uiOsLoadTotal;
extern Tuint16     uiIdleLoadCollect;
extern Tuint16     uiIdleLoadTotal;
extern Tuint08     uiIsrLoadTemp;
extern Tuint16     uiIsrLoadCollect;
extern Tuint16     uiIsrLoadTotal;
extern Tuint08     uiLastOsStatus;
extern Tuint08     uiOsStackMax;

#if ((StackSizeISR) > 0xFF)
  extern Tuint16   uiIsrStackMax;
#else
  extern Tuint08   uiIsrStackMax;
#endif

extern Tchar       StackISR[(StackSizeISR)];
extern Tchar       StackSHR[(StackSizeShared)];


extern void appTick00(void);
extern void appTick08(void);
extern void appTick16(void);
extern void appTickSleep(void);
extern void appEnterIdle(void);
extern void appEnterSleep(void);
extern void appExitSleep(void);
extern void appBoot(void);


extern const Tbyte * pxTCBlist[defNumberOfTasks];
extern const TtaskDefinitionBlock * pxTDBlist[defNumberOfTasks];
extern const fpInitTask pxInitlist[defNumberOfTasks];
extern const fpLoopTask pxLooplist[defNumberOfTasks];
extern const fpBarkTask pxBarklist[defNumberOfWatchdogTasks];
extern const Tbyte * pxQueulist[defNumberOfQueus];
extern const Tuint08 uiQueuSize[defNumberOfQueus];
extern const Tuint08 uiFileSpace[defFsNumberOfPreFiles];
extern const Tuint08 uiCapabilities[defNumberOfTasks];

extern Tuint08 uiQueuWrite[defNumberOfQueus];
extern Tuint08 uiQueuRead[defNumberOfQueus];


/* Below we define a couple of pseudo variables only meant to
 * provide the post processor with information about the
 * organization of the system. They cannot be called or used.*/
extern const Tbyte defInfoCouplingTask(TN_00)[0];
extern const Tbyte defInfoCouplingTask(TN_01)[1];
extern const Tbyte defInfoCouplingTask(TN_02)[2];
extern const Tbyte defInfoCouplingTask(TN_03)[3];
extern const Tbyte defInfoCouplingTask(TN_04)[4];
extern const Tbyte defInfoCouplingTask(TN_05)[5];
extern const Tbyte defInfoCouplingTask(TN_06)[6];
extern const Tbyte defInfoCouplingTask(TN_07)[7];
extern const Tbyte defInfoCouplingTask(TN_08)[8];
extern const Tbyte defInfoCouplingTask(TN_09)[9];
extern const Tbyte defInfoCouplingTask(TN_10)[10];
extern const Tbyte defInfoCouplingTask(TN_11)[11];
extern const Tbyte defInfoCouplingTask(TN_12)[12];
extern const Tbyte defInfoCouplingTask(TN_13)[13];
extern const Tbyte defInfoCouplingTask(TN_14)[14];
extern const Tbyte defInfoCouplingTask(TN_15)[15];

extern const Tbyte defInfoCouplingSlot(LN_01)[1];
extern const Tbyte defInfoCouplingSlot(LN_02)[2];
extern const Tbyte defInfoCouplingSlot(LN_03)[3];
extern const Tbyte defInfoCouplingSlot(LN_04)[4];
extern const Tbyte defInfoCouplingSlot(LN_05)[5];
extern const Tbyte defInfoCouplingSlot(LN_06)[6];
extern const Tbyte defInfoCouplingSlot(LN_07)[7];
extern const Tbyte defInfoCouplingSlot(LN_08)[8];
extern const Tbyte defInfoCouplingSlot(LN_09)[9];
extern const Tbyte defInfoCouplingSlot(LN_10)[10];
extern const Tbyte defInfoCouplingSlot(LN_11)[11];
extern const Tbyte defInfoCouplingSlot(LN_12)[12];
extern const Tbyte defInfoCouplingSlot(LN_13)[13];
extern const Tbyte defInfoCouplingSlot(LN_14)[14];
extern const Tbyte defInfoCouplingSlot(LN_15)[15];

extern const Tbyte defInfoCouplingFile(VN_00)[0];
extern const Tbyte defInfoCouplingFile(VN_01)[1];
extern const Tbyte defInfoCouplingFile(VN_02)[2];
extern const Tbyte defInfoCouplingFile(VN_03)[3];
extern const Tbyte defInfoCouplingFile(VN_04)[4];
extern const Tbyte defInfoCouplingFile(VN_05)[5];
extern const Tbyte defInfoCouplingFile(VN_06)[6];
extern const Tbyte defInfoCouplingFile(VN_07)[7];
extern const Tbyte defInfoCouplingFile(VN_08)[8];
extern const Tbyte defInfoCouplingFile(VN_09)[9];
extern const Tbyte defInfoCouplingFile(VN_10)[10];
extern const Tbyte defInfoCouplingFile(VN_11)[11];
extern const Tbyte defInfoCouplingFile(VN_12)[12];
extern const Tbyte defInfoCouplingFile(VN_13)[13];
extern const Tbyte defInfoCouplingFile(VN_14)[14];
extern const Tbyte defInfoCouplingFile(VN_15)[15];


#endif /* FEMTOOS_SHARED_H */
