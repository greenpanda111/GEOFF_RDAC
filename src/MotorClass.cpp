#include "Arduino.h"
#include "MotorClass.h"

#include "mbed.h"
using namespace mbed;

Motor::Motor(PinName PwmPin, PinName dirPin)
  : _PwmPin(PwmPin), _dirPin(dirPin) {}

void Motor::setup(void) {
  _PwmPin.period(PWM_Period);
  _PwmPin.write(0.0f);
}

void Motor::move(int direction, float power) {
  _dirPin.write(direction);
  _PwmPin.write(power);
}

void Motor::stop(void) {
  _PwmPin.write(0.0f);
}