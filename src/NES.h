#ifndef NES_H
#define NES_H

#include "mbed.h"
#include "MotorController.h"
#include "NESControllerInterface.h"

using namespace mbed;

#define NES_PIN_1 A6
#define NES_PIN_2 A3
#define NES_PIN_3 A2

#define DELAY 830000

void detectInput(void);

#endif