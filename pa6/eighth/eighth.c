#include "eighth.h"

unsigned int extractBits(unsigned int value, int start, int length) {
    unsigned int mask = (1U << length) - 1;
    return (value >> start) & mask;
}

double handleSubnormal(unsigned int fraction, int fracBits, int bias) {
    if (fraction == 0) {
        return 0.0;
    }
    
    double normalizedFrac = (double)fraction / (1U << fracBits);
    return ldexp(normalizedFrac, 1 - bias);
}

double handleNormal(unsigned int fraction, int fracBits, int exponent, int bias) {
    double normalizedFrac = 1.0 + (double)fraction / (1U << fracBits);
    return ldexp(normalizedFrac, exponent - bias);
}

unsigned int parseHexString(const char *hexStr) {
    unsigned int parsedValue;
    if (hexStr[0] == '0' && (hexStr[1] == 'x' || hexStr[1] == 'X')) {
        sscanf(hexStr + 2, "%x", &parsedValue);
    } else {
        sscanf(hexStr, "%x", &parsedValue);
    }
    return parsedValue;
}

void processFloatingPoint(const char *hexStr, int totalBits, int expBits, 
                         int fracBits, int precision) {
    unsigned int fullValue = parseHexString(hexStr);
    
    int signBit = extractBits(fullValue, totalBits - 1, 1);
    int exponent = extractBits(fullValue, fracBits, expBits);
    unsigned int fraction = extractBits(fullValue, 0, fracBits);
    
    int biasValue = (1 << (expBits - 1)) - 1;
    
    double result;
    
    if (exponent == 0) {
        result = handleSubnormal(fraction, fracBits, biasValue);
        if (result == 0.0) {
            printf("0.%.*f\n", precision, 0.0);
            return;
        }
    } 
    else if (exponent == ((1 << expBits) - 1)) {
        if (fraction == 0) {
            printf("%sInfinity\n", signBit ? "-" : "");
        } else {
            printf("NaN\n");
        }
        return;
    } 
    else {
        result = handleNormal(fraction, fracBits, exponent, biasValue);
    }
    
    result = signBit ? -result : result;
    printf("%.*f\n", precision, result);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <inputfile>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    FILE *inputFile = fopen(argv[1], "r");
    if (!inputFile) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }
    
    char hexValue[20];
    int totalBits, expBits, fracBits, precision;
    
    while (fscanf(inputFile, "%d %d %d %s %d",
                  &totalBits, &expBits, &fracBits, 
                  hexValue, &precision) == 5) {
        processFloatingPoint(hexValue, totalBits, expBits, 
                           fracBits, precision);
    }
    
    fclose(inputFile);
    return EXIT_SUCCESS;
}