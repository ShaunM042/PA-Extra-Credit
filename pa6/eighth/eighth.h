#ifndef EIGHTH_H
#define EIGHTH_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

void processFloatingPoint(const char *hexStr, 
                         int totalBits, 
                         int expBits,
                         int fracBits, 
                         int precision);

#endif 