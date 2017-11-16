#include "touch_disp.h"

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

void drawRating(Adafruit_ILI9341* tft, uint8_t rating, bool on) {
    int index = 5 - rating;
    int buttonY = BUTTON_RADIUS + index * BUTTON_DIAMETER;

    uint16_t fillColor = on ? ILI9341_WHITE : ILI9341_BLACK;
    uint16_t textColor = on ? ILI9341_BLACK : ILI9341_WHITE;

    tft->fillCircle(BUTTON_X, buttonY, BUTTON_RADIUS, ILI9341_RED);
    tft->fillCircle(BUTTON_X, buttonY, BUTTON_RADIUS-BORDER_WIDTH, fillColor);

    uint16_t charX = BUTTON_X - (RATING_WIDTH / 2);
    uint16_t charY = buttonY - (RATING_HEIGHT / 2);
    // We add 48 to convert the number to it's ASCII representation
    tft->drawChar(charX, charY, (char)(rating + 48), textColor, fillColor, RATING_SIZE);
}

void initTouch(Adafruit_ILI9341* tft) {
  for (uint8_t rating = 1; rating <= RATING_NUM; rating++) {
    drawRating(tft, rating, true);
  }
}

void setSelectedRating(Adafruit_ILI9341* tft, uint8_t& currRating, int touchX, int touchY) {

    bool ratingChanged = currRating != RATING_NUM - (touchY / (BUTTON_RADIUS * 2));

    currRating = RATING_NUM - (touchY / (BUTTON_RADIUS * 2));
    if (currRating < 1) currRating = 1;

    for (uint8_t rating = 1; rating <= RATING_NUM && ratingChanged; rating++) {
        bool on = rating >= currRating;
        drawRating(tft, rating, on);
    }
}
