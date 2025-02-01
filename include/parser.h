#ifndef PARSER_H
#define PARSER_H

#include "mips_state.h"

typedef void (*DataHandler)(MIPS_STATE*, const char*, int*);

typedef struct {
    const char* directive;
    DataHandler handler;
}DirectiveEntry;

static void process_data_line(MIPS_STATE *state, char *token, int *data_address);
static void process_text_line(MIPS_STATE *state, char *token);
void load_program(const char *filename, MIPS_STATE *state);

#endif