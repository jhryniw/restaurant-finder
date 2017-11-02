/*
 * @file rest_sd.cpp
 * @author James Hryniw 1431912
 */

#include "rest_sd.h"

Sd2Card card;
int restaurantIndex[NUM_TO_DISPLAY];

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

void displayAllRestaurants(int indexList[], int array_size) {
    tft.setTextSize(TEXT_SIZE);
    tft.setCursor(0, 0);
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    tft.setTextWrap(false);

    for (int i = 0; i < array_size; i++) {
        Restaurant temp_rest;
        getRestaurant(restaurantIndex[i], &temp_rest);
        writeName(temp_rest.name, i);
    }
}

// void displayAllRestaurants(Restaurant* rest_array, int array_size) {
//     tft.setTextSize(TEXT_SIZE);
//     tft.setCursor(0, 0);
//     tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
//     tft.setTextWrap(false);
//
//     for (int i = 0; i < array_size; i++) {
//         writeName(rest_array[i].name, i);
//     }
// }

void getRestaurantList(int32_t x, int32_t y) {
  uint32_t counter = 0;
  RestDist distances_all[NUM_RESTAURANTS];

	for (int i = 0; i < NUM_RESTAURANTS; i++) {
		Restaurant temp_rest;
		getRestaurant(i, &temp_rest);

		distances_all[i].index = i;
		distances_all[i].dist = abs(x-temp_rest.lon) + abs(y-temp_rest.lat);
	}

	ssort(distances_all, NUM_RESTAURANTS);

	for (int i = 0; i < NUM_TO_DISPLAY; i++) {
    restaurantIndex[i] = distances_all[i].index;
    Restaurant temp_rest;
    getRestaurant(distances_all[i].index, &temp_rest);
    Serial.print(temp_rest.name);
    Serial.print(". Lon/Lat: ");
    Serial.print(temp_rest.lon);
    Serial.print(", ");
    Serial.print(temp_rest.lat);
    Serial.print(". Manhattan dist: ");
    Serial.println(distances_all[i].dist);
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

    Restaurant temp_rest_old, temp_rest_new;

    getRestaurant(restaurantIndex[temp], &temp_rest_old);
    getRestaurant(restaurantIndex[selection], &temp_rest_new);

    writeName(temp_rest_old.name, temp);
    writeName(temp_rest_new.name, selection);
}

void goToListMode(int32_t x, int32_t y) {
    tft.fillScreen(ILI9341_BLACK);

    getRestaurantList(x, y);
    displayAllRestaurants(restaurantIndex, NUM_TO_DISPLAY);
}
