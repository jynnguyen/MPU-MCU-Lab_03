/*
 * input_reading.c
 *
 *  Created on: Oct 24, 2026
 *      Author: jynng
 */

#include "main.h"
#include "input_reading.h"

// The buffer that the final result is stored after debouncing
static GPIO_PinState buttonBuffer[BUTTON_COUNTS];

// Two buffers for debouncing
static GPIO_PinState debounceButtonBuffer1[BUTTON_COUNTS];
static GPIO_PinState debounceButtonBuffer2[BUTTON_COUNTS];

// Flag for a button pressed, and pressed more than 1 second
static uint8_t flagForButtonPress1s[BUTTON_COUNTS];
static uint8_t flagForButtonPress[BUTTON_COUNTS];

// We define counter for automatically increasing the value
// after the button is pressed more than 1 second
static uint16_t counterForButtonPress1s[BUTTON_COUNTS];

// Array holds 3 buttons' pins
static const uint16_t buttonPins[BUTTON_COUNTS] = {BTN_1_Pin, BTN_2_Pin, BTN_3_Pin};

void button_reading()
{
    for (char i = 0; i < BUTTON_COUNTS; i++)
    {
        debounceButtonBuffer2[i] = debounceButtonBuffer1[i];
        debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(GPIOA, buttonPins[i]);

        if (debounceButtonBuffer1[i] == debounceButtonBuffer2[i])
        {
            buttonBuffer[i] = debounceButtonBuffer1[i];

            if (buttonBuffer[i] == BUTTON_IS_PRESSED)
            {
                if (counterForButtonPress1s[i] == 0)
                	flagForButtonPress[i] = 1;

                if (counterForButtonPress1s[i] < DURATION_FOR_AUTO_INCREASING)
                    counterForButtonPress1s[i]++;
                else
                {
                    flagForButtonPress1s[i] = 1;
                    counterForButtonPress1s[i] = DURATION_FOR_AUTO_INCREASING - SPEED_FOR_INCREASING;
                }
            }
            else
            {
                counterForButtonPress1s[i] = 0;
                flagForButtonPress1s[i] = 0;
                flagForButtonPress[i] = 0;
            }
        }
    }
}

unsigned char is_button_pressed(uint8_t index)
{
	if (index >= BUTTON_COUNTS) return 0;
	if (flagForButtonPress[index] == 1)
	{
		flagForButtonPress[index] = 0;
	    return 1;
	}
	return 0;
}

unsigned char is_button_pressed_1s(unsigned char index)
{
	if (index >= BUTTON_COUNTS) return 0;
	if (flagForButtonPress1s[index] == 1)
	{
	    flagForButtonPress1s[index] = 0;
	    return 1;
	}
	return 0;
}
