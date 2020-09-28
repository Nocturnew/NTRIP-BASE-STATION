#include "Timer.h"
sig_atomic_t gotAlarm = 0; // Will be nonzero on reception of SIGALRM


/*
 * Creates timer with given settings.
 * timerVal: TimerVal (struct itimerval) structure
 * startSec: Timer start delay value in seconds
 * startUsec: Timer start delay value in microseconds, in addition to seconds
 * sec: Periodic time interval in seconds
 * usec: Periodic time interval in microseconds, in addition to seconds
 * For more information:
 * Michael Kerrisk - The Linux Programming Interface pg. 526
 */
void setTimer(TimerVal *timerVal, int startSec, int startUsec, int sec, int usec)
{
	struct sigaction sa;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags 	= 0;
	sa.sa_handler 	= sigalrmHandler;

	if(sigaction(SIGALRM, &sa, NULL) == -1)
	{
		perror("Error on creating signal (sigaction).");
	}

	timerVal->it_value.tv_sec 		= startSec;
	timerVal->it_value.tv_usec 		= startUsec;
	timerVal->it_interval.tv_sec 	= sec;
	timerVal->it_interval.tv_usec 	= usec;

	if (setitimer(ITIMER_REAL, timerVal, 0) == -1)
	{
		perror("Error on creating timer (setitimer).");
	}
}

/*
 * SIGALRM signal handler.
 */
void sigalrmHandler(int signum)
{
	gotAlarm = 1;
}
