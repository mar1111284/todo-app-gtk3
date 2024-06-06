#include "../include/login.h"

static LoginPage login_page;

void on_login_button_clicked(GtkButton *button, gpointer user_data) {

    /* Retrieve the username and password */
    const char *username = gtk_entry_get_text(GTK_ENTRY(login_page.entry_username));
    const char *password = gtk_entry_get_text(GTK_ENTRY(login_page.entry_password));

    GtkWidget *stack = login_page.stack;
    GtkWidget *error_label = login_page.error_label;

    if (g_strcmp0(username, "") != 0 && g_strcmp0(password, "") != 0) {
        gtk_stack_set_visible_child_name(GTK_STACK(stack), "dashboard");
        gtk_label_set_text(GTK_LABEL(error_label), "");
    } else {
        gtk_label_set_text(GTK_LABEL(error_label), "Invalid username or password");
        gtk_widget_set_visible(error_label, TRUE);
    }

    // Note: Database not implemented here
}

void login_init(GtkWidget *stack) {

    GtkBuilder *builder = gtk_builder_new();
    GError *error = NULL;

    if (!gtk_builder_add_from_file(builder, "ui/login.ui", &error)) {
        g_warning("Error loading UI file: %s", error->message);
        g_error_free(error);
        return;
    }

    GtkWidget *login_page_widget = GTK_WIDGET(gtk_builder_get_object(builder, "login_page_grid"));
    if (!login_page_widget) {
        g_warning("Failed to retrieve login page widget from UI file");
        g_object_unref(builder);
        return;
    }

    login_page.entry_username = GTK_WIDGET(gtk_builder_get_object(builder, "entry_username"));
    login_page.entry_password = GTK_WIDGET(gtk_builder_get_object(builder, "entry_password"));
    login_page.login_label = GTK_WIDGET(gtk_builder_get_object(builder, "login_label"));
    login_page.login_grid = GTK_GRID(gtk_builder_get_object(builder, "login_page_grid"));
    login_page.stack = stack;
    login_page.error_label = GTK_WIDGET(gtk_builder_get_object(builder, "error_label"));

    GtkWidget *login_button = GTK_WIDGET(gtk_builder_get_object(builder, "login_button"));
    if (!login_button) {
        g_warning("Failed to retrieve login button widget from UI file");
        g_object_unref(builder);
        return;
    }

    // Bind the css
    gtk_widget_set_name(GTK_WIDGET(login_page.login_grid), "login-grid");
    gtk_widget_set_name(GTK_WIDGET(login_page.error_label), "login-error-label");
    gtk_widget_set_name(GTK_WIDGET(login_page.login_label), "login-label");
    gtk_widget_set_name(GTK_WIDGET(login_button), "login-button");
    gtk_widget_set_name(GTK_WIDGET(login_page.entry_username) , "entry");
    gtk_widget_set_name(GTK_WIDGET(login_page.entry_password) , "entry");



    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_button_clicked), NULL);

    gtk_stack_add_titled(GTK_STACK(stack), login_page_widget, "login", "Login");
    g_object_unref(builder);
}
