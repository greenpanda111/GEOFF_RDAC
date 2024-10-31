#include "mbed.h"
#include "Encoders.h"

using namespace mbed;

Encoder::Encoder(PinName pinA, PinName pinB)
    : _pinA(pinA), _pinB(pinB)
{
}

void Encoder::setup(void){
    _pinA.rise(callback(this,&Encoder::countPulseA));
    _pinB.rise(callback(this,&Encoder::countPulseB));
     reset();
}

int Encoder::getRevA(void){
    return _shaftRevA;
}

int Encoder::getRevB(void){
    return _shaftRevB;
}

void Encoder::countPulseA(void)
{
    if (_countA % 6 == 0)
    {
        if (_countA % 100 == 0)
        {
            _shaftRevA++;
        }
    }
    _countA++;
}

void Encoder::countPulseB(void)
{
    if (_countB % 6 == 0)
    {
        if (_countB % 100 == 0)
        {
            _shaftRevB++;
        }
    }
    _countB++;
}

void Encoder::reset(void)
{
    _countA = 0;
    _countB = 0;

    _shaftRevA = 0;
    _shaftRevB = 0;
}
