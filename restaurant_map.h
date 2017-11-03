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
#define MAP_WIDTH          (DISPLAY_WIDTH-48)
#define MAP_HEIGHT         DISPLAY_HEIGHT
#define YEG_SIZE           2048
#define CURSOR_SIZE        10

#define LAT_NORTH 5361858l // bigger
#define LAT_SOUTH 5340953l // smaller
#define LON_WEST -11368652l // smaller
#define LON_EAST -11333496l // bigger

class RestaurantMap {
public:
    int16_t mapX;
    int16_t mapY;
    int16_t cursorX;
    int16_t cursorY;

    RestaurantMap(Adafruit_ILI9341* tft);

    void init();
    void drawCursor();
    void drawCursor(int x, int y);
    void redrawCursor(joy_state_t joy_state);
    void moveMap();
    void setPosition(int32_t lon, int32_t lat);
    void refresh();

    void setMap(lcd_image_t* map_image);


private:
    Adafruit_ILI9341* tft_;
    lcd_image_t* map_image_;
};

int32_t x_to_lon(int16_t x);
int32_t y_to_lat(int16_t y);
int16_t lon_to_x(int32_t lon);
int16_t lat_to_y(int32_t lat);


#endif
