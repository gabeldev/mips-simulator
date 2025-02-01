#include "mips_state.h"
#include <string.h>

const char* REGISTER_NAMES[NUM_REGISTERS] = {
    "$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3",
    "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
    "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
    "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"
};

void inicialize_state(MIPS_STATE *state) {
    memset(state->registers, 0, sizeof(state->registers));
    memset(state->memory, 0, sizeof(state->memory));
    memset(state->labels, 0, sizeof(state->labels));
    memset(state->data, 0, sizeof(state->data));
    memset(state->instructions, 0, sizeof(state->instructions));
    state->label_count = 0;
    state->data_count = 0;
    state->instruction_count = 0;
    state->pc = 0;
}