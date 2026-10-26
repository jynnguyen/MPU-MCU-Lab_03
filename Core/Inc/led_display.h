/*
 * led_display.h
 *
 *  Created on: Oct 24, 2026
 *      Author: jynng
 */

#ifndef INC_LED_DISPLAY_H_
#define INC_LED_DISPLAY_H_

typedef enum
{
	RED,
	AMBER,
	GREEN,
	TEMP
} LedColor;

enum TrafficState
{
    NS_RED_EW_GREEN,
    NS_RED_EW_AMBER,
    NS_GREEN_EW_RED,
    NS_AMBER_EW_RED
};

void updateTrafficLightPins();
void runTrafficLights();
void toggleLEDs(LedColor ledColor);
void set7Segment(int num);
void scan7Segment();
void updateLedBuffer();
void turnOffAllLeds();
void resetTrafficState();


#endif /* INC_LED_DISPLAY_H_ */
