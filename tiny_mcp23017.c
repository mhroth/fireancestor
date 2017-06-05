/**
 * Copyright (c) 2017, Martin Roth (mhroth@gmail.com)
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "tiny_mcp23017.h"

void tmcp23017_open(tinyMCP23017 *o, const char *i2cFilename) {
  assert(o != NULL);
  assert(i2cFilename != NULL);
  memset(o->gpio, 0, 16*sizeof(uint8_t));
  tinyI2C_open(&o->ti2c, i2cFilename);
  for (int i = 0; i < 8; ++i) {
    tinyI2C_write(&o->ti2c, TINYI2C_MCP23017_BASE_ADDRESS+i, 0x0, 0x00); // configure all GPIOA to write
    tinyI2C_write(&o->ti2c, TINYI2C_MCP23017_BASE_ADDRESS+i, 0x1, 0x00); // configure all GPIOB to write
  }
}

void tmcp23017_close(tinyMCP23017 *o) {
  assert(o != NULL);
  tinyI2C_close(&o->ti2c);
}

int tmcp23017_write_pin(tinyMCP23017 *o, uint8_t pin, bool state) {
  assert(o != NULL);
  assert(pin < 128);
  uint8_t deviceIndex = pin >> 4; // each MCP23017 has 16 GPIO pins; (pin/16)
  uint8_t gpio_index = pin & 0x7; // index of pin within GPIOA or GPIOB; (pin%8)
  uint8_t gpioab_index = (pin & 0xF) >> 3; // are we working with GPIOA or GPIOB (0 or 1); (pin%16 >= 8)
  uint8_t reg_addr = 0x12 + gpioab_index; // register address of GPIOA or GPIOB

  if (state) {
    o->gpio[2*deviceIndex+gpioab_index] |= (0x1 << gpio_index);
  } else {
    o->gpio[2*deviceIndex+gpioab_index] &= ~(0x1 << gpio_index);
  }

  // printf("deviceIndex:%i gpio_index:%i gpioab_index:%i reg_addr:0x%x state:0x%x\n",
  //     deviceIndex, gpio_index, gpioab_index, reg_addr, o->gpio[2*deviceIndex+gpioab_index]);

  return tinyI2C_write(&o->ti2c, TINYI2C_MCP23017_BASE_ADDRESS+deviceIndex, reg_addr, o->gpio[2*deviceIndex+gpioab_index]);
}
