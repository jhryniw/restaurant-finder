/*
 * @file rest_sd.h
 * @author James Hryniw 1431912
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

// We don't need to test the alignment because
// we added the packed attribute
struct restaurant {
  int32_t lat;
  int32_t lon;
  uint8_t rating;
  char name[55];
} __attribute__((packed));

extern Sd2Card card;
extern Adafruit_ILI9341 tft;

const int TEXT_SIZE = 1;

/** Initializes the SD card */
void initSd();

/**
 * Gets a restaurant at a particular index
 * optimized for consecutive calls to the same block of memory
 */
void getRestaurant(int restIndex, restaurant* rest);

void writeName(const char* name, int index, bool selection = false);
void changeSelection(int& selection, int new_selection);
void goToListMode();

#endif