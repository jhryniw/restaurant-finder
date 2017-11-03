/**
* @file restaurant_map.h
*
* @author James Hryniw 1431912
* @author Songhui Zhang 1499982
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

/**
 * Represents the map on the display
 */
class RestaurantMap {
public:
    int16_t mapX;
    int16_t mapY;
    int16_t cursorX;
    int16_t cursorY;

    /** Build restaurant map */
    RestaurantMap(Adafruit_ILI9341* tft);

    /** Initializing restaurant map */
    void init();

    /** Draws the cursor at (cursorX, cursorY) */
    void drawCursor();

    /**
     * Draws the cursor at specified (x, y)
     *
     * @param x x position on screen (pixels)
     * @param y y position on screen (pixels)
     */
    void drawCursor(int x, int y);

    /**
     * Redraw the cursor if it has moved
     * @param joy_state the joystick state
     */
    void redrawCursor(joy_state_t joy_state);

    /**
     * If cursor is moved to edge of screen, moves map in that direction
     * and relocates cursorX or cursorY to a suitable location on screen
     */
    void moveMap();

    /** Set the map image */
    void setMap(lcd_image_t* map_image);

    /**
     * Move the map to center on (lon, lat)
     *
     * @param lon longitude in 100000ths of a degree
     * @param lat latitude in 100000ths of a degree
     */
    void setPosition(int32_t lon, int32_t lat);

    /** Redraws the map at the current map position */
    void refresh();

private:
    Adafruit_ILI9341* tft_;
    lcd_image_t* map_image_;
};

/**
 * Convert from x position to longitude
 *
 * @param x x position on map (pixels)
 * @return longitude (100000ths of a degree)
 */
int32_t x_to_lon(int16_t x);

/**
 * Convert from y position to latitude
 *
 * @param y y position on map (pixels)
 * @return latitude (100000ths of a degree)
 */
int32_t y_to_lat(int16_t y);

/**
 * Convert from longitude to x position
 *
 * @param lon longitude (100000ths of a degree)
 * @return x position on map (pixels)
 */
int16_t lon_to_x(int32_t lon);

/**
 * Convert from latitude to y position
 *
 * @param lat latitude (100000ths of a degree)
 * @return y position on map (pixels)
 */
int16_t lat_to_y(int32_t lat);


#endif
