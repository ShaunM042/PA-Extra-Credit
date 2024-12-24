#include "seventh.h"

void calculateAndPrintMantissa(double value, int mantissaLength, int expValue) {
    double mantissaVal = value;
    if (expValue != 0 && value != 0) {
        mantissaVal = value - 1.0;
    }

    int mantissaDigits[128] = {0};  
    int digitCount = 0;
    
    for (int i = 0; i < mantissaLength + 2; i++) {
        mantissaVal *= 2;
        mantissaDigits[digitCount++] = (mantissaVal >= 1.0) ? 1 : 0;
        if (mantissaVal >= 1.0) {
            mantissaVal -= 1.0;
        }
    }

    if (digitCount > mantissaLength) {
        int needRound = mantissaDigits[mantissaLength];
        int extraBit = mantissaDigits[mantissaLength + 1];
        
        if (needRound && (extraBit || mantissaDigits[mantissaLength - 1])) {
            int pos = mantissaLength - 1;
            while (pos >= 0 && mantissaDigits[pos]) {
                mantissaDigits[pos] = 0;
                pos--;
            }
            if (pos >= 0) {
                mantissaDigits[pos] = 1;
            }
        }
    }

    for (int i = 0; i < mantissaLength; i++) {
        printf("%d", mantissaDigits[i]);
    }
}

int computeExponent(double* val, int expBits) {
    if (*val == 0) {
        return 0;
    }

    int biasValue = (1 << (expBits - 1)) - 1;
    int expValue = 0;
    
    while (*val >= 2.0) {
        *val /= 2.0;
        expValue++;
    }
    while (*val < 1.0) {
        *val *= 2.0;
        expValue--;
    }

    return expValue + biasValue;
}

void convertToIEEE754(double num, int totalBits, int expBits, int mantissaBits) {
    int sign = (num < 0) ? 1 : 0;
    printf("%d", sign);
    
    double absValue = fabs(num);
    int exponent = computeExponent(&absValue, expBits);
    
    if (num == 0) {
        exponent = 0;
        absValue = 0;
    } else if (exponent <= 0) {
        absValue = num * pow(2, (1 << (expBits - 1)) - 2);
        exponent = 0;
    } else if (exponent >= (1 << expBits) - 1) {
        exponent = (1 << expBits) - 1;
        absValue = 0;
    }

    for (int i = expBits - 1; i >= 0; i--) {
        printf("%d", (exponent >> i) & 1);
    }

    calculateAndPrintMantissa(absValue, mantissaBits, exponent);
    printf("\n");
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <inputfile>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *inFile = fopen(argv[1], "r");
    if (!inFile) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    double value;
    int total, exp, mantissa;
    
    while (fscanf(inFile, "%lf %d %d %d", &value, &total, &exp, &mantissa) == 4) {
        convertToIEEE754(value, total, exp, mantissa);
    }

    fclose(inFile);
    return EXIT_SUCCESS;
}