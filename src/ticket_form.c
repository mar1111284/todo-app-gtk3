#include "../include/ticket_form.h"
#include <gtk/gtk.h>
#include <time.h>

static TicketFormPage ticket_form_page;

void ticket_form_init(GtkWidget *stack) {
    GtkBuilder *builder = gtk_builder_new();
    GError *error = NULL;

    // Check for 
    if (!gtk_builder_add_from_file(builder, "ui/ticket_form.ui", &error)) {
        g_warning("Error loading UI file: %s", error->message);
        g_error_free(error);
        return;
    }

    // Fill structure 
    ticket_form_page.stack = stack;
    ticket_form_page.page_grid = GTK_WIDGET(gtk_builder_get_object(builder, "page_grid"));
    ticket_form_page.page_label= GTK_WIDGET(gtk_builder_get_object(builder, "page_label"));
    ticket_form_page.entry_title = GTK_WIDGET(gtk_builder_get_object(builder, "ticket_title_entry"));
    ticket_form_page.error_label = GTK_WIDGET(gtk_builder_get_object(builder, "error_label"));

    // ... Please chat gpt continue the work here, i missed lot of things hehe

    GtkWidget *ticket_form_page = GTK_WIDGET(gtk_builder_get_object(builder, "main_vertical_box"));
    GtkWidget *start_date_label = GTK_WIDGET(gtk_builder_get_object(builder, "ticket_start_date_label"));
    GtkWidget *submit_button = GTK_WIDGET(gtk_builder_get_object(builder, "submit_ticket_button"));

    // Set current date to the start_date_label
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char date_str[100];
    strftime(date_str, sizeof(date_str) - 1, "%Y-%m-%d", t);
    gtk_label_set_text(GTK_LABEL(start_date_label), date_str);

    // Optionally connect the submit button to a signal handler
    // g_signal_connect(submit_button, "clicked", G_CALLBACK(on_submit_ticket_button_clicked), stack);

    gtk_stack_add_titled(GTK_STACK(stack), ticket_form_page, "ticket_form", "Ticket Form");
    g_object_unref(builder);
}
