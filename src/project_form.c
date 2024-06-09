#include "../include/project_form.h"
#include "../include/database.h"
#include <sqlite3.h>
#include <gtk/gtk.h>
#include <time.h>


static ProjectFormPage project_form_page;


// Function to fetch the leader ID based on the leader name
int fetch_leader_id(const char *leader_name) {
    return get_leader_id_by_name(leader_name); // Replace with actual implementation
}

void on_submit_button_clicked(GtkButton *button, gpointer user_data) {

    // // Fetch form data
    // const char *title = gtk_entry_get_text(GTK_ENTRY(title_entry));
    // const char *description = gtk_entry_get_text(GTK_ENTRY(description_entry));
    // const char *leader = gtk_entry_get_text(GTK_ENTRY(leader_entry));
    // const char *start_date = gtk_entry_get_text(GTK_ENTRY(start_date_entry));
    // const char *deadline = gtk_entry_get_text(GTK_ENTRY(deadline_entry));

    // // Fetch the leader ID based on the leader name
    // int leader_id = fetch_leader_id(leader);

    // // Call insert_project with the correct leader_id type
    // int result = insert_project(title, description, leader_id, start_date, deadline);

    // if (result != -1) {
    //     g_print("Project inserted successfully.\n");
    // } else {
    //     g_print("Failed to insert project.\n");
    // }
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

    // Fill structure
    project_form_page.stack = stack;
    project_form_page.page_grid = GTK_GRID(gtk_builder_get_object(builder, "page_grid"));
    project_form_page.page_label = GTK_WIDGET(gtk_builder_get_object(builder, "dashboard_label"));
    project_form_page.entry_title = GTK_WIDGET(gtk_builder_get_object(builder, "project_title_entry"));
    project_form_page.entry_description = GTK_WIDGET(gtk_builder_get_object(builder, "project_description_textview"));
    project_form_page.entry_leader = GTK_WIDGET(gtk_builder_get_object(builder, "project_leader_combobox"));
    // ticket_form_page.error_label = GTK_WIDGET(gtk_builder_get_object(builder, "error_label"));
    project_form_page.submit_btn = GTK_WIDGET(gtk_builder_get_object(builder, "submit_project_button"));
    project_form_page.project_description_textview_container = GTK_WIDGET(gtk_builder_get_object(builder, "project_description_textview_container"));
    project_form_page.start_date_label = GTK_WIDGET(gtk_builder_get_object(builder, "project_start_date_label"));
    project_form_page.deadline_date_label= GTK_WIDGET(gtk_builder_get_object(builder, "project_deadline_label"));

    // Bind CSS
    gtk_widget_set_name(GTK_WIDGET(project_form_page.entry_title), "entry");
    gtk_widget_set_name(GTK_WIDGET(project_form_page.entry_description), "entry-description");
    gtk_widget_set_name(GTK_WIDGET(project_form_page.page_grid), "page-grid");
    gtk_widget_set_name(GTK_WIDGET(project_form_page.submit_btn), "submit-btn");
    gtk_widget_set_name(GTK_WIDGET(project_form_page.project_description_textview_container), "entry-description-container");

    // Add the page to the stack
    GtkWidget *project_form_page_widget = GTK_WIDGET(gtk_builder_get_object(builder, "main_vertical_box"));
    gtk_widget_show_all(project_form_page_widget);
    gtk_stack_add_titled(GTK_STACK(stack), project_form_page_widget, "project", "Project form");

    g_object_unref(builder);
}
