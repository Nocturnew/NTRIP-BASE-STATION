#ifndef TIMER_H
#define TIMER_H

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

typedef struct itimerval TimerVal;
void setTimer(TimerVal *timerVal, int startSec, int startUsec, int sec, int usec);
void sigalrmHandler(int signum);
#endif
