#include "../include/dashboard.h"

void dashboard_init(GtkWidget *stack) {
    GtkBuilder *builder = gtk_builder_new_from_file("ui/dashboard.ui");
    GtkWidget *dashboard_page_widget = GTK_WIDGET(gtk_builder_get_object(builder, "dashboard_box"));

    gtk_stack_add_titled(GTK_STACK(stack), dashboard_page_widget, "dashboard", "Dashboard");
    g_object_unref(builder);
}
