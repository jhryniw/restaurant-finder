#ifndef __TOUCH_DISP_H__
#define __TOUCH_DISP_H__

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <TouchScreen.h>

// These are the four touchscreen analog pins
#define YP A2  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM 5   // can be a digital pin
#define XP 4   // can be a digital pin

// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

#define MINPRESSURE 10
#define MAXPRESSURE 1000

#define SCREEN_WIDTH    320
#define MAP_WIDTH       (SCREEN_WIDTH - 48)
#define BUTTON_RADIUS   24
#define BUTTON_DIAMETER (BUTTON_RADIUS * 2)
#define BUTTON_X        (SCREEN_WIDTH - BUTTON_RADIUS)
#define BORDER_WIDTH    2
#define RATING_NUM      5
#define RATING_SIZE     3
#define RATING_WIDTH    (RATING_SIZE * 5)
#define RATING_HEIGHT   (RATING_SIZE * 7)


extern TouchScreen ts;

void initTouch(Adafruit_ILI9341* tft);
void setSelectedRating(Adafruit_ILI9341* tft, uint8_t& currRating, TSPoint pc);

#endif
