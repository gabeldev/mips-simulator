#ifndef DISPLAY_H
#define DISPLAY_H

#include "mips_state.h"

void display_registers(const MIPS_STATE *state);
void display_instruction(const MIPS_STATE *state, uint32_t pc);
void display_data_segment(const MIPS_STATE *state);

#endif