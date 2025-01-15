#include "MotorClass.h"
#include "Encoders.h"

#include "mbed.h"
using namespace mbed;

#define VELOCITY_TICKER_PERIOD 0.1

Motor::Motor(PinName PwmPin, PinName dirPin, bool forwardDirection, Encoder &encoder)
    : _PwmPin(PwmPin), _dirPin(dirPin), _forwardDirection(forwardDirection), _encoder(encoder) {}

void Motor::setup(void)
{
  _PwmPin.period(PWM_PERIOD);
  // Set motor power to 0 so they start off by default
  _PwmPin.write(0.0f);
  // Calculate velocity every ticker period
  _velocityTicker.attach(callback(this, &Motor::calculateCurrentVelocity), VELOCITY_TICKER_PERIOD);
}

void Motor::move(float power)
{
  // If power given is positive
  if (power > 0)
  {
    // Change motor direction so that it drives forward relative to the robot
    _dirPin.write(_forwardDirection);
  }
  else
  {
    // Reverse motor direction
    _dirPin.write(!_forwardDirection);
  }
  // Rotate the motor at the given power
  _PwmPin.write(abs(power));
}

void Motor::stop(void)
{
  _PwmPin.write(0.0f);
}

void Motor::calculateCurrentVelocity(void)
{
  // Velocity in mm/s
  // Change in distance over known time
  _currentVelocity = (abs(_encoder.getDistance()) - _lastEncoderDist) / VELOCITY_TICKER_PERIOD;
  _lastEncoderDist = abs(_encoder.getDistance());
}

float Motor::getEncoderDist(void)
{
  return _encoder.getDistance();
}

float Motor::getEncoderCount(void)
{
  return _encoder.getCount();
}