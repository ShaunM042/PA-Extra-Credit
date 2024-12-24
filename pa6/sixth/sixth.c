#include "sixth.h"

void printBinaryFraction(double fraction, int precision) {
    fraction = fabs(fraction);

    if (fraction < 1.0) {
        printf("0.");
        double temp = fraction;
        
        for (int i = 0; i < precision; i++) {
            temp *= 2;
            if (temp >= 1.0) {
                printf("1");
                temp -= 1.0;
            } else {
                printf("0");
            }
        }
        printf(" 0\n"); 
        return;
    }

    int exponent = 0;
    while (fraction >= 2.0) {
        fraction /= 2.0;
        exponent++;
    }
    while (fraction > 0 && fraction < 1.0) {
        fraction *= 2.0;
        exponent--;
    }

    printf("1.");
    fraction -= 1.0;
    
    for (int i = 0; i < precision; i++) {
        fraction *= 2;
        if (fraction >= 1.0) {
            printf("1");
            fraction -= 1.0;
        } else {
            printf("0");
        }
    }
    
    printf(" %d\n", exponent);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <inputfile>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    double fraction;
    int precision;
    while (fscanf(file, "%lf %d", &fraction, &precision) == 2) {
        printBinaryFraction(fraction, precision);
    }

    fclose(file);
    return EXIT_SUCCESS;
}