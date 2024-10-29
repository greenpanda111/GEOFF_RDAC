#include "Arduino.h"
#include "LED.h"

#include "mbed.h"
using namespace mbed;

LED::LED(PinName redPin, PinName greenPin, PinName bluePin)
  : _redLed(redPin), _greenLed(greenPin), _blueLed(bluePin) {
}

void LED::state(bool R, bool G, bool B) {
  _redLed = R;
  _greenLed = G;
  _blueLed = B;
}

void LED::cycle() {
  wait_us(LED_DELAY);
  led.state(WHITE);
  wait_us(LED_DELAY);
  led.state(RED);
  wait_us(LED_DELAY);
  led.state(GREEN);
  wait_us(LED_DELAY);
  led.state(BLUE);
  wait_us(LED_DELAY);
  led.state(ORANGE);
  wait_us(LED_DELAY);
  led.state(PURPLE);
  wait_us(LED_DELAY);
  led.state(YELLOW);
  wait_us(LED_DELAY);
  led.state(OFF);
  wait_us(LED_DELAY);
}