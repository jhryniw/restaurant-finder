#include "restaurant_map.h"

RestaurantMap::RestaurantMap(Adafruit_ILI9341* tft) :
    tft_(tft), mapX(0), mapY(0), cursorX(0), cursorY(0)
{
    Serial.print("Initializing SD card...");
    if (!SD.begin(SD_CS)) {
        Serial.print("failed! Is it inserted properly?");
        while(true) {}
    }
}

void RestaurantMap::init() {
    // Map starts in the center of the map
    mapX = 10; //YEG_SIZE/2 - (MAP_WIDTH)/2;
    mapY = 10; //YEG_SIZE/2 - DISPLAY_WIDTH/2;

    // Cursor starts in the center of the screen
    cursorX = (MAP_WIDTH) / 2;
    cursorY = MAP_HEIGHT / 2;
}

void RestaurantMap::drawCursor() {
    Serial.print("Printing cursor at ");
    Serial.print(cursorX);
    Serial.print(", ");
    Serial.print(cursorY);

    tft_->fillRect(cursorX, cursorY,
             CURSOR_SIZE, CURSOR_SIZE, ILI9341_RED);
}

/* Redraw the cursor if it has moved */
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

// If cursor is moved to edge of screen, moves map in that direction
// and relocates cursorX or cursorY to a suitable location on screen
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
    if (mapX < 0) {
        mapX = 0;
    }
    else if (mapX > YEG_SIZE-MAP_WIDTH) {
        mapX = YEG_SIZE-MAP_WIDTH;
    }
    else if (mapY < 0) {
        mapY = 0;
    }
    else if (mapY > YEG_SIZE-MAP_HEIGHT) {
        mapY = YEG_SIZE-MAP_HEIGHT;
    }

    // Redraws map and updates cursor
    if (moved) {
        refresh();
        drawCursor();
    }
}

void RestaurantMap::setMap(lcd_image_t* map_image) {
    map_image_ = map_image;
}

void RestaurantMap::refresh() {
    lcd_image_draw(map_image_, tft_,
           // coordinates in top left
           mapX, mapY,
           // start coordinates on display
           0, 0,
           // Display width and height
           MAP_WIDTH, DISPLAY_WIDTH);
}
