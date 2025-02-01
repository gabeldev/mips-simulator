#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parser.h"


// Handler to .word

static void handle_word(MIPS_STATE *state, const char *label, int *data_address) {
    if (state->data_count >= 100) {
        fprintf(stderr, "Erro: Limite de entradas em .data excedido (max 100).\n");
        exit(EXIT_FAILURE);
    }

    strcpy(state->data[state->data_count].name, label);
    state->data[state->data_count].address = *data_address;

    char *token = strtok(NULL, " \t\n");
                    
    while(token != NULL) {
        uint32_t value = (uint32_t) strtoul(token, NULL, 10);
        state->memory[*data_address] = (value >> 24) & 0xFF; // Byte 3
        state->memory[*data_address + 1] = (value >> 16) & 0xFF; // Byte 2
        state->memory[*data_address + 2] = (value >> 8) & 0xFF; // Byte 1
        state->memory[*data_address + 3] = value & 0xFF; // Byte 0
        *data_address += 4;
        token = strtok(NULL, " ,\t\n");
    }

    state->data_count++;
}

// Handler to .asciiz

static void handle_asciiz(MIPS_STATE *state, const char *label, int *data_address) {
    // Check capacity
    if (state->data_count >= 100 || *data_address >= MEMORY_SIZE) {
        fprintf(stderr, "Erro: Limite de .data ou memória excedido.\n");
        exit(EXIT_FAILURE);
    }

    char *token = strtok(NULL, "\n"); // Next token

    // Extract quotation marks
    char *start = strchr(token, '"');
    char *end = strrchr(token, '"');
    if (!start || !end || start >= end) {
        fprintf(stderr, "Erro: String inválida em .asciiz.\n");
        exit(EXIT_FAILURE);
    }

    // Calculate string length
    size_t length = end - start - 1;

    // Verify if there`s space in memory
    if (*data_address + length + 1 > MEMORY_SIZE) {
        fprintf(stderr, "Erro: Memória insuficiente para .asciiz.\n");
        exit(EXIT_FAILURE);
    }

    // copy character by character 
    for (size_t i = 0; i < length; i++) {
        state->memory[*data_address + i] = start[i + 1]; // Copia byte diretamente
    }

    // add null terminator '\0' (0x00)
    state->memory[*data_address + length] = 0x00;

    // Register in .data
    strncpy(state->data[state->data_count].name, label, 49);
    state->data[state->data_count].name[49] = '\0';
    state->data[state->data_count].address = *data_address;

    // update data_address to include null terminator.
    *data_address += length + 1;
    state->data_count++;
}

// Directive Table

static DirectiveEntry data_directives[] = {
    {".word", handle_word},
    {".asciiz", handle_asciiz},
    {NULL, NULL}
};

// Process Data Line
static void process_data_line(MIPS_STATE *state, char *token, int *data_address) {
    if(token[strlen(token) - 1] != ':') return; // There`s no label

    // Get Label

    char label[50];
    strncpy(label, token, sizeof(label) -1);
    label[strlen(token) - 1] = '\0';

    // Get directive 
    token = strtok(NULL, " \t\n");
    if(!token) return;

    // Search for handler
    for(int i = 0; data_directives[i].directive != NULL; i++) {
        if(strcmp(token, data_directives[i].directive) == 0) {
            data_directives[i].handler(state, label, data_address);
            break;
        }
    }
}

static void process_text_line(MIPS_STATE *state, char *token) {
    if(strchr(token, ':')) { // It`s Label
        token[strlen(token) - 1] = '\0';
        strcpy(state->labels[state->label_count].name, token);
        state->labels[state->label_count].address = state->instruction_count;
        state->label_count++;
    } else { // It`s Intruction
        if (state->instruction_count >= MAX_INSTRUCTIONS) {
            fprintf(stderr, "Erro: Limite de instruções excedido (max %d).\n", MAX_INSTRUCTIONS);
            exit(EXIT_FAILURE);
        }

        strcpy(state->instructions[state->instruction_count].opcode, token);
        int op_count = 0;

        while((token = strtok(NULL, ", \t\n")) && op_count < 3) {
            strcpy(state->instructions[state->instruction_count].operands[op_count], token);
            op_count++;
        }
        state->instruction_count++;
    }
}

void load_program(const char *filename, MIPS_STATE *state) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }

    char line[256];
    enum { NONE, DATA_SECTION, TEXT_SECTION } current_section = NONE;
    int data_address = 0;

    while(fgets(line, sizeof(line), file)) {
        char *token = strtok(line, " \t\n");
        if(!token) continue;

        if(strcmp(token, ".data") == 0) {
            current_section = DATA_SECTION;
            continue;
        } else if(strcmp(token, ".text") == 0) {
            current_section = TEXT_SECTION;
            continue;
        }

        switch(current_section) {
            case DATA_SECTION:
                process_data_line(state, token, &data_address);
                break;
            case TEXT_SECTION:
                process_text_line(state, token);
                break;
            default:
                break;
        }
    }
    fclose(file);
}