#ifndef TICKET_FORM_H
#define TICKET_FORM_H

#include <gtk/gtk.h>

typedef struct {
    int year;
    int month;
    int day;
} SelectedDate;

typedef struct {
    GtkWidget *stack;
    GtkGrid *page_grid;
    GtkWidget *page_label;
    GtkWidget *entry_title;
    GtkWidget *ticket_description_textview_container;
    GtkWidget *entry_description;
    GtkWidget *entry_priority;
    GtkWidget *entry_status;
    GtkWidget *entry_project;
    SelectedDate start_date; // Store the selected start date
    SelectedDate deadline_date; // Store the selected deadline date
    GtkWidget *entry_owner;
    GtkWidget *error_label;
    GtkWidget *submit_btn;
    GtkWidget *start_date_label; // Label for displaying the start date
    GtkWidget *deadline_date_label; // Label for displaying the deadline date
    int project_id;
} TicketFormPage;


void ticket_form_init(GtkWidget *stack);

#endif // TICKET_FORM_H
