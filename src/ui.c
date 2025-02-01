#include "ui.h"
#include "display.h"
#include "simulator.h"

UI_State ui;

// Next Button
static void on_next_clicked(GtkWidget *widget, gpointer data) {
    MIPS_STATE *state = (MIPS_STATE *)data;
    if(state->pc < state->instruction_count) {
        execute_instruction(state, state->pc); 
        state->pc++; 
        update_ui(state, &ui); 
    }
}

// Run all Button
static void on_run_all_clicked(GtkWidget *widget, gpointer data) {
    MIPS_STATE *state = (MIPS_STATE *)data;
    while(state->pc < state->instruction_count) {
        execute_instruction(state, state->pc);
        state->pc++;
        update_ui(state, &ui);
        while(gtk_events_pending()) gtk_main_iteration();
    }
}

// Update User interface
void update_ui(MIPS_STATE *state, UI_State *ui) {
    // Update Registers
    GtkTextIter start_reg, end_reg;
    gtk_text_buffer_get_bounds(ui->registers_buffer, &start_reg, &end_reg);
    gtk_text_buffer_delete(ui->registers_buffer, &start_reg, &end_reg);
    

    // Print registers on the left side
    for(int i = 0; i < NUM_REGISTERS; i++) {
        char line[100];
        snprintf(line, sizeof(line), "%-6s: 0x%08X (%d)\n", 
                REGISTER_NAMES[i], 
                state->registers[i], 
                state->registers[i]);
        gtk_text_buffer_insert(ui->registers_buffer, &start_reg, line, -1);
    }

    int last_pc = state->pc - 1;
    if(last_pc >= 0 && last_pc < state->instruction_count) {
        INSTRUCTION *inst = &state->instructions[last_pc];
        
        // Update and print Binary
        char binary_text[33];
        snprintf(binary_text, sizeof(binary_text), "%032b", inst->binary);
        gtk_label_set_text(GTK_LABEL(ui->binary_label), binary_text);
    }

    // Print executed instruction 
    if(state->pc > 0) {
        GtkTextIter iter;
        INSTRUCTION *inst = &state->instructions[state->pc - 1];
        
        gtk_text_buffer_get_end_iter(ui->instructions_buffer, &iter);
        char line[100];
        snprintf(line, sizeof(line), "PC %d: %s %s, %s, %s\n",
                state->pc - 1,
                inst->opcode,
                inst->operands[0],
                inst->operands[1],
                inst->operands[2]);
        gtk_text_buffer_insert(ui->instructions_buffer, &iter, line, -1);
    }
}

void create_gtk_ui(MIPS_STATE *state) {
    gtk_init(NULL, NULL);

    // Main window
    ui.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(ui.window), "MIPS Simulator");
    gtk_window_set_default_size(GTK_WINDOW(ui.window), 800, 600);

    // close event
    g_signal_connect(ui.window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    // ------------------------

    // Main Layout
    GtkWidget *paned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    
    // Registers Painel
    GtkWidget *left_panel = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    
    // Registers
    GtkWidget *scrolled_reg = gtk_scrolled_window_new(NULL, NULL);
    ui.registers_view = gtk_text_view_new();
    ui.registers_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(ui.registers_view));
    gtk_text_view_set_editable(GTK_TEXT_VIEW(ui.registers_view), FALSE);
    gtk_container_add(GTK_CONTAINER(scrolled_reg), ui.registers_view);
    

    // Right Painel, binary and instructions
    GtkWidget *right_panel = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    ui.instruction_label = gtk_label_new("");
    ui.binary_label = gtk_label_new("");

    GtkWidget *instructions_frame = gtk_frame_new("Instruções Executadas");
    GtkWidget *scrolled_instructions = gtk_scrolled_window_new(NULL, NULL);
    ui.instructions_view = gtk_text_view_new();
    ui.instructions_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(ui.instructions_view));
    gtk_text_view_set_editable(GTK_TEXT_VIEW(ui.instructions_view), FALSE);
    
    gtk_container_add(GTK_CONTAINER(scrolled_instructions), ui.instructions_view);
    gtk_container_add(GTK_CONTAINER(instructions_frame), scrolled_instructions);
    
    // Separete box instruction, binary, and text
    gtk_box_pack_start(GTK_BOX(right_panel), instructions_frame, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(right_panel), ui.instruction_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(right_panel), ui.binary_label, FALSE, FALSE, 0);

    // Output area
    GtkWidget *output_frame = gtk_frame_new("Saída do Programa");
    GtkWidget *scrolled_output = gtk_scrolled_window_new(NULL, NULL);
    ui.output_view = gtk_text_view_new();
    ui.output_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(ui.output_view));
    gtk_text_view_set_editable(GTK_TEXT_VIEW(ui.output_view), FALSE);
    
    gtk_container_add(GTK_CONTAINER(scrolled_output), ui.output_view);
    gtk_container_add(GTK_CONTAINER(output_frame), scrolled_output);
    gtk_box_pack_start(GTK_BOX(right_panel), output_frame, TRUE, TRUE, 0);
    
    // Buttons
    GtkWidget *button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    GtkWidget *btn_next = gtk_button_new_with_label("Próxima Instrução");
    GtkWidget *btn_run = gtk_button_new_with_label("Executar Tudo");
    GtkWidget *btn_exit = gtk_button_new_with_label("Sair");

    // Layout
    gtk_paned_add1(GTK_PANED(paned), left_panel);
    gtk_paned_add2(GTK_PANED(paned), right_panel);
    
    gtk_box_pack_start(GTK_BOX(left_panel), scrolled_reg, TRUE, TRUE, 0);

    
    gtk_box_pack_start(GTK_BOX(right_panel), ui.instruction_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(right_panel), ui.binary_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(right_panel), button_box, FALSE, FALSE, 0);
    
    gtk_box_pack_start(GTK_BOX(button_box), btn_next, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), btn_run, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), btn_exit, TRUE, TRUE, 0);

    // What each button do
    g_signal_connect(btn_next, "clicked", G_CALLBACK(on_next_clicked), state);
    g_signal_connect(btn_run, "clicked", G_CALLBACK(on_run_all_clicked), state);
    g_signal_connect(btn_exit, "clicked", G_CALLBACK(gtk_main_quit), ui.window);

    gtk_container_add(GTK_CONTAINER(ui.window), paned);
    gtk_widget_show_all(ui.window);
    
    update_ui(state, &ui);
    gtk_main();
}