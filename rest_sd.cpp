/*
 * @file rest_sd.cpp
 * @author James Hryniw 1431912
 */

#include "rest_sd.h"

Sd2Card card;
Restaurant closest[NUM_TO_DISPLAY];
extern

/** Initializes the SD card */
void initSD() {

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

// Swap two restaurants of RestDist struct
void swap_rest(RestDist *ptr_rest1, RestDist *ptr_rest2) {
  RestDist tmp = *ptr_rest1;
  *ptr_rest1 = *ptr_rest2;
  *ptr_rest2 = tmp;
}

// Selection sort
// rest_dist is an array of RestDist, from rest_dist[0] to rest_dist[len-1]
void ssort(RestDist *rest_dist, int len) {
  for (int i = len-1; i >= 1; --i) {
    // Find the index of furthest restaurant
    int max_idx = 0;
    for (int j = 1; j <= i; ++j) {
      if(rest_dist[j].dist > rest_dist[max_idx].dist) {
        max_idx = j;
      }
    }
    // Swap it with the last element of the sub-array
    swap_rest(&rest_dist[i], &rest_dist[max_idx]);
  }
}

/**
 * Gets a restaurant at a particular index
 * optimized for consecutive calls to the same block of memory
 */
void getRestaurant(int restIndex, Restaurant* rest) {
    // store cache as local static variables
    static Restaurant restCache[8];
    static uint32_t lastBlock = REST_START_BLOCK - 1;

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

void writeName(const char* name, int index) {
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

void displayAllRestaurants(Restaurant* rest_array, int array_size) {
    tft.setTextSize(TEXT_SIZE);
    tft.setCursor(0, 0);
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    tft.setTextWrap(false);

    for (int i = 0; i < array_size; i++) {
        writeName(rest_array[i].name, i);
    }
}

void getRestaurantList(int x, int y, Restaurant* restArr) {
	RestDist distances[NUM_RESTAURANTS];

	for (int i = 0; i < NUM_RESTAURANTS; i++) {
		Restaurant tempRest;
		getRestaurant(i, &tempRest);

		distances[i].index = i;
		distances[i].dist = abs(x-tempRest.lon) + abs(y-tempRest.lat);
	}

	ssort(distances, NUM_RESTAURANTS);

	for (int i = 0; i < NUM_TO_DISPLAY; i++) {
		uint16_t index = distances[i].index;
		getRestaurant(index, &restArr[i]);
	}

}

void changeSelection(int new_selection) {

    if (new_selection >= NUM_TO_DISPLAY) {
        new_selection = new_selection % NUM_TO_DISPLAY;
    }
    else if (new_selection < 0) {
        new_selection += NUM_TO_DISPLAY;
    }

    int temp = selection;
    selection = new_selection;

    writeName(closest[temp].name, temp);
    writeName(closest[selection].name, selection);
}

void goToListMode(int x, int y) {
    tft.fillScreen(ILI9341_BLACK);

    getRestaurantList(x, y, closest);
    displayAllRestaurants(closest, NUM_TO_DISPLAY);
}
