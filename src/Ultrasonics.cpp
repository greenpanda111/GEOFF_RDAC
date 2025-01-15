#include "mbed.h"
#include "Ultrasonics.h"
using namespace mbed;

Ultrasonic::Ultrasonic(PinName ultrasonicPin)
    : _ultrasonicPin(ultrasonicPin)
{
  _distance = 0;
}

void Ultrasonic::correction()
{
  // Reset timer
  _sonar.reset();
  // Start timer
  _sonar.start();
  _ultrasonicPin.input();
  while (_ultrasonicPin.read() == 2)
  {
  };
  // Stop timer
  _sonar.stop();
  // Save time taken
  _offset = _sonar.read_us();
}

int Ultrasonic::read()
{
  // Reset Timer
  _sonar.reset();
  // Send pulse
  _ultrasonicPin.output();
  _ultrasonicPin = 1;
  // Wait for set pulse width
  wait_us(10);
  // Stop ping
  _ultrasonicPin = 0;
  // Wait for echo high
  _ultrasonicPin.input();
  while (_ultrasonicPin.read() == 0)
  {
  };
  // Start timer
  _sonar.start();
  // Wait for echo low
  while (_ultrasonicPin.read() == 1)
  {
  };
  // Stop timer
  _sonar.stop();
  // Read timer
  // Convert and correct
  _distance = (_sonar.read_us() - _offset) / 58;
  return _distance;
}