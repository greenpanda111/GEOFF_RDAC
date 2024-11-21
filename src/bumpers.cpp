#include "mbed.h"
#include "MotorController.h"
#include "bumpers.h"
using namespace mbed;


Bumper::Bumper(PinName left, PinName right)
    : _left(left), _right(right)
{
}
void Bumper::setup(){
    _left.rise(callback(this, &Bumper::avoid));
    _right.rise(callback(this, &Bumper::avoid));
}

void Bumper::avoid(){
    Serial.println("avoiding");
    motorControl.reverseDist(100);
    motorControl.rotate(90);
}