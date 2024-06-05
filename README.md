# TODO APP C Programming / Gtk3-lib

## Summary

1 - The Project Structure
2 - The Stack Structure 
3 - The Pages Structure
4 - Database implementation

## Introduction

Welcome to my Todo App! This project is a simple yet functional Todo application developed using the C programming language and the GTK3 library, with CSS for styling.

**Why I Chose C Programming and GTK3?**
C programming provides fine-grained control over system resources and memory management, making it ideal for system programming and performance-critical applications. By using C, I aimed to enhance my skills in low-level programming and optimization.

The GTK3 library is a versatile toolkit for creating graphical user interfaces (GUIs) on Linux. It offers a comprehensive set of widgets and tools to build sophisticated applications. Leveraging GTK3, I aimed to create a modern and responsive user interface for my Todo App.

## The Project Structure

Here is an overview of the project structure:

```
.
├── includes      # Header files
├── src           # Source files
├── object        # Compiled object files
├── docs          # Documentation
├── ui            # User interface files (GtkBuilder .ui files)
├── Makefile      # Build script
├── app.db        # Database file
├── README.md     # Project documentation
├── style.css     # CSS file for styling

```

## The Stack structure

The application uses GtkStack to manage different pages within the main window. This allows seamless navigation between different sections of the app, such as the login page, dashboard, and ticket form.

**In the main.c file, the GtkStack is created and pages are initialized as follows:**

```

/* Initialize GTK */
gtk_init(&argc, &argv);

/* Create the main window */
GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
gtk_window_set_title(GTK_WINDOW(window), "GTK To-Do App");
gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
gtk_container_set_border_width(GTK_CONTAINER(window), 0);
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

```

**Each page is defined in a separate file and initialized using a specific function. For instance, the login page is initialized in login_init():**

```
void login_init(GtkWidget *stack) {
    GtkBuilder *builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "ui/login.ui", NULL);

    GtkWidget *login_page_widget = GTK_WIDGET(gtk_builder_get_object(builder, "login_page_grid"));
    login_page.entry_username = GTK_WIDGET(gtk_builder_get_object(builder, "entry_username"));
    login_page.entry_password = GTK_WIDGET(gtk_builder_get_object(builder, "entry_password"));
    login_page.stack = stack;

    GtkWidget *login_button = GTK_WIDGET(gtk_builder_get_object(builder, "login_button"));
    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_button_clicked), NULL);

    gtk_stack_add_titled(GTK_STACK(stack), login_page_widget, "login", "Login");
    g_object_unref(builder);
}

```

**When the login button is clicked, the on_login_button_clicked() function handles the event, switching to the dashboard page if the login is successful:**

void on_login_button_clicked(GtkButton *button, gpointer user_data) {
    const char *username = gtk_entry_get_text(GTK_ENTRY(login_page.entry_username));
    const char *password = gtk_entry_get_text(GTK_ENTRY(login_page.entry_password));

    if (g_strcmp0(username, "") != 0 && g_strcmp0(password, "") != 0) {
        gtk_stack_set_visible_child_name(GTK_STACK(login_page.stack), "dashboard");
    } else {
        gtk_label_set_text(GTK_LABEL(login_page.error_label), "Invalid username or password");
    }
}

By using GtkStack, the application maintains a clean and organized structure, allowing easy navigation and modularity. Each page can be independently developed and managed, ensuring a scalable and maintainable codebase.

## The Page structure 



in the header folder we create a structure to keep track of every entry, and label to access it more easely (get value, apply style
etc..)

by example:

typedef struct {
    GtkWidget *page_label;
    GtkGrid *page_grid;
    GtkWidget *entry_title;
    GtkWidget *entry_description;
    GtkWidget *stack;
    GtkWidget *error_label;
    GtkWidget *submit_btn;

} TicketFormPage;

Here we have the page wo contain the ticket form

![alt text](https://i.imgur.com/3drVIr7.jpeg)


![alt text](https://i.imgur.com/DtwrIra.jpeg)


