#ifndef PROJECT_FORM_H
#define PROJECT_FORM_H

#include <gtk/gtk.h>

typedef struct {
    int year;
    int month;
    int day;
} ProjectDate;

typedef struct {
    GtkWidget *stack;
    GtkGrid *page_grid;
    GtkWidget *page_label;
    GtkWidget *entry_title;
    GtkWidget *ticket_description_textview_container;
    GtkWidget *entry_description;
    ProjectDate start_date; // Store the selected start date
    ProjectDate deadline_date; // Store the selected deadline date
    GtkWidget *entry_leader;
    GtkWidget *error_label;
    GtkWidget *submit_btn;
    GtkWidget *start_date_label; // Label for displaying the start date
    GtkWidget *deadline_date_label; // Label for displaying the deadline date
    int project_id;
} ProjectFormPage;

void project_form_init(GtkWidget *stack);

#endif // PROJECT_FORM_H
