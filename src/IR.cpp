#include "mbed.h"
#include "IR.h"
using namespace mbed;

I2C i2c(P0_31, P0_2);

IR::IR(char mux_cmd):_mux_cmd(mux_cmd)
{
  _distance = 0;
}

int IR::read()
{
  _cmd[0] = 0x5E;
  _cmd[1] = 0x00;

  i2c.write(_mux_addr, &_mux_cmd, 1);
  i2c.write(_IR_addr, _cmd, 1);
  wait_us(500000);
  i2c.read(_IR_addr, _cmd, 2);

  _conversionArray[0] = _cmd[0], _cmd[1];
  return (_conversionArray[0] * 0.2);
}
