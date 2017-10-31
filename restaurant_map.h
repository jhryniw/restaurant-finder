/*
 * @file restaurant_map.h
 */

#ifndef __RESTAURANT_MAP_H__
#define __RESTAURANT_MAP_H__

#include <Arduino.h>
#include <SD.h>
#include "rest_sd.h"
#include "Adafruit_ILI9341.h"
#include "lcd_image.h"
#include "joy.h"

#define DISPLAY_WIDTH      320
#define DISPLAY_HEIGHT     240
#define MAP_WIDTH          DISPLAY_WIDTH-48
#define MAP_HEIGHT         DISPLAY_HEIGHT
#define YEG_SIZE           2048
#define CURSOR_SIZE        10

class RestaurantMap {
public:
    int mapX;
    int mapY;
    int cursorX;
    int cursorY;

    RestaurantMap(Adafruit_ILI9341* tft);

    void init();
    void drawCursor();
    void redrawCursor(joy_state_t joy_state);
    void moveMap();
    void refresh();

    void setMap(lcd_image_t* map_image);

private:
    Adafruit_ILI9341* tft_;
    lcd_image_t* map_image_;
};

#endif
