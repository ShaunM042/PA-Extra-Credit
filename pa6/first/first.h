#ifndef FIRST_H
#define FIRST_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_VARS 50
#define MAX_NAME_LEN 10
#define MAX_GATE_INPUTS 10

typedef struct {
    char name[MAX_NAME_LEN];
    bool value;
} Variable;

typedef struct {
    char type[MAX_NAME_LEN];
    char inputs[MAX_GATE_INPUTS][MAX_NAME_LEN];
    char output[MAX_NAME_LEN];
    int input_count;
} Gate;

void processFile(const char *filename);
#endif