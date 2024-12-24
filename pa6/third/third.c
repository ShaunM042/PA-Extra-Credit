#include "third.h"

void processFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char type[MAX_NAME_LEN], input1[MAX_NAME_LEN], input2[MAX_NAME_LEN] = "", output[MAX_NAME_LEN];
        if (sscanf(line, "%s %s %s %s", type, input1, input2, output) == 4 ||
            sscanf(line, "%s %s %s", type, input1, output) == 3) {
            strcpy(gates[gateCount].type, type);
            strcpy(gates[gateCount].inputs[0], input1);
            strcpy(gates[gateCount].inputs[1], input2);
            strcpy(gates[gateCount].output, output);
            gates[gateCount].used = false;
            gateCount++;
        }
    }
    fclose(file);
    simplifyCircuit();
    printCircuit();
}

void simplifyCircuit() {
    applyAssociativityAndCommutativity();
    applyDistributivity();
}

void applyAssociativityAndCommutativity() {
    for (int i = 0; i < gateCount; i++) {
        if (gates[i].used) continue;  

        for (int j = 0; j < gateCount; j++) {
            if (i != j && !gates[j].used &&
                strcmp(gates[i].type, gates[j].type) == 0 &&
                strcmp(gates[i].output, gates[j].inputs[0]) == 0) {
                strcpy(gates[j].inputs[0], gates[i].inputs[0]);  
                gates[i].used = true;  
            }
        }
    }
}

void applyDistributivity() {
    for (int i = 0; i < gateCount; i++) {
        if (gates[i].used) continue;
        if (strcmp(gates[i].type, "AND") == 0) {
            for (int j = 0; j < gateCount; j++) {
                if (!gates[j].used && strcmp(gates[j].type, "OR") == 0 &&
                    (strcmp(gates[i].inputs[0], gates[j].inputs[0]) == 0 ||
                     strcmp(gates[i].inputs[0], gates[j].inputs[1]) == 0)) {
                    strcpy(gates[j].type, "AND");
                    strcpy(gates[j].inputs[1], gates[j].output);  
                    strcpy(gates[j].output, gates[i].output);  
                    gates[i].used = true;
                }
            }
        }
    }
}

void printCircuit() {
    printf("Simplified Circuit:\n");
    for (int i = 0; i < gateCount; i++) {
        if (!gates[i].used) {  
            printf("%s %s %s %s\n", gates[i].type, gates[i].inputs[0], gates[i].inputs[1], gates[i].output);
        }
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }
    processFile(argv[1]);
    return EXIT_SUCCESS;
}