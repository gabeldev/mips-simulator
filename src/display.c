    #include <stdio.h>
    #include <string.h>

    #include "display.h"
    #include "mips_state.h"

    void display_registers(const MIPS_STATE *state) {
        printf("\n=== Registradores ===\n");
        for (int i = 0; i < NUM_REGISTERS; i++) {
            printf("%-6s: 0x%08X (%d)\n", 
                REGISTER_NAMES[i], 
                state->registers[i], 
                state->registers[i]);
        }
    }

    void display_instruction(const MIPS_STATE *state, uint32_t pc) {
        printf("\n=== Instrução Atual (PC = %d) ===\n", pc);
        printf("Endereço: 0x%08X\n", pc * 4);
        printf("Assembly: %s ", state->instructions[pc].opcode);
        for (int i = 0; i < 3; i++) {
            if (strlen(state->instructions[pc].operands[i]) > 0) {
                printf("%s ", state->instructions[pc].operands[i]);
            }
        }
    }

    void displayDataSegment(const MIPS_STATE *state) {
        printf("\n=== Data Segment ===\n");
        for(int i = 0; i < state->data_count; i++) {
            printf("%s (0x%04x): ", state->data[i].name, state->data[i].address);

            if(strchr(&state->memory[state->data[i].address], '\0') != NULL) { // if is string
                printf("\"%s\"\n", &state->memory[state->data[i].address]);
            } else { // if is word
                printf("%d\n", *((int*)&state->memory[state->data[i].address]));
            }
        }
    }