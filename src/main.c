/*
Nome dupla: Gabriel Oliveira de Jesus e Elder Nunes Gonçalves
*/

#include <stdio.h>
#include <stdlib.h>

#include "mips_state.h"
#include "simulator.h"
#include "parser.h"
#include "ui.h"

int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("Erro. Uso: %s <arquivo.asm>\n", argv[0]);
        return EXIT_FAILURE;
    }

    MIPS_STATE state;
    inicialize_state(&state);

    load_program(argv[1], &state);

    create_gtk_ui(&state); 

    return EXIT_SUCCESS;
}