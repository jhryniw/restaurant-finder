/*
 *  Created by: James Hryniw
 */

#ifndef __JOY_H__
#define __JOY_H__

#include <Arduino.h>

#define JOY_HORIZ A0
#define JOY_VERT  A1
#define JOY_SEL   2
#define JOY_DEADZONE  5
#define JOY_CENTER 524
#define MAX_CURSOR_SPEED 10

// The joystick direction bitmask
#define DIRECTION_MASK 0x0F
#define UP_MASK        0x01
#define DOWN_MASK      0x02
#define LEFT_MASK      0x04
#define RIGHT_MASK     0x08

/* Define the directions for the bitmask */
enum JoyDirection { UP = 1, DOWN = 2, LEFT = 4, RIGHT = 8, CENTER = 0 };

/* An object describing the joystick state*/
struct joy_state_t {
    // The x and y positions of the joystick, from -512 to +512
    int x_pos;
    int y_pos;
    // True means the button is pressed
    bool button_pressed;
    // A bitmask for the direction, only the last four bits are used
    uint8_t direction;
};

/** Initialize the joystick */
void initJoy();

/** Read the state of the joystick */
joy_state_t readJoy();

/** Move the cursor based on the joy state */
void moveCursor(int& cursor_x, int& cursor_y, joy_state_t joy_state);

#endif
