/*
 * input_processing.h
 *
 *  Created on: Oct 24, 2026
 *      Author: jynng
 */

#ifndef INC_INPUT_PROCESSING_H_
#define INC_INPUT_PROCESSING_H_
#include "led_display.h"

enum ModeState
{
    MODE_1_NORMAL = 0,
    MODE_2_MODIFY_RED,
    MODE_3_MODIFY_AMBER,
    MODE_4_MODIFY_GREEN,
	MODE_COUNTS
};

void fsm_for_input_processing();
void process_mode_1();
void process_mode_2();
void process_mode_3();
void process_mode_4();

enum ModeState getMode();
int getDuration(LedColor color);
int isDurationValid();

#endif /* INC_INPUT_PROCESSING_H_ */
