#ifndef INSTRUCTION_ENCODER_H
#define INSTRUCTION_ENCODER_H

#include "mips_state.h"

typedef enum { R_TYPE, I_TYPE, J_TYPE } InstructionType;

uint32_t encode_r_type(uint8_t funct, uint8_t shamt, uint8_t rd, uint8_t rs, uint8_t rt);
uint32_t encode_i_type(uint8_t opcode, uint8_t rs, uint8_t rt, int16_t immediate);
uint32_t encode_j_type(uint8_t opcode, uint32_t target);

#endif