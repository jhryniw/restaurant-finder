/*
 *  Created by: James Hryniw
 */

#include "joy.h"

/** Initialize the joystick */
void initJoy() {
    pinMode(JOY_SEL, INPUT_PULLUP);
}

/** Read the state of the joystick */
joy_state_t readJoy() {
    int xVal = analogRead(JOY_HORIZ);
    int yVal = analogRead(JOY_VERT);
    int buttonVal = digitalRead(JOY_SEL);

    // Get the joystick direction
    uint8_t direction = CENTER;

    if (xVal > JOY_CENTER + JOY_DEADZONE ) direction |= LEFT;
    else if (xVal < JOY_CENTER - JOY_DEADZONE ) direction |= RIGHT;

    if (yVal > JOY_CENTER + JOY_DEADZONE ) direction |= DOWN;
    else if (yVal < JOY_CENTER - JOY_DEADZONE ) direction |= UP;

    joy_state_t joy_state;
    joy_state.x_pos = xVal - JOY_CENTER;
    joy_state.y_pos = yVal - JOY_CENTER;
    joy_state.button_pressed = !buttonVal;
    joy_state.direction = direction;

    return joy_state;
}

/** Move the cursor based on the joy state */
void moveCursor(int& cursor_x, int& cursor_y, joy_state_t joy_state) {

    // If the joystick is up or down, map the y value to the cursor speed in the vertical direction
    cursor_y += joy_state.direction & UP_MASK || joy_state.direction & DOWN_MASK ?
            map(joy_state.y_pos, -JOY_CENTER, 1023 - JOY_CENTER, -MAX_CURSOR_SPEED, MAX_CURSOR_SPEED) : 0;

    // If the joystick is left or right, map the x value to the cursor speed in the horizontal direction
    cursor_x += joy_state.direction & LEFT_MASK || joy_state.direction & RIGHT_MASK ?
            map(joy_state.x_pos, -JOY_CENTER, 1023 - JOY_CENTER, MAX_CURSOR_SPEED, -MAX_CURSOR_SPEED) : 0;
}
