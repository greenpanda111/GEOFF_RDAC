#include "mbed.h"
#include "IR.h"
using namespace mbed;

I2C i2c(P0_31, P0_2);

IR::IR(char mux_cmd) : _mux_cmd(mux_cmd)
{
}

int IR::read()
{
  _cmd[0] = 0x5E; // Command to read 8 msb
  _cmd[1] = 0x00; // Command to read 8 lsb

  // Select mux address
  i2c.write(_mux_addr, &_mux_cmd, 1);
  // Send command to take reading
  i2c.write(_IR_addr, _cmd, 1);
  // Wait for sensor to take reading
  wait_us(IR_DELAY);
  // Read the results from the sensor reading
  i2c.read(_IR_addr, _cmd, 2);
  // Shift cmd0 and combine with cmd1
  _rawData = ((_cmd[0] << 8) | _cmd[1]);
  // Convert raw data to mm
  return (_rawData / IR_CONVERSION);
}
