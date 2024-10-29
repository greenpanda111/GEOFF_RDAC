#ifndef LED_h
#define LED_h
#include "Arduino.h"

#include "mbed.h"
using namespace mbed;

#define GPIO_PIN_2 P0_28
#define GPIO_PIN_3 P0_29
#define GPIO_PIN_4 P0_30

#define WHITE 1, 1, 1
#define RED 1, 0, 0
#define GREEN 0, 1, 0
#define BLUE 0, 0, 1
#define ORANGE 1, 1, 0
#define PURPLE 1, 0, 1
#define YELLOW 0, 1, 1
#define OFF 0, 0, 0


#define LED_DELAY 500000

class LED {
public:
  LED(PinName redPin, PinName greenPin, PinName bluePin);
  void state(bool R, bool G, bool B);
  void cycle(void);

private:
  DigitalOut _redLed;
  DigitalOut _greenLed;
  DigitalOut _blueLed;
};

extern LED led;

#endif