#include "NES.h"
#include "mbed.h"
#include "MotorController.h"
#include "NESControllerInterface.h"

using namespace mbed;

NESControllerInterface nes(A6, A3, A2); // GPIO 2,3,4

void detectInput(void)
{

    NESInput input = nes.getNESInput();

    if (input.buttonUp == true)
    {
        motorControl.reverse();
        wait_us(STD_DELAY * 0.01);
    }
    if (input.buttonDown == true)
    {
        motorControl.forward();
        wait_us(STD_DELAY * 0.01);
    }
    if (input.buttonLeft == true)
    {
        motorControl.rotateCounterClockwise();
        wait_us(STD_DELAY * 0.01);
    }
    if (input.buttonRight == true)
    {
        motorControl.rotateClockwise();
        wait_us(STD_DELAY * 0.01);
    }
    motorControl.stop();
}
