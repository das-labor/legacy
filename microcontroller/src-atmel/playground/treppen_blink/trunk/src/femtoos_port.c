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

#include "femtoos_core.h"
#include "femtoos_shared.h"


#if (cfgCheckReset == cfgTrue) || (defCheckReportingError == cfgTrue)
  /**
   * For reporting errors it may be handy to have a function that delays the message for some
   * time. Implement it in this function.
   */
  static void portShortDelay(Tuint08 uiLoops);
#endif


#if (defSysGCCstartup != cfgReplace)

void portInit(void)
{ /* Switch to the highest internal clock frequency (8MHz) Note that the need of this action
   * depends on the setting of the fuse CKDIV8. If that is unprogrammed (i.e. set to 1) these
   * instructions are not needed. However, the device is shipped with a programmed fuse. */
  #if (devCLKPCE != cfgUndefined)
    /* Not all hardware has prescalers but if it does, set the values here. We set it to
     * the highest clock frequency. This may not be what you want because of the power
     * consumption. Set an other value if needed. */
    devCLKPR = preBitSet1(0x00,devCLKPCE);
    devCLKPR = defClockPrescaleBits;
  #endif
  /* If we use low power sleep we disable watchdog, only works in  safety level 1
   * (in safety level 2 fuses need to be reset). Please note that it may be necessary
   * to perform this steps on devices that where switched off during sleep, and are
   * now used for other purposes. */
  #if (cfgUseLowPowerSleep == cfgTrue) || (includeGenReboot == cfgTrue)
    /* Disable the watchdog, since WDRF overrides the WDE, we must clear it first.
     * Furthermore, we clear all other flags, since we have no interest in them.
     * If you do, alter the code here to preserver the flags. */
    devMCUSR  =  preBitClr4(0x00,devWDRF,devBORF,devEXTRF,devPORF);
    /* Enable configuration change. */
    devWDTCR  =  preBitSet2(0x00,devWDCE,devWDE);
    /* Disable Watchdog, do NOT write any other bits! */
    devWDTCR  =  preBitClr1(0x00,devWDCE);
  #endif
  /* If we make use of tracing, we must configure it at the very beginning. Note we use
   * bitbanging because this is the most portable. */
  #if (cfgCheckTrace == cfgTrue)
    /* Set the data pin to output, we use it to transport the bits. */
    devTraceComDRR = preBitSet1(devTraceComDRR,devTraceDataPin);
    /* Set the ready pin to output, we use it to signal the data is available. */
    devTraceComDRR = preBitSet1(devTraceComDRR,devTraceReadyPin);
    /* Set the acknowledge pin to output, we use it see if the data has been read. */
    devTraceComDRR = preBitClr1(devTraceComDRR,devTraceAckPin);
  #endif
  /* Done */ }

#endif



#if (cfgCheckTrace == cfgTrue)

void portTrace(Tuint08 uiEvent)
{ /* This routine shifts one byte through an handshaked bitbanging protocol. */
  Tuint08 uiBitLoc = 0x80;
  /* uiBitLoc indicates the bit location of the bit from uiEvent that is about to be send. The loop
   * runs until all bits have been send. Since this is a full handshake protocol, make sure your
   * data collector is quick enough, otherwise timing errors may occur. */
  while (uiBitLoc != 0)
  { /* If the bit is set, set the output pin, otherwise clear it. */
    if ((uiEvent & uiBitLoc) != 0x00)
    { devTraceComPORT = preBitSet1(devTraceComPORT,devTraceDataPin); }
    else
    { devTraceComPORT = preBitClr1(devTraceComPORT,devTraceDataPin); }
    /* Send the listener that the data is ready. */
    devTraceComPORT = preBitSet1(devTraceComPORT,devTraceReadyPin);
    /* Wait until the data has been read and the listener told so. */
    while (preBitIsClr(devTraceComPIN,devTraceAckPin));
    /* Go to the next bit */
    uiBitLoc >>= 1;
    /* If the bit is set, set the output pin, otherwise clear it. */
    if ((uiEvent & uiBitLoc) != 0x00)
    { devTraceComPORT = preBitSet1(devTraceComPORT,devTraceDataPin); }
    else
    { devTraceComPORT = preBitClr1(devTraceComPORT,devTraceDataPin); }
    /* Send the listener that the data is ready. */
    devTraceComPORT = preBitClr1(devTraceComPORT,devTraceReadyPin);
    /* Wait until the data has been read and the listener told so. */
    while (preBitIsSet(devTraceComPIN,devTraceAckPin));
    /* Go to the next bit */
    uiBitLoc>>= 1; }
  /* Done. Note that if the transmission of even one bit fails, this protocol hangs for ever, so
   * keep your lines short. */ }

#endif



#if (defCheckReportingError == cfgTrue)

void portShowError(Tuint08 uiMessage, Tuint08 uiCallId, Tuint08 uiInfoTask)
{ /* We display the error code on devErrorComPORT. If you use the stk500 with eight leds, this can be
   * seen quite nicely. Specify that we want the message to be repeated 16 times. We arrive here
   * with disabled tick and global interrupts. */
  Tuint08 uiCount = 16;
  /* The error Port must be configured as output on all pins. We do not bother to store its original
   * value, since we must solve the error first. */
  devErrorComDDR = 0xFF;
  /* Send the error message several times. In order to make them catch the eye we have a special
   * blinking scheme. */
  while (uiCount--)
  { /* Draw the attention of the user by running a led. This also marks the beginning
     * of the sequence of information. */
    Tuint08 uiRun = 0x80;
    do
    { devErrorComPORT = ~uiRun;
      portShortDelay(0x02);
      uiRun >>= 1; }
    while (uiRun);
    /* Display the message. */
    devErrorComPORT = ~uiMessage;
    /* wait a little while */
    portShortDelay(0x20);
    /* Display the caller. */
    devErrorComPORT = ~uiCallId;
    /* wait a little while */
    portShortDelay(0x20);
    /* Display extra information. */
    devErrorComPORT = ~uiInfoTask;
    /* wait a somewhat longer */
    portShortDelay(0x20); }
  /* Done, we return, if the error was fatal, this method will be called again automatically. */ }

#endif


#if (cfgCheckReset == cfgTrue) || (defCheckReportingError == cfgTrue)

static void portShortDelay(Tuint08 uiLoops)
{ /* Disable all timer interrupts. Since we use this routine to display some (error) information
   * we do not want to disturbed by other sources. We only make it explicit for timer 0.
   * Note that other timer bits may be destroyed here. */
  devTIMSK = preBitClr2(0x00,devOCIE,devTOIE);
  /* Set the timer in normal 8 bit mode and activate the timer with the chosen prescaler.
   * Now, on some devices these bits are located on the same register, and on other they
   * are not. We must separate. Furthermore, it is assumed it is OK to clear the other
   * bits in the register.  */
  if (preEqualMacros(devTCCRB,devTCCRA))
  { devTCCRA = preBitClr1(0x00,devWGM) | defDelayPrescaleBits; }
  else
  { devTCCRA = preBitClr1(0x00,devWGM);
    devTCCRB = defDelayPrescaleBits; }
  /* operate in the standard 8 bit timer mode (some devices do not have a 16 bit mode.
   *  Note we use the same timer as the tick interrupt does. */
  /* reset the timer */
  devTCNT = 0x00;
  /* clear the overflow flag, so that we start with a clean timer. */
  devTIFR = preBitSet1(0x00,devTOV);
  /* If you use the simulator of Atmel, it cannot simulate the timer at this point
   * so just skip it. */
  #if (cfgSysDebug == cfgFalse)
    /* wait until the time is done. Note we do not use an interrupt, since that routine
     * is not at our disposal, but there is no need either. */
    while (uiLoops!=0)
    {  /* Wait until the timer overflows. */
       while (preBitIsClr(devTIFR,devTOV));
       /* Reset the overflow flag */
       devTIFR = preBitSet1(0x00,devTOV);
       /* Every overflow counts as one loop. */
       uiLoops--; }
  #endif
 /* Done, we have spend some time in this method. */ }

#endif

#if (cfgCheckReset == cfgTrue)

void portShowReset(void)
{ /* We blink all leds rapidly on portA. If you use the stk500 with eight leds, this can be
   * seen quite nicely. Specify that we want the message to be repeated 5 times. */
  Tuint08 uiCount = 5;
  /* Port A must be configured as output on all pins. */
  devErrorComDDR = 0xFF;
  /* Show the reset message a number of times. */
  while (uiCount--)
  { /* all leds on */
    devErrorComPORT = 0x00;
    /* wait a little while */
    portShortDelay(0x01);
    /* all leds off */
    devErrorComPORT = 0xFF;
    /* wait a little while */
    portShortDelay(0x01); }
 /* Done */ }

#endif


#if (cfgUseLowPowerSleep == cfgTrue) && ((cfgUseLowPowerOnDelay == cfgFalse) || !defined(devSigWatchdogTimeout))

void portSleep(Tuint08 uiTickBlockMinDelay)
{ /* We will go to sleep until we are woken by a kiss of the prins (i.e.
   * an external interrupt.) The parameter uiTickBlockMinDelay has no
   * relevance. Set the sleep mode to power down. This is needed,
   * since normally the sleep mode is different during the idle task.
   * Make sure we leave the other bits alone. */
  devSMCR = preBitClr2(preBitSet2(devSMCR,devSE,devSM1),devSM0,devSM2);
  /* Start sleeping, since we arrive here with interrupts disabled, they must be enabled again
   * just before sleeping, to make sure we actually go to sleep before an interrupt comes.
   * To make sure we only have one waking up interrupt we must immediately switch off
   * interrupts after wakeup. That way we can reliably determine which was the cause
   * of the awakening. */
  asm volatile (
   "sei        \n\t"   /* We reactivate the global interrupt just before we go to sleep so we know for      */
   "sleep      \n\t"   /* sure the sleep will indeed start off. In that way we do not miss an interrupt.    */
   "cli        \n\t"   /* And then we immediately disable them again so there cannot be an extra interrupt  */
    ::);               /* just after the first interrupt.                                                   */
  /* We woke up, first disable the sleep so we do not accidently fall asleep again. */
  devSMCR = preBitClr1(devSMCR,devSE);
  /* Cinderella has no idea how long she has slept, so it makes no sense to
   * correct the tick counter. We are done sleeping and may return to the OS
   * Note we must return with interrupts still switched off! */ }

#endif


#if (cfgUseLowPowerSleep == cfgTrue) && (cfgUseLowPowerOnDelay == cfgTrue)

void portSleep(Tuint08 uiTickBlockMinDelay)
{ /* We will go to sleep in periods given by cfgNumSleepPeriod and rounded to an allowed value
   * for this device. Global and timer interrupts are disabled at this moment, so we are
   * responsible for activating the interrupts on the right moment. Timer interrupts must
   * stay disabled. uiTickBlockMinDelay denotes the time we can sleep safely without
   * having the risk to wake up any delayed tasks. If uiTickBlockMinDelay is defSleepInfinite
   * (0xFF) there are no delayed tasks and we may sleep up to the moment an external interrupt
   * arrives. Of course we may sleep longer as uiTickBlockMinDelay but then you must activate
   * cfgUseLowPowerDelayRelease or face incorrect wakeup of delayed tasks. For infinite
   * sleep this does not matter, since there are no delays in that case and keeping the
   * tick counter does not make much sense. */
  /* Calculate how much blocks in terms of the watchdog we may sleep. Please note this generates
   * the shortest code when defSleepDivider is a power of two. See the discussion at its
   * definition.*/
  Tuint08 uiBlocksToSleep = (uiTickBlockMinDelay / defSleepDivider);
  /* The watchdog will be used to wake up, thus enable it (note that is does not seem
   * necessary to set the WDCE bit for changing the prescaler bits, contrary to what the manual
   * on page 45 states: "Bit 4 – WDCE: Watchdog Change Enable: .... This bit must also be set
   * when changing the prescaler bits") It seems this only is required at disabling the watchdog,
   * but i never got this to work as described. */
  /* Enable configuration change. */
  devWDTCR = preBitSet2(0x00,devWDCE,devWDE);
  devWDTCR = preBitSet4(0x00,devWDIF,devWDIE,devWDP2,devWDP1) | preBitClr4(0x00,devWDP3,devWDCE,devWDE,devWDP0);
  /* At this moment we have a fixed mode for sleeping which is the powerdown. Change it to your
   *  liking if needed. So, set the sleep mode to power down. This is needed, since normally
   * the sleep mode is different during the idle task. Make sure we leave the other bits alone. */
  devSMCR = preBitClr2(preBitSet2(devSMCR,devSE,devSM1),devSM0,devSM2);
  /* We use a counter to keep track of how many blocks we have slept, this in order to
   * correct the clock later on. */
  Tuint08 uiBlocksSleepCount = 0;
  /* Start sleeping. Because there are two sources of wakeup, i.e. the watchdog and some other
   * interrupt we must distinguish between the two. We do so by setting a sleepbit (we can use
   * the devAuxSysRegBit bit since within the sleep - wake loop no other use is made of it.) in
   * the devAuxSysReg. This bit is cleared by the watchdog and cleared just before sleep. If at
   * wakeup the bit is not set we know an other interrupt source was present and we must
   * break of sleeping at once. Otherwise we may continue if we are not done yet. So first
   * set the sleepBit to pass the first while loop.*/
  #if (cfgSysSqueezeState == cfgFalse)
  devAuxSysReg = preBitSet1(devAuxSysReg,devAuxSysRegBit);
  /* You may continue the loop when not all blocks are slept yet and we came from a watchdog interrupt */
  while ( ((uiBlocksSleepCount != uiBlocksToSleep)) && preBitIsSet(devAuxSysReg,devAuxSysRegBit) )
  { /* Clear the sleepbit, so it can be set by the watchdog interrupt */
    devAuxSysReg = preBitClr1(devAuxSysReg,devAuxSysRegBit);
    asm volatile (
     "sei        \n\t"   /* We reactivate the global interrupt just before we go to sleep so we know for      */
     "sleep      \n\t"   /* sure the sleep will indeed start off. In that way we do not miss an interrupt.    */
     "cli        \n\t"   /* And then we immideately disable them again so there cannot be an extra interrupt  */
     ::);                /* just after the first interrupt.                                                   */
    /* If we may sleep for ever we simple forbid sleepcounting so the loop will never reach its limit. Still other
     * interrupts may break the loop.*/
    if ((uiTickBlockMinDelay != defSleepInfinite)) uiBlocksSleepCount++;
    /* If a sleephook was installed it is called at every wakeup. */
    #if (callAppTickSleep == cfgTrue)
      appTickSleep();
    #endif
    /* Done, if we leave the loop here, the device has woken up */ }
  #else
    /* The code below does exactly the same as above, but now for the T bit instead of the AuxRegBit. Since
     * i don't know an efficient way to use the T bit in C code directly, i coded it in assembly. Save's two bytes
     * compared to what the gcc compiler makes of the code above, and six push/pop instructions, due to efficient
     * register use.
     * Note that you onw interrupt should not set the T bit on entry. Usually that does not happen since interrupts
     * should preserve the status register. And this has T bit cleared.  */
    asm volatile (
     "set                       \n\t"  /* The T bit functions as flag to detect if we come from an watchdog interrupt    */
     "76:                       \n\t"  /* or from an other interrupt. When it is set, we assume coming from a watchdog   */
     "cp %[_Count_],%[_Sleep_]  \n\t"  /* So start with the flag set. Now we compare the sleep counter with the number   */
     "breq 78f                  \n\t"  /* of blocks we must sleep. When equal, we are done, jump to the end.             */
     "brtc 78f                  \n\t"  /* Subsequently we check if the T bit is still set. If not, we had an non watch   */
     "clt                       \n\t"  /* dog interrupt and we may stop. Then, we start sleeping with T bit reset.       */
     "sei                       \n\t"  /* We reactivate the global interrupt just before we go to sleep so we know for   */
     "sleep                     \n\t"  /* sure the sleep will indeed start off. In that way we do not miss an interrupt. */
     "cli                       \n\t"  /* And then we immediately disable them again so there cannot be an extra inter-  */
     "cpi %[_Delay_],%[_Inf_]   \n\t"  /* rupt after the first interrupt. Then increase the sleep counter by one if      */
     "breq 77f                  \n\t"  /* sleep time is limited and otherwise keeping the counter constant will induce   */
     "subi %[_Count_],0xFF      \n\t"  /* Infinite looping.                                                               */
     "77:                       \n\t"  /*  */
    #if (callAppTickSleep == cfgTrue)  /* If we defined a hook on the heartbeat of sleeping call it.                     */
     defCALL " appTickSleep     \n\t"  /* Make use if defCall because we do not know how far we must go                  */
    #endif                             /*  */
     "rjmp 76b                  \n\t"  /*  Go for an other round of sleep.                                               */
     "78:                       \n\t"
     :
     [_Count_]    "=r" (uiBlocksSleepCount):
     "[_Count_]"       (uiBlocksSleepCount),
     [_Sleep_]    "r" (uiBlocksToSleep),
     [_Delay_]    "r" (uiTickBlockMinDelay),
     [_Inf_]      "i" (defSleepInfinite));

#endif
  /* To prevent accidentally falling asleep, clear the sleep enable bit. */
  devSMCR = preBitClr1(devSMCR,devSE);
  /* The watchdog must be disabled, since it may interrupt the OS with disastrous consequences.
   * Enable a configuration change. (There is no need now to clear the WDRF, we do not come here
   * after a reset. If we did not set the watchdog, there is nothing to clear here.  */
  devWDTCR  =  preBitSet2(0x00,devWDCE,devWDE);
  /* Disable Watchdog, do NOT write any other bits! Setting the prescaler seems not to be allowed
   * on this moment. Why, is unclear to me, but we do not need it here. */
  devWDTCR  =  preBitClr1(0x00,devWDCE);
  /* If we want to correct the tick counter ... */
  #if (includeGenAddtoTickCount == cfgTrue)
    /* Calculate the time slept by approximation. It does not make sense to adjust the clock
     * when we where allowed to sleep indefinitely. */
    if (uiTickBlockMinDelay != defSleepInfinite)
    { /* The number of sleepblocks slept have been counted, so it is relatively easy to calculate
       * the number of extra ticks. First each sleepblock accounts for defSleepDivider tickblocks,
       * and each tickblock is 256 ticks. */
      Tuint16 uiTicksSlept = (uiBlocksSleepCount * defSleepDivider << 8);
      /* And pass it on to the Femto OS */
      genAddtoTickCount(uiTicksSlept); }
  #endif
  /* We are done sleeping and may return to the OS
   * Note we must return with interrupts still switched off! */ }

#endif


#if (includeGenReboot == cfgTrue)

void portReboot(void)
{ /* These are the device specific instructions to reboot. Unfortunately there is no
   * way to reset the device directly. Depending on your situation you could jump to
   * __init, but that does not reset the registers, and your __init may even rely upon
   * the inital values of them, so the interrupts may not be cleared at the start!!
   * Hmm, it seems we must misuse the
   * watchdog for this. Make sure you disable the watchdog upon re-entry, set it to
   * the reset mode, and the shortest interval. (16ms)
   * TODO: test if the reset time is really 16ms, even if we have slept some other time
   * before. The point is, we do not set WDCE, although is should have been done
   * account. Page 45 of the manual, Bit 4 -WDCE: Watchdog Change Enable.
   * Found: for the attiny861 this is works also without, but for other devices it does not. */
  devWDTCR  =  preBitSet2(0x00,devWDCE,devWDE);
  devWDTCR  =  preBitSet1(0x00,devWDE) | preBitClr5(0x00,devWDIE,devWDP3,devWDP2,devWDP1,devWDP0);
  /* Now, wait on the reset, for we may not return. */
  while (true); }

#endif


void portIdle(void)
{ /* If we arrive here it is because there are no tasks that can be run. This is the idle
   * task. We must stay here until the next timer interrupt. */
  #if cfgSysDebug == cfgTrue
    /* If you use the simulator of Atmel, it cannot simulate the timer at this point
     * so we manually simulate it. */
    devSigTimerCompare();
  #else
    /* It is the responsibility of the portIdle implementation to activate the timer
     * interrupts, since there are no references with regard to timer activation/
     * deactivation inside the femto OS core. You may assume you arrive here with
     * timer interrupts switched off. */
    devTIMSK = preBitSet1(devTIMSK,devOCIE);
    /* the avr tiny has and idle state that we will utilize, this is the sleep idle mode,
     * enable it */
    devSMCR = preBitClr3(preBitSet1(devSMCR,devSE),devSM2,devSM1,devSM0);
    /* and start sleeping. An (timer) interrupt will wake the device */
    asm volatile ("sleep" :: );
    /* If we are woken by some other interrupt we return here after the interrupt is
     * done. However, we may not return, therefore we will just wait for the next
     * timer interrupt. */
    while (true);
  #endif
}

#ifndef portInitContext
/* Only include the code below if portInitContext is not defined, i.e. if we do not reroute a call to
 * portInitContext to privInitContext. The implementation below is identical to the one in the core.c */
Taddress portInitContext(Taddress pTaskStart, Taddress pStackTop, Tuint08 uiRegisterCount, Tuint08 uiInterruptStart)
{ /* In case you cannot make use of the privInitContext defined inside the Femto OS you can define your
   * own. This method does not contain any assembler, but the order in which status register and regular registers
   * are save is tightly coupled to the organization of portSaveContext and portRestorContext.
   * Note that the code below is identical to the code inside Femto OS and is not used in reality. See
   * port.h how to activate this (or your substitute) code. */
  Tuint16 uiStartAddress = (Tuint16) pTaskStart;
  /* At the bottom of the artificial stack the start address of each task is defined. This is a pointer
   * to your Loop code. */
  *(pStackTop--) = (Tuint08) uiStartAddress;
  *(pStackTop--) = (Tuint08) (uiStartAddress >> 8);
  /* If we have a program counter of more than 16 bit, call instructions push three bytes
   * into the stack. We must take that into account. Unfortunately, i believe, gcc is not
   * able to handle pointer larger as 64K words in the current setting. Thus this will
   * effectively be zero, thus although you need something like
   *   *(pStackTop--) = (Tuint08) (pTaskStart >> 16);
   * we will use: */
  #if (defThreeByteAddress == cfgTrue)
    *(pStackTop--) = 0;
  #endif
  /* DISCUSSION
   * You can if you wish fill the register space with other bytes. This is the place to do so,
   * otherwise these have the value of the defStackInitByte, which is usually set to zero. */
  #if (0)
    /* Clean the stack actively in case that we are reinitializing, or want to fill it with something else as 0.
     * Note that this also fill r1 since the option cfgSysClearUnusedR1 operates before the registers are restored. */
    while (uiRegisterCount--) { *(pStackTop--) = defYourRegisterInitByte; }
  #else
    /* We rely upon register cleaning done by the Femto OS, or the precleaning done by privTaskInit(). */
    pStackTop -= uiRegisterCount;
  #endif
  /* The way the status register is setup depends on the state of the interrupts. These can be specified per task
   * or (easier, and shorter) in general. Note that the optimization is not needed in a strict sense, the variable
   * uiInterruptStart constrains the interrupt start information, even if this is constant for all tasks. But, of course,
   * the lower part generates shorter code. */
  #if (defInterruptStartConstant == cfgFalse)
    /* If we specify the CPU status register per task, we need to prepare them per task. uiInterruptStart contains
     * the information about which interrupts must be activated. The other bits of the status register cannot be
     * set individually (there is no need in general) */
    Tuint08 uiInitCPUStatusRegister = defInitCPUStatusRegister | (0 << portInitModeInterruptLoc);
    /* Test if global interrupts must be activated at the start, if so set the specified bit. */
    #if (cfgIntGlobalOnly ==  cfgTrue)
      /* In case we have a mapping from tick interrupts on global interrupts, we require both interrupts
       * to be activated before activating the global interrupt. */
      if ((uiInterruptStart & ((cfgGlobSet | cfgTickSet) & defInitialInterruptGetMask)) == ((cfgGlobSet | cfgTickSet) & defInitialInterruptGetMask)) { uiInitCPUStatusRegister |= (1 << portInitGlobalInterruptLoc); }
    #else
      /* Otherwise we just test the global interrupt setting. */
      if ((uiInterruptStart & (cfgGlobSet & defInitialInterruptGetMask)) != defInitialInterruptAbsent) { uiInitCPUStatusRegister |= (1 << portInitGlobalInterruptLoc); }
    #endif
    /* Test if tick interrupts must be activated at the start, if so set the specified bit. */
    #if (cfgIntTickTrack == cfgTrue)
      if ((uiInterruptStart & (cfgTickSet & defInitialInterruptGetMask)) != defInitialInterruptAbsent) { uiInitCPUStatusRegister |= (1 << portInitTickInterruptLoc); }
    #endif
    /* The status register is put at the end of the stack. (See portSaveContext for the reason why) */
    *(pStackTop--) = uiInitCPUStatusRegister;
  #else
    /* Set up the status register with the initial interrupt states: global set and tick set */
    #if (((defInterruptStartFixed) & cfgGlobSet) == cfgGlobSet) && (((defInterruptStartFixed) & cfgTickSet) == cfgTickSet)
      *(pStackTop--) = defInitCPUStatusRegister | (0 << portInitModeInterruptLoc) | (1 << portInitGlobalInterruptLoc) | (1 << portInitTickInterruptLoc);
    /* Set up the status register with the initial interrupt states: global set and tick clear */
    #elif (((defInterruptStartFixed) & cfgGlobSet) == cfgGlobSet) && (((defInterruptStartFixed) & cfgTickClear) == cfgTickClear)
      #if (cfgIntGlobalOnly == cfgTrue)
        /* In case we have a mapping from tick interrupts on global interrupts, global interrupts cannot be activated if tick interrupts are not activated */
        *(pStackTop--) = defInitCPUStatusRegister | (0 << portInitModeInterruptLoc) | (0 << portInitGlobalInterruptLoc) | (0 << portInitTickInterruptLoc);
      #else
        /* Default situation. */
        *(pStackTop--) = defInitCPUStatusRegister | (0 << portInitModeInterruptLoc) | (1 << portInitGlobalInterruptLoc) | (0 << portInitTickInterruptLoc);
      #endif
    /* Set up the status register with the initial interrupt states: global clear and tick set */
    #elif (((defInterruptStartFixed) & cfgGlobClear) == cfgGlobClear) && (((defInterruptStartFixed) & cfgTickSet) == cfgTickSet)
      *(pStackTop--) = defInitCPUStatusRegister | (0 << portInitModeInterruptLoc) | (0 << portInitGlobalInterruptLoc) | (1 << portInitTickInterruptLoc);
    /* Set up the status register with the initial interrupt states: global clear and tick clear */
    #elif (((defInterruptStartFixed) & cfgGlobClear) == cfgGlobClear) && (((defInterruptStartFixed) & cfgTickClear) == cfgTickClear)
      *(pStackTop--) = defInitCPUStatusRegister | (0 << portInitModeInterruptLoc) | (0 << portInitGlobalInterruptLoc) | (0 << portInitTickInterruptLoc);
    #elif (defNumberOfTasks == 0)
      /* without tasks there is nothing to set. */
    #else
      /* well, we never come here. */
      #error "Parameter 'defInterruptStartFixed' is misspeld or incorrect (You should not arrive here)."
    #endif
  #endif
  /* Done, let the caller know where the stack ended. */
  return pStackTop; }
#endif


void portSetupTimerInterrupt(void)
{ /* We use a 8 bit timer to generate the tick. Note that it is not really possible to 'calculate' the
   * maximum value of the timer for general values of the tick frequency, this depends heavily on the
   * possibilities of the hardware. Therefore we work with a limited set of allowed values of the
   * prescaler, present in most devices.
   * Reset the prescaler, Set the largest  divider */
   /* In case of equidistant ticks we make use of the automatic cleaning of the subtick counter
    * If your hardware does not have such a mode, it must be coded by hand in the tick interrupt. */
   #if (cfgUseEquidistantTicks == cfgTrue)
     /* Use the 8 bit CTC mode */
     if (preEqualMacros(devTCCRB,devTCCRA))
     { devTCCRA = preBitSet1(0x00,devWGM) | defTimerPrescaleBits; }
     else
     { devTCCRA = preBitSet1(0x00,devWGM);
       devTCCRB = defTimerPrescaleBits; }
     /* Set the time at which the tick interrupt should take place. */
     devOCR = cfgSysSubTicksPerFullTick;
   #else
     /* Use a normal counting mode in case of variable time slices, clear is done through the call
      * portReadAndResetTimer */
     if (preEqualMacros(devTCCRB,devTCCRA))
     { devTCCRA = preBitClr1(0x00,devWGM) | defTimerPrescaleBits; }
     else
     { devTCCRA = preBitClr1(0x00,devWGM);
       devTCCRB = defTimerPrescaleBits; }
  #endif
  /* To be able to check for overflowing timers we must enable the overflow interrupt. */
  #if ((cfgUseEquidistantTicks == cfgFalse) && (cfgCheckTiming == cfgTrue))
    /* We  must make sure a overflow interrupt is not immediately generated, so reset the timer. */
    devTCNT = 0x00;
    /* Set the TOIE0  bit (enables the overflow interrupt). */
    devTIMSK = preBitSet1(devTIMSK,devTOIE);
  #endif
 /* Do not enable the tick interrupt itself. The OS is started with tick interrupts disabled. */ }


#if ((cfgUseEquidistantTicks == cfgTrue) && ((cfgCheckTiming == cfgTrue) || (cfgUseLoadMonitor == cfgTrue))) || ((cfgUseLoadMonitor == cfgTrue) && (cfgIntUserDefined == cfgTrue))

Tuint08 portReadTimer(void)
{  /* The intent of this method is to read the current value of the subtick timer. */
  #if cfgSysDebug == cfgTrue
    /* If you use the simulator of Atmel, it cannot simulate the timer at this point
     * so we manually simulate it, let's just return some fictitious value. */
    Tuint08 uiResult = cfgSysSubTicksPerFullTick / 8;
  #else
    /* We must have deactivated timer interrupts to get an reliable measurement. However, we do not
     * need to do that here, since the portReadTimer is only called from the OS (timer interrupts
     * are switched off, or from an isr (global interrupts switched off). */
    Tuint08 uiResult = devTCNT;
    /* if there is an unhandled interrupt, the read value must be increased with cfgSysSubTicksPerFullTick
     * since there is no way we could otherwise obtain this information. The only problem is that the
     * interrupt flag could become set in between the instruction above and the one below. That would lead to
     * a way to large value of the result. Now, since there is no atomic way to gain the information about the
     * subtick timer and the interrupt flag, we must check if adding cfgSysSubTicksPerFullTick makes sense. */
    if ( preBitIsSet(devTIFR,devOCF)  && (uiResult < cfgSysSubTicksPerFullTick/2) ) uiResult+=cfgSysSubTicksPerFullTick;
  #endif
  return uiResult; }
#endif


#if (cfgUseEquidistantTicks == cfgTrue) && (cfgIntManualTicks == cfgTrue)

Tbool portCheckTimer(void)
{ /* Check if the timer interrupt is due to fire. If so return true and release the trigger, otherwise
   * return false. This is used in situations where all tasks are cooperative and the timer never gets
   * a chance to fire. */
  Tbool bResult = false;
  /* Test the value of the timer interrupt flag. If set, reset it by writing a one to its position
   * and utilize the none zero result as true value. Note that we do NOT change the value of the timer,
   * we merely reset the timer flag, which is replaced by a manual tick inside the core code. */
  if (preBitIsSet(devTIFR,devOCF))  { devTIFR = bResult = preBitSet1(0x00,devOCF); }
  /* Done, return the result. */
  return bResult; }

#endif


#if (cfgUseEquidistantTicks == cfgFalse)

Tuint08 portReadAndResetTimer(Tuint08 uiSubTickInterrupt)
{ /* The intent of this method is to read the current value of the subtick timer and
   * reset it at the same time. Unfortunately this cannot be done in one instruction,
   * thus we make a systematic timing error here. If needed this timer can be implemented
   * as a 16 bit timer which would enhance the accuracy of the timer. In practice we never
   * measure real time through this mechanism so approximate values are sufficient here.
   * This method is called with tick interrupts disabled. */
  #if cfgSysDebug == cfgTrue
    /* If you use the simulator of Atmel, it cannot simulate the timer at this point
     * so we manually simulate it, let's just return some fictitious value. */
    Tuint08 uiResult = cfgSysSubTicksPerFullTick / 2;
  #else
    /* If we have OS protection, global interrupts are disabled, but otherwise ... */
    #if (cfgIntOsProtected == cfgFalse)
      /* ... if an other interrupt comes in between these two instructions, that will disturb the
       * timing even more. This can be prevented by conditionally disabling the interrupts
       * around these instructions. */
      privDisableGlobalInterrupts();
    #endif
      /* Read the value of the current sub tick timer (timer 0 on this device) */
      Tuint08 uiResult = devTCNT;
      /* reset the timer. This timer could be increased during the time needed to execute
       * the the former instruction. This results in a systematic loss of accuracy, for
       * which there is no real (short) cure. It is absolutely essential that no interrupts occur
       * between there two instructions.*/
      devTCNT = 0x00;
      /* Set the new value of the alarm, this determines the time the next task has. */
      devOCR = uiSubTickInterrupt;
      /* We clear the interrupt compare flag, to make sure the new task has the full
       * slice, if we would skip this, there could be the situation that that a previous
       * unhandled tick cases an unintended interrupt. */
      devTIFR = preBitSet1(0x00,devOCF);
    #if (cfgIntOsProtected == cfgFalse)
      /* From this point on interrupts are allowed again. */
      privEnableGlobalInterrupts();
    #endif
  #endif
  /* Done, let he caller know the previous time spend. */
  return uiResult; }

#endif


#if defined(devSigWatchdogTimeout) && (defSysGCCstartup != cfgReplace) && (cfgUseLowPowerSleep == cfgTrue) && (cfgUseLowPowerOnDelay == cfgTrue)

void devSigWatchdogTimeout(void)
{ /* We must signal the receiver we come from the watchdog interrupt. Unfortunately the device flag
   * is cleared because of this handeling. We have to implement some handling however, otherwise
   * the interrupt cannot wake up the device. */
#if (cfgSysSqueezeState == cfgFalse)

  asm volatile (
   "sbi  %[_ASR_],%[_ARB_] \n\t"  /* Set the WD bit to indicate a watchdog timeout occurred.                */
   "ret                    \n\t"  /* Get back to the sleepmethod, there is no need to set the interrupts.   */
   "" ::
   [_ARB_]  "i" (devAuxSysRegBit),
   [_ASR_]  "i" (_SFR_IO_ADDR(devAuxSysReg))
   );

#else
  /* Here we use the T bit to transmit we hit the watchdog. The H bit is less suitable since its state
   * may be destroyed (set) in the handling of alternative interrupts. It is easier to restrict the
   * use of the T-bit. */
  asm volatile (
   "set  \n\t" /* Set the T bit to indicate a watchdog timeout occurred.                 */
   "ret  \n\t" /* Get back to the sleep method, there is no need to set the interrupts.   */
   "" :: );
#endif
}

#endif


#if (defSysGCCstartup != cfgReplace) && (cfgUseEquidistantTicks == cfgFalse) && (cfgCheckTiming == cfgTrue)

void devSigTimerOverflow(void)
{ /* Let the OS know the subtick timer overflowed. This only makes sense when cfgUseEquidistantTicks == cfgFalse
  * since in the other case the timer cannot overflow. */
  portJump(privSubtickOverflow); }

#endif


#if (defSysGCCstartup != cfgReplace)

void devSigTimerCompare(void)
{ /* If your hardware does not support automatic reset of the sub tick counter, you must do so here,
   * but only if (cfgUseEquidistantTicks == cfgTrue). */
  /* Let the OS know the sub tick timer reached its interrupt level and it is
   * time to switch the task. */
  portJump(privTickYield); }

#endif

/* Define the most optimal handling for the TIMSK register, depending on its location.  */
#if (defTIMSKinIO == cfgTrue)
  #define LoadTIMSK(rx)   "in   " #rx ", %[_TIMSKio_]    \n\t"
  #define StoreTIMSK(rx)  "out  %[_TIMSKio_], " #rx "    \n\t"
#else
  #define LoadTIMSK(rx)   "lds   " #rx ", %[_TIMSKmem_]  \n\t"
  #define StoreTIMSK(rx)  "sts  %[_TIMSKmem_], " #rx "   \n\t"
#endif


#if (cfgUseFileSystem == cfgTrue) && (cfgUseFileSystemEconomyMode == cfgFalse)
/* If you arrive here the OS did already check writing is allowed via portFSWriteBusy, there
 * is not need to check it again. Make sure you erase the bytes if needed Global interrupts may
 * be activated, so save the state.*/
void portFSWriteByte(Taddress pAddress, Tbyte bValue)
{ /* We perfrom the actual writing in assembly code for optimization reasons. */
  asm volatile (
   "com %[_VAL_]             \n\t" /* The byte that we write is actually the inverse,                                                  */
  #if (devEEPROMsize > 0x0100U)
   "out %[_EEARH_],%B[_ADR_] \n\t" /* Fill the registers EEARH and EEARL with the location of the byte to be written. Since we are     */
  #endif
   "out %[_EEARL_],%A[_ADR_] \n\t" /* the only task that may write (and thus use these registers), we don't need to disable irq's yet. */
   "clr r26                  \n\t" /* Make one register 0 (we could have used r1, but i am not absolutely certain it is zero always.   */
   "out %[_EECR_], r26       \n\t" /* Set the erase/Write mode (0<<EEPM1)|(0<<EEPM0), clear interrupt mode,                            */
   "out %[_EEDR_], %[_VAL_]  \n\t" /* Put the data at the location for writing.                                                        */
   "in  r26, __SREG__        \n\t" /* At this point we must disable interrupts, since the two instructions that follow, must be        */
   "cli                      \n\t" /* executed within 4 clockcyles of each other.                                                      */
   "sbi %[_EECR_],%[_EEMPE_] \n\t" /* Enable writing, and (in next instruction ) commence writing. Note the writing is still busy when */
   "sbi %[_EECR_],%[_EEPE_]  \n\t" /* this function completes. That is OK, we call portFSWriteReady() before next writing.             */
   "out __SREG__, r26        \n\t" /* Restore the interrupt state and we are done.                                                     */
   "" ::
   [_ADR_]   "r" (pAddress),
   [_VAL_]   "r" (bValue),
   [_EECR_]  "i" (_SFR_IO_ADDR(devEECR)),
   [_EEDR_]  "i" (_SFR_IO_ADDR(devEEDR)),
   [_EEARL_] "i" (_SFR_IO_ADDR(devEEARL)),
  #if (devEEPROMsize > 0x0100U)
   [_EEARH_] "i" (_SFR_IO_ADDR(devEEARH)),
  #endif
   [_EERE_]  "i" (devEERE),
   [_EEPE_]  "i" (devEEPE),
   [_EEMPE_] "i" (devEEMPE):
   "r26" ); }

#endif

/* DISCUSSION
 *
 * Because we make use of -mint8 we cannot make use of the instruction cbr Rd,K. This
 * is because gcc emits signed integers as operands and thus something like
 *   cbr r24, 0xF0
 * generates
 *   cbr r24, -128
 * this can then not be translated into opcodes, for internally the operation
 * (0xFF - -128) cannot be performed. Imho this is a bug in the compiler.
 * The workaround is:
 *   andi r24, ~0xFO
 */

#if (cfgUseFileSystem == cfgTrue) && (cfgUseFileSystemEconomyMode == cfgTrue)
/* First make sure the device has features for split-byte programming, if not,
 * you should not call this (test is done in check.h)
 * If you arrive here the OS did already check writing is allowed via portFSWriteBusy, there
 * is not need to check it again. Make sure you erase the bytes if needed Global interrupts may
 * be activated, so save the state. This method implements a more careful way of writing bytes,
 * making use of the split erase / write mode of the device. In fact, we only need to erase if
 * there are some bits to be set and only need to write if there are some bits to be cleared
 * in the byte to be written. We read first to inspect the situation. Off course this introduces
 * overhead, but extends the lifetime of the eeprom. Also, on average, the call completes more
 * quickly compared to the standard implementation. */
void portFSWriteByte(Taddress pAddress, Tbyte bValue)
{ asm volatile (
  #if (devEEPROMsize > 0x0100U)
   "out %[_EEARH_],%B[_ADR_] \n\t" /* Fill the registers EEARH and EEARL with the location of the byte to be written. Since we are    */
  #endif
   "out %[_EEARL_],%A[_ADR_] \n\t" /* the only task that may write (and thus use these registers), we don't need to disable irqs yet. */
   "sbi %[_EECR_],%[_EERE_]  \n\t" /* Start reading, we need the information of the byte present to sort out what to do.              */
   "in  r24, %[_EEDR_]       \n\t" /* Fetch the data.                                                                                 */
   "ldi r23, %[_EECOMb_]     \n\t" /* EEPM1,EEPM0, EEMPE set 0x34                                                                     */
   "mov r25,24               \n\t" /* Copy the data (data is retained for later use)                                                  */
   "or  r25,%[_VAL_]         \n\t" /* r25=d|m (determines which positions must be set                                                 */
   "cpi r25,0xFF             \n\t" /* d|m ?= 0xFF If that results equals 'all set' we don't need to erase                             */
   "breq 2f                  \n\t" /* Skip to the next test                                                                           */
   "andi r23, %[_nEEPM1b_]   \n\t" /* Activate the erase bit in the action register r23 (bit _EEPM1_)                                 */
   "ldi r24,0xFF             \n\t" /* d=0xFF  replace the data byte by 0xFF                                                           */
   "2:                       \n\t" /* Continue operations here is there is nothing to erase.                                          */
   "and r24,%[_VAL_]         \n\t" /* r24=d&m (determines which positions remain cleared.                                             */
   "cpi r24,0x00             \n\t" /* d&m ?= 0 If all positions remain cleared, there is noting to write                              */
   "breq 3f                  \n\t" /* Skip to the next action                                                                         */
   "andi r23, %[_nEEPM0b_]   \n\t" /* Activate the write bit in the action register r23 (bit _EEPM0_)                                 */
   "3:                       \n\t" /* Continue operations here is there is nothing to write.                                          */
   "cpi r23, %[_EECOMb_]     \n\t" /* Now, check if there are any actions to take.                                                    */
   "breq 4f                  \n\t" /* If not, there is nothing to do and we are done.                                                 */
   "com %[_VAL_]             \n\t" /* The byte that we write is actually the inverse, this is to save write operations.               */
   "out %[_EEDR_],%[_VAL_]   \n\t" /* Put the byte in the correct location for writing.                                               */
   "in   r24, __SREG__       \n\t" /* Writing (and/or erasing) must be prepared whitout the risk of an interrupt, so copy the status  */
   "cli                      \n\t" /* register, and clear the interrupts.                                                             */
   "out %[_EECR_],r23        \n\t" /* Enable writing (and set write/erase mode according to action register at the same time!), and   */
   "sbi %[_EECR_],%[_EEPE_]  \n\t" /* commence writing. Note the writing is still busy when this function completes. That is ok, we   */
   "out __SREG__, r24        \n\t" /* call portFSWriteReady() before next writing. Restore the interrupt state and we are done.       */
   "4:                       \n\t"
   "" ::
   [_ADR_]     "r" (pAddress),
   [_VAL_]     "r" (bValue),
   [_EECR_]    "i" (_SFR_IO_ADDR(devEECR)),
   [_EEDR_]    "i" (_SFR_IO_ADDR(devEEDR)),
   [_EEARL_]   "i" (_SFR_IO_ADDR(devEEARL)),
  #if (devEEPROMsize > 0x0100U)
   [_EEARH_]   "i" (_SFR_IO_ADDR(devEEARH)),
  #endif
   [_EERE_]    "i" (devEERE),
   [_EEPE_]    "i" (devEEPE),
   [_EEMPE_]   "i" (devEEMPE),
   [_EEPM0_]   "i" (devEEPM0),
   [_EEPM1_]   "i" (devEEPM1),
   [_nEEPM0b_] "i" (preBitClr1(0xFF,devEEPM0)),
   [_nEEPM1b_] "i" (preBitClr1(0xFF,devEEPM1)),
   [_EECOMb_]  "i" (preBitSet3(0x00,devEEPM1,devEEPM0,devEEMPE)):
   "r23");
 }

#endif

#if (cfgUseFileSystem == cfgTrue)
/* Reading is allowed with interrupts on. */
Tbyte portFSReadByte(Taddress pAddress)
{ Tuint08 result;
   /* We perform the actual reading in assembly code for optimization reasons. */
  asm volatile (
   "in  r26, __SREG__        \n\t" /* Copy the status register as quickly as possible and disable interrupts, even if it was cleared  */
   "cli                      \n\t" /* already. Although the read operation in itself is allowed to be interrupted, we cannot allow    */
  #if (devEEPROMsize > 0x0100U)
   "out %[_EEARH_],%B[_ADR_] \n\t" /* the registers EEARH and EEARL to be destroyed by an other task. Load the EEARH and EEARL        */
  #endif
   "out %[_EEARL_],%A[_ADR_] \n\t" /* registers.                                                                                      */
   "sbi %[_EECR_],%[_EERE_]  \n\t" /* Start reading                                                                                   */
   "in  %A[_RES_],%[_EEDR_]  \n\t" /* Fetch the data read.                                                                            */
   "out __SREG__, r26        \n\t" /* Done.                                                                                           */
   "" :
   [_RES_]   "=r" (result):
   [_ADR_]   "0" (pAddress),
   [_EECR_]  "i" (_SFR_IO_ADDR(devEECR)),
   [_EEDR_]  "i" (_SFR_IO_ADDR(devEEDR)),
   [_EEARL_] "i" (_SFR_IO_ADDR(devEEARL)),
  #if (devEEPROMsize > 0x0100U)
   [_EEARH_] "i" (_SFR_IO_ADDR(devEEARH)),
  #endif
   [_EERE_]  "i" (devEERE),
   [_EEPE_]  "i" (devEEPE),
   [_EEMPE_] "i" (devEEMPE),
   [_EEPM0_] "i" (devEEPM0),
   [_EEPM1_] "i" (devEEPM1):
   "r26" );
  /* Note we invert the result. All bytes are stored inverted, so that a completely erased eerprom
   * appears to be filled with zero's */
  return ~result; }

#endif


/* DISCUSSION
 * The port(Enter/Exit)(Global/Tick)Interrupts methods must temporarily store
 * the value of the interrupt state and restore it when needed. These methods
 * are only called from within the Femto OS and are never nested, and you should not
 * call them yourself. (other functions are available). Since we did not want to make
 * inserts (although you can still do so, by turning them into defines)
 * because that takes a lot of extra bytes (a call is shorter) we cannot store
 * the status on the stack. Thus we decided to use, for example a general purpose
 * I/O register (devAuxSysReg) of the AVR tiny as temporary variable. This is used in
 * portSave- and portRestore Context also, but since we know we will never call a context
 * switch from within one of these Enter/Exit constructions, there is no risk
 * of double use. (Remember, you should not use these functions, the results
 * can be bizarre!)
 * Furthermore we make use of one bit of devAuxSysReg to catch the state of the current
 * global interrupt when a context switch occurs. Therefore, when running a task that
 * bit must always be zero; it is set if global interrupts are set on context switch.
 * Note that the functions portEnterGlobalInterrupts/portExitGlobalInterrupts
 * cannot rely on bit 7 being zero since these functions are used in isr's also.
 * This has been corrected by clearing this bit in the PortSaveContext.
 * From this it follows that it is forbidden to activate global or tick interrupts
 * in their respective critical sections by hand. It is not a very logical thing
 * to do either.
 * An other point of attention is the interrupt nesting due to external interrupts.
 * We do not allow interrupt nesting in principle, so you may not activate interrupts
 * inside an isr. If we have not protected the OS from outside interrupts, you may
 * not call genXXX methods, and if we do EnterSysCritical translates to global
 * interrupt management. Thus we can use for both portEnterGlobalInterrupts/portExit-
 * GlobalInterrupts and portEnterTickInterrupts/portExitTickInterrupts the same
 * storage room for the interrupt state since they can never be nested.
 * Remember for user critical nesting there are other facilities. The former are
 * solely for the OS and are meant to be as brief as possible.
 */

#if (defNonSwitchingCallPresent == cfgTrue)

#if (cfgSysSqueezeState == cfgFalse)

void portEnterGlobalInterrupts(void)
{ /* We make use of the fact that devAuxSysRegBit must equal zero upon entry. */
  asm volatile (
   "brid 41f               \n\t"  /* We rely upon devAuxSysRegBit being zero. Thus we test if global interrupts are activated.   */
   "cli                    \n\t"  /* If not we skip, since there is nothing to do. If so, directly clear them, and set the bit   */
   "sbi %[_ASR_],%[_ARB_]  \n\t"  /* in devAuxSysRegBit to one, indicating the interrupts where activated. If we skip, the       */
   "41:                    \n\t"  /* value in devAuxSysRegBit is already correct.                                                */
   "ret     " ::                  /* Return to caller.                                                                           */
   [_ARB_]  "i" (devAuxSysRegBit),
   [_ASR_]  "i" (_SFR_IO_ADDR(devAuxSysReg))); }

#else

void portEnterGlobalInterrupts(void)
{ /* We keep the state of the interrupt in the Tbit. This is perfectly save since we know code in between
   * portEnterGlobalInterrupts and portExitGlobalInterrupts is Femto OS code only, which is verified to
   * use no T bit. */
  asm volatile (
   "clt                    \n\t"  /* We cannot rely upon T bit being zero, so clean that first. We do not have to preserve T     */
   "brid 41f               \n\t"  /* since we came here manually. Now test  if global interrupts are activated.                  */
   "cli                    \n\t"  /* If not we skip, since there is nothing to do. If so, directly clear them, and set the T bit */
   "set                    \n\t"  /* to one, indicating the interrupts where activated. If we skip, the T bit remains zero       */
   "41:                    \n\t"  /* which is correct.                                                                           */
   "ret     " ::
   [_Tb_]   "i" (preBitSet1(0x00,SREG_T)) ); }

#endif

#endif


#if (defNonSwitchingCallPresent == cfgTrue)

#if (cfgSysSqueezeState == cfgFalse)

void portExitGlobalInterrupts(void)
{ asm volatile (
   "sbis %[_ASR_],%[_ARB_] \n\t" /* If devAuxSysRegBit is zero global interrupts where disabled on enter, nothing to do            */
   "ret                    \n\t" /* So we can just return                                                                          */
   "cbi  %[_ASR_],%[_ARB_] \n\t" /* Otherwise, devAuxSysRegBit of must be zero before e.g. a portSaveContext occurs, so clear it   */
   "reti                   \n\t" /* And since it was set we must activate interrupts upon return                                   */
   "" ::
   [_ARB_]  "i" (devAuxSysRegBit) ,
   [_ASR_]  "i" (_SFR_IO_ADDR(devAuxSysReg))
   ); }

#else

void portExitGlobalInterrupts(void)
{ /* We keep the state of the interrupt in the Tbit. This is perfectly save since we know code in between
   * portEnterGlobalInterrupts and portExitGlobalInterrupts is Femto OS code only, which is verified to
   * use no T bit. */
  asm volatile (
   "brts 40f               \n\t" /* The previous state of the interrupt was stored in the T bit, test if it was set, if so return  */
   "ret                    \n\t" /* with a reti, otherwise no interrupt was set and return with a ret.                             */
   "40:                    \n\t" /*                                                                                                */
   "reti                   \n\t" /*                                                                                                */
   "" ::
   [_Tb_]       "i" (preBitSet1(0x00,SREG_T)) ); }

#endif

#endif


/* DISCUSSION
 * We don not need the portEnterTickInterrupts when we only allow for global interrupts. Switched on
 * tick interrupts are then replaced by global interrupts. If not these calls are used by Femto OS
 * to protect the internals of the API methods. They are only used when cfgIntOsProtected == cfgFalse.
 * In that case interrupts from outside may interrupt these methods. Note that, because of this,
 * it is never possible that the Enter/Exit Tick Interrupts get nested, for when they are allowed,
 * user defined interrupts may not yield, but must return to the point of interrupt. Therefore we
 * do not need to save the the state of tick interrupt itself, we can simply switch it off and on,
 * if there is no tick interrupt tracking. In the latter case we save the previous state of the
 * tick interrupt on some save place, to restore it later on.
 * Of course, we must keep the other bits in the TIMSK, so when changing the bit, global interrupts
 * must be switched off.
 */


#if (defNonSwitchingCallPresent == cfgTrue) && (cfgIntGlobalOnly == cfgFalse) && (cfgIntOsProtected == cfgFalse)

void portEnterTickInterrupts(void)
{ /* Since these calls are not nested, we can simply switch off / switch on the tick interrupts. Only if
   * we make use of tick tracking we must store the previous value and restore it upon return. */
  asm volatile (
   "in   r25, __SREG__      \n\t" /* Copy the status register as quickly as possible and disable interrupts, even if it was cleared  */
   "cli                     \n\t" /* already. This is needed for a tick may intervene and alter other bits of the TIMSK register.    */
   LoadTIMSK(r24)                 /* Load the TIMSK register, holding the state of the tick interrupt on bit OCIE0A,                 */
  #if (cfgIntTickTrack == cfgTrue)      /* If we make use of tick tracking we must store the value of the tick interrupt somewhere   */
   #if (cfgSysSqueezeState == cfgTrue)  /* At squeezing, we want spare our registers, so we utilize the T bit for storage, as with   */
   "bst r24, %[_OCIE0A_]    \n\t" /* the global interrupts, the T bit is not used, neither is it combined with the global enter      */
   #else                          /* Thus, copy the value of the tick interrupt bit into the T bit. In case we do not want to use    */
   "sbrc r24, %[_OCIE0A_]   \n\t" /* the status register at all, we use our devAuxSysRegBit to store the value. This is always       */
   "sbi %[_ASR_],%[_ARB_]   \n\t" /* cleared upon entry, so we only need to set it when the tick interrupt is set.                   */
   #endif
  #endif
   "andi r24, %[_nOCIE0Ab_] \n\t" /* Reset the timer0 interrupt enable bit                                                           */
   StoreTIMSK(r24)                /* Rewrite the TIMSK register, so interrupts are disabled after this instruction                   */
   "out  __SREG__, r25      \n\t" /* Restore the state of the global interrupts.                                                     */
   "ret                     \n\t" /* Done.                                                                                           */
   "" ::
   [_ARB_]       "i" (devAuxSysRegBit),
   [_ASR_]       "i" (_SFR_IO_ADDR(devAuxSysReg)),
   [_TIMSKio_]   "i" (_SFR_IO_ADDR(devTIMSK)),
   [_TIMSKmem_]  "i" (_SFR_MEM_ADDR(devTIMSK)),
   [_OCIE0A_]    "i" (devOCIE),
   [_nOCIE0Ab_]  "i" (preBitClr1(0xFF,devOCIE)):
   "r24","r25" ); }

#endif


#if (defNonSwitchingCallPresent == cfgTrue) && (cfgIntGlobalOnly == cfgFalse) && (cfgIntOsProtected == cfgFalse)

void portExitTickInterrupts(void)
{ /* Since these calls are not nested, we can simply switch off / switch on the tick interrupts. At entrance the tick interrupts
   * are switched of, so if we make use of tick tracking, and the restore state is switched off interrupts, there is
   * nothing to do */
  asm volatile (
  #if (cfgIntTickTrack == cfgTrue)      /* If we make use of tick tracking we must restore the value of the tick interrupt         */
   #if (cfgSysSqueezeState == cfgTrue)  /* At squeezing, it was stored in the T bit, so we ask if the T bit is set, if not we are  */
   "brtc  1f               \n\t" /*  done since the value of the tick interrupt bit is already zero which it must stay.            */
   #else                         /* Otherwise, the last value was stored in devAuxSysRegBit, and we must test if that bit was set  */
   "sbis %[_ASR_],%[_ARB_] \n\t" /* If not, we are done too, and we do not even need to clean the devAuxSysRegBit bit because it   */
   "rjmp 1f                \n\t" /* is already zero. (Any interrupt that might become after this is not allowed to change that)    */
   "cbi  %[_ASR_],%[_ARB_] \n\t" /* Since we know now that devAuxSysRegBit is set, we must clear it for use after this routine     */
   #endif                        /* And there are, at this point, not interrupts possible that may rely on the fact of             */
  #endif                         /* devAuxSysRegBit being zero [no context switch, no enter global, no switching isr etc]          */
   "in   r25, __SREG__    \n\t" /* Copy the status register as quickly as possible and disable interrupts, even if it was cleared  */
   "cli                   \n\t" /* already. This is needed for an other interrupt may intervene and alter other bits of the TIMSK  */
   LoadTIMSK(r24)               /* Load the TIMSK register, holding the state of the tick interrupt on bit OCIE0A,                 */
   "sbr r24, %[_OCIE0Ab_] \n\t" /* Set the timer0 interrupt enable bit                                                             */
   StoreTIMSK(r24)              /* Rewrite the TIMSK register, so interrupts are enabled after this instruction                    */
   "out  __SREG__, r25    \n\t" /* Restore the state of the global interrupts.                                                     */
   "1:                    \n\t" /* We jump to this point if there is nothing to do.                                                */
   "ret                   \n\t" /* Done.                                                                                           */
   "" ::
   [_ARB_]       "i" (devAuxSysRegBit),
   [_ASR_]       "i" (_SFR_IO_ADDR(devAuxSysReg)),
   [_TIMSKio_]   "i" (_SFR_IO_ADDR(devTIMSK)),
   [_TIMSKmem_]  "i" (_SFR_MEM_ADDR(devTIMSK)),
   [_OCIE0A_]    "i" (devOCIE),
   [_OCIE0Ab_]   "i" (preBitSet1(0x00,devOCIE)):
   "r24","r25" ); }

#endif


#if ((cfgIntGlobalOnly == cfgFalse) && (cfgIntTickTrack == cfgTrue)) || (defCheckReportingError == cfgTrue)

void portDisableTickInterrupts(void)
{ /* This simply disables the tick interrupts. Nothing more. The call is not used by the OS itself, (portEnterTickInterrupts /
   * portExitTickInterrupts is used to that end), with the exception of being called from the error handling. */
  asm volatile (
   "in   r25, __SREG__      \n\t" /* Copy the status register as quickly as possible and disable interrupts, even if it was cleared  */
   "cli                     \n\t" /* already. This is needed for a tick may intervene and alter other bits of the TIMSK register.    */
   LoadTIMSK(r24)                 /* Load the TIMSK register, holding the state of the tick interrupt on bit OCIE0A,                 */
   "andi r24, %[_nOCIE0Ab_] \n\t" /* Reset the timer0 interrupt enable bit                                                           */
   StoreTIMSK(r24)                /* Rewrite the TIMSK register, so interrupts are disabled after this instruction                   */
   "out  __SREG__, r25      \n\t" /* Restore the state of the global interrupts.                                                     */
   "ret                     \n\t" /* Done.                                                                                           */
   "" ::
   [_TIMSKio_]   "i" (_SFR_IO_ADDR(devTIMSK)),
   [_TIMSKmem_]  "i" (_SFR_MEM_ADDR(devTIMSK)),
   [_OCIE0A_]    "i" (devOCIE),
   [_nOCIE0Ab_]  "i" (preBitClr1(0xFF,devOCIE)):
   "r24","r25" ); }

#endif


#if (cfgIntGlobalOnly == cfgFalse) && (cfgIntTickTrack == cfgTrue)

void portEnableTickInterrupts(void)
{ /* This simply enables the tick interrupts. Nothing more. The call is not used by the OS itself, (portEnterTickInterrupts /
   * portExitTickInterrupts is used to that end). */
  asm volatile (
   "in   r25, __SREG__    \n\t" /* Copy the status register as quickly as possible and disable interrupts, even if it was cleared  */
   "cli                   \n\t" /* already. This is needed for an other interrupt may intervene and alter other bits of the TIMSK  */
   LoadTIMSK(r24)               /* Load the TIMSK register, holding the state of the tick interrupt on bit OCIE0A,                 */
   "sbr r24, %[_OCIE0Ab_] \n\t" /* Set the timer0 interrupt enable bit                                                             */
   StoreTIMSK(r24)              /* Rewrite the TIMSK register, so interrupts are enabled after this instruction                    */
   "out  __SREG__, r25    \n\t" /* Restore the state of the global interrupts.                                                     */
   "ret                   \n\t" /* Done.                                                                                           */
   "" ::
   [_TIMSKio_]  "i" (_SFR_IO_ADDR(devTIMSK)),
   [_TIMSKmem_] "i" (_SFR_MEM_ADDR(devTIMSK)),
   [_OCIE0A_]   "i" (devOCIE),
   [_OCIE0Ab_]  "i" (preBitSet1(0x00,devOCIE)):
   "r24","r25" ); }

#endif


#if (cfgSysClearUnusedR1 == cfgTrue)
  /* Checking on r1 is special since gcc always assumes the register to be zero. */
  #define CheckRegisterR1 "cpse r1,r30 \n\t" /* Thus we compare not to r2 but to r31 which contains zero at the place this is inserted */
  #define ClearRegisterR1 "clr  r1     \n\t" /* If we make use of the gcc we are save by always clearing r1                            */
  #define FillRegisterR1  ""                 /* The register is not filled with r2 which contains some magic byte to check with.       */
#else
  /* Not using gcc r1 is just like other registers */
  #define CheckRegisterR1 "cpse r1,r29 \n\t" /* r1 is now compared with the magic check byte just like the other registers             */
  #define ClearRegisterR1 ""                 /* r1 is not cleared.                                                                     */
  #define FillRegisterR1  "mov  r1,r2  \n\t" /* r1 is now filled with the magic check byte just like the other registers               */
#endif


/* The return value is placed in r24, the standard register used by gcc. */
#if (cfgUseSynchronization != cfgSyncNon) && (defUseBoolReturns == cfgTrue)
  #define ReturnState(arg) \
   "lds  r16," #arg "           \n\t" /* Now load the return value in r16, which is still free for use               */\
   "sbrs r16,%[_RET1_]          \n\t" /* If the bit 1 is set, we don't copy the return value              ...        */\
   "mov  r24, r16               \n\t" /* load the result.                                                            */
  /* This contains the value that must be returned (false, non-false) */
  #define ReturnField           &xOS.pxSave.uiReturn
#else
  #define ReturnState(arg)      ""
  #define ReturnField           0
#endif


/* If we protect the OS from interruptions, we allow for call to the operating system
 * from isr. We must have some mechanism to detect that we are calling from an isr.
 * Thus we replace the uiOSstatus by a value which indicates that we are in an
 * isr, but we must restore the original status as we leave the isr. Note that we
 * may replace since we may never call functions from within the OS which need the
 * other bits. Note, this code may not corrupt the status register. The uiOSstatus is
 * temporarily saved in an unused background variable, and (thus) we may never call this
 * when we want to execute an isr in OS space with an OS stack.
 */
#if (cfgIntOsProtected == cfgTrue)
  /* Replace the uiOSstatus with the key that indicates we are in the isr context */
  #define SaveOsStatus(stat,save,key)  \
   "lds  r31," #stat "   \n\t" /* Load the uiOSstatus                                                                            */\
   "sts  " #save ", r31  \n\t" /* Save that value in r31 background variable.                                                    */\
   "ldi  r31," #key "    \n\t" /* Load the defContextStateIsr key                                                                */\
   "sts  " #stat ", r31  \n\t" /* Store that key in the uiOSstatus. Destruction of other bits is allowed.                        */
  /* Repair the uiOSstatus */
  #define RestoreOsStatus(stat,save)   \
   "lds  r31," #save "   \n\t" /* Recall the original value of the uiOSstatus                                                    */\
   "sts  " #stat ", r31  \n\t" /* Restore that Status                                                                            */
#else
  #define SaveOsStatus(stat,save,key)   ""
  #define RestoreOsStatus(stat,save)    ""
#endif


#if (cfgIntUserDefined == cfgTrue) && (cfgIntOsProtected == cfgTrue) && (includeIsrEnter == cfgTrue) &&  (cfgIntSwitchUsesOSstack == cfgFalse)

void portEnterISR(void)
/* Enters the isr with the isrStack, thus performs a stack switch and an update of the uiOSstatus. The return address is
 * remembered so we will return to the code implementing the isr when the stack switch is over. Since we replace the
 * stack pointer, global interrupts must be switched off. (Note that defContextStateIsr must be 0xb11000000.)
 * Note that for up-growing stacks this code is quite different, and not implemented yet
 */
{ asm volatile (
   "pop  r24                 \n\t" /* Get the first byte of the return address, which is on top of the current stack.                */
   "sts  %[_ISR1_], r24      \n\t" /* Place it on the new isr stack in the correct position.                                         */
   "pop  r24                 \n\t" /* Get the second byte of the return address, which is now on top of the current stack.           */
   "sts  %[_ISR2_], r24      \n\t" /* Place it on the new isr stack in the correct position.                                         */
  #if (defThreeByteAddress == cfgTrue)
   "pop  r24                 \n\t" /* Get the third byte of the return address, which is now on top of the current stack.            */
   "sts  %[_ISR3_], r24      \n\t" /* Place it on the new isr stack in the correct position.                                         */
  #endif
   "lds  r24,%[_Status_]     \n\t" /* Load the uiOSstatus, now indicating we are running something else.                             */
   "ori  r24,%[_ConIsr_]     \n\t" /* Place the key bits in place (do not overwrite the other bits, we do not save the uiOSstatus!)  */
   "sts  %[_Status_], r24    \n\t" /* Replace the uiOSstatus register with bits adapted.                                             */
   "ldi  r24,lo8(%[_ISRst_]) \n\t" /* Now load the low byte of the new (isr) stack                                                   */
   "out  __SP_L__,r24        \n\t" /* Replace the stack pointer                                                                      */
  #if (defStackSpaceExceedsOneByte == cfgTrue)
   "ldi  r24,hi8(%[_ISRst_]) \n\t" /* Now load the high byte of the new (isr) stack                                                  */
   "out  __SP_H__,r24        \n\t" /* Replace the stack pointer                                                                      */
  #endif
   "ret                      \n\t" /* Done, return to the isr with a brand new stack.                                                */
   "" ::
   [_ISR1_]    "i" (&StackISR[ISRstackFirstByte]),
   [_ISR2_]    "i" (&StackISR[ISRstackSecondByte]),
   [_ISR3_]    "i" (&StackISR[ISRstackThirdByte]),
   [_ISRst_]   "i" (&StackISR[ISRstackStart]),
   [_ConIsr_]  "i" (defContextStateIsr),
   [_Status_]  "i" (&uiOsStatus) )
   ; }

#endif


#if (cfgIntUserDefined == cfgTrue) && (cfgIntOsProtected == cfgTrue) && (includeIsrEnter == cfgTrue) &&  (cfgIntSwitchUsesOSstack == cfgTrue)

void portEnterISR(void)
/* Enters the isr with the OsStack. Since this is performed after a context switch we are already in
 * Os Space. so no stack transplant needed. What remains is changing the OsStatus. (Note that defContextStateIsr
 * must be 0xb11000000.)
 */
{ asm volatile (
   "lds  r24,%[_Status_]     \n\t" /* Load the uiOSstatus, now indicating we are running something else.                             */
   "ori  r24,%[_ConIsr_]     \n\t" /* Place the key bits in place (do not overwrite the other bits, we do not save the uiOSstatus!)  */
   "sts  %[_Status_], r24    \n\t" /* Replace the uiOSstatus register with bits adapted.                                             */
   "ret                      \n\t" /* Done, return to the isr with a brand new stack.                                                */
   "" ::
   [_ConIsr_]  "i" (defContextStateIsr),
   [_Status_]  "i" (&uiOsStatus) )
   ; }

#endif



#if (defUseIsrStack == cfgTrue) && (includeIsrBegin == cfgTrue)

void portBeginISR(void)
/* Call this if you want to start an isr directly, thus without a context switch first. It may even interrupt
 * the OS. The stack must be switched but the registers and the status must not be effected. It is up to the
 * user to save these.  Of course, this routine may only be called with interrupts switched off. */
{ asm volatile (
   "sts  %[_ISRlst_], r31    \n\t" /* Save the current value of register r31 (Note, not in the background r31, this is not free!)    */
   "pop  r31                 \n\t" /* Get the first byte of the return address, which is on top of the current stack.                */
   "sts  %[_ISR1_], r31      \n\t" /* Place it on the new isr stack in the correct position.                                         */
   "pop  r31                 \n\t" /* Get the second byte of the return address, which is now on top of the current stack.           */
   "sts  %[_ISR2_], r31      \n\t" /* Place it on the new isr stack in the correct position.                                         */
  #if (defThreeByteAddress == cfgTrue)
   "pop  r31                 \n\t" /* Get the third byte of the return address, which is now on top of the current stack.            */
   "sts  %[_ISR3_], r31      \n\t" /* Place it on the new isr stack in the correct position.                                         */
  #endif
   "in   r31,__SP_L__        \n\t" /* The current stack pointer must be rescued, its value is put below(!) the return address of the */
   "sts  %[_ISRspl_], r31    \n\t" /* isr stack. (Normally these values are not used for anything else)                              */
  #if (defStackSpaceExceedsOneByte == cfgTrue)
   "in   r31,__SP_H__        \n\t" /* Get the high byte of the current stack pointer                                                 */
   "sts  %[_ISRsph_], r31    \n\t" /* Save it.                                                                                       */
  #endif
   "ldi  r31,lo8(%[_ISRst_]) \n\t" /* Now make the stack switch, Load the pointer at which we want to start (above the return        */
   "out  __SP_L__,r31        \n\t" /* address) and store it in the low and high byte respectively.                                   */
  #if (defStackSpaceExceedsOneByte == cfgTrue)
   "ldi  r31,hi8(%[_ISRst_]) \n\t" /* Get the high byte of the new                                                                   */
   "out  __SP_H__,r31        \n\t" /* Save it.                                                                                       */
  #endif
    SaveOsStatus(%[_Status_],%[_s31_],%[_ConISR_])  /* If we have OS protection, we may perform genXXX calls, inform the system      */
   "lds  r31,%[_ISRlst_]     \n\t" /* Restore the value of r31                                                                       */
   "ret                      \n\t" /* Return with the registers and stack intact                                                     */
   "" ::
   [_ISRspl_]  "i" (&StackISR[ISRstackSPL]),
   [_ISRsph_]  "i" (&StackISR[ISRstackSPH]),
   [_ISR1_]    "i" (&StackISR[ISRstackFirstByte]),
   [_ISR2_]    "i" (&StackISR[ISRstackSecondByte]),
   [_ISR3_]    "i" (&StackISR[ISRstackThirdByte]),
   [_ISRlst_]  "i" (&StackISR[ISRstackLastByte]),
   [_s31_]     "i" (&xOS.pxSave.r31),
   [_ISRst_]   "i" (&StackISR[ISRstackStart]),
   [_ConISR_]  "i" (defContextStateIsr),
   [_Status_]  "i" (&uiOsStatus) )
   ; }

#endif


#if (defUseIsrStack == cfgTrue) && (includeIsrEndReturn == cfgTrue)

void portReturnISR(void)
/* Call this to end you isr. Do not call reti from your isr, the return is performed by this method. It restores the
 * stack and, if needed the OS stack. You are responsible for the registers and the status. Interrupts must (still)
 * be disabled when calling this routine. */
{ asm volatile (
   "sts  %[_ISRlst_], r31            \n\t" /* Save the current value of register r31 (Note, not in the background r31, this is not free!)    */
    RestoreOsStatus(%[_Status_],%[_s31_])  /* If we have OS protection, we have replaced the uiOSstatus, which must be repaired              */
   "lds  r31,%[_ISRspl_]             \n\t" /* Load the old low byte stack pointer                                                            */
   "out  __SP_L__,r31                \n\t" /* Restore it                                                                                     */
  #if (defStackSpaceExceedsOneByte == cfgTrue)
   "lds  r31,%[_ISRsph_]             \n\t" /* Load the old high byte stack pointer                                                           */
   "out  __SP_H__,r31                \n\t" /* Restore it                                                                                     */
  #endif
   "lds  r31,%[_ISRlst_]             \n\t" /* Restore the value of r31                                                                       */
   "reti                             \n\t" /* Return to the interrupted code with interrupts activated.                                      */
   "" ::
   [_ISRspl_]  "i" (&StackISR[ISRstackSPL]),
   [_ISRsph_]  "i" (&StackISR[ISRstackSPH]),
   [_s31_]     "i" (&xOS.pxSave.r31),
   [_ISRlst_]  "i" (&StackISR[ISRstackLastByte]),
   [_Status_]  "i" (&uiOsStatus) )
   ; }

#endif


#if (defUseIsrStack == cfgTrue) && (cfgIntOsProtected == cfgTrue) && (includeIsrEndYield == cfgTrue) && (includeTaskYield == cfgTrue)

void portYieldISR(void)
/* Identical functionality as portReturnISR but does not return to the original point of interrupt, but directly
 * performs a context switch, possibly to activate a resumed task. Of course this functionality is only available
 * when a task is interrupted thus when we have OS protection on. */
{ asm volatile (
   "sts  %[_ISRlst_], r31            \n\t" /* Save the current value of register r31 (Note, not in the background r31, this is not free!)    */
    RestoreOsStatus(%[_Status_],%[_s31_])  /* If we have OS protection, we have replaced the uiOSstatus, which must be repaired              */
   "lds  r31,%[_ISRspl_]             \n\t" /* Load the old low byte stack pointer                                                            */
   "out  __SP_L__,r31                \n\t" /* Restore it                                                                                     */
  #if (defStackSpaceExceedsOneByte == cfgTrue)
   "lds  r31,%[_ISRsph_]             \n\t" /* Load the old high byte stack pointer                                                           */
   "out  __SP_H__,r31                \n\t" /* Restore it                                                                                     */
  #endif
   "lds  r31,%[_ISRlst_]             \n\t" /* Restore the value of r31                                                                       */
   "" ::
   [_ISRspl_]  "i" (&StackISR[ISRstackSPL]),
   [_ISRsph_]  "i" (&StackISR[ISRstackSPH]),
   [_s31_]     "i" (&xOS.pxSave.r31),
   [_ISRlst_]  "i" (&StackISR[ISRstackLastByte]),
   [_Status_]  "i" (&uiOsStatus) );
  /* Don't return but directly perform a context switch, use a portJump because the jump may
   * not be possible with the rjmp. */
   portJump(taskYield); }

#endif


/* DISCUSSION
 * Below you will find the portSave and portRestore context. The code in these routines is build on the settings
 * of the configuration parameters of the different tasks. First, we distinguish to main blocks, of which only
 * one is compiled depending on the setting of cfgCheckRegisters, cfgCheckTaskStack, cfgCheckWatermarks. If one of
 * these is cfgTrue, checks are performed on the use of the registers or stack, i.e. this is protected context switching.
 * For every group of registers it is
 * decided separately if check or save/restore actions are needed. In general this is the approach:
 * (1) Registers that are used in all tasks are always saved/restored, and no code is added to test if
 *     these actions should be skipped, since there is no need.
 * (2) Registers that are used in some tasks are conditionally saved/restored.
 * (3) Registers that are used in none of the tasks are completely left out of save/restore.
 * The same kind of scheme holds with respect to register use checking, since this may be specified separately,
 * but now for filling with the check byte and checking against the check byte.
 * Of course this code is all AVR specific.
 */

/* Save registers r28r29r30r31 if they are used, in any of the tasks. */
#if ((defRegisterUseCollect & r28r29r30r31) == r28r29r30r31)
  /* This define is for protected context switching. */
  #define SaveGroup7b \
   "lds  r28,%[_r28_]    \n\t" /* Load original value of register r28     */ \
   "push r28             \n\t" /* Push it on the right place on the stack */ \
   "lds  r28,%[_r29_]    \n\t" /* Load original value of register r29     */ \
   "push r28             \n\t" /* Push it on the right place on the stack */ \
   "lds  r28,%[_r30_]    \n\t" /* Load original value of register r30     */ \
   "push r28             \n\t" /* Push it on the right place on the stack */ \
   "lds  r28,%[_r31_]    \n\t" /* Load original value of register r31     */ \
   "push r28             \n\t" /* Push it on the right place on the stack */

  /* This define is for standard context switching. */
  #define SaveGroup7a \
   "lds  r31,%[_r31_]    \n\t" \
   "push r28             \n\t" \
   "push r29             \n\t" \
   "push r30             \n\t" \
   "push r31             \n\t"
#else
  #define SaveGroup7a  ""
  #define SaveGroup7b  ""
#endif

/* Save registers r24r25r26r27 if they are used, in any of the tasks. */
#if ((defRegisterUseCollect & r24r25r26r27) == r24r25r26r27)
  #define SaveGroup6 \
   "push r24             \n\t" \
   "push r25             \n\t" \
   "push r26             \n\t" \
   "push r27             \n\t"
#else
  #define SaveGroup6  ""
#endif

/* Save registers r20r21r22r23 if they are used, in any of the tasks. */
#if ((defRegisterUseCollect & r20r21r22r23) == r20r21r22r23)
  #define SaveGroup5 \
   "push r20             \n\t" \
   "push r21             \n\t" \
   "push r22             \n\t" \
   "push r23             \n\t"
#else
  #define SaveGroup5  ""
#endif

/* Save registers r16r17r18r19 if they are used, in any of the tasks. */
#if ((defRegisterUseCollect & r16r17r18r19) == r16r17r18r19)
  #define SaveGroup4 \
   "push r16             \n\t" \
   "push r17             \n\t" \
   "push r18             \n\t" \
   "push r19             \n\t"
#else
  #define SaveGroup4  ""
#endif

/* Save registers r12r13r14r15 if they are used, in any of the tasks. */
#if ((defRegisterUseCollect & r12r13r14r15) == r12r13r14r15)
  #define SaveGroup3 \
   "push r12             \n\t" \
   "push r13             \n\t" \
   "push r14             \n\t" \
   "push r15             \n\t"
#else
  #define SaveGroup3  ""
#endif

/* Save registers r08r09r10r11 if they are used, in any of the tasks. */
#if ((defRegisterUseCollect & r08r09r10r11) == r08r09r10r11)
  #define SaveGroup2 \
   "push r8              \n\t" \
   "push r9              \n\t" \
   "push r10             \n\t" \
   "push r11             \n\t"
#else
  #define SaveGroup2  ""
#endif

/* Save registers r04r05r06r07 if they are used, in any of the tasks. */
#if ((defRegisterUseCollect & r04r05r06r07) == r04r05r06r07)
  #define SaveGroup1 \
   "push r4              \n\t" \
   "push r5              \n\t" \
   "push r6              \n\t" \
   "push r7              \n\t"
#else
  #define SaveGroup1  ""
#endif

/* Save registers r00r01r02r03 if they are used, in any of the tasks. */
#if ((defRegisterUseCollect & r00r01r02r03) == r00r01r02r03)
  #define SaveGroup0 \
   "push r0              \n\t" \
   "push r1              \n\t" \
   "push r2              \n\t" \
   "push r3              \n\t"
#else
  #define SaveGroup0  ""
#endif


/* Test if the registergroup r28r29r30r31 must be saved for the current task */
#if ((defRegisterUseVariable & r28r29r30r31) == r28r29r30r31)
  #define TestSaveGroup7 \
   "sbrs r31, 7          \n\t" \
   "rjmp 7f              \n\t"
#else
  #define TestSaveGroup7  ""
#endif

/* Test if the registergroup r24r25r26r27 must be saved for the current task */
#if ((defRegisterUseVariable & r24r25r26r27) == r24r25r26r27)
  #define TestSaveGroup6 \
   "sbrs r31, 6          \n\t" \
   "rjmp 6f              \n\t"
#else
  #define TestSaveGroup6  ""
#endif

/* Test if the registergroup r00r01r02r03 must be saved for the current task */
#if ((defRegisterUseVariable & r20r21r22r23) == r20r21r22r23)
  #define TestSaveGroup5 \
   "sbrs r31, 5          \n\t" \
   "rjmp 5f              \n\t"
#else
  #define TestSaveGroup5  ""
#endif

/* Test if the registergroup r16r17r18r19 must be saved for the current task */
#if ((defRegisterUseVariable & r16r17r18r19) == r16r17r18r19)
  #define TestSaveGroup4 \
   "sbrs r31, 4          \n\t" \
   "rjmp 4f              \n\t"
#else
  #define TestSaveGroup4  ""
#endif

/* Test if the registergroup r00r01r02r03 must be saved for the current task */
#if ((defRegisterUseVariable & r12r13r14r15) == r12r13r14r15)
  #define TestSaveGroup3 \
   "sbrs r31, 3          \n\t" \
   "rjmp 3f              \n\t"
#else
  #define TestSaveGroup3  ""
#endif

/* Test if the registergroup r08r09r10r11 must be saved for the current task */
#if ((defRegisterUseVariable & r08r09r10r11) == r08r09r10r11)
  #define TestSaveGroup2 \
   "sbrs r31, 2          \n\t" \
   "rjmp 2f              \n\t"
#else
  #define TestSaveGroup2  ""
#endif

/* Test if the registergroup r04r05r06r07 must be saved for the current task */
#if ((defRegisterUseVariable & r04r05r06r07) == r04r05r06r07)
  #define TestSaveGroup1 \
   "sbrs r31, 1          \n\t" \
   "rjmp 1f              \n\t"
#else
  #define TestSaveGroup1  ""
#endif

/* Test if the registergroup r00r01r02r03 must be saved for the current task */
#if ((defRegisterUseVariable & r00r01r02r03) == r00r01r02r03)
  #define TestSaveGroup0 \
   "sbrs r31, 0          \n\t" \
   "rjmp 0f              \n\t"
#else
  #define TestSaveGroup0  ""
#endif


/* Check the content of registergroup r28r29r30r31. */
#if ((defRegisterCheckCollect & r28r29r30r31) == r28r29r30r31)
  #define CheckGroup7 \
   "lds  r28,%[_r28_]    \n\t" \
   "cpse r28,r29         \n\t" \
   "sbr  r30,0x80        \n\t" \
   "lds  r28,%[_r29_]    \n\t" \
   "cpse r28,r29         \n\t" \
   "sbr  r30,0x80        \n\t" \
   "lds  r28,%[_r30_]    \n\t" \
   "cpse r28,r29         \n\t" \
   "sbr  r30,0x80        \n\t" \
   "lds  r28,%[_r31_]    \n\t" \
   "cpse r28,r29         \n\t" \
   "sbr  r30,0x80        \n\t"
#else
  #define CheckGroup7  ""
#endif

/* Check the content of registergroup r24r25r26r27. */
#if ((defRegisterCheckCollect & r24r25r26r27) == r24r25r26r27)
  #define CheckGroup6 \
   "cpse r24,r29         \n\t" \
   "sbr  r30,0x40        \n\t" \
   "cpse r25,r29         \n\t" \
   "sbr  r30,0x40        \n\t" \
   "cpse r26,r29         \n\t" \
   "sbr  r30,0x40        \n\t" \
   "cpse r27,r29         \n\t" \
   "sbr  r30,0x40        \n\t"
#else
  #define CheckGroup6  ""
#endif

/* Check the content of registergroup r20r21r22r23. */
#if ((defRegisterCheckCollect & r20r21r22r23) == r20r21r22r23)
  #define CheckGroup5 \
   "cpse r20,r29         \n\t" \
   "sbr  r30,0x20        \n\t" \
   "cpse r21,r29         \n\t" \
   "sbr  r30,0x20        \n\t" \
   "cpse r22,r29         \n\t" \
   "sbr  r30,0x20        \n\t" \
   "cpse r23,r29         \n\t" \
   "sbr  r30,0x20        \n\t"
#else
  #define CheckGroup5  ""
#endif

/* Check the content of registergroup r16r17r18r19. */
#if ((defRegisterCheckCollect & r16r17r18r19) == r16r17r18r19)
  #define CheckGroup4 \
   "cpse r16,r29         \n\t" \
   "sbr  r30,0x10        \n\t" \
   "cpse r17,r29         \n\t" \
   "sbr  r30,0x10        \n\t" \
   "cpse r18,r29         \n\t" \
   "sbr  r30,0x10        \n\t" \
   "cpse r19,r29         \n\t" \
   "sbr  r30,0x10        \n\t"
#else
  #define CheckGroup4  ""
#endif

/* Check the content of registergroup r12r13r14r15. */
#if ((defRegisterCheckCollect & r12r13r14r15) == r12r13r14r15)
  #define CheckGroup3 \
   "cpse r12,r29         \n\t" \
   "sbr  r30,0x08        \n\t" \
   "cpse r13,r29         \n\t" \
   "sbr  r30,0x08        \n\t" \
   "cpse r14,r29         \n\t" \
   "sbr  r30,0x08        \n\t" \
   "cpse r15,r29         \n\t" \
   "sbr  r30,0x08        \n\t"
#else
  #define CheckGroup3  ""
#endif

/* Check the content of registergroup r08r09r10r11. */
#if ((defRegisterCheckCollect & r08r09r10r11) == r08r09r10r11)
  #define CheckGroup2 \
   "cpse r8,r29          \n\t" \
   "sbr  r30,0x04        \n\t" \
   "cpse r9,r29          \n\t" \
   "sbr  r30,0x04        \n\t" \
   "cpse r10,r29         \n\t" \
   "sbr  r30,0x04        \n\t" \
   "cpse r11,r29         \n\t" \
   "sbr  r30,0x04        \n\t"
#else
  #define CheckGroup2  ""
#endif

/* Check the content of registergroup r04r05r06r07. */
#if ((defRegisterCheckCollect & r04r05r06r07) == r04r05r06r07)
  #define CheckGroup1 \
   "cpse r4,r29          \n\t" \
   "sbr  r30,0x02        \n\t" \
   "cpse r5,r29          \n\t" \
   "sbr  r30,0x02        \n\t" \
   "cpse r6,r29          \n\t" \
   "sbr  r30,0x02        \n\t" \
   "cpse r7,r29          \n\t" \
   "sbr  r30,0x02        \n\t"
#else
  #define CheckGroup1  ""
#endif

/* Check the content of registergroup r00r01r02r03. */
#if ((defRegisterCheckCollect & r00r01r02r03) == r00r01r02r03)
  #define CheckGroup0 \
    CheckRegisterR1 \
   "sbr  r30,0x01        \n\t" \
   "cpse r0,r29          \n\t" \
   "sbr  r30,0x01        \n\t" \
   "cpse r2,r29          \n\t" \
   "sbr  r30,0x01        \n\t" \
   "cpse r3,r29          \n\t" \
   "sbr  r30,0x01        \n\t"
#else
  #define CheckGroup0  ""
#endif


void portSaveContext(void)
{ /* PortSaveContext must do what it promises, thus save all used registers for  the current task
   * on the stack, and save the device status register too. Furthermore, it must switch from task
   * stack to OS stack, store if global interrupt was enabled on the moment
   * we left the task, disable the tick interrupt, and if requested, check if the stack will
   * not overflow (it must not save the context if the stack will overflow) and check if
   * no registers are changed which the task promised not to use. And all this must be done while
   * leaving ALL registers unchanged, since they may contain parameters for some OS function that
   * was called. The device status register must be left unchanged, until it is saved, which is
   * at the end of the the routine, i.e. the status is saved last. The latter seems highly unpractical
   * but the reason is that, if we save the status first, we must restore it last, after all registers
   * are restored. At that time we do have any registers to work with left. This can be circumvented
   * off course, but this costs extra code, and the current implementation does not.
   * Note that, since the stack changes in this method global interrupts must be switched off.
   */

#if (cfgCheckRegisters == cfgTrue) || (cfgCheckTaskStack == cfgTrue) || (cfgCheckWatermarks == cfgTrue)

  /* This is the code for protected context saving. */
  asm volatile (
   "sts  %[_r31_], r31       \n\t" /* We need quite some space to work, so we start of by saving the registers r31,r30,r29,r28.      */
   "sts  %[_r30_], r30       \n\t" /* These are placed at a special reserved place on the OS stack, since that space is not used at  */
   "sts  %[_r29_], r29       \n\t" /* the moment.                                                                                    */
   "sts  %[_r28_], r28       \n\t" /*                                                                                                */
   "pop  r31                 \n\t" /* Now we must save the return address. The top address of the stack is the address which called  */
   "sts  %[_OS1_], r31       \n\t" /* portSaveContext, and this is the place where we must return to. Since we change stack at the   */
   "pop  r31                 \n\t" /* end we displace this address to the OS stack. After this, the top address on the current stack */
   "sts  %[_OS2_], r31       \n\t" /* is the address at which the task must be restarted after the portRestoreContext.               */
  #if (defThreeByteAddress == cfgTrue)
   "pop  r31                 \n\t" /* Get the third byte of the return address, which is now on top of the current stack.            */
   "sts  %[_OS3_], r31       \n\t" /* Place it on the OS stack in the correct position.                                              */
  #endif
   "lds  r31,%[_Status_]     \n\t" /* Now we check from which place we are coming. If this is not a task, but the idle task (info    */
   "sbrc r31,%[_ConBit_]     \n\t" /* is in the uiOSstatus) we can skip this, since there is nothing to be saved or checked. Jump in */
   "rjmp 41f                 \n\t" /* that case directly to the place where timer interrupt is disabled and the stack is changed.    */
   "in   r28, __SREG__       \n\t" /* Make a copy of the device status register, cause we must do some calculus below, preserve r28  */
   "in   r30,__SP_L__        \n\t" /* Now we will check if the stack call sustain all registers that must be pushed onto it. If not, */
   "lds  r29,(%[_StLim_])    \n\t" /* we will not even try, thereby rescuing the system from a crash, but we will simply inform the  */
   "sub  r30,r29             \n\t" /* OS that we did not succeed. The OS will stop the current task, so it is not rescheduled.       */
   "ldi  r30,0x00            \n\t" /* Load the StackPointer and the precalculated stack limit, contained in %[_StLim_],and calculate */
  #if (defStackSpaceExceedsOneByte == cfgTrue)  /* difference. We are not interested in the actual value, only the carry. We need r30*/
   "in   r31,__SP_H__        \n\t" /* to be zero below. If we make use of large stacks, We must repeat the subtraction for the       */
   "lds  r29,(%[_StLim_])+1  \n\t" /* high byte. Continue the subtraction with sbc. If we have a small stack, we need not to take    */
   "sbc  r31,r29             \n\t" /*  into account the high byte SP_H (which still may be present though)                           */
  #endif
   "brcc 20f                 \n\t" /* and skip if the stack will fit the space                                                       */
   "sbr  r30,%[_STKBITb_]    \n\t" /* if not, indicate the error in uiStackTCheck (this is why r30 needed to be clean)               */
   "20:                      \n\t"
   "sts  %[_StChk_],r30      \n\t" /* save the result in the uiStackTCheck                                                           */
   "ldi  r31,0x00            \n\t" /* Clear r31 to compare with and to collect the register test if we may continue, only if the     */
   "cpse r30,r31             \n\t" /* value of uiStackTCheck == 0, if not zero (r31!=r30), we do not save the stack (but do calcu-   */
   "rjmp 8f                  \n\t" /* late the new stack level), the task will be terminated, registers saved and not checked.       */
   "21:                      \n\t" /* Now we are ready to save and check the registers. (btw we know r30==0 since r30==r31)          */
   "lds  r31,%[_RegUse_]     \n\t" /* Load the parameter that describes which registers must be saved.                               */
   "ldi  r29,%[_RegByt_]     \n\t" /* Load the parameter that describes which registers must be checked.                             */
    CheckGroup0                    /* Check if register group r00r01r03r03 must be checked (if included), collect result in r30      */
    TestSaveGroup0                 /* Check if register group r00r01r03r03 must be saved (if included), use r31 for that info        */
    SaveGroup0                     /* Save register group r00r01r03r03 (if included).                                                */
   "0:                       \n\t" /* Copy the contents of the status register to a save place, freeing r28 for other use. We can    */
   "mov r1,r28               \n\t" /* use r1 for this purpose, since it is cleared afterwards and never used for parameters.         */
    CheckGroup1                    /* Same as handling of Group0                                                                     */
    TestSaveGroup1                 /* */
    SaveGroup1                     /* */
   "1:                       \n\t" /* */
    CheckGroup2                    /* Same as handling of Group0                                                                     */
    TestSaveGroup2                 /* */
    SaveGroup2                     /* */
   "2:                       \n\t" /* */
    CheckGroup3                    /* Same as handling of Group0                                                                     */
    TestSaveGroup3                 /* */
    SaveGroup3                     /* */
   "3:                       \n\t" /* */
    CheckGroup4                    /* Same as handling of Group0                                                                     */
    TestSaveGroup4                 /* */
    SaveGroup4                     /* */
   "4:                       \n\t" /* */
    CheckGroup5                    /* Same as handling of Group0                                                                     */
    TestSaveGroup5                 /* */
    SaveGroup5                     /* */
   "5:                       \n\t" /* */
    CheckGroup6                    /* Same as handling of Group0                                                                     */
    TestSaveGroup6                 /* */
    SaveGroup6                     /* */
   "6:                       \n\t" /* */
    CheckGroup7                    /* The last instructions are a bit different since we must now check the registers we are using   */
    TestSaveGroup7                 /* */
    SaveGroup7b                    /* */
   "7:                       \n\t" /* All registers are saved, r30 contains the result of the check.                                 */
   "mov   r31, r1            \n\t" /* Now we start storing the status register. Move it to r31 for bit maintenance and storage ...   */
  #if (cfgSysSqueezeState == cfgTrue) /* We must distinguish between several situations. First in case we have a squeezed state      */
   #if (cfgIntTickTrack == cfgTrue)/* If we have tracking of the timer interrupts we must store the actual value. We are ordered to  */
   LoadTIMSK(r29)                  /* squeeze it into the status register. The only bit not used by gcc except for the H flag are I  */
   "bst r29, %[_OCIE0A_]     \n\t" /* and V flag (overflow), thus copy the value of the tick bit unto the I flag (which is not used) */
   "bld r31, %[_I_]          \n\t" /* at the moment loading it in T and replacing it on the correct position.                        */
   #endif                          /* The previous interrupt state is contained in the H bit, and that has already been set.         */
  #else                            /* If we do not squeeze, we have the state of the global interrupt at the moment the context      */
   #if (cfgIntTickTrack == cfgTrue)/* switch stored in devAuxSysRegBit. Now we must see if we must store the tick interrupts as well */
   "sbr  r31,%[_Ib_]         \n\t" /* This is very hard since we lack the space. The trick is to store the mode (preemptive, equals  */
    LoadTIMSK(r29)                 /* both interrupts active as bit set in I, and cooperative as bit zero in I. In the coop mode the */
   "bst r29, %[_OCIE0A_]     \n\t" /* the status register is of no importance, thus may be used to store whatever you like. Thus     */
   "clh                      \n\t" /* first copy the tick interrupt state to the T-flag and the global interrupt state to the H-flag */
   "sbic %[_ASR_],%[_ARB_]   \n\t" /* Then we must determine in what mode we were when we arrived here. If one of the interrupts is  */
   "seh                      \n\t" /* reset we must be in the cooperative mode (tick interrupt not possible, and the secret route    */
   "brtc 50f                 \n\t" /* via an user interrupt and context switching is not possible when TickTrack is active. Other-   */
   "brhs 51f                 \n\t" /* wise we could be (but not need to be) in the preemptive mode. Former, we save both interrupts. */
   "50:                      \n\t" /* We arrive here if one of the interrupts is switched off, thus we store the current manipulated */
   "in r31, __SREG__         \n\t" /* status register where T and H represent the Tick and Global interrupt states, otherwise we     */
   "51:                      \n\t" /* store the original status register with I location bit set, indicating preemptive mode.        */
   #else                           /* If not we only need to store the global interrupt, which can best be done on its place holder  */
   "sbic %[_ASR_],%[_ARB_]   \n\t" /* Get the state of the interrupt at the moment the context switch occurred and set the I bit     */
   "sbr  r31,%[_Ib_]         \n\t" /* to one if the interrupts where set. Per default that bit is zero                               */
   #endif
  #endif
   "push r31                 \n\t" /* The status register is the last register that is pushed (for the reason, see above)            */
   "lds  r28,%[_RegChk_]     \n\t" /* uiRegisterCheck contains the registers we want to check, others may be used but are ignored    */
   "and  r30,r28             \n\t" /* deliberately, thus we calculate an and between the registers found to be used (in r30) and the */
   "sts  %[_RegChk_],r30     \n\t" /* ones we are interested in. Save the result at the same place.                                  */
   "8:                       \n\t" /* We calculate the new stack level below.                                                        */
   "lds  r1,%[_StOff_]       \n\t" /* First load the stack offset, then load the low stack pointer. The difference is the new        */
   "in   r31,__SP_L__        \n\t" /* stack level. This even holds true over an page border. Standard: 0x31-0x21 = 0x10 and over a   */
   "sub  r1,r31              \n\t" /* page border: 0x0101 - 0x00F1 = 0x0010 becomes 0x01 - 0xF1 = 0x10                               */
   "lds  r30,%[_StLev_]      \n\t" /* Load the address of the stack level.                                                           */
   "lds  r31,(%[_StLev_])+1  \n\t" /* in the Z register for indirect storage.                                                        */
   "st   Z,r1                \n\t" /* Store the stack level, directly the right place.                                               */
  #if (defStackSizeExceedsOneByte == cfgTrue)
   "lds  r1,(%[_StOff_])+1   \n\t" /* In case we have two byte stack level, we must also process the high byte. Load the stack high  */
   "in   r31,__SP_H__        \n\t" /* byte of the offset, and the high byte of the stack pointer. The difference is the high byte of */
   "sbc  r1,r31              \n\t" /* the stack level, provided we take the carry of the previous subtraction into account. Reload   */
   "lds  r31,(%[_StLev_])+1  \n\t" /* r31 (r30 is preserved) and take care to store the high byte stack level result at the correct  */
   "std   Z+1,r1             \n\t" /* location, which is one further as the base address. (We may only work with r1,r30,r31 here)    */
  #endif                           /* Btw, if the StackSize exceeds one byte so must the StackSpace do, so we do not test for it     */
   "41:                      \n\t" /* Below we switch of the tick interrupts. We do that here since we may skip the part at label 7. */
    LoadTIMSK(r30)                 /* Load the TIMSK in a register                                                                   */
   "andi  r30, %[_nOCIE0Ab_] \n\t" /* Clear the tick interrupt bit (timer 0)                                                         */
    StoreTIMSK(r30)                /* write the new state to the timer register, disabling tick interrupts                           */
   "ldi  r31,lo8(%[_OSst_])  \n\t" /* Load the low byte of the OS stack.                                                             */
   "out  __SP_L__,r31        \n\t" /* Save it.                                                                                       */
  #if (defStackSpaceExceedsOneByte == cfgTrue)
   "ldi  r31,hi8(%[_OSst_])  \n\t" /* Load the high byte of the OS stack.                                                            */
   "out  __SP_H__,r31        \n\t" /* Save it.                                                                                       */
  #endif
   "lds  r30,%[_r30_]        \n\t" /* Now we restore r30 and r31. Registers r28 and r29 do not need to be restored since if they we  */
   "lds  r31,%[_r31_]        \n\t" /* used by the task they where restored in SaveGroup7b, and if not they where already checked and */
  #if (cfgSysSqueezeState == cfgFalse)
   "cbi %[_ASR_],%[_ARB_]    \n\t" /* Make sure the portSaveContext is left with zero devAuxSysRegBit for interrupt routines that    */
  #endif                           /* may make use of genXXX routines. */
   "clr  r1                  \n\t" /* the contents should not be important any more. Clear r1 since gcc generated code expects it.   */
   "ret                      \n\t" /* Done, return without enabling the global interrupt, that is done inside the OS if needed.      */
   "" ::\
   [_OS1_]      "i" (&xOS.StackOS[OSstackFirstByte]),
   [_OS2_]      "i" (&xOS.StackOS[OSstackSecondByte]),
   [_OS3_]      "i" (&xOS.StackOS[OSstackThirdByte]),
   [_OSst_]     "i" (&xOS.StackOS[OSstackStart]),
   [_r31_]      "i" (&xOS.pxSave.r31),
   [_r30_]      "i" (&xOS.pxSave.r30),
   [_r29_]      "i" (&xOS.pxSave.r29),
   [_r28_]      "i" (&xOS.pxSave.r28),
   [_RegByt_]   "i" (cfgSysRegisterCheckByte),
   [_RegUse_]   "i" (&xOS.pxSave.uiRegisterUse),
   [_RegChk_]   "i" (&xOS.pxSave.uiRegisterCheck),
   [_StOff_]    "i" (&xOS.pxSave.pcStackOffset),
   [_StLev_]    "i" (&xOS.pxSave.pcStackLevel),
   [_StLim_]    "i" (&xOS.pxSave.pcStackLimit),
   [_StChk_]    "i" (&xOS.pxSave.uiStackTCheck),
   [_ConBit_]   "i" (defContextStateSaveBit),
   [_Status_]   "i" (&uiOsStatus),
   [_I_]        "i" (SREG_I),
   [_Ib_]       "i" (preBitSet1(0x00,SREG_I)),
   [_Tb_]       "i" (preBitSet1(0x00,SREG_T)),
   [_STKBIT_]   "i" (defCheckStackBit),
   [_STKBITb_]  "i" (preBitSet1(0x00,defCheckStackBit)),
   [_TIMSKio_]  "i" (_SFR_IO_ADDR(devTIMSK)),
   [_TIMSKmem_] "i" (_SFR_MEM_ADDR(devTIMSK)),
   [_OCIE0A_]   "i" (devOCIE),
   [_nOCIE0Ab_] "i" (preBitClr1(0xFF,devOCIE)),
   [_ARB_]      "i" (devAuxSysRegBit),
   [_ASR_]      "i" (_SFR_IO_ADDR(devAuxSysReg)));

#else

  /* This is the code standard context saving. */
  asm volatile (
   "sts  %[_r31_], r31      \n\t" /* We need some space to work, so we start of by saving the registers r31,r30.                    */
   "sts  %[_r30_], r30      \n\t" /* These are placed at a special reserved place on the OS stack, not used at this moment          */
   "pop  r31                \n\t" /* Now we must save the return address. The top address of the stack is the address which called  */
   "sts  %[_OS1_], r31      \n\t" /* portSaveContext, and this is the place where we must return to. Since we change stack at the   */
   "pop  r31                \n\t" /* end we displace this address to the OS stack. After this, the top address on the current stack */
   "sts  %[_OS2_], r31      \n\t" /* is the address at which the task must be restarted after the portRestoreContext.               */
  #if (defThreeByteAddress == cfgTrue)
   "pop  r31                \n\t" /* Get the third byte of the return address, which is now on top of the current stack.           */
   "sts  %[_OS3_], r31      \n\t" /* Place it on the OS stack in the correct position.                                         */
  #endif
   "lds  r31,%[_Status_]    \n\t" /* Now we check from which place we are coming. If this is not a task, but the idle task (info    */
   "sbrc r31,%[_ConBit_]    \n\t" /* is in the uiOSstatus) we can skip this, since there is nothing to be saved or checked. Jump in */
   "rjmp 41f                \n\t" /* that case directly to the place where timer interrupt is disabled and the stack is changed.    */
   "lds  r31,%[_RegUse_]    \n\t" /* Load the parameter that describes which registers must be saved.                               */
    TestSaveGroup0                /* Check if registergroup r00r01r03r03 must be saved (if included).                               */
    SaveGroup0                    /* Save registergroup r00r01r03r03 (if included).                                                 */
   "0:                      \n\t" /* */
    TestSaveGroup1                /* Same as handling of Group0                                                                     */
    SaveGroup1                    /* */
   "1:                      \n\t" /* */
    TestSaveGroup2                /* Same as handling of Group0                                                                     */
    SaveGroup2                    /* */
   "2:                      \n\t" /* */
    TestSaveGroup3                /* Same as handling of Group0                                                                     */
    SaveGroup3                    /* */
   "3:                      \n\t" /* */
    TestSaveGroup4                /* Same as handling of Group0                                                                     */
    SaveGroup4                    /* */
   "4:                      \n\t" /* */
    TestSaveGroup5                /* Same as handling of Group0                                                                     */
    SaveGroup5                    /* */
   "5:                      \n\t" /* */
    TestSaveGroup6                /* Same as handling of Group0                                                                     */
    SaveGroup6                    /* */
   "6:                      \n\t" /* */
    TestSaveGroup7                /* Same as handling of Group0                                                                     */
    SaveGroup7a                   /* */
   "7:                      \n\t" /* */
   "in   r31, __SREG__      \n\t" /* Until this point the status register must be unchanged, now is the time to save it.            */
  #if (cfgSysSqueezeState == cfgTrue) /* We must distinguish between several situations. First in case we have a squeezed state      */
   #if (cfgIntTickTrack == cfgTrue)/* If we have tracking of the timer interrupts we must store the actual value. We are ordered to  */
   LoadTIMSK(r30)                  /* squeeze it into the status register. The only bit not used by gcc except for the H flag are I  */
   "bst r30, %[_OCIE0A_]     \n\t" /* and V flag (overflow), thus copy the value of the tick bit unto the I flag (which is not used) */
   "bld r31, %[_I_]          \n\t" /* at the moment loading it in T and replacing it on the correct position.                                      */
   #endif                          /* The previous interrupt state is contained in the H bit, and that has already been set.         */
  #else                            /* If we do not squeeze, we have the state of the global interrupt at the moment the context      */
   #if (cfgIntTickTrack == cfgTrue)/* switch stored in devAuxSysRegBit. Now we must see if we must store the tick interrupts as well */
   "sbr  r31,%[_Ib_]         \n\t" /* This is very hard since we lack the space. The trick is to store the mode (preemptive, equals  */
    LoadTIMSK(r30)                 /* both interrupts active as bit set in I, and cooperative as bit zero in I. In the coop mode the */
   "bst r30, %[_OCIE0A_]     \n\t" /* the status register is of no importance, thus may be used to store whatever you like. Thus     */
   "clh                      \n\t" /* first copy the tick interrupt state to the T-flag and the global interrupt state to the H-flag */
   "sbic %[_ASR_],%[_ARB_]   \n\t" /* Then we must determine in what mode we were when we arrived here. If one of the interrupts is  */
   "seh                      \n\t" /* reset we must be in the cooperative mode (tick interrupt not possible, and the secret route    */
   "brtc 50f                 \n\t" /* via an user interrupt and context switching is not possible when TickTrack is active. Other-   */
   "brhs 51f                 \n\t" /* wise we could be (but not need to be) in the preemptive mode. Former, we save both interrupts. */
   "50:                      \n\t" /* We arrive here if one of the interrupts is switched off, thus we store the current manipulated */
   "in r31, __SREG__         \n\t" /* status register where T and H represent the Tick and Global interrupt states, otherwise we     */
   "51:                      \n\t" /* store the original status register with I location bit set, indicating preemptive mode.         */
   #else                           /* If not we only need to store the global interrupt, which can best be done on its place holder  */
   "sbic %[_ASR_],%[_ARB_]   \n\t" /* Get the state of the interrupt at the moment the context switch occurred and set the I bit     */
   "sbr  r31,%[_Ib_]         \n\t" /* to one if the interrupts where set. Per default that bit is zero                               */
   #endif
  #endif
   "push r31                \n\t" /* The status register is the last register that is pushed (for the reason, see above)            */
   "lds  r1,%[_StOff_]      \n\t" /* First load the stack offset, then load the low stack pointer. The difference is the new        */
   "in   r31,__SP_L__       \n\t" /* stack level. This even holds true over an page border. Standard: 0x31-0x21 = 0x10 and over a   */
   "sub  r1,r31             \n\t" /* page border: 0x0101 - 0x00F1 = 0x0010 becomes 0x01 - 0xF1 = 0x10                               */
   "lds  r30,%[_StLev_]     \n\t" /* Load the address of the stack level.                                                           */
   "lds  r31,(%[_StLev_])+1 \n\t" /* in the Z register for indirect storage.                                                        */
   "st   Z,r1               \n\t" /* Store the stack level, directly the right place.                                               */
  #if (defStackSizeExceedsOneByte == cfgTrue)
   "lds  r1,(%[_StOff_])+1  \n\t" /* In case we have two byte stack level, we must also process the high byte. Load the stack high  */
   "in   r31,__SP_H__       \n\t" /* byte of the offset, and the high byte of the stack pointer. The difference is the high byte of */
   "sbc  r1,r31             \n\t" /* the stack level, provided we take the carry of the previous subtraction into account. Reload   */
   "lds  r31,(%[_StLev_])+1 \n\t" /* r31 (r30 is preserved) and take care to store the high byte stack level result at the correct  */
   "std   Z+1,r1            \n\t" /* location, which is one further as the base address. (We may only work with r1,r30,r31 here)    */
  #endif                          /* Btw, if the StackSize exceeds one byte so must the StackSpace do, so we do not test for it     */
   "41:                     \n\t" /* Below we switch off the tick interrupts. We do that here since we may skip the part at label 7.*/
    LoadTIMSK(r30)                /* Load the TIMSK in a register                                                                   */
   "andi  r30, %[_nOCIE0Ab_]\n\t" /* Clear the tick interrupt bit (timer 0)                                                         */
    StoreTIMSK(r30)               /* write the new state to the timer register, disabling tick interrupts                           */
   "ldi  r31,lo8(%[_OSst_]) \n\t" /* Load the low byte of the OS stack.                                                             */
   "out  __SP_L__,r31       \n\t" /* Save it.                                                                                       */
  #if (defStackSpaceExceedsOneByte == cfgTrue)
   "ldi  r31,hi8(%[_OSst_]) \n\t" /* Load the high byte of the OS stack.                                                            */
   "out  __SP_H__,r31       \n\t" /* Save it.                                                                                       */
  #endif
   "lds  r30,%[_r30_]       \n\t" /* Now we restore r30 and r31.                                                                    */
   "lds  r31,%[_r31_]       \n\t" /* */
  #if (cfgSysSqueezeState == cfgFalse)
   "cbi %[_ASR_],%[_ARB_]   \n\t" /* Make sure the portSaveContext is left with zero devAuxSysRegBit for interrupt routines that    */
  #endif                          /* may make use of genXXX routines. */
   "clr  r1                 \n\t" /* Clear r1 since gcc generated code expects it.                                                  */
   "ret                     \n\t" /* Done, return without enabling the global interrupt, that is done inside the OS if needed.      */
   "" ::
   [_OS1_]      "i" (&xOS.StackOS[OSstackFirstByte]),
   [_OS2_]      "i" (&xOS.StackOS[OSstackSecondByte]),
   [_OS3_]      "i" (&xOS.StackOS[OSstackThirdByte]),
   [_OSst_]     "i" (&xOS.StackOS[OSstackStart]),
   [_r31_]      "i" (&xOS.pxSave.r31),
   [_r30_]      "i" (&xOS.pxSave.r30),
   [_RegUse_]   "i" (&xOS.pxSave.uiRegisterUse),
   [_StOff_]    "i" (&xOS.pxSave.pcStackOffset),
   [_StLev_]    "i" (&xOS.pxSave.pcStackLevel),
   [_ConBit_]   "i" (defContextStateSaveBit),
   [_Status_]   "i" (&uiOsStatus),
   [_I_]        "i" (SREG_I),
   [_Ib_]       "i" (preBitSet1(0x00,SREG_I)),
   [_Tb_]       "i" (preBitSet1(0x00,SREG_T)),
   [_Vb_]       "i" (preBitSet1(0x00,SREG_V)),
   [_STKBIT_]   "i" (defCheckStackBit),
   [_STKBITb_]  "i" (preBitSet1(0x00,defCheckStackBit)),
   [_TIMSKio_]  "i" (_SFR_IO_ADDR(devTIMSK)),
   [_TIMSKmem_] "i" (_SFR_MEM_ADDR(devTIMSK)),
   [_OCIE0A_]   "i" (devOCIE),
   [_nOCIE0Ab_] "i" (preBitClr1(0xFF,devOCIE)),
   [_ARB_]      "i" (devAuxSysRegBit),
   [_ASR_]      "i" (_SFR_IO_ADDR(devAuxSysReg)));

#endif
}

/* DISCUSSION
 * One might wonder why there is so much work on the Save Context. Would porting not be
 * a lot simpler if we just saved the registers and the status, and left stack switching
 * and interrupt capture to the OS? Well, yes but there is a problem too. Some of the
 * OS function calls are switching, which means that as the application calls the function
 * a task switch must be performed also. Standard OS implementation perform function handling
 * in task space, issuing a copy of all OS handling on every stack. We use the OS stack so
 * we must switch stack and save the interrupts while leaving ALL registers in unchanged since
 * they might contain parameters of the calling function. GCC is not capable of generating
 * such code, so it must be done by hand.
 */

/* Restore registers r28r29r30r31 if they are used, in any of the tasks. */
#if ((defRegisterUseCollect & r28r29r30r31) == r28r29r30r31)
  #define RestoreGroup7 \
   "pop  r31             \n\t" \
   "pop  r30             \n\t" \
   "pop  r29             \n\t" \
   "pop  r28             \n\t"
#else
  #define RestoreGroup7  ""
#endif

/* Restore registers r24r25r26r27 if they are used, in any of the tasks. */
#if ((defRegisterUseCollect & r24r25r26r27) == r24r25r26r27)
  #define RestoreGroup6 \
   "pop  r27             \n\t" \
   "pop  r26             \n\t" \
   "pop  r25             \n\t" \
   "pop  r24             \n\t"
#else
  #define RestoreGroup6  ""
#endif

/* Restore registers r20r21r22r23 if they are used, in any of the tasks. */
#if ((defRegisterUseCollect & r20r21r22r23) == r20r21r22r23)
  #define RestoreGroup5 \
   "pop  r23             \n\t" \
   "pop  r22             \n\t" \
   "pop  r21             \n\t" \
   "pop  r20             \n\t"
#else
  #define RestoreGroup5  ""
#endif

/* Restore registers r16r17r18r19 if they are used, in any of the tasks. */
#if ((defRegisterUseCollect & r16r17r18r19) == r16r17r18r19)
  #define RestoreGroup4 \
   "pop  r19             \n\t" \
   "pop  r18             \n\t" \
   "pop  r17             \n\t" \
   "pop  r16             \n\t"
#else
  #define RestoreGroup4  ""
#endif

/* Restore registers r12r13r14r15 if they are used, in any of the tasks. */
#if ((defRegisterUseCollect & r12r13r14r15) == r12r13r14r15)
  #define RestoreGroup3 \
   "pop  r15             \n\t" \
   "pop  r14             \n\t" \
   "pop  r13             \n\t" \
   "pop  r12             \n\t"
#else
  #define RestoreGroup3  ""
#endif

/* Restore registers r08r09r10r11 if they are used, in any of the tasks. */
#if ((defRegisterUseCollect & r08r09r10r11) == r08r09r10r11)
  #define RestoreGroup2 \
   "pop  r11             \n\t" \
   "pop  r10             \n\t" \
   "pop  r9              \n\t" \
   "pop  r8              \n\t"
#else
  #define RestoreGroup2  ""
#endif

/* Restore registers r04r05r06r07 if they are used, in any of the tasks. */
#if ((defRegisterUseCollect & r04r05r06r07) == r04r05r06r07)
  #define RestoreGroup1 \
   "pop  r7              \n\t" \
   "pop  r6              \n\t" \
   "pop  r5              \n\t" \
   "pop  r4              \n\t"
#else
  #define RestoreGroup1  ""
#endif

/* Restore registers r00r01r02r03 if they are used, in any of the tasks. */
#if ((defRegisterUseCollect & r00r01r02r03) == r00r01r02r03)
  #define RestoreGroup0 \
   "pop  r3              \n\t" \
   "pop  r2              \n\t" \
   "pop  r1              \n\t" \
   "pop  r0              \n\t"
#else
  #define RestoreGroup0  ""
#endif

/* Test if the registergroup r28r29r30r31 must be restored for the current task */
#if ((defRegisterUseVariable & r28r29r30r31) == r28r29r30r31)
  #define TestRestoreGroup7 \
   "sbrs r0, 7           \n\t" \
   "rjmp 7f              \n\t"
#else
  #define TestRestoreGroup7  ""
#endif

/* Test if the registergroup r24r25r26r27 must be restored for the current task */
#if ((defRegisterUseVariable & r24r25r26r27) == r24r25r26r27)
  #define TestRestoreGroup6 \
   "sbrs r0, 6           \n\t" \
   "rjmp 6f              \n\t"
#else
  #define TestRestoreGroup6  ""
#endif

/* Test if the registergroup r20r21r22r23 must be restored for the current task */
#if ((defRegisterUseVariable & r20r21r22r23) == r20r21r22r23)
  #define TestRestoreGroup5 \
   "sbrs r0, 5           \n\t" \
   "rjmp 5f              \n\t"
#else
  #define TestRestoreGroup5  ""
#endif

/* Test if the registergroup r16r17r18r19 must be restored for the current task */
#if ((defRegisterUseVariable & r16r17r18r19) == r16r17r18r19)
  #define TestRestoreGroup4 \
   "sbrs r0, 4           \n\t" \
   "rjmp 4f              \n\t"
#else
  #define TestRestoreGroup4  ""
#endif

/* Test if the registergroup r12r13r14r15 must be restored for the current task */
#if ((defRegisterUseVariable & r12r13r14r15) == r12r13r14r15)
  #define TestRestoreGroup3 \
   "sbrs r0, 3           \n\t" \
   "rjmp 3f              \n\t"
#else
  #define TestRestoreGroup3  ""
#endif

/* Test if the registergroup r08r09r10r11 must be restored for the current task */
#if ((defRegisterUseVariable & r08r09r10r11) == r08r09r10r11)
  #define TestRestoreGroup2 \
   "sbrs r0, 2           \n\t" \
   "rjmp 2f              \n\t"
#else
  #define TestRestoreGroup2  ""
#endif

/* Test if the registergroup r04r05r06r07 must be restored for the current task */
#if ((defRegisterUseVariable & r04r05r06r07) == r04r05r06r07)
  #define TestRestoreGroup1 \
   "sbrs r0, 1           \n\t" \
   "rjmp 1f              \n\t"
#else
  #define TestRestoreGroup1  ""
#endif

/* Test if the registergroup r00r01r02r03 must be restored for the current task */
#if ((defRegisterUseVariable & r00r01r02r03) == r00r01r02r03)
  #define TestRestoreGroup0 \
   "sbrs r0, 0           \n\t" \
   "rjmp 0f              \n\t"
  #define SkipFillGroup0 \
   "rjmp 10f             \n\t"
#else
  #define TestRestoreGroup0 ""
  #define SkipFillGroup0    ""
#endif

/* Test if the registergroup r00r01r02r03 must be filled for the current task */
#if ((defRegisterUseVariable & r00r01r02r03) == r00r01r02r03)
  #define TestFillGroup0 \
   "sbrc r0, 0           \n\t" \
   "rjmp 0f              \n\t"
  #define SkipRestoreGroup0 \
   "rjmp 10f             \n\t"
#else
  #define TestFillGroup0     ""
  #define SkipRestoreGroup0  ""
#endif

/* Fill registergroup r28r29r30r31 with the check byte */
#if ((defRegisterCheckCollect & r28r29r30r31) == r28r29r30r31)
  #define FillGroup7 \
   "mov  r30,r2          \n\t" \
   "mov  r29,r2          \n\t" \
   "mov  r28,r2          \n\t"
#else
  #define FillGroup7  ""
#endif

/* Fill registergroup r24r25r26r27 with the check byte */
#if ((defRegisterCheckCollect & r24r25r26r27) == r24r25r26r27)
  #define FillGroup6 \
   "mov  r27,r2          \n\t" \
   "mov  r26,r2          \n\t" \
   "mov  r25,r2          \n\t" \
   "mov  r24,r2          \n\t"
#else
  #define FillGroup6  ""
#endif

/* Fill registergroup r20r21r22r23 with the check byte */
#if ((defRegisterCheckCollect & r20r21r22r23) == r20r21r22r23)
  #define FillGroup5 \
   "mov  r23,r2          \n\t" \
   "mov  r22,r2          \n\t" \
   "mov  r21,r2          \n\t" \
   "mov  r20,r2          \n\t"
#else
  #define FillGroup5  ""
#endif

/* Fill registergroup r16r17r18r19 with the check byte */
#if ((defRegisterCheckCollect & r16r17r18r19) == r16r17r18r19)
  #define FillGroup4 \
   "mov  r19,r2          \n\t" \
   "mov  r18,r2          \n\t" \
   "mov  r17,r2          \n\t" \
   "mov  r16,r2          \n\t"
#else
  #define FillGroup4  ""
#endif

/* Fill registergroup r12r13r14r15 with the check byte */
#if ((defRegisterCheckCollect & r12r13r14r15) == r12r13r14r15)
  #define FillGroup3 \
   "mov  r15,r2          \n\t" \
   "mov  r14,r2          \n\t" \
   "mov  r13,r2          \n\t" \
   "mov  r12,r2          \n\t"
#else
  #define FillGroup3  ""
#endif

/* Fill registergroup r20r21r22r23 with the check byte */
#if ((defRegisterCheckCollect & r08r09r10r11) == r08r09r10r11)
  #define FillGroup2 \
   "mov  r11,r2          \n\t" \
   "mov  r10,r2          \n\t" \
   "mov  r9,r2           \n\t" \
   "mov  r8,r2           \n\t"
#else
  #define FillGroup2  ""
#endif

/* Fill registergroup r04r05r06r07 with the check byte */
#if ((defRegisterCheckCollect & r04r05r06r07) == r04r05r06r07)
  #define FillGroup1 \
   "mov  r7,r2           \n\t" \
   "mov  r6,r2           \n\t" \
   "mov  r5,r2           \n\t" \
   "mov  r4,r2           \n\t"
#else
  #define FillGroup1  ""
#endif

/* Fill registergroup r00r01r02r03 with the check byte */
#if ((defRegisterCheckCollect & r00r01r02r03) == r00r01r02r03)
  #define FillGroup0 \
   "mov  r3,r2           \n\t" \
    FillRegisterR1             \
   "mov  r0,r2           \n\t"
#else
  #define FillGroup0        ""
#endif


void portRestoreContext(void)
{

#if (cfgCheckRegisters == cfgTrue) || (cfgCheckTaskStack == cfgTrue) || (cfgCheckWatermarks == cfgTrue)

  /* This is the code for protected context restoring. */
  asm volatile (
    ClearRegisterR1                /* GCC generated code expects r1 to be zero, clear it if the option cfgSysClearUnusedR1 is set   */
    LoadTIMSK(r30)                 /* We must restore the status register as well as set the tick interrupt correctly. Load the     */
   "pop  r31                \n\t"  /* latter into r30 and the first into r31 for manipulation. Then sort out if we squeeze or not.  */
  #if (cfgSysSqueezeState == cfgTrue) /* If we have a squeezed state, the global interrupt was stored at the H bit location. Done   */
   #if (cfgIntTickTrack == cfgTrue)/* If we keep track of tick interrupts, the information was stored at the I bit location.        */
   "bst  r31, %[_I_]         \n\t" /* Copy the value of the I bit location to the the place where the timer interrupt is situated   */
   "bld  r30, %[_OCIE0A_]    \n\t" /* by moving it through the T bit (shortest method).                                             */
   "andi r31,%[_nIb_]        \n\t" /* Don't forget to clear its location, we do not want to activate global interrupts.             */
   #else                           /* If we do not track tick interrupts the tick interrupt is not saved, restore global only       */
   "sbr  r30,%[_OCIE0Ab_]   \n\t"  /* If we do not keep track of tick interrupts we must set the timer interrupt per default        */
   #endif
  #else                            /* If we not squeeze, the information is packed differently, (see save context also).            */
   #if (cfgIntTickTrack == cfgTrue) /* If we keep track of the tick interrupt state, we must sort out if we were in the preemptive  */
   "sbr  r30,%[_OCIE0Ab_]   \n\t"  /* mode or not. If so we need to activate both global and tick interrupts, do so here by default */
   "sbi  %[_ASR_],%[_ARB_]  \n\t"  /* by setting the bit locations on their registers.                                              */
   "sbrc r31,%[_I_]         \n\t"  /* Now look if the preemptive mode was set (I bit location set)                                  */
   "rjmp 52f                \n\t"  /* If so, we are almost done skip the following                                                  */
   "sbrs r31,%[_T_]         \n\t"  /* If not we must reset the tick interrupt if the T-flag was not set, test if it was set         */
   "andi r30,%[_nOCIE0Ab_]  \n\t"  /* if not, reset that bit otherwise just skip, since it was set already.                         */
   "sbrs r31,%[_H_]         \n\t"  /* Also test the H-flag indicating the new state of the global interrupt, test if it was set     */
   "cbi  %[_ASR_],%[_ARB_]  \n\t"  /* if not, reset that bit otherwise just skip, since it was set already.                         */
   "52:                     \n\t"  /* Arrive here when place holders of tick and global interrupt are setup correctly               */
   "andi  r31,%[_nIb_]      \n\t" /* Make sure we do not activate the global interrupt just yet.                                    */
   #else                          /* If we do not track tick interrupts the tick interrupt is not saved, restore global only        */
   "cbi  %[_ASR_],%[_ARB_]  \n\t" /* We want to temporarily save the status of the global interrupt in the devAuxSysRegBit, clear   */
   "sbrc r31,%[_I_]         \n\t" /* that bit first, then check the value of the position of the I bit, if it is set, then          */
   "sbi  %[_ASR_],%[_ARB_]  \n\t" /* set the devAuxSysRegBit, if not we can leave it cleared.                                       */
   "andi r31,%[_nIb_]       \n\t" /* Make sure the I bit is cleared. (we do not want to activate global interrupts here).           */
   "sbr  r30,%[_OCIE0Ab_]   \n\t" /* If we do not keep track of tick interrupts we must set the timer interrupt per default         */
   #endif
  #endif
   "out  __SREG__, r31      \n\t" /* Restore the status register.                                                                  */
    StoreTIMSK(r30)               /* Restore the TIMSK register holding the tick interrupt                                         */
   "lds  r0,%[_RegUse_]     \n\t" /* Load the parameter describing which register are used, in order to restore them.               */
   "ldi  r31,%[_RegByt_]    \n\t" /* Load the byte we use to fill unused register in under to check if they are misused.            */
   "mov  r2,r31             \n\t" /* Since we need to restore higher number register first, copy that value to r2                   */
    FillGroup7                    /* Fill the register group with checkbytes (if used)                                              */
    TestRestoreGroup7             /* Check if the register group must be restored (if used)                                         */
    RestoreGroup7                 /* Restore the register group (if used)                                                           */
   "7:                      \n\t" /* */
    FillGroup6                    /* Same handling as group 7                                                                       */
    TestRestoreGroup6             /* */
    RestoreGroup6                 /* */
   "6:                      \n\t" /* */
    ReturnState(%[_RetPar_])      /* Now r24 is restored we can fill it with a return value when needed, plenty of registers free   */
    FillGroup5                    /* Same handling as group 7                                                                       */
    TestRestoreGroup5             /* */
    RestoreGroup5                 /* */
   "5:                      \n\t" /* */
    FillGroup4                    /* Same handling as group 7                                                                       */
    TestRestoreGroup4             /* */
    RestoreGroup4                 /* */
   "4:                      \n\t" /**/
    FillGroup3                    /* Same handling as group 7                                                                       */
    TestRestoreGroup3             /* */
    RestoreGroup3                 /* */
   "3:                      \n\t" /* */
    FillGroup2                    /* Same handling as group 7                                                                       */
    TestRestoreGroup2             /* */
    RestoreGroup2                 /* */
   "2:                      \n\t" /* */
    FillGroup1                    /* Same handling as group 7                                                                       */
    TestRestoreGroup1             /* */
    RestoreGroup1                 /* */
   "1:                      \n\t" /* */
    TestFillGroup0                /* Since we must test if we need to restore (and this uses register r0, and at the same time fill */
    FillGroup0                    /* r0, we have a problem. Since we cannot fill and overwrite, thus we must skip, if the Test was  */
    SkipRestoreGroup0             /* was present. Otherwise there is nothing to skip.                                               */
   "0:                      \n\t" /* */
    RestoreGroup0                 /* */
   "10:                     \n\t" /* */
  #if (cfgSysSqueezeState == cfgFalse)
   "sbis %[_ASR_],%[_ARB_]  \n\t" /* devAuxSysReg contains a copy of the status register, test for the global interrupt bit         */
   "ret                     \n\t" /* if it is not set return normally (interrupt keeps deactivated) otherwise reti                  */
   "cbi %[_ASR_],%[_ARB_]   \n\t" /* bit ARB of must be zero before a portSaveContext occurs.                                       */
   "reti                    \n\t" /* */
  #else
   "brhs 13f                \n\t" /* The H bit contains the interrupt state, so if it is not set                                    */
   "ret                     \n\t" /* return without interrupts activated, and otherwise                                             */
   "13:                     \n\t" /* */
   "reti                    \n\t" /* set the interrupts while returning.                                                            */
  #endif
   "" ::
   [_RegUse_]   "i" (&xOS.pxSave.uiRegisterUse),
   [_RegByt_]   "i" (cfgSysRegisterCheckByte),
   [_RetPar_]   "i" (ReturnField),
   [_I_]        "i" (SREG_I),
   [_H_]        "i" (SREG_H),
   [_T_]        "i" (SREG_T),
   [_Ib_]       "i" (preBitSet1(0x00,SREG_I)),
   [_Tb_]       "i" (preBitSet1(0x00,SREG_T)),
   [_nITb_]     "i" (preBitClr2(0xFF,SREG_I,SREG_T)),
   [_nIb_]      "i" (preBitClr1(0xFF,SREG_I)),
   [_RET0_]     "i" (defRet0),
   [_RET1_]     "i" (defRet1),
   [_STKBIT_]   "i" (defCheckStackBit),
   [_STKBITb_]  "i" (preBitSet1(0x00,defCheckStackBit)),
   [_TIMSKio_]  "i" (_SFR_IO_ADDR(devTIMSK)),
   [_TIMSKmem_] "i" (_SFR_MEM_ADDR(devTIMSK)),
   [_OCIE0A_]   "i" (devOCIE),
   [_OCIE0Ab_]  "i" (preBitSet1(0x00,devOCIE)),
   [_nOCIE0Ab_] "i" (preBitClr1(0xFF,devOCIE)),
   [_ARB_]      "i" (devAuxSysRegBit),
   [_ASR_]      "i" (_SFR_IO_ADDR(devAuxSysReg)));

#else

  /* This is the code standard context restoring. */
  asm volatile (
    ClearRegisterR1               /* GCC generated code expects r1 to be zero, clear it if the option cfgSysClearUnusedR1 is set    */
    LoadTIMSK(r30)                 /* We must restore the status register as well as set the tick interrupt correctly. Load the     */
   "pop  r31                \n\t"  /* latter into r30 and the first into r31 for manipulation. Then sort out if we squeeze or not.  */
  #if (cfgSysSqueezeState == cfgTrue) /* If we have a squeezed state, the global interrupt was stored at the H bit location. Done   */
   #if (cfgIntTickTrack == cfgTrue)/* If we keep track of tick interrupts, the information was stored at the I bit location.        */
   "bst  r31, %[_I_]         \n\t" /* Copy the value of the I bit location to the the place where the timer interrupt is situated   */
   "bld  r30, %[_OCIE0A_]    \n\t" /* by moving it through the T bit (shortest method).                                             */
   "andi r31,%[_nIb_]        \n\t" /* Don't forget to clear its location, we do not want to activate global interrupts.             */
   #else                           /* If we do not track tick interrupts the tick interrupt is not saved, restore global only       */
   "sbr  r30,%[_OCIE0Ab_]   \n\t"  /* If we do not keep track of tick interrupts we must set the timer interrupt per default        */
   #endif
  #else                            /* If we not squeeze, the information is packed differently, (see save context also).            */
   #if (cfgIntTickTrack == cfgTrue) /* If we keep track of the tick interrupt state, we must sort out if we were in the preemptive  */
   "sbr  r30,%[_OCIE0Ab_]   \n\t"  /* mode or not. If so we need to activate both global and tick interrupts, do so here by default */
   "sbi  %[_ASR_],%[_ARB_]  \n\t"  /* by setting the bit locations on their registers.                                              */
   "sbrc r31,%[_I_]         \n\t"  /* Now look if the preemptive mode was set (I bit location set)                                  */
   "rjmp 52f                \n\t"  /* If so, we are almost done skip the following                                                  */
   "sbrs r31,%[_T_]         \n\t"  /* If not we must reset the tick interrupt if the T-flag was not set, test if it was set         */
   "andi r30,%[_nOCIE0Ab_]  \n\t"  /* if not, reset that bit otherwise just skip, since it was set already.                         */
   "sbrs r31,%[_H_]         \n\t"  /* Also test the H-flag indicating the new state of the global interrupt, test if it was set     */
   "cbi  %[_ASR_],%[_ARB_]  \n\t"  /* if not, reset that bit otherwise just skip, since it was set already.                         */
   "52:                     \n\t"  /* Arrive here when place holders of tick and global interrupt are setup correctly               */
   "andi r31,%[_nIb_]       \n\t" /* Make sure we do not activate the global interrupt just yet.                                   */
   #else                           /* If we do not track tick interrupts the tick interrupt is not saved, restore global only       */
   "cbi  %[_ASR_],%[_ARB_]  \n\t" /* We want to temporarily save the status of the global interrupt in the devAuxSysRegBit, clear   */
   "sbrc r31,%[_I_]         \n\t" /* that bit first, then check the value of the position of the I bit, if it is set, then          */
   "sbi  %[_ASR_],%[_ARB_]  \n\t" /* set the devAuxSysRegBit, if not we can leave it cleared.                                       */
   "andi r31,%[_nIb_]       \n\t" /* Make sure the I bit is cleared. (we do not want to activate global interrupts here).           */
   "sbr  r30,%[_OCIE0Ab_]   \n\t" /* If we do not keep track of tick interrupts we must set the timer interrupt per default        */
   #endif
  #endif
   "out  __SREG__, r31      \n\t" /* Restore the status register.                                                                  */
    StoreTIMSK(r30)               /* Restore the TIMSK register holding the tick interrupt                                         */
   "lds  r0,%[_RegUse_]     \n\t" /* Load the parameter describing which register are used, in order to restore them.               */
    TestRestoreGroup7             /* Check if the registergroup must be restored (if used)                                          */
    RestoreGroup7                 /* Restore the registergroup (if used)                                                            */
   "7:                      \n\t" /* */
    TestRestoreGroup6             /* Same handling as group 7                                                                       */
    RestoreGroup6                 /* */
   "6:                      \n\t" /* */
    ReturnState(%[_RetPar_])         /* Now r24 is restored we can fill it with a return value when needed, plenty of registers free   */
    TestRestoreGroup5             /* Same handling as group 7                                                                       */
    RestoreGroup5                 /* */
   "5:                      \n\t" /* */
    TestRestoreGroup4             /* Same handling as group 7                                                                       */
    RestoreGroup4                 /* */
   "4:                      \n\t" /* */
    TestRestoreGroup3             /* Same handling as group 7                                                                       */
    RestoreGroup3                 /* */
   "3:                      \n\t" /* */
    TestRestoreGroup2             /* Same handling as group 7                                                                       */
    RestoreGroup2                 /* */
   "2:                      \n\t" /* */
    TestRestoreGroup1             /* Same handling as group 7                                                                       */
    RestoreGroup1                 /* */
   "1:                      \n\t" /* */
    TestRestoreGroup0             /* Same handling as group 7                                                                       */
    RestoreGroup0                 /* */
   "0:                      \n\t" /* */
  #if (cfgSysSqueezeState == cfgFalse)
   "sbis %[_ASR_],%[_ARB_]  \n\t" /* devAuxSysReg contains a copy of the status register, test for the global interrupt bit         */
   "ret                     \n\t" /* if it is not set return normally (interrupt keeps deactivated) otherwise reti                  */
   "cbi %[_ASR_],%[_ARB_]   \n\t" /* bit ARB of must be zero before a portSaveContext occurs.                                       */
   "reti                    \n\t" /* */
  #else
   "brhs 13f                \n\t" /* The H bit contains the interrupt state, so if it is not set                                    */
   "ret                     \n\t" /* return without interrupts activated, and otherwise                                             */
   "13:                     \n\t" /* */
   "reti                    \n\t" /* set the interrupts while returning.                                                            */
  #endif
   "" ::
   [_RegUse_]   "i" (&xOS.pxSave.uiRegisterUse),
   [_RetPar_]   "i" (ReturnField),
   [_I_]        "i" (SREG_I),
   [_H_]        "i" (SREG_H),
   [_T_]        "i" (SREG_T),
   [_Ib_]       "i" (preBitSet1(0x00,SREG_I)),
   [_Tb_]       "i" (preBitSet1(0x00,SREG_T)),
   [_nITb_]     "i" (preBitClr2(0xFF,SREG_I,SREG_T)),
   [_nIb_]      "i" (preBitClr1(0xFF,SREG_I)),
   [_RET0_]     "i" (defRet0),
   [_RET1_]     "i" (defRet1),
   [_STKBIT_]   "i" (defCheckStackBit),
   [_STKBITb_]  "i" (preBitSet1(0x00,defCheckStackBit)),
   [_TIMSKio_]  "i" (_SFR_IO_ADDR(devTIMSK)),
   [_TIMSKmem_] "i" (_SFR_MEM_ADDR(devTIMSK)),
   [_OCIE0A_]   "i" (devOCIE),
   [_OCIE0Ab_]  "i" (preBitSet1(0x00,devOCIE)),
   [_nOCIE0Ab_] "i" (preBitClr1(0xFF,devOCIE)),
   [_ARB_]      "i" (devAuxSysRegBit),
   [_ASR_]      "i" (_SFR_IO_ADDR(devAuxSysReg)));

 #endif
}
