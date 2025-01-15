#include "MotorClass.h"
#include "Encoders.h"

#include "mbed.h"
using namespace mbed;

#define VELOCITY_TICKER_PERIOD 0.1

Motor::Motor(PinName PwmPin, PinName dirPin, bool forwardDirection, Encoder &encoder)
    : _PwmPin(PwmPin), _dirPin(dirPin), _forwardDirection(forwardDirection), _encoder(encoder) {}

void Motor::setup(void)
{
  _PwmPin.period(PWM_Period);
  _PwmPin.write(0.0f);
  _velocityTicker.attach(callback(this, &Motor::calculateCurrentVelocity), VELOCITY_TICKER_PERIOD);
}

void Motor::move(float power)
{
  if (power > 0)
  {
    _dirPin.write(_forwardDirection);
  }
  else
  {
    _dirPin.write(!_forwardDirection);
  }
  _PwmPin.write(abs(power));
}

void Motor::stop(void)
{
  _PwmPin.write(0.0f);
}

void Motor::calculateCurrentVelocity(void)
{
  // velocity in mm/s
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