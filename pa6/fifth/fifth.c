#include "fifth.h"

void convertAndPrint(const char *input, int bits, char srcFormat, char destFormat) {
    unsigned int mask = (1U << bits) - 1;  

    if (srcFormat == 'u' && destFormat == 's') {
        unsigned int uvalue = strtoul(input, NULL, 10) & mask;
        if (uvalue & (1U << (bits - 1))) {
            int svalue = -(int)((~uvalue + 1) & mask);
            printf("%d\n", svalue);
        } else {
            printf("%d\n", (int)uvalue);
        }
    } else if (srcFormat == 's' && destFormat == 'u') {
        int svalue = strtol(input, NULL, 10);
        unsigned int uvalue = ((unsigned int)svalue) & mask;
        printf("%u\n", uvalue);
    }
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

    int bits;
    char srcFormat, destFormat;
    char input[64];
    
    while (fscanf(file, "%s %d %c %c", input, &bits, &srcFormat, &destFormat) == 4) {
        convertAndPrint(input, bits, srcFormat, destFormat);
    }

    fclose(file);
    return EXIT_SUCCESS;
}