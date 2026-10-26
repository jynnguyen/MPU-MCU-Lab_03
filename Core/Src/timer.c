/*
 * timer.c
 *
 *  Created on: Oct 24, 2026
 *      Author: jynng
 */
#include "timer.h"
#include "main.h"
#include "input_reading.h"

static int timer_counters[TIMER_COUNTS] = {0};
static int timer_flags[TIMER_COUNTS] = {0};
static int TIMER_TICK = 10;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
        button_reading();
        timerRun();
    }
}

void setTimer(int index, int duration_ms)
{
    if (index < 0 || index >= TIMER_COUNTS)
    	return;

    timer_counters[index] = duration_ms / TIMER_TICK;
    timer_flags[index] = 0;
}

void timerRun()
{
    for (int i = 0; i < TIMER_COUNTS; i++)
    {
        if (timer_counters[i] > 0)
        {
            timer_counters[i]--;
            if (timer_counters[i] <= 0)
                timer_flags[i] = 1;
        }
    }
}

int isTimerExpired(int index)
{
    if (index >= 0 && index < TIMER_COUNTS)
    {
        if (timer_flags[index] == 1)
        {
            timer_flags[index] = 0;
            return 1;
        }
    }
    return 0;
}

void setTimerTick(int value)
{
	TIMER_TICK = value;
}
