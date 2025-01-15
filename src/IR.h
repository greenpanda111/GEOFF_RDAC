#ifndef IR_h
#define IR_h
#include "Arduino.h"

#include "mbed.h"
using namespace mbed;

// Addresses of all four IR sensor busses
#define LEFT_SIDE 0x01
#define RIGHT_SIDE 0x02
#define RIGHT_FRONT 0x08
#define LEFT_FRONT 0x04
// Delay to allow IR to take accurate reading
#define IR_DELAY 5000
// Calibrated value to change the raw output of the IR sensors into mm
#define IR_CONVERSION 100

class IR
{
public:
    IR(char mux_cmd);
    int read();

private:
    // Stores the raw output from the IR sensors
    int _rawData;
    // I2C address
    const char _mux_addr = 0xEE;
    const char _IR_addr = 0x80;
    // Array to store the 8 MSB and 8 LSB of an IR reading
    char _cmd[2];
    char _mux_cmd;
};

extern IR ir;

#endif // IR_H