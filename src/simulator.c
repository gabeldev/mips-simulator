#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "simulator.h"
#include "utils.h"
#include "instruction_encoder.h"
#include "display.h"
#include "ui.h"

extern UI_State ui;

static void handle_add(MIPS_STATE *state, INSTRUCTION *inst) {
    int rd = get_register_number(inst->operands[0]);
    int rs = get_register_number(inst->operands[1]);
    int rt = get_register_number(inst->operands[2]);
    state->registers[rd] = state->registers[rs] + state->registers[rt];

    inst->binary = encode_r_type(0x20, 0, rd, rs, rt);
}

static void handle_addi(MIPS_STATE *state, INSTRUCTION *inst) {
    int rt = get_register_number(inst->operands[0]);
    int rs = get_register_number(inst->operands[1]);
    int16_t imm = atoi(inst->operands[2]);
    state->registers[rt] = state->registers[rs] + imm;

    inst->binary = encode_i_type(0x08, rs, rt, imm);
}

static void handle_sub(MIPS_STATE *state, INSTRUCTION *inst) {
    int rd = get_register_number(inst->operands[0]);
    int rs = get_register_number(inst->operands[1]);
    int rt = get_register_number(inst->operands[2]);
    state->registers[rd] = state->registers[rs] - state->registers[rt];

    inst->binary = encode_r_type(0x22, 0, rd, rs, rt);
}

static void handle_mult(MIPS_STATE *state, INSTRUCTION *inst) {
    int rd = get_register_number(inst->operands[0]);
    int rs = get_register_number(inst->operands[1]);
    int rt = get_register_number(inst->operands[2]);
    state->registers[rd] = state->registers[rs] * state->registers[rt];

    inst->binary = encode_r_type(0x18, 0, rd, rs, rt);
}

static void handle_and(MIPS_STATE *state, INSTRUCTION *inst) {
    int rd = get_register_number(inst->operands[0]);
    int rs = get_register_number(inst->operands[1]);
    int rt = get_register_number(inst->operands[2]);

    state->registers[rd] = state->registers[rs] & state->registers[rt];

    inst->binary = encode_r_type(0x24, 0, rd, rs, rt);
}

static void handle_or(MIPS_STATE *state, INSTRUCTION *inst) {
    int rd = get_register_number(inst->operands[0]);
    int rs = get_register_number(inst->operands[1]);
    int rt = get_register_number(inst->operands[2]);

    state->registers[rd] = state->registers[rs] | state->registers[rt];
    inst->binary = encode_r_type(0x25, 0, rd, rs, rt);
}

static void handle_sll(MIPS_STATE *state, INSTRUCTION *inst) {
    int rd = get_register_number(inst->operands[0]);
    int rt = get_register_number(inst->operands[1]);
    int shamt = atoi(inst->operands[2]);

    state->registers[rd] = state->registers[rt] << shamt;
    inst->binary = encode_r_type(0x00, shamt, rd, 0, rt);
}

static void handle_slt(MIPS_STATE *state, INSTRUCTION *inst) {
    int rd = get_register_number(inst->operands[0]);
    int rs = get_register_number(inst->operands[1]);
    int rt = get_register_number(inst->operands[2]);

    state->registers[rd] = state->registers[rt] < state->registers[rs] ? 1 : 0;
    inst->binary = encode_r_type(0x2A, 0, rd, rs, rt);
}

static void handle_slti(MIPS_STATE *state, INSTRUCTION *inst) {
    int rt = get_register_number(inst->operands[0]);
    int rs = get_register_number(inst->operands[1]);
    int16_t imm = atoi(inst->operands[2]);

    state->registers[rt] = state->registers[rs] < imm ? 1 : 0;
    inst->binary = encode_i_type(0x0A, rs, rt, imm);
}

static void handle_lui(MIPS_STATE *state, INSTRUCTION *inst) {
    int rt = get_register_number(inst->operands[0]);
    int16_t imm = atoi(inst->operands[1]);

    state->registers[rt] = imm << 16;
    inst->binary = encode_i_type(0x0F, 0, rt, imm);
}

static void handle_lw(MIPS_STATE *state, INSTRUCTION *inst) {
    int rt = get_register_number(inst->operands[0]);

    // Parse
    uint16_t offset;
    char base_reg[10];
    parse_offset(inst->operands[1], &offset, base_reg);
    int rs = get_register_number(base_reg);

    // Effective address
    uint32_t effective_address = state->registers[rs] + offset;

    if (effective_address % 4 != 0 || effective_address >= MEMORY_SIZE - 3) {
        fprintf(stderr, "Erro (lw): Acesso inválido à memória em 0x%08X\n", effective_address);
        exit(EXIT_FAILURE);
    }

    // Read in memory (big-endian)
    state->registers[rt] = read_big_endian(state->memory, effective_address);
    inst->binary = encode_i_type(0x23, rs, rt, offset);
}

static void handle_sw(MIPS_STATE *state, INSTRUCTION *inst) {
    int rt = get_register_number(inst->operands[0]);

    // Parse
    uint16_t offset;
    char base_reg[10];
    parse_offset(inst->operands[1], &offset, base_reg);
    int rs = get_register_number(base_reg);

    // Effective address
    uint32_t effective_address = state->registers[rs] + offset;

    if (effective_address % 4 != 0 || effective_address >= MEMORY_SIZE - 3) {
        fprintf(stderr, "Erro (sw): Acesso inválido à memória em 0x%08X\n", effective_address);
        exit(EXIT_FAILURE);
    }

    // Write in memory (big-endian)
    write_big_endian(state->memory, effective_address, state->registers[rt]);
    inst->binary = encode_i_type(0x2B, rs, rt, offset);
}

static void handle_li(MIPS_STATE *state, INSTRUCTION *inst) {
    int rt = get_register_number(inst->operands[0]);
    int16_t imm = atoi(inst->operands[1]);
    state->registers[rt] = imm;

    inst->binary = encode_i_type(0x08, 0, rt, imm);
}

static void handle_la(MIPS_STATE *state, INSTRUCTION *inst) {
    int rt = get_register_number(inst->operands[0]);

        char label[50];
        strncpy(label, inst->operands[1], sizeof(label) - 1);
        label[sizeof(label) - 1] = '\0';

        int address = find_label_address(state, label);

       
        state->registers[rt] = address;
        inst->binary = encode_i_type(0x08, 0, rt, address);
}   

static void handle_j(MIPS_STATE *state, INSTRUCTION *inst) {
    // Extract operand Label
    char *target_label = inst->operands[0];

    // Search Label Address
    int target_address = find_label_address(state, target_label);

    if(target_address != -1) {
        state->pc = target_address;
    } else {
        fprintf(stderr, "Erro: Label '%s' não encontrada.\n", target_label);
        exit(EXIT_FAILURE);
    }
    inst->binary = encode_j_type(0x02, target_address);
}

static void handle_jal(MIPS_STATE *state, INSTRUCTION *inst) {
    // Extract operand Label
    char *target_label = inst->operands[0];

    // Search Label Address
    int target_address = find_label_address(state, target_label);

    if(target_address != -1) {
        state->pc = target_address;
    } else {
        fprintf(stderr, "Erro: Label '%s' não encontrada.\n", target_label);
        exit(EXIT_FAILURE);
    }


    //Save pc + 4 in $ra
    state->registers[31] = (state->pc + 1) * 4; // get the real address in memory
    inst->binary = encode_j_type(0x03, target_address);
}

static void handle_syscall(MIPS_STATE *state, INSTRUCTION *inst) {
    switch (state->registers[2]) {
        case 1: { // Print integer
            GtkTextIter iter;
            gtk_text_buffer_get_end_iter(ui.output_buffer, &iter);
            char output[50];
            snprintf(output, sizeof(output), "%d", state->registers[4]);
            gtk_text_buffer_insert(ui.output_buffer, &iter, output, -1);
            break;
        }
        case 4: { // Print string
            GtkTextIter iter;
            gtk_text_buffer_get_end_iter(ui.output_buffer, &iter);
            char *str = (char*)&state->memory[state->registers[4]];
            gtk_text_buffer_insert(ui.output_buffer, &iter, str, -1);
            break;
        }
        case 5: // Read Integer
            scanf("%d", &state->registers[2]);
            break;
        case 10: // Exit
            state->pc = state->instruction_count; // Para execução sem fechar janela
            break;
        default:
            fprintf(stderr, "Syscall não implementada: %d\n", state->registers[2]);
            break;
    }
    inst->binary = encode_r_type(0x0C, 0, 0, 0, 0);
}

static InstructionEntry instruction_table[] = {
    {"add", handle_add},
    {"addi", handle_addi},
    {"sub", handle_sub},
    {"mult", handle_mult},
    {"and", handle_and},
    {"or", handle_or},
    {"sll", handle_sll},
    {"slt", handle_slt},
    {"slti", handle_slti},
    {"lui", handle_lui},
    {"lw", handle_lw},
    {"sw", handle_sw},
    {"li", handle_li},
    {"la", handle_la},
    {"j", handle_j},
    {"jal", handle_jal},
    {"syscall", handle_syscall}, 
    {NULL, NULL}
};

void execute_instruction(MIPS_STATE *state, int pc) {
    INSTRUCTION *inst = &state->instructions[pc];
    for(int i = 0; instruction_table[i].opcode != NULL; i++) {
        if(strcmp(inst->opcode, instruction_table[i].opcode) == 0) {
            instruction_table[i].handler(state, inst);
            return;
        }
    }
    printf("Instrução não suportada: %s\n", inst->opcode);
}

void run_simulator(MIPS_STATE *state) {
    char choice;
    int running = 1;

    while (running && state->pc < state->instruction_count) {
        execute_instruction(state, state->pc);
        display_registers(state);
        display_instruction(state, state->pc);
        printf("\nBinário: %032b\n", state->instructions[state->pc].binary);

        printf("\n=== Simulador MIPS ===\n");
        printf("p: Próxima instrução\n");
        printf("r: Executar até o fim\n");
        printf("s: Sair\n");
        printf("Escolha: ");
        scanf(" %c", &choice);
        getchar(); 

        switch (tolower(choice)) {
            case 'p': 
                state->pc++;
                break;

            case 'r': 
                while (state->pc < state->instruction_count) {
                    execute_instruction(state, state->pc);
                    state->pc++;
                }
                running = 0;
                display_registers(state);
                break;

            case 's': 
                running = 0;
                break;

            default:
                printf("Opção inválida!\n");
                break;
        }
    }
}