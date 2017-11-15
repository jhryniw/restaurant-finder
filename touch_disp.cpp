#include "touch_disp.h"

void initTouch(Adafruit_ILI9341* tft) {

  for (int i = 0; i < 5; i++) {
    tft->fillCircle(296, 24+48*i, 48, ILI9341_WHITE);
    tft->drawCircle(296, 24+48*i, 48, ILI9341_RED);

    tft->setCursor(296, 24+48*i);
    tft->setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    tft->print(5-i);
  }

}

bool isInRadius(int x1, int y1, int x2, int y2, int r) {

  return (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) < r*r;

}

void setSelectedRating(uint8_t* currRating, TSPoint p, Adafruit_ILI9341* tft) {

  for (int i = 0; i < 5; i++) {

    if ( isInRadius(296, 24+48*i, p.x, p.y, 48) ) {
      *currRating = (uint8_t) 5-i;

      for (int j = i; j < 5; j++) {
        tft->fillCircle(296, 24+48*j, 48, ILI9341_BLACK);
        tft->drawCircle(296, 24+48*j, 48, ILI9341_RED);

        tft->setCursor(296, 24+48*j);
        tft->setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft->print(5-j);
      }

      break;
    }

  }

}
