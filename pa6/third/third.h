#ifndef THIRD_H
#define THIRD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_GATES 100
#define MAX_VARS 50
#define MAX_NAME_LEN 20

typedef struct {
    char type[MAX_NAME_LEN];
    char inputs[2][MAX_NAME_LEN]; 
    char output[MAX_NAME_LEN];
    bool used;                    
} Gate;

Gate gates[MAX_GATES];
int gateCount = 0;

void processFile(const char *filename);
void simplifyCircuit();
void printCircuit();
void applyAssociativityAndCommutativity();
void applyDistributivity();

#endif