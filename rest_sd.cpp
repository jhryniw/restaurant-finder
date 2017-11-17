/**
 * @file rest_sd.cpp
 * @author James Hryniw 1431912
 * @author Songhui Zhang 1499982
 */

#include "rest_sd.h"

Sd2Card card;
int restaurantIndex[MAX_REST_IN_LIST];
int topDispRestIndex = 0;
int maxRestIndex = 0;

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

// Perform the pivot procedure for quicksort
int pivot(RestDist* a, int n, int pi) {
    // Flip pivot to end
    swap_rest(a + pi, a + n - 1);

    int lo = 0;
    int hi = n - 2;

    while(hi >= lo) {
        if(a[lo].dist <= a[n-1].dist) {
            lo++;
        }
        else if(a[hi].dist > a[n-1].dist) {
            hi--;
        }
        else {
            swap_rest(a + lo, a + hi);
            // We can increment/decrement
            // both indexes after a swap
            lo++; hi--;
        }
    }

    // Flip pivot into its place
    swap_rest(a + lo, a + n - 1);

    // Return the new pivot
    return lo;
}

// Sort an array of n RestDist elements using Quick Sort
void qsort(RestDist* rest_dist, int len) {
    // if n <= 1 do nothing
    if (len <= 1) return;

    // pivot at len / 2
    int new_pi = pivot(rest_dist, len, len / 2);

    // recursively call quicksort on each side of the new pivot
    qsort(rest_dist, new_pi);
    qsort(rest_dist + new_pi + 1, len - new_pi - 1);
}

// Selection sort
// rest_dist is an array of RestDist, from rest_dist[0] to rest_dist[len-1]
void ssort(RestDist* rest_dist, int len) {
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
 * @param restIndex Index number of restaurant in memory
 * @param rest Pointer to Restaurant struct to change
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

        for(int i = 0; i < 8; i++) {
            restCache[i].rating =
                max(floor((restCache[i].rating + 1)/2), 1);
        }
    }

    *rest = restCache[restIndex % 8];
}

/**
 * Prints name of restaurant
 * Text and background colours change
 * if restaurant is currently selected
 * @param name String to display
 * @param index Index number of restaurant in displayed list
 */
void writeName(const char* name, uint8_t rating, int index) {
    tft.setCursor(0, ((TEXT_SIZE * 7) + 1) * index);
    if (index == selection) {
        tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
    }
    else {
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    }
    tft.print(name);

    tft.setCursor(SCREEN_WIDTH-(TEXT_SIZE * 5 +1), ((TEXT_SIZE * 7) + 1) * index);
    tft.println(rating);
}

/**
 * Sets up screen to display restaurant names
 * Then prints each once
 */
 void displayAllRestaurants() {
     tft.fillScreen(ILI9341_BLACK);
     tft.setTextSize(TEXT_SIZE);
     tft.setCursor(0, 0);
     tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
     tft.setTextWrap(false);

     // Gets restaurant order from global restaurantIndex[]
     // of size 30

     for (int i = topDispRestIndex;
         i < min(topDispRestIndex+NUM_TO_DISPLAY, maxRestIndex); i++) {
        Restaurant temp_rest;
        getRestaurant(restaurantIndex[i], &temp_rest);
        writeName(temp_rest.name, temp_rest.rating, i % NUM_TO_DISPLAY);
     }
 }

/**
 * Finds the closest 30 restaurants from cursor position
 * in Manhattan distance at time of click and updates restaurantIndex[] values.
 * @param x Longitude of cursor
 * @param y Latitude of cursor
 */
 void getRestaurantList(int32_t x, int32_t y, uint8_t sr) {
   // Temporary array to save memory
   RestDist distances_all[NUM_RESTAURANTS];

   int lastIndex = 0;

   // Enters restaurant index and Manhattan distance
   // for each restaurant then sorts it by closest distance.
 	for (int i = 0; i < NUM_RESTAURANTS &&
            lastIndex < MAX_REST_IN_LIST; i++) {
 		Restaurant temp_rest;
 		getRestaurant(i, &temp_rest);

        if (temp_rest.rating >= sr) {
            distances_all[lastIndex].index = i;
     		distances_all[lastIndex].dist = abs(x-temp_rest.lon) + abs(y-temp_rest.lat);
            lastIndex++;
        }
 	}

    maxRestIndex = lastIndex;
    //Serial.println(maxRestIndex);

 	qsort(distances_all, maxRestIndex);

   // Updates global restaurantIndex[] with
   // index values of closest 30 restaurants
 	for (int i = 0; i <= maxRestIndex; i++) {
     restaurantIndex[i] = distances_all[i].index;
 	}

 }

/**
 * Updates value of selection when cursor moves in list mode.
 * Rolls over/under if cursor goes too low/high.
 * Also reprints last/currently selected restaurant names.
 * @param new_selection Value of next selection
 */
void changeSelection(int new_selection) {

    if (new_selection >= NUM_TO_DISPLAY &&
        topDispRestIndex < (maxRestIndex - NUM_TO_DISPLAY)) {

        selection = 0;
        topDispRestIndex += NUM_TO_DISPLAY;
        displayAllRestaurants();
    }
    else if (new_selection < 0 &&
        topDispRestIndex >= NUM_TO_DISPLAY) {

        selection = NUM_TO_DISPLAY-1;
        topDispRestIndex -= NUM_TO_DISPLAY;
        displayAllRestaurants();
    }
    else {
        int displayed = min(NUM_TO_DISPLAY, maxRestIndex - topDispRestIndex);
        new_selection = constrain(new_selection, 0, displayed-1);

        int temp = selection;
        selection = new_selection;

        // Gets last/currently selected restaurants
        Restaurant temp_rest_old, temp_rest_new;
        getRestaurant(restaurantIndex[topDispRestIndex+temp], &temp_rest_old);
        getRestaurant(restaurantIndex[topDispRestIndex+selection], &temp_rest_new);

        writeName(temp_rest_old.name, temp_rest_old.rating, temp);
        writeName(temp_rest_new.name, temp_rest_new.rating, selection);
    }
}

/**
 * Called once when switching from map to list mode.
 * Updates restaurantIndex[] and displays restaurants.
 * @param x Longitude of cursor on click.
 * @param y Latitude of cursor on click.
 */
 void goToListMode(int32_t x, int32_t y, int8_t sr) {
     getRestaurantList(x, y, sr);
     displayAllRestaurants();
 }
