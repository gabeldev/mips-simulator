#ifndef MIPS_STATE_H
#define MIPS_STATE_H

#include <stdint.h>

#define NUM_REGISTERS 32
#define MEMORY_SIZE 4096
#define MAX_INSTRUCTIONS 100

extern const char* REGISTER_NAMES[NUM_REGISTERS];

typedef struct {
    char name[50];
    uint32_t address;
} LABEL;

typedef struct {
    char name[50];
    uint32_t address;
} DATA;

typedef struct {
    char opcode[10];
    char operands[3][10];
    uint32_t binary;
} INSTRUCTION; 

typedef struct {
    uint32_t registers[NUM_REGISTERS];
    uint8_t memory[MEMORY_SIZE];
    LABEL labels[100];
    DATA data[100];
    INSTRUCTION instructions[MAX_INSTRUCTIONS];
    int label_count;
    int data_count;
    int instruction_count;
    uint32_t pc;
} MIPS_STATE;


//inicialize state
void inicialize_state(MIPS_STATE *state);


#endif