/**
* @file restaurant-finder.cpp
*
* @author James Hryniw 1431912
* @author Songhui Zhang 1499982
*/

#include <Arduino.h>
#include "Adafruit_ILI9341.h" // Hardware-specific library
#include <SPI.h>
#include <SD.h>
#include "lcd_image.h"
#include "joy.h"
#include "rest_sd.h"
#include "restaurant_map.h"

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
lcd_image_t yegImage = { "yeg-big.lcd" , YEG_SIZE, YEG_SIZE };
RestaurantMap restaurantMap(&tft);

bool mapState;
int selection = 0;

void changeState() {
    Serial.print("Changing state from ");
    mapState = !mapState;

    if (!mapState) {
        Serial.println("map to list.");
        int32_t cursor_lon = x_to_lon(restaurantMap.mapX + restaurantMap.cursorX + CURSOR_SIZE/2);
        int32_t cursor_lat = y_to_lat(restaurantMap.mapY + restaurantMap.cursorY + CURSOR_SIZE/2);
        Serial.print("Lon, Lat: ");
        Serial.print(cursor_lon);
        Serial.print(", ");
        Serial.println(cursor_lat);
        selection = 0;
        goToListMode(cursor_lon, cursor_lat);
    }
    else {
        Serial.println("list to map.");
        tft.fillScreen(0);
        // Get selected restaurant
        Restaurant temp_rest;
        getRestaurant(restaurantIndex[selection], & temp_rest);
        int32_t rest_lon = temp_rest.lon;
        int32_t rest_lat = temp_rest.lat;
        Serial.print("Lon, Lat: ");
        Serial.print(rest_lon);
        Serial.print(", ");
        Serial.println(rest_lat);
        restaurantMap.setPosition(rest_lon, rest_lat);
    }
}

void setup() {
   init();

   Serial.begin(9600);

   tft.begin();
   tft.setRotation(3);
   tft.fillScreen(0);

   initSD();

   mapState = true;
   restaurantMap.init();
   restaurantMap.setMap(&yegImage);
   restaurantMap.refresh();
   restaurantMap.drawCursor();

   initJoy();
}



int main() {
    setup();

    while(true) {
        // Read the joystick state
        joy_state_t joy_state = readJoy();

        if (joy_state.button_pressed) {
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
            if (joy_state.direction & UP_MASK) {
                changeSelection(selection - 1);
            }
            else if (joy_state.direction & DOWN_MASK) {
                changeSelection(selection + 1);
            }
            delay(100);
        }
    }

   Serial.end();
   return 0;
}
