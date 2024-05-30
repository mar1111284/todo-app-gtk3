#include "../include/login.h"

static LoginPage login_page;

void on_login_button_clicked(GtkButton *button, gpointer user_data) {
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
}

void login_init(GtkWidget *stack) {
    GtkBuilder *builder = gtk_builder_new_from_file("ui/login.ui");
    GtkWidget *login_page_widget = GTK_WIDGET(gtk_builder_get_object(builder, "login_box"));

    login_page.entry_username = GTK_WIDGET(gtk_builder_get_object(builder, "entry_username"));
    login_page.entry_password = GTK_WIDGET(gtk_builder_get_object(builder, "entry_password"));
    login_page.stack = stack;
    login_page.error_label = GTK_WIDGET(gtk_builder_get_object(builder, "error_label"));

    GtkWidget *login_button = GTK_WIDGET(gtk_builder_get_object(builder, "login_button"));
    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_button_clicked), NULL);

    gtk_stack_add_titled(GTK_STACK(stack), login_page_widget, "login", "Login");
    g_object_unref(builder);
}
