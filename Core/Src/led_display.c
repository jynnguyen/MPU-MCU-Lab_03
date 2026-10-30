/*
 * led_display.c
 *
 *  Created on: Oct 24, 2026
 *      Author: jynng
 */

#include "main.h"
#include "led_display.h"
#include "timer.h"
#include "input_processing.h"

int countdown_NS = 3;
int countdown_EW = 2;
enum TrafficState trafficState = NS_RED_EW_GREEN;

void initTrafficLights()
{
	countdown_NS = getDuration(RED);
	countdown_EW = getDuration(GREEN);
}

void updateTrafficLightPins()
{
    switch (trafficState)
    {
        case NS_RED_EW_GREEN:
            HAL_GPIO_WritePin(GPIOA, NS_RED_Pin | EW_GREEN_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOA, NS_AMBER_Pin | NS_GREEN_Pin | EW_RED_Pin | EW_AMBER_Pin, GPIO_PIN_SET);
            break;
        case NS_RED_EW_AMBER:
            HAL_GPIO_WritePin(GPIOA, NS_RED_Pin | EW_AMBER_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOA, NS_AMBER_Pin | NS_GREEN_Pin | EW_RED_Pin | EW_GREEN_Pin, GPIO_PIN_SET);
            break;
        case NS_GREEN_EW_RED:
            HAL_GPIO_WritePin(GPIOA, NS_GREEN_Pin | EW_RED_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOA, NS_RED_Pin | NS_AMBER_Pin | EW_AMBER_Pin | EW_GREEN_Pin, GPIO_PIN_SET);
            break;
        case NS_AMBER_EW_RED:
            HAL_GPIO_WritePin(GPIOA, NS_AMBER_Pin | EW_RED_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOA, NS_RED_Pin | NS_GREEN_Pin | EW_AMBER_Pin | EW_GREEN_Pin, GPIO_PIN_SET);
            break;
    }
}

void runTrafficLights()
{
    if (!isTimerExpired(TIMER_NORMAL))
        return;

    countdown_NS--;
    countdown_EW--;
    switch (trafficState)
       {
           case NS_RED_EW_GREEN:
               if (countdown_EW <= 0)
               {
                   trafficState = NS_RED_EW_AMBER;
                   countdown_EW = getDuration(AMBER);
               }
               break;
           case NS_RED_EW_AMBER:
               if (countdown_EW <= 0)
               {
                   trafficState = NS_GREEN_EW_RED;
                   countdown_NS = getDuration(GREEN);
                   countdown_EW = getDuration(RED);
               }
               break;
           case NS_GREEN_EW_RED:
               if (countdown_NS <= 0)
               {
                   trafficState = NS_AMBER_EW_RED;
                   countdown_NS = getDuration(AMBER);
               }
               break;
           case NS_AMBER_EW_RED:
               if (countdown_NS <= 0)
               {
                   trafficState = NS_RED_EW_GREEN;
                   countdown_NS = getDuration(RED);
                   countdown_EW = getDuration(GREEN);
               }
               break;
       }

    updateTrafficLightPins();
    updateLedBuffer();
    setTimer(TIMER_NORMAL, NORMAL_TIME);
}

void toggleLEDs(LedColor ledColor)
{
	if(!isTimerExpired(TIMER_BLINK))
		return;

	switch(ledColor)
	{
		case RED:
			HAL_GPIO_TogglePin(GPIOA, NS_RED_Pin | EW_RED_Pin);
		break;
		case AMBER:
			HAL_GPIO_TogglePin(GPIOA, NS_AMBER_Pin | EW_AMBER_Pin);
		break;
		case GREEN:
			HAL_GPIO_TogglePin(GPIOA, NS_GREEN_Pin | EW_GREEN_Pin);
		break;
		default:
		break;
	}

	setTimer(TIMER_BLINK, BLINK_TIME);
}

void set7Segment(int num)
{
    static const uint8_t segmentArr[10] = {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x10};
    static const uint16_t segPins[7] ={A_Pin, B_Pin, C_Pin, D_Pin, E_Pin, F_Pin, G_Pin};

    if (num < 0 || num > 9)
    	return;

    for (int i = 0; i < 7; i++)
    {
        HAL_GPIO_WritePin(GPIOB, segPins[i],
            (segmentArr[num] & (1 << i)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
}

int led_buffer[4] = {0, 0, 0, 0};
void updateLedBuffer()
{
	enum ModeState mState = getMode();
	int mode_display = mState + 1;
    if (mState == MODE_1_NORMAL)
    {
        led_buffer[0] = countdown_NS / 10;
        led_buffer[1] = countdown_NS % 10;

        led_buffer[2] = countdown_EW / 10;
        led_buffer[3] = countdown_EW % 10;
    }
    else
    {
        led_buffer[0] = mode_display / 10;
        led_buffer[1] = mode_display % 10;

        led_buffer[2] = getDuration(TEMP) / 10;
        led_buffer[3] = getDuration(TEMP) % 10;
    }
}

void scan7Segment()
{
    static int led_id = 0;
    static const uint16_t enPins[4] = {EN0_Pin, EN1_Pin, EN2_Pin, EN3_Pin};

    if (!isTimerExpired(TIMER_SCAN_7SEG))
        return;

    HAL_GPIO_WritePin(GPIOB, EN0_Pin | EN1_Pin | EN2_Pin | EN3_Pin, GPIO_PIN_SET);
    set7Segment(led_buffer[led_id]);
    HAL_GPIO_WritePin(GPIOB, enPins[led_id], GPIO_PIN_RESET);

    led_id = (led_id + 1) % 4;
    setTimer(TIMER_SCAN_7SEG, SCAN_7SEG_TIME);
}

void turnOffAllLeds()
{
	HAL_GPIO_WritePin(GPIOA, NS_RED_Pin | EW_RED_Pin | NS_AMBER_Pin | EW_AMBER_Pin | NS_GREEN_Pin | EW_GREEN_Pin, GPIO_PIN_SET);
}

void resetTrafficState()
{
	countdown_NS = getDuration(RED);
	countdown_EW = getDuration(GREEN);
	trafficState = NS_RED_EW_GREEN;
}


