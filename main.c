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
#include <signal.h>
#include <stdio.h>
#include <time.h>

#include "tiny_mcp23017.h"

static volatile bool _keepRunning = true;

static void sigintHandler(int x) {
  printf("Termination signal received.\n"); // handle Ctrl+C
  _keepRunning = false;
}

int main(int narg, char **argc) {
  // register the SIGINT handler
  signal(SIGINT, &sigintHandler);

  struct timespec sleep_nano;

  tinyMCP23017 ti2c;
  tmcp23017_open(&ti2c, "/dev/i2c-1");

  uint32_t i = 1;
  while (_keepRunning) {
    tmcp23017_write_pin(&ti2c, (i-1)%16, false);
    tmcp23017_write_pin(&ti2c, i%16, true);

    sleep_nano.tv_sec = 0;
    sleep_nano.tv_nsec = 100000000;
    nanosleep(&sleep_nano, NULL);
    ++i;
  }

  tmcp23017_close(&ti2c);

  return 0;
}
