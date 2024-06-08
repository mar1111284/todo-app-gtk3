#ifndef DATABASE_H
#define DATABASE_H

typedef struct {
    int id;
    char title[256];
    char description[512];
    char start_date[20];
    char deadline[20];
    char register_date[20];
    int leader_id;
} Project;

int initialize_database();
Project* fetch_all_projects(int* num_projects);
void free_projects(Project* projects, int num_projects);

#endif
