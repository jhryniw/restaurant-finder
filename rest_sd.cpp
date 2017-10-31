/*
 * @file rest_sd.cpp
 * @author James Hryniw 1431912
 */

#include "rest_sd.h"

Sd2Card card;
//Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

/** Initializes the SD card */
void initSd() {

    // We assume this has been called
    // tft.begin();

    Serial.print("Initializing SPI communication for raw reads...");
    if (!card.init(SPI_HALF_SPEED, SD_CS)) {
        Serial.println("failed! Is the card inserted properly?");
        while (true) {}
    }
    else {
        Serial.println("OK!");
    }
}

/**
 * Gets a restaurant at a particular index
 * optimized for consecutive calls to the same block of memory
 */
void getRestaurant(int restIndex, restaurant* rest) {
    // store cache as local static variables
    static restaurant restCache[8];
    static uint32_t lastBlock = -1;

    uint32_t blockNum = REST_START_BLOCK + restIndex / 8;

    // Read from the SD card only if it is not in the cache
    if (lastBlock != blockNum) {
        lastBlock = blockNum;

        while (!card.readBlock(blockNum, (uint8_t*) restCache)) {
            Serial.println("Read block failed, trying again");
        }
    }

    *rest = restCache[restIndex % 8];
}

void writeName(const char* name, int index, bool selection) {
    tft.setCursor(0, ((TEXT_SIZE * 7) + 1) * index);

    if (index == selection) {
        tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
        tft.println(name);
    }
    else {
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        tft.println(name);
    }
}

void displayAllRestaurants(restaurant* rest_array, int array_size) {
    tft.setTextSize(TEXT_SIZE);
    tft.setCursor(0, 0);
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    tft.setTextWrap(false);

    for (int i = 0; i < array_size; i++) {
        writeName(rest_array[i].name, i);
    }
}

void changeSelection(int& selection, int new_selection) {

    if (new_selection < 0 || new_selection >= 20) return;

    int temp = selection;
    selection = new_selection;

    //writeName(rests[temp].name, temp, true);
    //writeName(rests[selection].name, selection);
}

void goToListMode() {
    /*tft.fillScreen(ILI9341_BLACK);

    for (int i = 0; i < 20; i++) {
        getRestaurant(i, rests + i);
    }

    displayAllRestaurants(rests, 20);*/
}
