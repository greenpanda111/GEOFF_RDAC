#include "Arduino.h"
#include "MotorClass.h"
#include "Encoders.h"

#include "mbed.h"
using namespace mbed;

#define DISTANCE_KP 0.0055
#define DISTANCE_KI 0
#define DISTANCE_KD 0

#define VELOCITY_KP 0.01
#define VELOCITY_KI 0
#define VELOCITY_KD 0

#define ROTATING_KP 0.1
#define ROTATING_KI 0
#define ROTATING_KD 0

#define PID_TICKER_PERIOD 0.1
#define VELOCITY_TICKER_PERIOD 0.1

Motor::Motor(PinName PwmPin, PinName dirPin, bool forwardDirection, Encoder &encoder)
    : _PwmPin(PwmPin), _dirPin(dirPin), _forwardDirection(forwardDirection), _encoder(encoder) {}

void Motor::setup(void)
{
  _PwmPin.period(PWM_Period);
  _PwmPin.write(0.0f);
  _encoder.setup();
  _isRotating = false;
  _PIDTicker.attach(callback(this, &Motor::PID), PID_TICKER_PERIOD);
  _velocityTicker.attach(callback(this, &Motor::calculateCurrentVelocity), VELOCITY_TICKER_PERIOD);
  _PIDDistError = 0;
  _PIDVelError = 0;
  _PIDLastDistError = 0;
  _PIDLastVelError = 0;
  _PIDIntegral = 0;
  _PIDDerivative = 0;
  _PIDOutput = 0;
  _lastEncoderDist = 0;
  _movementMode = STOP;
}

void Motor::move(float distance)
{
  _encoder.reset();
  _targetDistance = abs(distance);
  _targetVelocity = (20);

  if (distance > 0)
  {
    _dirPin.write(_forwardDirection);
  }
  else
  {
    _dirPin.write(!_forwardDirection);
  }
  _movementMode = CONSTANT_VELOCITY;
}

void Motor::stop(void)
{
  _PwmPin.write(0);
  _targetDistance = 0;
  _targetVelocity = 0;
  _movementMode = STOP;
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
void Motor::PID(void)
{
  MovementMode mode = _movementMode;
  switch (mode)
  {
  case STOP:
    _PIDOutput = 0;
    _PIDIntegral = 0;
    _PIDDerivative = 0;
    _PIDLastDistError = 0;
    _PIDLastVelError = 0;
    break;

  case CONSTANT_VELOCITY:
    _PIDDistError = (abs(_targetDistance) * STEP_CONVERSION) - abs(_encoder.getCount());

    // if close to target switch to exact distance mode
    if ((abs(_targetDistance) * STEP_CONVERSION - abs(_encoder.getCount())) < ONE_ROTATION_TICKS / 8)
    {
      _PIDIntegral = 0;
      _PIDDerivative = 0;
      _PIDLastDistError = 0;
      _PIDLastVelError = 0;
      _movementMode = EXACT_DISTANCE;
      break;
    }
    // if rotating switch to rotating mode
    
    else if (_isRotating == true)
    {
      _PIDIntegral = 0;
      _PIDDerivative = 0;
      _PIDLastDistError = 0;
      _PIDLastVelError = 0;
      _movementMode = ROTATING;
      break;
    }
    
    else{
    _PIDVelError = abs(_targetVelocity - abs(_currentVelocity));
    _PIDIntegral += _PIDVelError * PID_TICKER_PERIOD;
    _PIDDerivative = ((_PIDVelError - _PIDLastVelError) / PID_TICKER_PERIOD);

    _PIDOutput = _PIDVelError * VELOCITY_KP + _PIDIntegral * VELOCITY_KI + _PIDDerivative * VELOCITY_KD;

    _PIDLastVelError = _PIDVelError;
    }
    break;

  case EXACT_DISTANCE:

    _PIDDistError = (_targetDistance * STEP_CONVERSION) - _encoder.getCount();

    if (_PIDDistError < 10)
    {
      _targetDistance = 0;
      _targetVelocity = 0;
      _PIDOutput = 0;
      _PwmPin.write(_PIDOutput);
      _movementMode = STOP;
      break;
    }
    else{
    _PIDIntegral += _PIDDistError * PID_TICKER_PERIOD;
    _PIDDerivative = ((_PIDDistError - _PIDLastDistError) / PID_TICKER_PERIOD);
    _PIDOutput = _PIDDistError * DISTANCE_KP + _PIDIntegral * DISTANCE_KI + _PIDDerivative * DISTANCE_KD;

    // update last error
    _PIDLastDistError = _PIDDistError;
    }
    break;

  case ROTATING:
    _PIDDistError = abs(_targetDistance) - _encoder.getDistance();

    _PIDIntegral += _PIDDistError * PID_TICKER_PERIOD;
    _PIDDerivative = ((_PIDDistError - _PIDLastDistError) / PID_TICKER_PERIOD);

    _PIDOutput = _PIDDistError * ROTATING_KD + _PIDIntegral * ROTATING_KI + _PIDDerivative * ROTATING_KD;

    // update last error
    _PIDLastDistError = _PIDDistError;

    break;
  }

  // clamp output
  _PIDOutput = fmax(fmin(_PIDOutput, 0.4), -0.4);
  // write output to motor
  _PwmPin.write(_PIDOutput);
}

void Motor::setTargetDistance(float target)
{
  _encoder.reset();
  _targetDistance = abs(target);
  move(target);
}

void Motor::setIsRotating(bool value)
{
  _isRotating = value;
}
