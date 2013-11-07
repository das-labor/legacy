//////////////////////////////////////////////////////////////////////////////
// Timer.cpp
// =========
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

#include "timer.h"
#include <stdlib.h>

///////////////////////////////////////////////////////////////////////////////
// constructor
///////////////////////////////////////////////////////////////////////////////

void Timer_init(mytimer_t *t)
{
#ifdef WIN32
    QueryPerformanceFrequency(&t->frequency);
    t->startCount.QuadPart = 0;
    t->endCount.QuadPart = 0;
#else
    t->startCount.tv_sec = t->startCount.tv_usec = 0;
    t->endCount.tv_sec = t->endCount.tv_usec = 0;
#endif

    t->stopped = 0;
    t->startTimeInMicroSec = 0;
    t->endTimeInMicroSec = 0;
}

///////////////////////////////////////////////////////////////////////////////
// start timer.
// startCount will be set at this point.
///////////////////////////////////////////////////////////////////////////////
void Timer_start(mytimer_t *t)
{
    t->stopped = 0; // reset stop flag
#ifdef WIN32
    QueryPerformanceCounter(&t->startCount);
#else
    gettimeofday(&t->startCount, NULL);
#endif
}

///////////////////////////////////////////////////////////////////////////////
// stop the timer.
// endCount will be set at this point.
///////////////////////////////////////////////////////////////////////////////
void Timer_stop(mytimer_t *t)
{
    t->stopped = 1; // set timer stopped flag

#ifdef WIN32
    QueryPerformanceCounter(&t->endCount);
#else
    gettimeofday(&t->endCount, NULL);
#endif
}

///////////////////////////////////////////////////////////////////////////////
// compute elapsed time in micro-second resolution.
// other getElapsedTime will call this first, then convert to correspond resolution.
///////////////////////////////////////////////////////////////////////////////
double Timer_getElapsedTimeInMicroSec(mytimer_t *t)
{
#ifdef WIN32
    if(!stopped)
        QueryPerformanceCounter(&t->endCount);

    t->startTimeInMicroSec = t->startCount.QuadPart * (1000000.0 / t->frequency.QuadPart);
    t->endTimeInMicroSec = t->endCount.QuadPart * (1000000.0 / t->frequency.QuadPart);
#else
    if(!t->stopped)
        gettimeofday(&t->endCount, NULL);

    t->startTimeInMicroSec = (t->startCount.tv_sec * 1000000.0) + t->startCount.tv_usec;
    t->endTimeInMicroSec = (t->endCount.tv_sec * 1000000.0) + t->endCount.tv_usec;
#endif

    return t->endTimeInMicroSec - t->startTimeInMicroSec;
}

///////////////////////////////////////////////////////////////////////////////
// divide elapsedTimeInMicroSec by 1000
///////////////////////////////////////////////////////////////////////////////
double Timer_getElapsedTimeInMilliSec(mytimer_t *t)
{
    return Timer_getElapsedTimeInMicroSec(t) * 0.001;
}

///////////////////////////////////////////////////////////////////////////////
// divide elapsedTimeInMicroSec by 1000000
///////////////////////////////////////////////////////////////////////////////
double Timer_getElapsedTimeInSec(mytimer_t *t)
{
    return Timer_getElapsedTimeInMicroSec(t) * 0.000001;
}

///////////////////////////////////////////////////////////////////////////////
// same as getElapsedTimeInSec()
///////////////////////////////////////////////////////////////////////////////
double Timer_getElapsedTime(mytimer_t *t)
{
    return Timer_getElapsedTimeInSec(t);
}
