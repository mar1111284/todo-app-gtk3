#include "../include/dashboard.h"
#include "../include/database.h"

static DashboardPage dashboard_page;

void on_add_button_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *stack = GTK_WIDGET(user_data);

    // Switch to the ticket form page
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "ticket_form");

    // Optional: Print a message to the console to indicate which button was clicked
    const char *button_name = gtk_widget_get_name(GTK_WIDGET(button));
    g_print("Add button clicked: %s\n", button_name);
}

void on_new_project_button_clicked(GtkButton *button, gpointer user_data) {
    // Handle new project button click
    g_print("New project button clicked\n");
}

void dashboard_init(GtkWidget *stack) {
    GtkBuilder *builder = gtk_builder_new();
    GError *error = NULL;

    if (!gtk_builder_add_from_file(builder, "ui/dashboard.ui", &error)) {
        g_warning("Error loading UI file: %s", error->message);
        g_error_free(error);
        return;
    }

    GtkWidget *dashboard_page_widget = GTK_WIDGET(gtk_builder_get_object(builder, "dashboard_grid"));
    dashboard_page.dashboard_grid = GTK_GRID(gtk_builder_get_object(builder, "dashboard_grid"));

    if (!dashboard_page_widget) {
        g_warning("Failed to retrieve dashboard page widget from UI file");
        g_object_unref(builder);
        return;
    }

    dashboard_page.dashboard_label = GTK_WIDGET(gtk_builder_get_object(builder, "dashboard_label"));
    dashboard_page.projects_dropdown = GTK_WIDGET(gtk_builder_get_object(builder, "projects_dropdown"));
    dashboard_page.todo_label = GTK_WIDGET(gtk_builder_get_object(builder, "todo_label"));
    dashboard_page.in_progress_label = GTK_WIDGET(gtk_builder_get_object(builder, "in_progress_label"));
    dashboard_page.pending_label = GTK_WIDGET(gtk_builder_get_object(builder, "pending_label"));
    dashboard_page.done_label = GTK_WIDGET(gtk_builder_get_object(builder, "done_label"));

    // Columns
    dashboard_page.todo_column = GTK_WIDGET(gtk_builder_get_object(builder, "todo_column"));
    dashboard_page.progress_column = GTK_WIDGET(gtk_builder_get_object(builder, "progress_column"));
    dashboard_page.pending_column = GTK_WIDGET(gtk_builder_get_object(builder, "pending_column"));
    dashboard_page.done_column = GTK_WIDGET(gtk_builder_get_object(builder, "done_column"));

    // Buttons
    dashboard_page.new_project_button = GTK_WIDGET(gtk_builder_get_object(builder, "new_project_button"));
    dashboard_page.add_button_todo = GTK_WIDGET(gtk_builder_get_object(builder, "add_button_todo"));
    dashboard_page.add_button_progress = GTK_WIDGET(gtk_builder_get_object(builder, "add_button_progress"));
    dashboard_page.add_button_pending = GTK_WIDGET(gtk_builder_get_object(builder, "add_button_pending"));
    dashboard_page.add_button_done = GTK_WIDGET(gtk_builder_get_object(builder, "add_button_done"));

    dashboard_page.stack = stack;

    if (!dashboard_page.new_project_button) {
        g_warning("Failed to retrieve new project button widget from UI file");
        g_object_unref(builder);
        return;
    }

    // Bind the CSS
    gtk_widget_set_name(GTK_WIDGET(dashboard_page.dashboard_label), "dashboard-label");
    gtk_widget_set_name(GTK_WIDGET(dashboard_page.dashboard_grid), "dashboard-grid");
    gtk_widget_set_name(GTK_WIDGET(dashboard_page.todo_label), "todo-label");
    gtk_widget_set_name(GTK_WIDGET(dashboard_page.in_progress_label), "in-progress-label");
    gtk_widget_set_name(GTK_WIDGET(dashboard_page.pending_label), "pending-label");
    gtk_widget_set_name(GTK_WIDGET(dashboard_page.done_label), "done-label");

    // Set name for columns
    gtk_widget_set_name(GTK_WIDGET(dashboard_page.todo_column), "todo-column");
    gtk_widget_set_name(GTK_WIDGET(dashboard_page.progress_column), "progress-column");
    gtk_widget_set_name(GTK_WIDGET(dashboard_page.pending_column), "pending-column");
    gtk_widget_set_name(GTK_WIDGET(dashboard_page.done_column), "done-column");

    // Set name for the button
    gtk_widget_set_name(GTK_WIDGET(dashboard_page.new_project_button), "new-project-button");
    gtk_widget_set_name(GTK_WIDGET(dashboard_page.add_button_todo), "add-button-todo");
    gtk_widget_set_name(GTK_WIDGET(dashboard_page.add_button_progress), "add-button-progress");
    gtk_widget_set_name(GTK_WIDGET(dashboard_page.add_button_pending), "add-button-pending");
    gtk_widget_set_name(GTK_WIDGET(dashboard_page.add_button_done), "add-button-done");

    // Button events
    g_signal_connect(dashboard_page.new_project_button, "clicked", G_CALLBACK(on_new_project_button_clicked), NULL);
    g_signal_connect(dashboard_page.add_button_todo, "clicked", G_CALLBACK(on_add_button_clicked), stack);
    g_signal_connect(dashboard_page.add_button_progress, "clicked", G_CALLBACK(on_add_button_clicked), stack);
    g_signal_connect(dashboard_page.add_button_pending, "clicked", G_CALLBACK(on_add_button_clicked), stack);
    g_signal_connect(dashboard_page.add_button_done, "clicked", G_CALLBACK(on_add_button_clicked), stack);

    // Set name for the projects dropdown
    gtk_widget_set_name(GTK_WIDGET(dashboard_page.projects_dropdown), "projects-dropdown");

    // Fetch and add projects to the dropdown
    int num_projects;
    Project* projects = fetch_all_projects(&num_projects);

    if (projects) {
        GtkComboBoxText *dropdown = GTK_COMBO_BOX_TEXT(dashboard_page.projects_dropdown);
        // Add placeholder
        gtk_combo_box_text_append_text(dropdown, "Select a project");
        gtk_combo_box_set_active(GTK_COMBO_BOX(dropdown), 0);

        // Add project titles to the dropdown
        for (int i = 0; i < num_projects; i++) {
            gtk_combo_box_text_append_text(dropdown, projects[i].title);
        }
        free_projects(projects, num_projects);
    } else {
        g_print("Failed to fetch projects\n");
    }

    gtk_stack_add_titled(GTK_STACK(stack), dashboard_page_widget, "dashboard", "Dashboard");
    g_object_unref(builder);
}
