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
  _sonar.reset();
  _sonar.start();
  _ultrasonicPin.input();
  while (_ultrasonicPin.read() == 2)
  {
  };
  _sonar.stop();
  _offset = _sonar.read_us();
}

int Ultrasonic::read()
{
  _sonar.reset();
  // send ping
  _ultrasonicPin.output();
  _ultrasonicPin = 1;
  // wait for set pulse width
  wait_us(10);
  // stop ping
  _ultrasonicPin = 0;
  // wait for echo high
  _ultrasonicPin.input();
  while (_ultrasonicPin.read() == 0)
  {
  };
  // start timer
  _sonar.start();
  // wait for echo low
  while (_ultrasonicPin.read() == 1)
  {
  };
  // stop timer
  _sonar.stop();
  // read timer and convert and correct
  _distance = (_sonar.read_us() - _offset) / 58;
  return _distance;
}