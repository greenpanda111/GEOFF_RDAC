#include "Arduino.h"
#include "MotorClass.h"
#include "Encoders.h"

#include "mbed.h"
using namespace mbed;

#define DISTANCE_KP 0.1
#define DISTANCE_KI 0
#define DISTANCE_KD 0

#define VELOCITY_KP 0.1
#define VELOCITY_KI 0
#define VELOCITY_KD 0

#define ROTATING_KP 0.4
#define ROTATING_KI 0
#define ROTATING_KD 0

#define TICKER_DELAY 0.1

Motor::Motor(PinName PwmPin, PinName dirPin, bool forwardDirection, Encoder &encoder)
    : _PwmPin(PwmPin), _dirPin(dirPin), _forwardDirection(forwardDirection), _encoder(encoder) {}

void Motor::setup(void)
{
  _PwmPin.period(PWM_Period);
  _PwmPin.write(0.0f);
  _encoder.setup();
  _PIDSet = false;
  _isRotating = false;
  _PIDTicker.attach(callback(this, &Motor::PID), TICKER_DELAY);
  _velocityTicker.attach(callback(this, &Motor::calculateCurrentVelocity), TICKER_DELAY);
  _PIDDistError = 0;
  _PIDVelError = 0;
  _PIDLastDistError = 0;
  _PIDLastVelError = 0;
  _PIDIntegral = 0;
  _PIDDerivative = 0;
  _PIDOutput = 0;
  _lastEncoderDist = 0;
}

void Motor::move(float power)
{

  _PIDSet = true;
  setTargetVelocity(abs(power * 500));
  if (power > 0)
  {
    _dirPin.write(_forwardDirection);
    //_PwmPin.write(power);
  }
  else
  {
    _dirPin.write(!_forwardDirection);
    //_PwmPin.write(abs(power));
  }
}

void Motor::stop(void)
{
  _PwmPin.write(0.0f);
  _PIDSet = false;
}

void Motor::calculateCurrentVelocity(void)
{
  _currentVelocity = abs(_encoder.getDistance()) - _lastEncoderDist / 0.1;
  _lastEncoderDist = abs(_encoder.getDistance());
}

void Motor::resetEncoder(void)
{
  _encoder.reset();
}

float Motor::getEncoderDist(void)
{
  return _encoder.getDistance();
}

void Motor::PID(void)
{
  if (_PIDSet == false)
  {
    _PIDDistError = 0;
    _PIDVelError = 0;
    _PIDLastDistError = 0;
    _PIDLastVelError = 0;
    _PIDIntegral = 0;
    _PIDDerivative = 0;
    _isRotating = false;
    return;
  }
  _PIDDistError = _targetDistance - _currentDistance;
  // stopping
  if (_PIDDistError == 0)
  {
    _PIDOutput = 0;
  }

  // constant velocity
  else if ((_PIDDistError > WHEEL_CIRCUMFERENCE) & (_isRotating = false))
  {
    _PIDVelError = _targetVelocity - _currentVelocity;
    // update last error
    _PIDLastVelError = _PIDVelError;

    _PIDIntegral += _PIDVelError * TICKER_DELAY;
    _PIDDerivative = ((_PIDVelError - _PIDLastVelError) / TICKER_DELAY);

    _PIDOutput = _PIDVelError * DISTANCE_KP + _PIDIntegral * DISTANCE_KI + _PIDDerivative * DISTANCE_KD;
  }
  // exact distance
  else
  {
    // update last error
    _PIDLastDistError = _PIDDistError;

    _PIDIntegral += _PIDDistError * TICKER_DELAY;
    _PIDDerivative = ((_PIDDistError - _PIDLastDistError) / TICKER_DELAY);
    if (_isRotating == false)
    {
      _PIDOutput = _PIDDistError * DISTANCE_KP + _PIDIntegral * DISTANCE_KI + _PIDDerivative * DISTANCE_KD;
    }
    else
    {
      _PIDOutput = _PIDDistError * ROTATING_KD + _PIDIntegral * ROTATING_KI + _PIDDerivative * ROTATING_KD;
    }
  }

  // clamp output between 0.5 and -0.5
  if (_PIDOutput > 0.5f)
  {
    _PIDOutput = 0.5f;
  }
  if (_PIDOutput < -0.5f)
  {
    _PIDOutput = -0.5f;
  }

  _PwmPin.write(_PIDOutput);
}

void Motor::setTargetDistance(float target)
{
  _targetDistance = target;
}

void Motor::setCurrentDistance(float current)
{
  _currentDistance = current;
}

void Motor::setTargetVelocity(float target)
{
  _targetVelocity = target;
}

void Motor::setCurrentVelocity(float current)
{
  _currentVelocity = current;
}

float Motor::getCurrentVelocity(void)
{
  return _currentVelocity;
}

void Motor::setIsRotating(bool value)
{
  _isRotating = value;
}
