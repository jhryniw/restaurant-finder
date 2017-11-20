#include "touch_disp.h"

// Initializes the touchscreen
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// Draws one rating circle on the screen based on if it's on or off
void drawRating(Adafruit_ILI9341* tft, uint8_t rating, bool on) {
    int index = RATING_NUM - rating;
    int buttonY = BUTTON_RADIUS + index * BUTTON_DIAMETER;

    // Fill is selected based on if the button is on or off
    uint16_t fillColor = on ? ILI9341_WHITE : ILI9341_BLACK;
    uint16_t textColor = on ? ILI9341_BLACK : ILI9341_WHITE;

    // Two-layered circle for fill and border
    tft->fillCircle(BUTTON_X, buttonY, BUTTON_RADIUS, ILI9341_RED);
    tft->fillCircle(BUTTON_X, buttonY, BUTTON_RADIUS-BORDER_WIDTH, fillColor);

    uint16_t charX = BUTTON_X - (RATING_WIDTH / 2);
    uint16_t charY = buttonY - (RATING_HEIGHT / 2);

    // We add 48 to convert the number to it's ASCII representation
    tft->drawChar(charX, charY, (char)(rating + 48), textColor, fillColor, RATING_SIZE);
}

/**
 * Draws all ratings to the screen based on the selected minimum rating
 * Ratings are "on" if they are more or equal to the minimum rating
 * @param tft pointer for the display
 * @param currRating the current minimum rating
 */
void drawAllRatings(Adafruit_ILI9341* tft, uint8_t currRating) {
  for (uint8_t rating = 1; rating <= RATING_NUM; rating++) {
      bool on = rating >= currRating;
      drawRating(tft, rating, on);
  }
}

/**
 * Sets the minimum rating based on the location of a touch on the touchscreen
 * @param tft pointer for the display
 * @param currRating the minimum rating (returned)
 * @param touchX the x coordinate of the touch
 * @param touchY the y coordinate of the touch
 */
void setSelectedRating(Adafruit_ILI9341* tft, uint8_t& currRating, int touchX, int touchY) {

    uint8_t selectedRating = RATING_NUM - (touchY / (BUTTON_RADIUS * 2));
    if (selectedRating < 1) selectedRating = 1;

    bool ratingChanged = currRating != selectedRating;

    if (ratingChanged) {
        currRating = selectedRating;
        drawAllRatings(tft, currRating);
    }
}
