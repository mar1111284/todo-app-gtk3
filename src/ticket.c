#include "../include/ticket.h"
#include <stdlib.h>

// Ticket fake_tickets[] = {
//     {"As a developer, I want to create a component", "Description for ticket 1", "Urgent", "todo"},
//     {"As a user, I want to login to the system", "Description for ticket 2", "High", "pending"},
//     {"As a user, I want to search for products", "Description for ticket 3", "Medium", "done"},
//     // Add more fake tickets as needed
// };

// Ticket* ticket_new(const char *title_text, const char *description_text) {
//     Ticket *ticket = g_malloc(sizeof(Ticket));
//     ticket->box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    
//     ticket->title = gtk_label_new(title_text);
//     ticket->description = gtk_label_new(description_text);
    
//     ticket->done_button = gtk_button_new_with_label("Done");
//     ticket->delete_button = gtk_button_new_with_label("Delete");

//     gtk_box_pack_start(GTK_BOX(ticket->box), ticket->title, FALSE, FALSE, 0);
//     gtk_box_pack_start(GTK_BOX(ticket->box), ticket->description, FALSE, FALSE, 0);
//     gtk_box_pack_start(GTK_BOX(ticket->box), ticket->done_button, FALSE, FALSE, 0);
//     gtk_box_pack_start(GTK_BOX(ticket->box), ticket->delete_button, FALSE, FALSE, 0);

//     return ticket;
// }

// void ticket_free(Ticket *ticket) {
//     g_free(ticket);
// }
