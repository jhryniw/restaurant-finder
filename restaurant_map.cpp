/**
* @file restaurant_map.cpp
*
* @author James Hryniw 1431912
* @author Songhui Zhang 1499982
*/

#include "restaurant_map.h"

/** Build restaurant map */
RestaurantMap::RestaurantMap(Adafruit_ILI9341* tft) :
    tft_(tft), mapX(0), mapY(0), cursorX(0), cursorY(0)
{

}

/** Initializing restaurant map */
void RestaurantMap::init() {

    Serial.print("Initializing SD card...");
    if (!SD.begin(SD_CS)) {
        Serial.print("failed! Is it inserted properly?");
        while(true) {}
    }
    else {
        Serial.println("OK!");
    }

    // Map starts in the center of the map
    mapX = YEG_SIZE/2 - MAP_WIDTH/2;
    mapY = YEG_SIZE/2 - DISPLAY_WIDTH/2;

    // Cursor starts in the center of the screen
    cursorX = MAP_WIDTH / 2 - CURSOR_SIZE / 2;
    cursorY = MAP_HEIGHT / 2 - CURSOR_SIZE / 2;
}

/** Draws the cursor at (cursorX, cursorY) */
void RestaurantMap::drawCursor() {
    tft_->fillRect(cursorX, cursorY,
             CURSOR_SIZE, CURSOR_SIZE, ILI9341_RED);
}

/** Draws the cursor at specified (x, y) */
void RestaurantMap::drawCursor(int x, int y) {
    cursorX = x;
    cursorY = y;

    drawCursor();
}

/** Redraw the cursor if it has moved */
void RestaurantMap::redrawCursor(joy_state_t joy_state) {

    // i.e the joystick is not in the center (will move)
    if (joy_state.direction) {

        // Draw over the old cursor postition with the map
        lcd_image_draw(map_image_, tft_,
                        // coordinates in top left
                        mapX + cursorX, mapY + cursorY,
                        // start coordinates on display
                        cursorX, cursorY,
                        CURSOR_SIZE, CURSOR_SIZE);

        // Move the cursor, note that cursorX and cursorY are mutated by this function
        moveCursor(cursorX, cursorY, joy_state);

        // Constrain the cursor position to the screen
        cursorX = constrain(cursorX, 0, MAP_WIDTH-CURSOR_SIZE);
        cursorY = constrain(cursorY, 0, MAP_HEIGHT-CURSOR_SIZE);

        // Draw the cursor at the new position
        drawCursor();
    }
}

/**
 * If cursor is moved to edge of screen, moves map in that direction
 * and relocates cursorX or cursorY to a suitable location on screen
 */
void RestaurantMap::moveMap() {
    bool moved = true;

    // If cursor is on left
    if  (mapX > 0 && cursorX == 0) {
        mapX -= MAP_WIDTH;
        cursorX = MAP_WIDTH-CURSOR_SIZE*2;
    }
    // Cursor on right
    else if (mapX < YEG_SIZE-MAP_WIDTH && cursorX == MAP_WIDTH-CURSOR_SIZE) {
        mapX += MAP_WIDTH;
        cursorX = CURSOR_SIZE;
    }
    // Cursor on top
    else if (mapY > 0 && cursorY == 0) {
        mapY -= MAP_HEIGHT;
        cursorY = MAP_HEIGHT-CURSOR_SIZE*2;
    }
    // Cursor on bottom
    else if (mapY < YEG_SIZE-MAP_HEIGHT && cursorY == MAP_HEIGHT-CURSOR_SIZE) {
        mapY += MAP_HEIGHT;
        cursorY = CURSOR_SIZE;
    }
    else {
        moved = false;
    }

    // If the edges of overall map is reached,
    // map is redrawn only until the edge
    mapX = constrain(mapX, 0, YEG_SIZE-MAP_WIDTH);
    mapY = constrain(mapY, 0, YEG_SIZE-MAP_HEIGHT);

    // Redraws map and updates cursor
    if (moved) {
        refresh();
        drawCursor();
    }
}

/** Set the map image */
void RestaurantMap::setMap(lcd_image_t* map_image) {
    map_image_ = map_image;
}

/** Move the map to center on (lon, lat) */
void RestaurantMap::setPosition(int32_t lon, int32_t lat) {
    int16_t x = lon_to_x(lon)-MAP_WIDTH/2;
    int16_t y = lat_to_y(lat)-MAP_HEIGHT/2;

    // Do not let the map go off the edge of our image
    mapX = constrain(x, 0, YEG_SIZE - MAP_WIDTH);
    mapY = constrain(y, 0, YEG_SIZE - MAP_HEIGHT);

    // If we are off the map in x, apply the difference to the cursor
    if (mapX != x) {
        int16_t dx = x - mapX;
        cursorX = (MAP_WIDTH / 2) + dx;
        // Contrain to the edge of the image
        cursorX = constrain(cursorX, 0, MAP_WIDTH - CURSOR_SIZE);
    }
    else {
        // Otherwise center the cursor
        cursorX = MAP_WIDTH / 2;
    }

    // If we are off the map in y, apply the difference to the cursor
    if (mapY != y) {
        int16_t dy = y - mapY;
        cursorY = (MAP_HEIGHT / 2) + dy;
        // Contrain to the edge of the image
        cursorY = constrain(cursorY, 0, MAP_HEIGHT - CURSOR_SIZE);
    }
    else {
        // Otherwise center the cursor
        cursorY = MAP_HEIGHT / 2;
    }

    // Apply result to the screen
    refresh();
    drawCursor();
}

/** Redraws the map at the current map position */
void RestaurantMap::refresh() {
    lcd_image_draw(map_image_, tft_,
           // coordinates in top left
           mapX, mapY,
           // start coordinates on display
           0, 0,
           // Display width and height
           MAP_WIDTH, DISPLAY_WIDTH);
}

/** Convert from x position to longitude */
int32_t x_to_lon(int16_t x) {
    return map(x, 0, YEG_SIZE, LON_WEST, LON_EAST);
}

/** Convert from y position to latitude */
int32_t y_to_lat(int16_t y) {
    return map(y, 0, YEG_SIZE, LAT_NORTH, LAT_SOUTH);
}

/** Convert from longitude to x position */
int16_t lon_to_x(int32_t lon) {
    return map(lon, LON_WEST, LON_EAST, 0, YEG_SIZE);
}

/** Convert from latitude to y position */
int16_t lat_to_y(int32_t lat) {
    return map(lat, LAT_NORTH, LAT_SOUTH, 0, YEG_SIZE);
}
