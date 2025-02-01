#ifndef UTILS_H
#define UTILS_H

#include "mips_state.h"
#include <string.h>

int get_register_number(const char *reg_name);
void parse_offset(const char *operand, uint16_t *offset, char *base_reg);
uint32_t read_big_endian(const uint8_t *memory, uint32_t address);
void write_big_endian(uint8_t *memory, uint32_t address, uint32_t value);
int find_label_address(MIPS_STATE *state, const char *label);

#endif