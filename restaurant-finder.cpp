/**
* @file restaurant-finder.cpp
*
* @author James Hryniw 1431912
* @author Songhui Zhang 1499982
*/

#include <Arduino.h>
#include "Adafruit_ILI9341.h"
#include <SPI.h>
#include <SD.h>
#include "lcd_image.h"
#include "joy.h"
#include "rest_sd.h"
#include "restaurant_map.h"

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);        // The display
lcd_image_t yegImage = { "yeg-big.lcd" , YEG_SIZE, YEG_SIZE };  // The map image
RestaurantMap restaurantMap(&tft);                              // The map instance + handler

bool mapState;                                                  // Mode: True if showing map, false if in list mode
int selection = 0;                                              // Selected item in the list

/**
 * Transition from the map mode to the list mode and vice-versa
 */
void changeState() {
    mapState = !mapState;

    // Going into list mode
    if (!mapState) {
        // Get the lat and lon of the cursor position when we pressed the joystick
        int32_t cursor_lon = x_to_lon(restaurantMap.mapX + restaurantMap.cursorX + CURSOR_SIZE/2);
        int32_t cursor_lat = y_to_lat(restaurantMap.mapY + restaurantMap.cursorY + CURSOR_SIZE/2);

        // Reset the selection
        selection = 0;
        goToListMode(cursor_lon, cursor_lat);
    }
    // Going into map mode
    else {
        tft.fillScreen(0);
        // Get selected restaurant
        Restaurant temp_rest;

        // Set the cursor position based on selected restaurant
        getRestaurant(restaurantIndex[selection], & temp_rest);
        int32_t rest_lon = temp_rest.lon;
        int32_t rest_lat = temp_rest.lat;
        restaurantMap.setPosition(rest_lon, rest_lat);
    }
}

void setup() {
   init();

   // Set up serial
   Serial.begin(9600);

   // Set up the display
   tft.begin();
   tft.setRotation(3);
   tft.fillScreen(0);

   // Initialize SD card
   initSD();

   // Initialize map
   mapState = true;
   restaurantMap.init();
   restaurantMap.setMap(&yegImage);
   restaurantMap.refresh();
   restaurantMap.drawCursor();

   // Initialize joystick
   initJoy();
}

int main() {
    setup();

    while(true) {
        // Read the joystick state
        joy_state_t joy_state = readJoy();

        if (joy_state.button_pressed) {
            // Change modes
            changeState();
            delay(100);
        }

        if (mapState) {
            // Redraw the cursor
            restaurantMap.redrawCursor(joy_state);
            restaurantMap.moveMap();

            delay(5);
        }
        else {
            // Move up the list
            if (joy_state.direction & UP_MASK) {
                changeSelection(selection - 1);
            }
            // Move down the list
            else if (joy_state.direction & DOWN_MASK) {
                changeSelection(selection + 1);
            }
            delay(100);
        }
    }

   Serial.end();
   return 0;
}
