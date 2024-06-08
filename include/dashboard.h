#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <gtk/gtk.h>

typedef struct {
    GtkWidget *dashboard_label;
    GtkGrid *dashboard_grid;
    GtkWidget *projects_dropdown;
    GtkWidget *todo_label;
    GtkWidget *in_progress_label;
    GtkWidget *pending_label;
    GtkWidget *done_label;
    GtkWidget *stack;

    // Column
    GtkWidget *todo_column;
    GtkWidget *progress_column;
    GtkWidget *pending_column;
    GtkWidget *done_column;

    // Container
    GtkWidget *todo_ticket_container;
    GtkWidget *progress_ticket_container;
    GtkWidget *pending_ticket_container;
    GtkWidget *done_ticket_container;

    // Button
    GtkWidget *new_project_button;
    GtkWidget *add_button_todo;
    GtkWidget *add_button_progress;
    GtkWidget *add_button_pending;
    GtkWidget *add_button_done;





} DashboardPage;

void dashboard_init(GtkWidget *stack);

#endif // DASHBOARD_H
