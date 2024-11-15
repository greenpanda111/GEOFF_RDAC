#include "mbed.h"
#include "Adafruit_Neopixel.h"
#include "Adafruit_GFX.h"
#include "Adafruit_NeoMatrix.h"
#include "Neopixel.h"

using namespace mbed;

void matrixSetup(Adafruit_NeoMatrix matrix)
{

  const uint16_t colors[] = {
      matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255)};

  matrix.begin();
  matrix.show();
  matrix.setTextWrap(false);
  matrix.setBrightness(30);
  matrix.setTextColor(colors[2]);
  matrix.setCursor(0, 0);
}

void printScroll(Adafruit_NeoMatrix matrix, const char *text, int x)
{
  matrix.fillScreen(0);
  matrix.setCursor(x, 0);
  matrix.print(F(text));
  if (--x < -36)
  {
    x = matrix.width();
  }
  matrix.show();
  wait_us(200000);
}
