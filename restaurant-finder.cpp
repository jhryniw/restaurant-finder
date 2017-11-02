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

<<<<<<< HEAD
int yegX, yegY;
int cursorX, cursorY;
bool mapState = true;
=======
bool mapState;
>>>>>>> origin/master
int selection = 0;

void changeState() {
<<<<<<< HEAD
    mapState = !mapState;
    selection = 0;
=======
>>>>>>> origin/master
    Serial.println("Changing state");
    mapState = !mapState;

    if (!mapState) {
        selection = 0;
        goToListMode();
    }
}

void setup() {
   init();

   Serial.begin(9600);

   tft.begin();
   tft.setRotation(3);
   //tft.fillScreen(0);

   initSD();

<<<<<<< HEAD
   yegX = YEG_SIZE/2 - (MAP_WIDTH)/2;
   yegY = YEG_SIZE/2 - DISPLAY_WIDTH/2;

   lcd_image_draw(&yegImage, &tft,
                  // coordinates in top left
                  yegX, yegY,
                  // start coordinates on display
                  0, 0,
                  // Display width and height
                  MAP_WIDTH, MAP_WIDTH);

   // Cursor starts in the center of the screen
   cursorX = MAP_WIDTH / 2;
   cursorY = MAP_WIDTH / 2;
=======
   mapState = true;
   restaurantMap.init();
   restaurantMap.setMap(&yegImage);
   restaurantMap.refresh();
   restaurantMap.drawCursor();
>>>>>>> origin/master

   initJoy();
}

int main() {
    setup();

    while(true) {
        // Read the joystick state
        joy_state_t joy_state = readJoy();

        if (joy_state.button_pressed) {
            changeState();

            if (mapState){
                tft.fillScreen(ILI9341_BLACK);
                lcd_image_draw(&yegImage, &tft,
                               // coordinates in top left
                               yegX, yegY,
                               // start coordinates on display
                               0, 0,
                               // Display width and height
                               MAP_WIDTH, MAP_WIDTH);

                // Cursor starts in the center of the screen
                cursorX = MAP_WIDTH / 2;
                cursorY = MAP_WIDTH / 2;

            }
            else {
                goToListMode();
            }

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
