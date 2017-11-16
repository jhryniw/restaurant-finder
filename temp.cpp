// Since I cannot compile the program,
// this file contains the code to be copy-pasted

/*
Sd2Card card;
int restaurantIndex[NUM_RESTAURANTS];
int topDispRestIndex = 0;


void displayAllRestaurants(uint8_t sr) {
    tft.setTextSize(TEXT_SIZE);
    tft.setCursor(0, 0);
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    tft.setTextWrap(false);

    // Gets restaurant order from global restaurantIndex[]
    // of size 30
    int topRest = topDispRestIndex;
    int numRead = 0;
    int dispCount = 0;

    while (dispCount < NUM_TO_DISPLAY) {
      Restaurant temp_rest;
      getRestaurant(restaurantIndex[topRest+numRead], &temp_rest);

      if (temp_rest.rating >= sr) {
        writeName(temp_rest.name, topRest+numRead);
        dispCount++;
      }

      numRead++;
    }

}


void getRestaurantList(int32_t x, int32_t y, uint8_t sr) {
  // Temporary array to save memory
  RestDist distances_all[NUM_RESTAURANTS];

  // Enters restaurant index and Manhattan distance
  // for each restaurant then sorts it by closest distance.
	for (int i = 0; i < NUM_RESTAURANTS; i++) {
		Restaurant temp_rest;
		getRestaurant(i, &temp_rest);

		distances_all[i].index = i;
		distances_all[i].dist = abs(x-temp_rest.lon) + abs(y-temp_rest.lat);
	}
	ssort(distances_all, NUM_RESTAURANTS);

  // Updates global restaurantIndex[] with
  // index values of closest 30 restaurants
	for (int i = 0; i < NUM_RESTAURANTS; i++) {
    restaurantIndex[i] = distances_all[i].index;
	}

}


void changeSelection(int new_selection, uint8_t sr) {
    if (new_selection >= NUM_TO_DISPLAY) {
        if (topDispRestIndex == NUM_RESTAURANTS-NUM_TO_DISPLAY) {
          new_selection == NUM_TO_DISPLAY
        }
        else {
          new_selection = 0;
          topDispRestIndex = constrain(topDispRestIndex+NUM_TO_DISPLAY, 0,
            NUM_RESTAURANTS-NUM_TO_DISPLAY);
          displayAllRestaurants(sr);
        }
    }
    else if (new_selection < 0) {
        if (topDispRestIndex == 0) {
          new_selection = 0;
        }
        else {
          new_selection += NUM_TO_DISPLAY;
          topDispRestIndex = constrain(topDispRestIndex-NUM_TO_DISPLAY, 0,
            NUM_RESTAURANTS-NUM_TO_DISPLAY);
          displayAllRestaurants(sr);
        }
    }

    int temp = selection;
    selection = new_selection;

    // Gets last/currently selected restaurants
    Restaurant temp_rest_old, temp_rest_new;
    getRestaurant(restaurantIndex[temp], &temp_rest_old);
    getRestaurant(restaurantIndex[selection], &temp_rest_new);

    writeName(temp_rest_old.name, temp);
    writeName(temp_rest_new.name, selection);
}


void goToListMode(int32_t x, int32_t y, int8_t sr) {
    tft.fillScreen(ILI9341_BLACK);

    getRestaurantList(x, y);
    displayAllRestaurants(sr);
}

*/
