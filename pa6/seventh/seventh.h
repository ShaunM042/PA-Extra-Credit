#ifndef SEVENTH_H
#define SEVENTH_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void calculateAndPrintMantissa(double value, int mantissaLength, int expValue);

int computeExponent(double* val, int expBits);

void convertToIEEE754(double num, int totalBits, int expBits, int mantissaBits);

#endif 