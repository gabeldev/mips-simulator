#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "mips_state.h"

typedef void (*InstructionHandler)(MIPS_STATE*, INSTRUCTION*);
void execute_instruction(MIPS_STATE *state, int pc);
void run_simulator(MIPS_STATE *state);

typedef struct {
    const char *opcode;
    InstructionHandler handler;
} InstructionEntry;

#endif