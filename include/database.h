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
int insert_ticket(const char *title, const char *description, const char *priority, const char *status, const char *owner, const char *start_date, const char *deadline, int project_id);
char** fetch_all_project_names(int* num_projects);
int get_project_id(const char *project_name);
void print_tickets_with_projects();
int insert_project(const char *title, const char *description, int leader_id, const char *start_date, const char *deadline);
int get_leader_id_by_name(const char *leader_name); // Add this function prototype


#endif // DATABASE_H
