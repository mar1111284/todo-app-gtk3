#ifndef DATABASE_H
#define DATABASE_H

typedef struct {
    int id;
    char title[256];
    char description[512];
    char priority[64];
    char status[64];
    int owner_id;
    int project_id;
    char start_date[64];
    char deadline[64];
    char register_date[64];
} Ticket;

typedef struct {
    int id;
    char title[256];
    char description[512];
    char start_date[64];
    char deadline[64];
    char register_date[64];
    int leader_id;
} Project;

int initialize_database();
Project* fetch_all_projects(int *num_projects);
void free_projects(Project *projects, int num_projects);
int validate_user_credentials(const char *username, const char *password);
Ticket* fetch_tickets_by_project_id(int project_id, int *num_tickets);
void free_tickets(Ticket *tickets);

#endif // DATABASE_H
