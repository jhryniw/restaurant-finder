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

void setSelectedRating(Adafruit_ILI9341* tft, uint8_t& currRating, TSPoint p) {

    if (p.x >= MAP_WIDTH) {
        uint8_t selected = RATING_NUM - (p.y / (BUTTON_RADIUS * 2));
        if (selected < 1) {
            selected = 1;
        }
    }


  for (int i = 0; i < RATING_NUM; i++) {

    /*if ( isInRadius(BUTTON_X, BUTTON_RADIUS * (2*i + 1), p.x, p.y, BUTTON_RADIUS) ) {
      *currRating = (uint8_t) 5-i;

      for (int j = i; j < 5; j++) {
        tft->fillCircle(296, 24+48*j, BUTTON_RADIUS, ILI9341_BLACK);
        tft->drawCircle(296, 24+48*j, BUTTON_RADIUS, ILI9341_RED);

        tft->setCursor(296, 24+48*j);
        tft->setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft->print(5-j);
      }

      break;
  }*/

  }

}
