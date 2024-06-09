#include "../include/dashboard.h"
#include "../include/project_form.h"
#include "../include/database.h"

static DashboardPage dashboard_page;

void clear_ticket_container(GtkWidget *container) {
    // Get the list of children widgets within the container
    GList *children = gtk_container_get_children(GTK_CONTAINER(container));
    
    // Iterate through the list and destroy each child widget
    for (GList *iter = children; iter != NULL; iter = g_list_next(iter)) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    
    // Free the list
    g_list_free(children);
}


void add_ticket_to_container(GtkWidget *container, Ticket *ticket) {
    // Create a label for the ticket
    GtkWidget *ticket_label = gtk_label_new(ticket->title);

    // Set name for the ticket label (optional)
    gtk_widget_set_name(ticket_label, "ticket-label");

    // Add the ticket label to the container
    gtk_container_add(GTK_CONTAINER(container), ticket_label);

    // Show the ticket label
    gtk_widget_show(ticket_label);
}


void on_add_button_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *stack = GTK_WIDGET(user_data);

    // Switch to the ticket form page
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "ticket_form");

    // Optional: Print a message to the console to indicate which button was clicked
    const char *button_name = gtk_widget_get_name(GTK_WIDGET(button));
    g_print("Add button clicked: %s\n", button_name);
}
void on_new_project_button_clicked(GtkButton *button, gpointer user_data) {
    // Print the value of user_data to debug
    g_print("User data pointer: %p\n", user_data);

    // Assuming user_data is a GtkWidget pointer
    GtkWidget *stack = GTK_WIDGET(user_data);

    // Check if stack is a GtkStack widget
    if (!GTK_IS_STACK(stack)) {
        g_warning("User data is not a GtkStack widget");
        return;
    }

    // Set the visible child name
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "project");

    // Check if the operation was successful
    const gchar *current_page = gtk_stack_get_visible_child_name(GTK_STACK(stack));
    if (g_strcmp0(current_page, "project") != 0) {
        g_warning("Failed to switch to the project form page. Current page: %s", current_page);
        return;
    }
    
    g_print("New project button clicked\n");
}




void on_project_selected(GtkComboBox *widget, gpointer user_data) {
    GtkComboBoxText *dropdown = GTK_COMBO_BOX_TEXT(widget);
    const gchar *selected_project = gtk_combo_box_text_get_active_text(dropdown);

    if (selected_project && g_strcmp0(selected_project, "Select a project") != 0) {
        // Here, we assume the project title is unique and fetch the project ID accordingly.
        // Ideally, the project ID should be fetched from the database or stored alongside the project title.
        int project_id = -1;

        // Fetch all projects to determine the selected project ID
        int num_projects;
        Project *projects = fetch_all_projects(&num_projects);
        if (projects) {
            for (int i = 0; i < num_projects; i++) {
                if (g_strcmp0(projects[i].title, selected_project) == 0) {
                    project_id = projects[i].id;
                    break;
                }
            }
            free_projects(projects, num_projects);
        }

        if (project_id != -1) {
            // Fetch tickets for the selected project
            int num_tickets;
            Ticket *tickets = fetch_tickets_by_project_id(project_id, &num_tickets);
            if (tickets) {
                g_print("Tickets for project '%s':\n", selected_project);

                // Clear existing columns here
                clear_ticket_container(dashboard_page.todo_ticket_container);
                clear_ticket_container(dashboard_page.progress_ticket_container);
                clear_ticket_container(dashboard_page.pending_ticket_container);
                clear_ticket_container(dashboard_page.done_ticket_container);

                // Add tickets to corresponding columns
                for (int i = 0; i < num_tickets; i++) {
                    g_print("Ticket %d: %s\n", tickets[i].id, tickets[i].title);
                    if (g_strcmp0(tickets[i].status, "TODO") == 0) {
                        add_ticket_to_container(dashboard_page.todo_ticket_container, &tickets[i]);
                    } else if (g_strcmp0(tickets[i].status, "In Progress") == 0) {
                        add_ticket_to_container(dashboard_page.progress_ticket_container, &tickets[i]);
                    } else if (g_strcmp0(tickets[i].status, "Pending") == 0) {
                        add_ticket_to_container(dashboard_page.pending_ticket_container, &tickets[i]);
                    } else if (g_strcmp0(tickets[i].status, "Done") == 0) {
                        add_ticket_to_container(dashboard_page.done_ticket_container, &tickets[i]);
                    }
                }
                free_tickets(tickets);
            } else {
                g_print("No tickets found for project '%s'\n", selected_project);
            }
        } else {
            g_print("Failed to determine project ID for '%s'\n", selected_project);
        }
    }
}

void dashboard_init(GtkWidget *stack) {
    GtkBuilder *builder = gtk_builder_new();
    GError *error = NULL;

    print_tickets_with_projects();

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

    // Tickets container
    dashboard_page.todo_ticket_container= GTK_WIDGET(gtk_builder_get_object(builder, "todo_ticket_container"));
    dashboard_page.progress_ticket_container = GTK_WIDGET(gtk_builder_get_object(builder, "progress_ticket_container"));
    dashboard_page.pending_ticket_container = GTK_WIDGET(gtk_builder_get_object(builder, "pending_ticket_container"));
    dashboard_page.done_ticket_container = GTK_WIDGET(gtk_builder_get_object(builder, "done_ticket_container"));

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
    g_signal_connect(dashboard_page.add_button_todo, "clicked", G_CALLBACK(on_add_button_clicked), stack);
    g_signal_connect(dashboard_page.add_button_progress, "clicked", G_CALLBACK(on_add_button_clicked), stack);
    g_signal_connect(dashboard_page.add_button_pending, "clicked", G_CALLBACK(on_add_button_clicked), stack);
    g_signal_connect(dashboard_page.add_button_done, "clicked", G_CALLBACK(on_add_button_clicked), stack);

    // Set name for the projects dropdown
    gtk_widget_set_name(GTK_WIDGET(dashboard_page.projects_dropdown), "projects-dropdown");

    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dashboard_page.projects_dropdown), "Select a project");
    // Set the placeholder as the active item
    gtk_combo_box_set_active(GTK_COMBO_BOX(dashboard_page.projects_dropdown), 0);

    // Populate the projects dropdown
    int num_projects;
    Project *projects = fetch_all_projects(&num_projects);
    if (projects) {
        for (int i = 0; i < num_projects; i++) {
            gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dashboard_page.projects_dropdown), projects[i].title);
        }
        free_projects(projects, num_projects);
    }

    // Project selection event
    g_signal_connect(dashboard_page.projects_dropdown, "changed", G_CALLBACK(on_project_selected), NULL);
    // Button events
    g_signal_connect(dashboard_page.new_project_button, "clicked", G_CALLBACK(on_new_project_button_clicked), stack);


    gtk_stack_add_titled(GTK_STACK(stack), dashboard_page_widget, "dashboard", "Dashboard");
    g_object_unref(builder);
}
