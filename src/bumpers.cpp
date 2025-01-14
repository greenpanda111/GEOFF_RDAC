#include "mbed.h"
#include "MotionController.h"
#include "bumpers.h"

using namespace mbed;

Bumper::Bumper(PinName left, PinName right)
    : _left(left), _right(right)
{
}
void Bumper::setup(){
    //Run avoid ISR when either bumper is pressed
    _left.fall(callback(this, &Bumper::avoid));
    _right.fall(callback(this, &Bumper::avoid));
}

void Bumper::avoid(){
    //set the stuck flag high so that the motors will exit their movement loops
    motorControl.setStuck(true);
}