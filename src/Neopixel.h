#ifndef Neopixel
#define Neopixel
#include "mbed.h"
#include "Adafruit_Neopixel.h"
#include "Adafruit_GFX.h"
#include "Adafruit_NeoMatrix.h"

using namespace mbed;

void matrixSetup(Adafruit_NeoMatrix matrix);
void printScroll(Adafruit_NeoMatrix matrix, const char *text, int x);

#endif