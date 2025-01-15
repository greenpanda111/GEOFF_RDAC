#ifndef IR_h
#define IR_h
#include "Arduino.h"

#include "mbed.h"
using namespace mbed;

// addresses of all four IR sensors
#define LEFT_SIDE 0x01
#define RIGHT_SIDE 0x02
#define RIGHT_FRONT 0x08
#define LEFT_FRONT 0x04
// delay to allow IR to take accurate reading
#define IR_DELAY 5000
// calibrated value to change the raw output of the IR sensors into mm
#define IR_CONVERSION 100

class IR
{
public:
    IR(char mux_cmd);
    int read();

private:
    // stores the raw output from the IR sensors
    int _rawData;
    
    const char _mux_addr = 0xEE;
    const char _IR_addr = 0x80;
    // array to store the 8 MSB and 8 LSB of an IR reading
    char _cmd[2];
    char _mux_cmd;
};

extern IR ir;

#endif // IR_H