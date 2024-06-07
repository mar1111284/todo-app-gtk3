# TODO APP C Programming / Gtk3-lib

### Summary

1. **Project Structure**
   - Overview of folders and files.
   
2. **Stack Structure**
   - Usage of `GtkStack` for managing different pages.
   
3. **Pages Structure**
   - How different pages (login, dashboard, etc.) are organized and initialized.
   
4. **Database Implementation**
   - Integration with SQLite for storing todo items.

5. **Screenshots**

### Introduction

Welcome to my Todo App! This project is a simple yet functional Todo application developed using the C programming language and the GTK3 library, with CSS for styling.

**Why I Chose C Programming and GTK3?**
C programming provides fine-grained control over system resources and memory management, making it ideal for system programming and performance-critical applications. By using C, I aimed to enhance my skills in low-level programming and optimization.

The GTK3 library is a versatile toolkit for creating graphical user interfaces (GUIs) on Linux. It offers a comprehensive set of widgets and tools to build sophisticated applications. Leveraging GTK3, I aimed to create a modern and responsive user interface for my Todo App.

### Project Structure

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

### Stack Structure

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

```
void on_login_button_clicked(GtkButton *button, gpointer user_data) {
    const char *username = gtk_entry_get_text(GTK_ENTRY(login_page.entry_username));
    const char *password = gtk_entry_get_text(GTK_ENTRY(login_page.entry_password));

    if (g_strcmp0(username, "") != 0 && g_strcmp0(password, "") != 0) {
        gtk_stack_set_visible_child_name(GTK_STACK(login_page.stack), "dashboard");
    } else {
        gtk_label_set_text(GTK_LABEL(login_page.error_label), "Invalid username or password");
    }
}
```

By using GtkStack, the application maintains a clean and organized structure, allowing easy navigation and modularity. Each page can be independently developed and managed, ensuring a scalable and maintainable codebase.

### Page Structure

Let's see how it works by coding example of the Ticket Form page, users can create new tickets by providing various details. Let's delve into how this page is implemented:

The Ticket Form page is an essential component of the Todo App, facilitating the creation of new tasks. It's meticulously structured to ensure a seamless user experience. Here's a breakdown of its implementation:

#### Header file

In the header file, we define the structure TicketFormPage to encapsulate UI widgets specific to the Ticket Form page. This abstraction promotes modularity and ease of maintenance.

```
#ifndef TICKET_FORM_H
#define TICKET_FORM_H

#include <gtk/gtk.h>

// Structure to hold UI widgets
typedef struct {
    GtkWidget *page_label;
    GtkGrid *page_grid;
    GtkWidget *entry_title;
    GtkWidget *entry_description;
    GtkWidget *entry_priority;
    GtkWidget *entry_status;
    GtkWidget *entry_start_date;
    GtkWidget *entry_deadline_date;
    GtkWidget *entry_owner;
    GtkWidget *stack;
    GtkWidget *error_label;
    GtkWidget *submit_btn;
} TicketFormPage;

void ticket_form_init(GtkWidget *stack);

#endif // TICKET_FORM_H

```

#### Builder file

The UI file (ticket_form.ui) defines the visual layout of the Ticket Form page using GtkBuilder. It includes widgets like entry fields, labels, and buttons arranged in a grid to facilitate user interaction. This UI file defines the layout of the ticket form page using GtkBuilder. It includes widgets for entering ticket details like title, description, priority, status, start date, deadline, owner, and a submit button.

```
<?xml version="1.0" encoding="UTF-8"?>
<interface>
  <requires lib="gtk+" version="3.20"/>
  
  <!-- Main Vertical Box to hold everything -->
  <object class="GtkBox" id="main_vertical_box">
    <property name="visible">True</property>
    <property name="orientation">vertical</property>
    <property name="halign">center</property>
    <property name="valign">center</property>
    <property name="spacing">20</property>
    
    <!-- Inner Box for Centering Content -->
    <child>
      <object class="GtkBox" id="centered_box">
        <property name="visible">True</property>
        <property name="orientation">vertical</property>
        <property name="halign">center</property>
        <property name="valign">center</property>
        
        <!-- Ticket Form Grid -->
        <child>
          <object class="GtkGrid" id="ticket_form_grid">
            <property name="visible">True</property>
            <property name="column_homogeneous">True</property>
            <property name="row_spacing">10</property>
            <property name="column_spacing">10</property>

            <!-- Page Title -->
            <child>
              <object class="GtkLabel" id="dashboard_label">
                <!-- Properties omitted for brevity -->
              </object>
              <packing>
                <!-- Properties omitted for brevity -->
              </packing>
            </child>

            <!-- Title ticket entry -->
            <child>
              <object class="GtkEntry" id="ticket_title_entry">
                <!-- Properties omitted for brevity -->
              </object>
              <packing>
                <!-- Properties omitted for brevity -->
              </packing>
            </child>

            <!-- Other entry fields and widgets -->
            <!-- ... -->

            <!-- Submit Button -->
            <child>
              <object class="GtkButton" id="submit_ticket_button">
                <!-- Properties omitted for brevity -->
              </object>
              <packing>
                <!-- Properties omitted for brevity -->
              </packing>
            </child>
          </object>
        </child>
      </object>
    </child>
  </object>
</interface>

```

The layout is achieved using a GtkGrid container, which arranges its children in rows and columns. Each child widget is packed into the grid with specific parameters like position and size, ensuring a well-organized and user-friendly interface.

#### Ticket_form.c

In ticket_form_init(), we initialize the ticket form page by loading the UI file, filling the TicketFormPage structure with UI widgets, setting the current date to the start date label, and adding the page to the stack.

```
#include "../include/ticket_form.h"
#include <gtk/gtk.h>
#include <time.h>

static TicketFormPage ticket_form_page;

void ticket_form_init(GtkWidget *stack) {
    // Initialize GtkBuilder
    GtkBuilder *builder = gtk_builder_new();
    GError *error = NULL;

    // Load UI file
    if (!gtk_builder_add_from_file(builder, "ui/ticket_form.ui", &error)) {
        g_warning("Error loading UI file: %s", error->message);
        g_error_free(error);
        return;
    }

    // Fill structure with UI widgets
    ticket_form_page.stack = stack;
    ticket_form_page.page_grid = GTK_WIDGET(gtk_builder_get_object(builder, "page_grid"));
    ticket_form_page.page_label = GTK_WIDGET(gtk_builder_get_object(builder, "page_label"));
    ticket_form_page.entry_title = GTK_WIDGET(gtk_builder_get_object(builder, "ticket_title_entry"));
    ticket_form_page.error_label = GTK_WIDGET(gtk_builder_get_object(builder, "error_label"));
    //...

    // Get current date and set to start date label
    GtkWidget *start_date_label = GTK_WIDGET(gtk_builder_get_object(builder, "ticket_start_date_label"));
    //...

    // Add the page to the stack
    GtkWidget *ticket_form_page = GTK_WIDGET(gtk_builder_get_object(builder, "main_vertical_box"));
    gtk_stack_add_titled(GTK_STACK(stack), ticket_form_page, "ticket_form", "Ticket Form");

    // Cleanup
    g_object_unref(builder);
}

```

#### Applying CSS

To apply CSS to the ticket form page, we can target specific widgets or classes within the page's UI. For example:

```
/* Example CSS for ticket form page */
#page_label {
    font-weight: bold;
    font-size: 20px;
}

#ticket_title_entry,
#ticket_description_textview,
#ticket_priority_combobox,
#ticket_status_combobox,
#ticket_deadline_entry,
#ticket_owner_entry {
    width: 300px;
}

```

By example this CSS snippet applies styles to the page label and various entry fields on the ticket form page, ensuring a consistent and visually appealing layout.

### Database implementation

#### Table User

| Column          | Type    | Description                          |
|-----------------|---------|--------------------------------------|
| id              | INTEGER | Primary key                          |
| username        | TEXT    | Username of the user                 |
| password        | TEXT    | Password of the user                 |
| email           | TEXT    | Email address of the user            |
| register_date   | TEXT    | Date when the user registered        |
| role            | TEXT    | Role of the user (e.g., admin, user) |


#### Table project

| Column          | Type    | Description                          |
|-----------------|---------|--------------------------------------|
| id              | INTEGER | Primary key                          |
| title           | TEXT    | Title of the project                 |
| description     | TEXT    | Description of the project           |
| start_date      | TEXT    | Start date of the project            |
| deadline_date   | TEXT    | Deadline date of the project         |
| register_date   | TEXT    | Date when the project was registered |
| project_leader  | INTEGER | User ID of the project leader        |


#### Table ticket

| Column          | Type    | Description                          |
|-----------------|---------|--------------------------------------|
| id              | INTEGER | Primary key                          |
| title           | TEXT    | Title of the ticket                  |
| description     | TEXT    | Description of the ticket            |
| priority        | TEXT    | Priority of the ticket               |
| status          | TEXT    | Status of the ticket                 |
| owner           | INTEGER | User ID of the ticket owner          |
| project_id      | INTEGER | Project ID to which the ticket belongs|
| start_date      | TEXT    | Start date of the ticket             |
| deadline_date   | TEXT    | Deadline date of the ticket          |
| register_date   | TEXT    | Date when the ticket was registered  |


### ScreenShots

![alt text](https://i.imgur.com/1CAUj21.jpeg)

![alt text](https://i.imgur.com/53uayvq.jpeg)

![alt text](https://i.imgur.com/cKai0aE.jpeg)
