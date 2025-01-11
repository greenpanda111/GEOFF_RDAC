#include "mbed.h"
#include "MotionController.h"
#include "bumpers.h"
using namespace mbed;


Bumper::Bumper(PinName left, PinName right)
    : _left(left), _right(right)
{
}
void Bumper::setup(){
    _left.fall(callback(this, &Bumper::avoid));
    _right.fall(callback(this, &Bumper::avoid));
}

void Bumper::avoid(){
    motorControl.setStuck(true);
}