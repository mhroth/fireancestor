#!/bin/bash

# release
# clang main.c tiny_i2c.c tiny_mcp23017.c -std=gnu11 -Werror -O3 -DNDEBUG -lm -lrt -o ancesfire

# debug
clang main.c tiny_i2c.c tiny_mcp23017.c -std=gnu11 -Werror -O0 -g -lm -lrt -o ancesfire
