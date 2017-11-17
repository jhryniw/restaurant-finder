/*
 * @file rest_sd.h
 * @author James Hryniw 1431912
 * @author Songhui Zhang 1499982
 */

#ifndef __REST_SD_H__
#define __REST_SD_H__

#include <Arduino.h>
#include <Adafruit_ILI9341.h>
#include <SD.h>

#define TFT_DC 9
#define TFT_CS 10
#define SD_CS 6

#define REST_START_BLOCK 4000000
#define NUM_RESTAURANTS  1066

#define NUM_TO_DISPLAY 30

#define SCREEN_WIDTH 320

// We don't need to test the alignment because
// we added the packed attribute
struct Restaurant {
  int32_t lat;
  int32_t lon;
  uint8_t rating;
  char name[55];
} __attribute__((packed));

struct RestDist {
  uint16_t index; // index of restaurant from 0 to NUM_RESTAURANTS-1
  uint16_t dist;  // Manhatten distance to cursor position
};

extern Sd2Card card;
extern Adafruit_ILI9341 tft;
extern int selection;
extern int* restaurantIndex;
extern int topDispRestIndex;

const int TEXT_SIZE = 1;

/** Initializes the SD card */
void initSD();

/**
 * Gets a restaurant at a particular index
 * optimized for consecutive calls to the same block of memory
 * @param restIndex Index number of restaurant in memory
 * @param rest Pointer to Restaurant struct to change
 */
void getRestaurant(int restIndex, Restaurant* rest);

/**
 * Prints name of restaurant
 * Text and background colours change
 * if restaurant is currently selected
 * @param name String to display
 * @param index Index number of restaurant in displayed list
 */
void writeName(const char* name, uint8_t rating, int index);

/**
 * Updates value of selection when cursor moves in list mode.
 * Rolls over/under if cursor goes too low/high.
 * Also reprints last/currently selected restaurant names.
 * @param new_selection Value of next selection
 */
void changeSelection(int new_selection);

/**
 * Finds the closest 30 restaurants from cursor position
 * in Manhattan distance at time of click and updates restaurantIndex[] values.
 * @param x Longitude of cursor
 * @param y Latitude of cursor
 */
void getRestaurantList(int32_t x, int32_t y, uint8_t sr);

/**
 * Called once when switching from map to list mode.
 * Updates restaurantIndex[] and displays restaurants.
 * @param x Longitude of cursor on click.
 * @param y Latitude of cursor on click.
 */
void goToListMode(int32_t x, int32_t y, int8_t sr);

#endif
