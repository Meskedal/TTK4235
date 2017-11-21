#include <unistd.h>
#include <sys/time.h>
#include "timer.h"

static  double          timerEndTime;
static  int             timerActive;

static double get_time(void) {
	struct timeval time;
	gettimeofday(&time, NULL);
	return (double)time.tv_sec + (double)time.tv_usec * .000001;
}

void timer_start(double duration) {
	timerEndTime = get_time() + duration;
	timerActive = 1;
}

void timer_stop(void) {
	timerActive = 0;
}

int timer_isTimeOut(void) {
	return timerActive && (get_time() > timerEndTime);
}