#ifndef TICKET_FORM_H
#define TICKET_FORM_H

#include <gtk/gtk.h>

typedef struct {
    GtkWidget *page_label;
    GtkGrid *page_grid;
    GtkWidget *entry_title;
    GtkWidget *entry_description;
    GtkWidget *entry_priority;
    GtkWidget *entry_status;
    GtkWidget *entry_start_date;
    GtkWidget *entry_deadline_date;
    GtkWidget *entry_owner;
    GtkWidget *stack;
    GtkWidget *error_label;
    GtkWidget *submit_btn;

} TicketFormPage;

void ticket_form_init(GtkWidget *stack);

#endif // TICKET_FORM_H
