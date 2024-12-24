#include "second.h"

void processFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    int numVars;
    fscanf(file, "%d", &numVars);

    if (numVars > MAX_VARS) {
        fprintf(stderr, "Error: Too many variables. Maximum supported is %d.\n", MAX_VARS);
        fclose(file);
        return;
    }

    size_t size = (1 << numVars); 
    bool *values = malloc(size * sizeof(bool));
    if (!values) {
        perror("Memory allocation failed");
        fclose(file);
        return;
    }

    for (size_t i = 0; i < size; i++) {
        fscanf(file, "%d", (int *)&values[i]);
    }

    for (size_t i = 0; i < size; i++) {
        printf("Value[%zu]: %d\n", i, values[i]);
    }

    free(values);
    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    processFile(argv[1]);
    return 0;
}