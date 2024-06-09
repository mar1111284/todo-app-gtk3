#include "../include/ticket_form.h"
#include "../include/database.h"
#include <sqlite3.h>
#include <gtk/gtk.h>
#include <time.h>

static TicketFormPage ticket_form_page;

// Function to handle submit button click event
static void on_submit_button_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *stack = GTK_WIDGET(user_data);

    // Get the text from the form fields
    const char *title = gtk_entry_get_text(GTK_ENTRY(ticket_form_page.entry_title));
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(ticket_form_page.entry_description));
    GtkTextIter start, end;
    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_buffer_get_end_iter(buffer, &end);
    gchar *description = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
    const char *priority = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(ticket_form_page.entry_priority));
    const char *project_name = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(ticket_form_page.entry_project));
    const char *status = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(ticket_form_page.entry_status));
    const char *owner = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(ticket_form_page.entry_owner));
    const char *start_date = gtk_label_get_text(GTK_LABEL(ticket_form_page.start_date_label));
    const char *deadline = gtk_label_get_text(GTK_LABEL(ticket_form_page.deadline_date_label));

    // Retrieve the project ID based on the project name
    int project_id = get_project_id(project_name);

    // Print ticket details to console
    g_print("Title: %s\n", title);
    g_print("Description: %s\n", description);
    g_print("Priority: %s\n", priority);
    g_print("Status: %s\n", status);
    g_print("Owner: %s\n", owner);
    g_print("Start Date: %s\n", start_date);
    g_print("Deadline: %s\n", deadline);
    g_print("project ID: %d\n", project_id);

    // Validate the project ID
    if (project_id < 0) {
        g_print("Error: Invalid project\n");
        g_free(description);
        return;
    }

    // Validate the form fields
    if (strlen(title) == 0 || strlen(description) == 0 || !priority || !status || !owner || strlen(start_date) == 0 || strlen(deadline) == 0) {
        g_print("Error: All fields must be filled out\n");
        g_free(description);
        return;
    }

    // Insert the ticket into the database
    int result = insert_ticket(title, description, priority, status, owner, start_date, deadline, project_id);
    if (result != SQLITE_OK) {
        g_print("Error: Failed to insert ticket into the database\n");
    } else {
        g_print("Ticket inserted successfully\n");
        // Switch to the ticket form page
        gtk_stack_set_visible_child_name(GTK_STACK(stack), "dashboard");
    }

    // Free the allocated description
    g_free(description);
}

void on_date_button_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *calendar;

    // Get the start date label from the ticket form page structure
    GtkWidget *start_date_label = ticket_form_page.start_date_label;
    
    // Get the deadline date label from the ticket form page structure
    GtkWidget *deadline_date_label = ticket_form_page.deadline_date_label;

    // Create a new dialog
    dialog = gtk_dialog_new_with_buttons("Select Date",
                                         NULL,
                                         GTK_DIALOG_MODAL,
                                         "OK",
                                         GTK_RESPONSE_OK,
                                         NULL);

    // Get the content area of the dialog
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    // Create a new calendar widget
    calendar = gtk_calendar_new();

    // Pack the calendar into the dialog's content area
    gtk_box_pack_start(GTK_BOX(content_area), calendar, TRUE, TRUE, 0);

    // Show all widgets in the dialog
    gtk_widget_show_all(dialog);

    // Run the dialog and wait for user response
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
        // Get the selected date from the calendar
        gint year, month, day;
        gtk_calendar_get_date(GTK_CALENDAR(calendar), &year, &month, &day);

        // Format the selected date
        char date_str[20];
        snprintf(date_str, sizeof(date_str), "%04d-%02d-%02d", year, month + 1, day);

        // Update the start date label
        gtk_label_set_text(GTK_LABEL(start_date_label), date_str);

        // Update the deadline date label if it exists
        if (deadline_date_label != NULL) {
            gtk_label_set_text(GTK_LABEL(deadline_date_label), date_str);
        }
    }

    // Destroy the dialog when done
    gtk_widget_destroy(dialog);
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
    ticket_form_page.page_grid = GTK_GRID(gtk_builder_get_object(builder, "page_grid"));
    ticket_form_page.page_label = GTK_WIDGET(gtk_builder_get_object(builder, "dashboard_label"));
    ticket_form_page.entry_title = GTK_WIDGET(gtk_builder_get_object(builder, "ticket_title_entry"));
    ticket_form_page.entry_description = GTK_WIDGET(gtk_builder_get_object(builder, "ticket_description_textview"));
    ticket_form_page.entry_priority = GTK_WIDGET(gtk_builder_get_object(builder, "ticket_priority_combobox"));
    ticket_form_page.entry_status = GTK_WIDGET(gtk_builder_get_object(builder, "ticket_status_combobox"));
    ticket_form_page.entry_owner = GTK_WIDGET(gtk_builder_get_object(builder, "ticket_owner_combobox"));
    ticket_form_page.entry_project = GTK_WIDGET(gtk_builder_get_object(builder, "ticket_project_combobox"));
    ticket_form_page.error_label = GTK_WIDGET(gtk_builder_get_object(builder, "error_label"));
    ticket_form_page.submit_btn = GTK_WIDGET(gtk_builder_get_object(builder, "submit_ticket_button"));
    ticket_form_page.ticket_description_textview_container = GTK_WIDGET(gtk_builder_get_object(builder, "ticket_description_textview_container"));
    ticket_form_page.start_date_label = GTK_WIDGET(gtk_builder_get_object(builder, "ticket_start_date_label"));
    ticket_form_page.deadline_date_label= GTK_WIDGET(gtk_builder_get_object(builder, "ticket_deadline_label"));

    // Set current date to the start_date_label
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char date_str[100];
    strftime(date_str, sizeof(date_str) - 1, "%Y-%m-%d", t);

    // Bind the CSS
    gtk_widget_set_name(GTK_WIDGET(ticket_form_page.entry_title), "entry");
    gtk_widget_set_name(GTK_WIDGET(ticket_form_page.entry_description), "entry-description");
    gtk_widget_set_name(GTK_WIDGET(ticket_form_page.page_grid), "page-grid");
    gtk_widget_set_name(GTK_WIDGET(ticket_form_page.submit_btn), "submit-btn");
    gtk_widget_set_name(GTK_WIDGET(ticket_form_page.ticket_description_textview_container), "entry-description-container");

    // Create a text buffer with placeholder text for the description text view
    GtkTextBuffer *buffer = gtk_text_buffer_new(NULL);
    gtk_text_buffer_set_text(buffer, "Enter description here...", -1);
    gtk_text_view_set_buffer(GTK_TEXT_VIEW(ticket_form_page.entry_description), buffer);

    // Add placeholder and items to the priority combobox
    gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(ticket_form_page.entry_priority), 0, "Select Priority");
    gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(ticket_form_page.entry_status), 0, "Select Status");
    gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(ticket_form_page.entry_owner), 0, "Select owner");
    
    // Populate project names into the project combobox
    int num_projects;
    char** project_names = fetch_all_project_names(&num_projects);

    if (!project_names) {
        g_warning("Failed to fetch project names from database");
        return;
    }

    // Populate the combobox
    for (int i = 0; i < num_projects; ++i) {
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(ticket_form_page.entry_project), project_names[i]);
    }

    // Free allocated memory
    for (int i = 0; i < num_projects; ++i) {
        free(project_names[i]);
    }
    free(project_names);

    // Set the placeholder as the active item
    gtk_combo_box_set_active(GTK_COMBO_BOX(ticket_form_page.entry_priority), 0);
    gtk_combo_box_set_active(GTK_COMBO_BOX(ticket_form_page.entry_status), 0);
    gtk_combo_box_set_active(GTK_COMBO_BOX(ticket_form_page.entry_owner), 0);
    gtk_combo_box_set_active(GTK_COMBO_BOX(ticket_form_page.entry_project), 0);

    // Connect the submit button clicked signal to the handler
    g_signal_connect(ticket_form_page.submit_btn, "clicked", G_CALLBACK(on_submit_button_clicked), NULL);

    // Connect the start date and deadline button clicked signals to show the calendar dialog
    GtkWidget *start_date_button = GTK_WIDGET(gtk_builder_get_object(builder, "ticket_start_date_button"));
    GtkWidget *deadline_button = GTK_WIDGET(gtk_builder_get_object(builder, "ticket_deadline_button"));

    g_signal_connect(start_date_button, "clicked", G_CALLBACK(on_date_button_clicked), &ticket_form_page);
    g_signal_connect(deadline_button, "clicked", G_CALLBACK(on_date_button_clicked), &ticket_form_page);

    // Add the page to the stack
    GtkWidget *ticket_form_page_widget = GTK_WIDGET(gtk_builder_get_object(builder, "main_vertical_box"));
    gtk_widget_show_all(ticket_form_page_widget);
    gtk_stack_add_named(GTK_STACK(stack), ticket_form_page_widget, "ticket_form");
}
