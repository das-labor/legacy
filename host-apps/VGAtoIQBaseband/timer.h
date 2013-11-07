//////////////////////////////////////////////////////////////////////////////
// Timer.h
// =======
// High Resolution Timer.
// This timer is able to measure the elapsed time with 1 micro-second accuracy
// in both Windows, Linux and Unix system 
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2003-01-13
// UPDATED: 2006-01-13
//
// Copyright (c) 2003 Song Ho Ahn
//////////////////////////////////////////////////////////////////////////////

#ifndef TIMER_H_DEF
#define TIMER_H_DEF

#ifdef WIN32   // Windows system specific
#include <windows.h>
#else          // Unix based system specific
#include <sys/time.h>
#endif

typedef struct {
double startTimeInMicroSec;                 // starting time in micro-second
double endTimeInMicroSec;                   // ending time in micro-second
int    stopped;                             // stop flag 
#ifdef WIN32
    LARGE_INTEGER frequency;                    // ticks per second
    LARGE_INTEGER startCount;                   //
    LARGE_INTEGER endCount;                     //
#else
    struct timeval startCount;                         //
    struct timeval endCount;                           //
#endif
}mytimer_t;

void Timer_init(mytimer_t *t);
void Timer_start(mytimer_t *t);
void Timer_stop(mytimer_t *t);
double Timer_getElapsedTimeInMicroSec(mytimer_t *t);
double Timer_getElapsedTimeInMilliSec(mytimer_t *t);
double Timer_getElapsedTimeInSec(mytimer_t *t);
double Timer_getElapsedTime(mytimer_t *t);

#endif // TIMER_H_DEF
