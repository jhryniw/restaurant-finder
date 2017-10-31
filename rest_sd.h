#ifndef __REST_SD_H__
#define __REST_SD_H__

#include <Arduino.h>
#include <SD.h>

#define SD_CS 6
#define REST_START_BLOCK 4000000
#define NUM_RESTAURANTS 1066

struct Restaurant {
	int32_t lat;
	int32_t lon;
	uint8_t rating;
	char name[55];
};

struct RestDist {
  uint16_t index; // index of restaurant from 0 to NUM_RESTAURANTS-1
  uint16_t dist;  // Manhatten distance to cursor position
};

void initSD();
void get20Restaurants(int x, int y, Restaurant* restArr);

#endif
