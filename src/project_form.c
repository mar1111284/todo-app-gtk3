#include "../include/project_form.h"
#include "../include/database.h"

// Declare the form widgets globally
GtkWidget *title_entry;
GtkWidget *description_entry;
GtkWidget *leader_entry;
GtkWidget *start_date_entry;
GtkWidget *deadline_entry;
GtkWidget *submit_button;

// Function to fetch the leader ID based on the leader name
int fetch_leader_id(const char *leader_name) {
    return get_leader_id_by_name(leader_name); // Replace with actual implementation
}

void on_submit_button_clicked(GtkButton *button, gpointer user_data) {

    // Fetch form data
    const char *title = gtk_entry_get_text(GTK_ENTRY(title_entry));
    const char *description = gtk_entry_get_text(GTK_ENTRY(description_entry));
    const char *leader = gtk_entry_get_text(GTK_ENTRY(leader_entry));
    const char *start_date = gtk_entry_get_text(GTK_ENTRY(start_date_entry));
    const char *deadline = gtk_entry_get_text(GTK_ENTRY(deadline_entry));

    // Fetch the leader ID based on the leader name
    int leader_id = fetch_leader_id(leader);

    // Call insert_project with the correct leader_id type
    int result = insert_project(title, description, leader_id, start_date, deadline);

    if (result != -1) {
        g_print("Project inserted successfully.\n");
    } else {
        g_print("Failed to insert project.\n");
    }
}

void project_form_init(GtkWidget *stack) {
    // Check if the stack widget is a GtkStack
    if (!GTK_IS_STACK(stack)) {
        g_warning("Error: The provided widget is not a GtkStack.");
        return;
    }

    GtkBuilder *builder = gtk_builder_new();
    GError *error = NULL;

    if (!gtk_builder_add_from_file(builder, "ui/project_form.ui", &error)) {
        g_warning("Error loading UI file: %s", error->message);
        g_error_free(error);
        return;
    }

    // Add the page to the stack
    GtkWidget *project_form_page_widget = GTK_WIDGET(gtk_builder_get_object(builder, "main_vertical_box"));
    gtk_widget_show_all(project_form_page_widget);
    gtk_stack_add_titled(GTK_STACK(stack), project_form_page_widget, "project", "Project form");

    g_object_unref(builder);
}
