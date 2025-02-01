#include "instruction_encoder.h"

uint32_t encode_r_type(uint8_t funct, uint8_t shamt, uint8_t rd, uint8_t rs, uint8_t rt) {
    rd &= 0x1F;  // Ensure rd is 5 bits
    rs &= 0x1F;  // Ensure rS is 5 bits
    rt &= 0x1F;  // Ensure rt is 5 bits

    return (0 << 26) | (rs << 21) | (rt << 16) | (rd << 8) | (shamt << 6) | funct; 
}

uint32_t encode_i_type(uint8_t opcode, uint8_t rs, uint8_t rt, int16_t immediate) {
    rs &= 0x1F;  // Ensure rs is 5 bits
    rt &= 0x1F;  // Ensure rt is 5 bits

    return (opcode << 26) | (rs << 21) | (rt << 16) | (immediate & 0xFFFF);
}

uint32_t encode_j_type(uint8_t opcode, uint32_t target) {
    return (opcode << 26) | ((target >> 2) & 0x03FFFFFF);
}
