/*
 * timer.h
 *
 *  Created on: Oct 24, 2026
 *      Author: jynng
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#define NORMAL_TIME 1000
#define BLINK_TIME 250
#define SCAN_7SEG_TIME 50

enum TimerID
{
	TIMER_NORMAL,
	TIMER_BLINK,
    TIMER_SCAN_7SEG,
    TIMER_COUNTS
};

void setTimer(int index, int duration_ms);
void timerRun();
int isTimerExpired(int index);
void setTimerTick(int value);

#endif /* INC_TIMER_H_ */
