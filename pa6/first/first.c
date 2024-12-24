#include "first.h"

Variable vars[MAX_VARS];
int var_count = 0;
Gate gates[MAX_VARS];
int gate_count = 0;
bool expected_outputs[16][65536];
int num_inputs = 0;
int num_outputs = 0;
char output_names[MAX_VARS][MAX_NAME_LEN];
int output_count = 0;
char assigned_types[MAX_VARS][MAX_NAME_LEN];

bool evaluateGate(Gate* gate) {
    bool inputs[MAX_GATE_INPUTS] = {false};
    
    if (strcmp(gate->type, "MULTIPLEXER") == 0) {
        bool data_inputs[4] = {false};  
        bool select_inputs[2] = {false}; 
        
        for (int i = 0; i < 4; i++) {
            if (gate->inputs[i][0] == '0') data_inputs[i] = false;
            else if (gate->inputs[i][0] == '1') data_inputs[i] = true;
            else {
                for (int j = 0; j < var_count; j++) {
                    if (strcmp(vars[j].name, gate->inputs[i]) == 0) {
                        data_inputs[i] = vars[j].value;
                        break;
                    }
                }
            }
        }
        
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < var_count; j++) {
                if (strcmp(vars[j].name, gate->inputs[i + 4]) == 0) {
                    select_inputs[i] = vars[j].value;
                    break;
                }
            }
        }
        
        int select = (select_inputs[0] ? 1 : 0) | ((select_inputs[1] ? 1 : 0) << 1);
        return data_inputs[select];
    }
    
    for (int i = 0; i < gate->input_count; i++) {
        if (gate->inputs[i][0] == '0') inputs[i] = false;
        else if (gate->inputs[i][0] == '1') inputs[i] = true;
        else {
            for (int j = 0; j < var_count; j++) {
                if (strcmp(vars[j].name, gate->inputs[i]) == 0) {
                    inputs[i] = vars[j].value;
                    break;
                }
            }
        }
    }
    
    if (strcmp(gate->type, "AND") == 0) {
        bool result = true;
        for (int i = 0; i < gate->input_count; i++) {
            result = result && inputs[i];
        }
        return result;
    } else if (strcmp(gate->type, "OR") == 0) {
        bool result = false;
        for (int i = 0; i < gate->input_count; i++) {
            result = result || inputs[i];
        }
        return result;
    } else if (strcmp(gate->type, "XOR") == 0) {
        return inputs[0] != inputs[1];
    } else if (strcmp(gate->type, "NOT") == 0) {
        return !inputs[0];
    }
    return false;
}

void setInputValues(int permutation) {
    for (int i = 0; i < num_inputs; i++) {
        vars[i].value = (permutation >> i) & 1;
    }
}

void simulateCircuit() {
    for (int i = 0; i < gate_count; i++) {
        bool result = evaluateGate(&gates[i]);
        
        for (int j = 0; j < var_count; j++) {
            if (strcmp(vars[j].name, gates[i].output) == 0) {
                vars[j].value = result;
                break;
            }
        }
    }
}

bool checkOutputs() {
    int max_combinations = 1 << num_inputs;
    
    for (int perm = 0; perm < max_combinations; perm++) {
        setInputValues(perm);
        simulateCircuit();
        
        for (int i = 0; i < output_count; i++) {
            bool found = false;
            bool actual_value = false;
            
            for (int j = 0; j < var_count; j++) {
                if (strcmp(vars[j].name, output_names[i]) == 0) {
                    actual_value = vars[j].value;
                    found = true;
                    break;
                }
            }
            
            if (!found || actual_value != expected_outputs[i][perm]) {
                return false;
            }
        }
    }
    return true;
}

bool tryGateAssignment(int gate_index) {
    if (gate_index >= gate_count) {
        return checkOutputs();
    }
    
    if (gates[gate_index].type[0] != 'G') {
        return tryGateAssignment(gate_index + 1);
    }
    
    const char* gate_types[] = {"OR", "AND", "XOR", "NOT"};
    int num_types = 4;
    
    if (gates[gate_index].input_count > 1) {
        num_types--;
    }
    
    for (int i = 0; i < num_types; i++) {
        strcpy(gates[gate_index].type, gate_types[i]);
        strcpy(assigned_types[gate_index], gate_types[i]);
        
        if (tryGateAssignment(gate_index + 1)) {
            return true;
        }
    }
    
    char temp[MAX_NAME_LEN];
    snprintf(temp, MAX_NAME_LEN, "G%d", gate_index + 1);
    strcpy(gates[gate_index].type, temp);
    return false;
}

void processFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) return;
    
    char line[256];
    bool reading_outputval = false;
    int output_index = 0;
    
    while (fgets(line, sizeof(line), file)) {
        char command[20];
        if (sscanf(line, "%s", command) != 1) continue;
        
        if (strcmp(command, "INPUTVAR") == 0) {
            sscanf(line, "%*s %d", &num_inputs);
            char *ptr = strchr(line, ' ');
            if (!ptr) continue;
            ptr = strchr(ptr + 1, ' ');
            if (!ptr) continue;
            ptr++;
            
            char varName[MAX_NAME_LEN];
            for (int i = 0; i < num_inputs && sscanf(ptr, "%s", varName) == 1; i++) {
                strncpy(vars[var_count].name, varName, MAX_NAME_LEN - 1);
                vars[var_count++].value = false;
                while (*ptr && *ptr != ' ' && *ptr != '\n') ptr++;
                while (*ptr && (*ptr == ' ' || *ptr == '\n')) ptr++;
            }
        }
        else if (strcmp(command, "OUTPUTVAR") == 0) {
            sscanf(line, "%*s %d", &num_outputs);
            char *ptr = strchr(line, ' ');
            if (!ptr) continue;
            ptr = strchr(ptr + 1, ' ');
            if (!ptr) continue;
            ptr++;
            
            char varName[MAX_NAME_LEN];
            for (int i = 0; i < num_outputs && sscanf(ptr, "%s", varName) == 1; i++) {
                strncpy(output_names[output_count], varName, MAX_NAME_LEN - 1);
                strncpy(vars[var_count].name, varName, MAX_NAME_LEN - 1);
                vars[var_count++].value = false;
                output_count++;
                while (*ptr && *ptr != ' ' && *ptr != '\n') ptr++;
                while (*ptr && (*ptr == ' ' || *ptr == '\n')) ptr++;
            }
        }
        else if (strcmp(command, "OUTPUTVAL") == 0) {
            reading_outputval = true;
            output_index = 0;
        }
        else if (reading_outputval && output_index < output_count) {
            char* ptr = strchr(line, ' ');
            if (ptr) {
                ptr++;
                int val_index = 0;
                while (*ptr && *ptr != '\n' && val_index < 65536) {
                    if (*ptr == '0' || *ptr == '1') {
                        expected_outputs[output_index][val_index++] = (*ptr == '1');
                    }
                    ptr++;
                }
            }
            output_index++;
        }
        else if (strcmp(command, "MULTIPLEXER") == 0) {
            Gate gate = {0};
            strcpy(gate.type, "MULTIPLEXER");
            
            int num_inputs;
            sscanf(line, "%*s %d", &num_inputs);
            gate.input_count = num_inputs + 2; 
            
            char* token = strtok(line, " \n");
            token = strtok(NULL, " \n"); 
            
            for (int i = 0; i < gate.input_count && token; i++) {
                token = strtok(NULL, " \n");
                if (token) strncpy(gate.inputs[i], token, MAX_NAME_LEN - 1);
            }
            
            token = strtok(NULL, " \n");
            if (token) strncpy(gate.output, token, MAX_NAME_LEN - 1);
            
            gates[gate_count++] = gate;
        }
        else if (strncmp(command, "G", 1) == 0 || 
                strcmp(command, "AND") == 0 || 
                strcmp(command, "OR") == 0 ||
                strcmp(command, "XOR") == 0 || 
                strcmp(command, "NOT") == 0) {
            
            Gate gate = {0};
            strncpy(gate.type, command, MAX_NAME_LEN - 1);
            
            if (gate.type[0] == 'G') {
                char* token = strtok(line, " \n");
                token = strtok(NULL, " \n");
                
                if (token) {
                    gate.input_count = atoi(token);
                    
                    for (int i = 0; i < gate.input_count && i < MAX_GATE_INPUTS; i++) {
                        token = strtok(NULL, " \n");
                        if (token) strncpy(gate.inputs[i], token, MAX_NAME_LEN - 1);
                    }
                    
                    token = strtok(NULL, " \n");
                    if (token) strncpy(gate.output, token, MAX_NAME_LEN - 1);
                }
            } else {
                if (strcmp(gate.type, "AND") == 0) {
                    int declared_count = 0;
                    char* token = strtok(line, " \n");
                    token = strtok(NULL, " \n");
                    
                    if (token) {
                        declared_count = atoi(token);
                        if (declared_count != 2) { 
                            fclose(file);
                            printf("INVALID\n");
                            exit(0);
                        }
                    }
                    gate.input_count = 2;
                } else if (strcmp(gate.type, "NOT") == 0) {
                    gate.input_count = 1;
                } else {
                    gate.input_count = 2;
                }
                
                char* token = strtok(line, " \n");
                token = strtok(NULL, " \n"); 
                
                for (int i = 0; i < gate.input_count && i < MAX_GATE_INPUTS; i++) {
                    token = strtok(NULL, " \n");
                    if (token) strncpy(gate.inputs[i], token, MAX_NAME_LEN - 1);
                }
                
                token = strtok(NULL, " \n");
                if (token) strncpy(gate.output, token, MAX_NAME_LEN - 1);
            }
            
            gates[gate_count++] = gate;
        }
    }
    
    fclose(file);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    processFile(argv[1]);
    
    if (!tryGateAssignment(0)) {
        printf("INVALID\n");
    } else {
        for (int i = 0; i < gate_count; i++) {
            if (gates[i].type[0] == 'G') {
                int gate_num;
                sscanf(gates[i].type + 1, "%d", &gate_num);
                printf("G%d %s\n", gate_num, assigned_types[i]);
            }
        }
    }
    
    return EXIT_SUCCESS;
}