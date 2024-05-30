#ifndef LOGIN_H
#define LOGIN_H

#include <gtk/gtk.h>

typedef struct {
    GtkWidget *entry_username;
    GtkWidget *entry_password;
    GtkWidget *stack;
    GtkWidget *error_label;
} LoginPage;

void login_init(GtkWidget *stack);
void on_login_button_clicked(GtkButton *button, gpointer user_data);

#endif // LOGIN_H
