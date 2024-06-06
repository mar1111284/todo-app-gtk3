#include <gtk/gtk.h>
#include <sqlite3.h>
#include "../include/database.h"
#include "../include/login.h"
#include "../include/dashboard.h"
#include "../include/ticket_form.h"

int main(int argc, char *argv[]) {

    /* Initialize GTK */
    gtk_init(&argc, &argv);

    /* Initialize the database */ 
    int db_init_result = initialize_database();

    if (db_init_result != SQLITE_OK) {
        fprintf(stderr, "Error initializing the database.\n");
        return db_init_result;
    }

    /* Create the main window */
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GTK To-Do App");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    gtk_container_set_border_width(GTK_CONTAINER(window),0);
    gtk_widget_set_name(window, "main_window"); // Add ID to the main window
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    /* Create the Stack Object */
    GtkWidget *stack = gtk_stack_new();
    gtk_container_add(GTK_CONTAINER(window), stack);

    /* Initialize pages */ 
    login_init(stack);
    dashboard_init(stack);
    ticket_form_init(stack);

    /* Load CSS */
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "style.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    /* Show the main window & start the App */
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
