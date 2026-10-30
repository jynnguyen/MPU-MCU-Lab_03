/*
 * input_reading.h
 *
 *  Created on: Oct 24, 2026
 *      Author: jynng
 */

#ifndef INC_INPUT_READING_H_
#define INC_INPUT_READING_H_

// Timer interrupt duration is 10ms, so to pass 1 second,
// we need to jump to the interrupt service routine 100 times
#define DURATION_FOR_AUTO_INCREASING 100
#define SPEED_FOR_INCREASING 		 50
#define BUTTON_IS_PRESSED            GPIO_PIN_RESET
#define BUTTON_IS_RELEASED           GPIO_PIN_SET

enum ButtonID
{
	BTN_1 = 0,
	BTN_2,
    BTN_3,
    BUTTON_COUNTS
};

void button_reading();
unsigned char is_button_pressed(unsigned char index);
unsigned char is_button_pressed_1s(unsigned char index);

#endif /* INC_INPUT_READING_H_ */
