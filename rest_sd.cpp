#include "rest_sd.h"

Sd2Card card2;
Restaurant restBlock[8];
uint32_t lastBlock = -1;

void initSD() {
	Serial.print("Initializing SD card...");
	if (!SD.begin(SD_CS)) {
			Serial.print("failed! Is it inserted properly?");
			while(true) {}
	}
	Serial.print("Initializing SPI communication for raw reads...");
  if (!card2.init(SPI_HALF_SPEED, SD_CS)) {
    Serial.println("failed! Is the card inserted properly?");
    while (true) {}
  }

	Serial.println("OK!");
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

void getRestaurant(int restIndex, Restaurant* restPtr) {
  // Calculates block index
	uint32_t blockNum = REST_START_BLOCK + restIndex/8;

  // Reads a new block if the current block is
  // not the same as the last block read
	if (blockNum != lastBlock) {
		while (!card2.readBlock(blockNum, (uint8_t*) restBlock)) {
	    Serial.println("Read block failed, trying again.");
	  }
    // Updates block index
		lastBlock = blockNum;
	}

  // Changes restaurant val at pointer
	*restPtr = restBlock[restIndex % 8];

}

void get20Restaurants(int x, int y, Restaurant* restArr) {
	RestDist distances[NUM_RESTAURANTS];

	for (int i = 0; i < NUM_RESTAURANTS; i++) {
		Restaurant tempRest;
		getRestaurant(i, &tempRest);

		distances[i].index = i;
		distances[i].dist = abs(x-tempRest.lon) + abs(y-tempRest.lat);
	}

	ssort(distances, NUM_RESTAURANTS);

	for (int i = 0; i < 20; i++) {
		uint16_t index = distances[i].index;
		getRestaurant(index, &restArr[i]);
	}

}
