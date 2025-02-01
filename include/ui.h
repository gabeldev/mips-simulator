#ifndef UI_H
#define UI_H

#include <gtk/gtk.h>
#include "mips_state.h"

typedef struct {
    // Componentes principais
    GtkWidget *window;
    GtkWidget *paned;
    // Registers
    GtkWidget *registers_view;
    GtkTextBuffer *registers_buffer;
    // Data Segmentation
    GtkWidget *data_view;
    GtkTextBuffer *data_buffer;
    // Controll
    GtkWidget *instruction_label;
    GtkWidget *binary_label;    
    // Output
    GtkWidget *output_view;
    GtkTextBuffer *output_buffer;
    // Instructions control
    GtkWidget *instructions_view;  
    GtkTextBuffer *instructions_buffer; 
} UI_State;

extern UI_State ui;

void create_gtk_ui(MIPS_STATE *state);
void update_ui(MIPS_STATE *state, UI_State *ui);

#endif