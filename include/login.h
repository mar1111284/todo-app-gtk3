#ifndef LOGIN_H
#define LOGIN_H

#include <gtk/gtk.h>

typedef struct {
    GtkWidget *login_label;
    GtkGrid *login_grid;
    GtkWidget *entry_username;
    GtkWidget *entry_password;
    GtkWidget *stack;
    GtkWidget *error_label;
    GtkWidget *login_btn;

} LoginPage;

void login_init(GtkWidget *stack);
void on_login_button_clicked(GtkButton *button, gpointer user_data);

#endif // LOGIN_H
