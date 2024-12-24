#include "fourth.h"

long long convertNumber(int num, int bits, char source, char dest) {
    unsigned long long max = 1;
    for (int i = 0; i < bits; i++) {
        max *= 2;
    }
    max -= 1;
    
    if (source == 'u') {
        unsigned long long unum = (unsigned int)num & max;
        if (dest == 's') {
            unsigned long long signBit = max / 2 + 1;
            if (unum & signBit) {
                return (long long)(unum | ~max);
            }
            return unum;
        }
        return unum;
    } else { 
        if (dest == 'u') {
            return (unsigned long long)num & max;
        }
        return num & max;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <inputfile>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    int number, bits;
    char source, dest;
    
    while (fscanf(file, "%d %d %c %c", &number, &bits, &source, &dest) == 4) {
        printf("%lld\n", convertNumber(number, bits, source, dest));
    }

    fclose(file);
    return EXIT_SUCCESS;
}