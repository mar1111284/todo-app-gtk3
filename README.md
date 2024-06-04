## About the page structure

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


