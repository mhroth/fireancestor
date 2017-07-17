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
#include <arpa/inet.h>
#include <fcntl.h> // for open
#include <ifaddrs.h>
#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <time.h> // nanosleep, clock_gettime
#include <unistd.h> // for close

#include "tinyosc.h"

#include "tiny_mcp23017.h"

static volatile bool _keepRunning = true;

static void sigintHandler(int x) {
  printf("Termination signal received.\n"); // handle Ctrl+C
  _keepRunning = false;
}

static int openReceiveSocket() {
  int fd = socket(AF_INET, SOCK_DGRAM, 0);
  fcntl(fd, F_SETFL, O_NONBLOCK); // set the socket to non-blocking
  struct sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_port = htons(2015);
  sin.sin_addr.s_addr = INADDR_ANY;
  bind(fd, (struct sockaddr *) &sin, sizeof(struct sockaddr_in));
  return fd;
}

int main(int narg, char **argc) {
  // register the SIGINT handler
  signal(SIGINT, &sigintHandler);

  // for receiving commands from TouchOSC
  const int fd_receive = openReceiveSocket();

  struct timespec sleep_nano;

  tinyMCP23017 ti2c;
  tmcp23017_open(&ti2c, "/dev/i2c-1");

  // buffer into which network data is received
  char buffer[1024];

  struct sockaddr_in sin;
  int len = 0;
  int sa_len = sizeof(struct sockaddr_in);
  tosc_message osc;

  while (_keepRunning) {
    while ((len = recvfrom(fd_receive, buffer, sizeof(buffer), 0, (struct sockaddr *) &sin, (socklen_t *) &sa_len)) > 0) {
      if (!tosc_parseMessage(&osc, buffer, len)) {
        if (!strncmp(tosc_getAddress(&osc), "/1/flame/", 9)) {
          if (!strcmp(tosc_getAddress(&osc)+9, "alloff")) {
            tmcp23017_all_off(&ti2c);
          } else {
            int pin = atoi(tosc_getAddress(&osc)+9) - 1; // pins are 1-indexed
            bool state = tosc_getNextFloat(&osc) != 0.0f;
            tmcp23017_write_pin(&ti2c, pin, state);
          }
        } else if (!strncmp(tosc_getAddress(&osc), "/1/mallet/", 10)) {

        } else {
          tosc_printOscBuffer(buffer, len);
        }
      }
    }
  }

  // uint32_t i = 1;
  // while (_keepRunning) {
  //   tmcp23017_write_pin(&ti2c, (i-1)%16, false);
  //   tmcp23017_write_pin(&ti2c, i%16, true);
  //
  //   sleep_nano.tv_sec = 0;
  //   sleep_nano.tv_nsec = 100000000;
  //   nanosleep(&sleep_nano, NULL);
  //   ++i;
  // }

  close(fd_receive); // the listening socket

  tmcp23017_close(&ti2c);

  return 0;
}
