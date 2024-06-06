#include "../include/ticket_form.h"
#include <gtk/gtk.h>
#include <time.h>

static TicketFormPage ticket_form_page;

// Function to handle submit button click event
static void on_submit_button_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *stack = GTK_WIDGET(user_data);

    // Switch to the ticket form page
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "dashboard");
    g_print("Submit button clicked\n");
    // Additional logic to handle form submission can be added here
}

void ticket_form_init(GtkWidget *stack) {
    GtkBuilder *builder = gtk_builder_new();
    GError *error = NULL;

    if (!gtk_builder_add_from_file(builder, "ui/ticket_form.ui", &error)) {
        g_warning("Error loading UI file: %s", error->message);
        g_error_free(error);
        return;
    }

    // Fill structure
    ticket_form_page.stack = stack;
    ticket_form_page.page_grid = GTK_GRID(gtk_builder_get_object(builder, "ticket_form_grid"));
    ticket_form_page.page_label = GTK_WIDGET(gtk_builder_get_object(builder, "dashboard_label"));
    ticket_form_page.entry_title = GTK_WIDGET(gtk_builder_get_object(builder, "ticket_title_entry"));
    ticket_form_page.entry_description = GTK_WIDGET(gtk_builder_get_object(builder, "ticket_description_textview"));
    ticket_form_page.entry_priority = GTK_WIDGET(gtk_builder_get_object(builder, "ticket_priority_combobox"));
    ticket_form_page.entry_status = GTK_WIDGET(gtk_builder_get_object(builder, "ticket_status_combobox"));
    ticket_form_page.entry_start_date = GTK_WIDGET(gtk_builder_get_object(builder, "ticket_start_date_label"));
    ticket_form_page.entry_deadline_date = GTK_WIDGET(gtk_builder_get_object(builder, "ticket_deadline_entry"));
    ticket_form_page.entry_owner = GTK_WIDGET(gtk_builder_get_object(builder, "ticket_owner_entry"));
    ticket_form_page.error_label = GTK_WIDGET(gtk_builder_get_object(builder, "error_label"));
    ticket_form_page.submit_btn = GTK_WIDGET(gtk_builder_get_object(builder, "submit_ticket_button"));

    // Set current date to the start_date_label
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char date_str[100];
    strftime(date_str, sizeof(date_str) - 1, "%Y-%m-%d", t);
    gtk_label_set_text(GTK_LABEL(ticket_form_page.entry_start_date), date_str);

    // Bind the CSS
    gtk_widget_set_name(GTK_WIDGET(ticket_form_page.entry_title), "entry");
    gtk_widget_set_name(GTK_WIDGET(ticket_form_page.entry_description), "entry_description");

    // Create a text buffer with placeholder text for the description text view
    GtkTextBuffer *buffer = gtk_text_buffer_new(NULL);
    gtk_text_buffer_set_text(buffer, "Enter description here...", -1);
    gtk_text_view_set_buffer(GTK_TEXT_VIEW(ticket_form_page.entry_description), buffer);

    // Add placeholder and items to the priority combobox
    gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(ticket_form_page.entry_priority), 0, "Select Priority");
    gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(ticket_form_page.entry_status), 0, "Select Status");

    // Set the placeholder as the active item
    gtk_combo_box_set_active(GTK_COMBO_BOX(ticket_form_page.entry_priority), 0);
    gtk_combo_box_set_active(GTK_COMBO_BOX(ticket_form_page.entry_status), 0);

    // Connect the submit button clicked signal to the handler
    g_signal_connect(ticket_form_page.submit_btn, "clicked", G_CALLBACK(on_submit_button_clicked), NULL);

    // Add the page to the stack
    GtkWidget *ticket_form_page_widget = GTK_WIDGET(gtk_builder_get_object(builder, "main_vertical_box"));
    gtk_stack_add_titled(GTK_STACK(stack), ticket_form_page_widget, "ticket_form", "Ticket Form");

    g_object_unref(builder);
}
