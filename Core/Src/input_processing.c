/*
 * input_processing.c
 *
 *  Created on: Oct 24, 2026
 *      Author: jynng
 */

#include "main.h"
#include "input_processing.h"
#include "input_reading.h"
#include "led_display.h"
#include "timer.h"

int red_duration = 5;
int amber_duration = 2;
int green_duration = 3;
int temp_duration = 0;
enum ModeState modeState = MODE_1_NORMAL;

void fsm_for_input_processing()
{
	int isMode_2_4 = modeState == MODE_2_MODIFY_RED || modeState == MODE_3_MODIFY_AMBER
						|| modeState == MODE_4_MODIFY_GREEN;

	if(isMode_2_4 && (is_button_pressed(BTN_2) || is_button_pressed_1s(BTN_2)))
	{
		temp_duration++;
		if (temp_duration > 99)
			temp_duration = 1;
		updateLedBuffer();
	}

	scan7Segment();

    switch (modeState)
    {
        case MODE_1_NORMAL:
        	process_mode_1();
        break;

        case MODE_2_MODIFY_RED:
        	process_mode_2();
        break;

        case MODE_3_MODIFY_AMBER:
        	process_mode_3();
        break;

        case MODE_4_MODIFY_GREEN:
        	process_mode_4();
        break;
        default:
        break;
    }
}

void process_mode_1()
{
	runTrafficLights();

	if (is_button_pressed(BTN_1))
	{
		modeState = (modeState + 1) % MODE_COUNTS;
	  	temp_duration = red_duration;

	  	turnOffAllLeds();
	  	setTimer(TIMER_BLINK, BLINK_TIME);
	  	updateLedBuffer();
	}
}
void process_mode_2()
{
	toggleLEDs(RED);
	if (is_button_pressed(BTN_1))
	{
		modeState = (modeState + 1) % MODE_COUNTS;
		temp_duration = amber_duration;

		turnOffAllLeds();
		setTimer(TIMER_BLINK, BLINK_TIME);
		updateLedBuffer();
	}

	if (is_button_pressed(BTN_3))
	  red_duration = temp_duration;
}
void process_mode_3()
{
	toggleLEDs(AMBER);
	if (is_button_pressed(BTN_1))
	{
		modeState = (modeState + 1) % MODE_COUNTS;
		temp_duration = green_duration;

		turnOffAllLeds();
		setTimer(TIMER_BLINK, BLINK_TIME);
		updateLedBuffer();
	}

	if (is_button_pressed(BTN_3))
	  amber_duration = temp_duration;
}
void process_mode_4()
{
	toggleLEDs(GREEN);
	if (is_button_pressed(BTN_1))
	{
		modeState = (modeState + 1) % MODE_COUNTS;

		turnOffAllLeds();
		resetTrafficState();
		updateTrafficLightPins();
		setTimer(TIMER_NORMAL, NORMAL_TIME);
		updateLedBuffer();
	}

	if (is_button_pressed(BTN_3))
	  green_duration = temp_duration;
}

enum ModeState getMode()
{
	return modeState;
}

int getDuration(LedColor color)
{
	return (color == RED ? red_duration : color == AMBER ? amber_duration :
			color == GREEN ? green_duration:temp_duration);
}

int isDurationValid()
{
	return red_duration == amber_duration + green_duration;
}


