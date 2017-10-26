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

 #define TFT_DC 9
 #define TFT_CS 10
 #define SD_CS  6

 Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

 #define DISPLAY_WIDTH      320
 #define DISPLAY_HEIGHT     240
 #define MAP_WIDTH DISPLAY_WIDTH-48
 #define MAP_HEIGHT DISPLAY_HEIGHT
 #define YEG_SIZE          2048
 #define CURSOR_SIZE         10

 lcd_image_t yegImage = { "yeg-big.lcd" , YEG_SIZE, YEG_SIZE };

 int yegX, yegY;
 int cursorX, cursorY;

void drawCursor() {
  tft.fillRect(cursorX, cursorY,
               CURSOR_SIZE, CURSOR_SIZE, ILI9341_RED);
}

 /* Redraw the cursor if it has moved */
 void redrawCursor(joy_state_t joy_state) {

     // i.e the joystick is not in the center (will move)
     if (joy_state.direction) {

         // Draw over the old cursor postition with the map
         lcd_image_draw(&yegImage, &tft,
                        // coordinates in top left
                        yegX + cursorX, yegY + cursorY,
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

 void setup() {
     init();

     Serial.begin(9600);
     tft.begin();

     Serial.print("Initializing SD card...");
     if (!SD.begin(SD_CS)) {
         Serial.print("failed! Is it inserted properly?");
         while(true) {}
     }

     tft.setRotation(3);

     tft.fillScreen(0);

     yegX = YEG_SIZE/2 - (MAP_WIDTH)/2;
     yegY = YEG_SIZE/2 - DISPLAY_WIDTH/2;

     lcd_image_draw(&yegImage, &tft,
                    // coordinates in top left
                    yegX, yegY,
                    // start coordinates on display
                    0, 0,
                    // Display width and height
                    MAP_WIDTH, DISPLAY_WIDTH);

     // Cursor starts in the center of the screen
     cursorX = DISPLAY_WIDTH / 2;
     cursorY = (MAP_WIDTH) / 2;

     drawCursor();
     initJoy();
 }


 // If cursor is moved to edge of screen, moves map in that direction
 // and relocates cursorX or cursorY to a suitable location on screen
 void moveMap() {
   bool moved = true;

   // If cursor is on left
   if  (yegX > 0 && cursorX == 0) {
     yegX -= MAP_WIDTH;
     cursorX = MAP_WIDTH-CURSOR_SIZE*2;
   }
   // Cursor on right
   else if (yegX < YEG_SIZE-MAP_WIDTH && cursorX == MAP_WIDTH-CURSOR_SIZE) {
       yegX += MAP_WIDTH;
       cursorX = CURSOR_SIZE;
   }
   // Cursor on top
   else if (yegY > 0 && cursorY == 0) {
     yegY -= MAP_HEIGHT;
     cursorY = MAP_HEIGHT-CURSOR_SIZE*2;
   }
   // Cursor on bottom
   else if (yegY < YEG_SIZE-MAP_HEIGHT && cursorY == MAP_HEIGHT-CURSOR_SIZE) {
       yegY += MAP_HEIGHT;
       cursorY = CURSOR_SIZE;
   }
   else {
     moved = false;
   }

   // If the edges of overall map is reached,
   // map is redrawn only until the edge
   if (yegX < 0) {
     yegX = 0;
   }
   else if (yegX > YEG_SIZE-MAP_WIDTH) {
     yegX = YEG_SIZE-MAP_WIDTH;
   }
   else if (yegY < 0) {
     yegY = 0;
   }
   else if (yegY > YEG_SIZE-MAP_HEIGHT) {
     yegY = YEG_SIZE-MAP_HEIGHT;
   }

   // Redraws map and updates cursor
   if (moved) {
     lcd_image_draw(&yegImage, &tft,
        yegX, yegY,
        0, 0,
        MAP_WIDTH, MAP_HEIGHT);
    drawCursor();
   }
 }

 int main() {
     setup();

     while(true) {
         // Read the joystick state
         joy_state_t joy_state = readJoy();

         // Redraw the cursor
         redrawCursor(joy_state);

         moveMap();

         delay(5);
     }

     Serial.end();
     return 0;
 }
