#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "utils.h"

int get_register_number(const char *reg_name) {
    for(int i = 0; i < NUM_REGISTERS; i++) {
        if(strcmp(REGISTER_NAMES[i], reg_name) == 0) {
            return i;
        }
    }

    fprintf(stderr, "Registrador inválido: %s\n", reg_name);
    exit(EXIT_FAILURE);
}

void parse_offset(const char *operand, uint16_t *offset, char *base_reg) {
    char *open_paren = strchr(operand, '(');
    if(!open_paren) return;

    // Extract offset
    sscanf(operand, "%d", offset);

    // Extract base register
    char *close_paren = strchr(operand, ')');
    strncpy(base_reg, open_paren + 1, close_paren - open_paren - 1);
    base_reg[close_paren - open_paren - 1] = '\0';
}

uint32_t read_big_endian(const uint8_t *memory, uint32_t address) {
    return (memory[address] << 24) |
           (memory[address + 1] << 16) |
           (memory[address + 2] << 8) |
           (memory[address + 3]);
}

void write_big_endian(uint8_t *memory, uint32_t address, uint32_t value) {
    memory[address] = (value >> 24) & 0xFF;
    memory[address + 1] = (value >> 16) & 0xFF;
    memory[address + 2] = (value >> 8) & 0xFF;
    memory[address + 3] = value & 0xFF;
}

int find_label_address(MIPS_STATE *state, const char *label) {
    for (int i = 0; i < state->data_count; i++) {
        if (strcmp(state->data[i].name, label) == 0) {
            return state->data[i].address;
        }
    }
    fprintf(stderr, "Label inválido: %s\n", label);
    exit(EXIT_FAILURE);
}