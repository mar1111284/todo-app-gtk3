#include "../include/ticket_form.h"

void ticket_form_init(GtkWidget *stack) {
    GtkWidget *ticket_form_page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    GtkWidget *label = gtk_label_new("Ticket Form");
    gtk_box_pack_start(GTK_BOX(ticket_form_page), label, FALSE, FALSE, 0);

    gtk_stack_add_titled(GTK_STACK(stack), ticket_form_page, "ticket_form", "Ticket Form");
}
