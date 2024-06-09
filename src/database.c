#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>


#include "../include/database.h"

// New function to insert a project
int insert_project(const char *title, const char *description, int leader_id, const char *start_date, const char *deadline) {
    sqlite3 *db;
    char *errmsg = 0;
    int rc;
    char sql[1024];

    // Open the database
    rc = sqlite3_open("app.db", &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return rc;
    }
    printf("Database opened successfully.\n");

    // SQL statement to insert a project
    snprintf(sql, sizeof(sql),
        "INSERT INTO projects (title, description, start_date, deadline, register_date, leader_id) "
        "VALUES ('%s', '%s', '%s', '%s', date('now'), %d)",
        title, description, start_date, deadline, leader_id);
    printf("SQL Query: %s\n", sql);

    // Execute the SQL statement
    rc = sqlite3_exec(db, sql, 0, 0, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        sqlite3_close(db);
        return rc;
    }
    printf("Project inserted successfully.\n");

    // Close the database
    sqlite3_close(db);
    printf("Database closed.\n");

    return rc;
}

// New function to fetch all leader names
char** fetch_all_leader_names(int* num_leaders) {
    sqlite3 *db;
    int rc;
    sqlite3_stmt *stmt;

    rc = sqlite3_open("app.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    const char *sql = "SELECT username FROM users WHERE role = 'leader';";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }

    char** leader_names = NULL;
    int count = 0;

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        if (!leader_names) {
            leader_names = malloc(sizeof(char*));
        } else {
            leader_names = realloc(leader_names, (count + 1) * sizeof(char*));
        }

        if (!leader_names) {
            fprintf(stderr, "Out of memory\n");
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return NULL;
        }

        leader_names[count] = strdup((const char*)sqlite3_column_text(stmt, 0));
        count++;
    }

    *num_leaders = count;

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return leader_names;
}


// Function to print every ticket with its associated project ID and name
void print_tickets_with_projects() {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    const char *sql = "SELECT t.id, t.title AS ticket_title, t.project_id, p.title AS project_title FROM tickets t INNER JOIN projects p ON t.project_id = p.id";
    
    // Open the database
    if (sqlite3_open("app.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Prepare the SQL query
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // Execute the query and print the results
    printf("Tickets with associated project ID and name:\n");
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int ticket_id = sqlite3_column_int(stmt, 0);
        const char *ticket_title = (const char *)sqlite3_column_text(stmt, 1);
        int project_id = sqlite3_column_int(stmt, 2);
        const char *project_title = (const char *)sqlite3_column_text(stmt, 3);

        printf("Ticket ID: %d, Ticket Title: %s, Project ID: %d, Project Title: %s\n", ticket_id, ticket_title, project_id, project_title);
    }

    // Finalize statement and close the database
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

// Function to retrieve the project ID based on its name
int get_project_id(const char *project_name) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int project_id = -1;

    // Open the database
    if (sqlite3_open("app.db", &db) != SQLITE_OK) {
        g_warning("Cannot open database: %s", sqlite3_errmsg(db));
        return -1;
    }

    // Prepare the SQL query
    const char *sql = "SELECT id FROM projects WHERE title = ?";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        g_warning("Failed to prepare statement: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    // Bind the project name to the query
    if (sqlite3_bind_text(stmt, 1, project_name, -1, SQLITE_STATIC) != SQLITE_OK) {
        g_warning("Failed to bind project name: %s", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return -1;
    }

    // Execute the query
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        project_id = sqlite3_column_int(stmt, 0);
    } else {
        g_warning("No project found with name: %s", project_name);
    }

    // Clean up
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return project_id;
}


char** fetch_all_project_names(int* num_projects) {
    sqlite3 *db;
    int rc;
    sqlite3_stmt *stmt;

    rc = sqlite3_open("app.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    const char *sql = "SELECT title FROM projects;";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }

    char** project_names = NULL;
    int count = 0;

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        if (!project_names) {
            project_names = malloc(sizeof(char*));
        } else {
            project_names = realloc(project_names, (count + 1) * sizeof(char*));
        }

        if (!project_names) {
            fprintf(stderr, "Out of memory\n");
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return NULL;
        }

        project_names[count] = strdup((const char*)sqlite3_column_text(stmt, 0));
        count++;
    }

    *num_projects = count;

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return project_names;
}


// Function to insert a ticket into the database
int insert_ticket(const char *title, const char *description, const char *priority, const char *status, const char *owner, const char *start_date, const char *deadline, int project_id) {
    sqlite3 *db;
    char *errmsg = 0;
    int rc;
    char sql[1024];

    // Open the database
    rc = sqlite3_open("app.db", &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return rc;
    }
    printf("Database opened successfully.\n");

    // SQL statement to insert a ticket
    snprintf(sql, sizeof(sql),
        "INSERT INTO tickets (title, description, priority, status, owner_id, project_id, start_date, deadline, register_date) "
        "VALUES ('%s', '%s', '%s', '%s', '%s', %d, '%s', '%s', date('now'))",
        title, description, priority, status, owner, project_id, start_date, deadline);
    printf("SQL Query: %s\n", sql);

    // Execute the SQL statement
    rc = sqlite3_exec(db, sql, 0, 0, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        sqlite3_close(db);
        return rc;
    }
    printf("Ticket inserted successfully.\n");

    // Close the database
    sqlite3_close(db);
    printf("Database closed.\n");

    return rc;
}


int initialize_database() {
    sqlite3 *db;
    int rc;

    rc = sqlite3_open("app.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    // Check if tables already exist
    int tables_exist = 0;
    const char *sql_check_tables = "SELECT count(*) FROM sqlite_master WHERE type='table';";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql_check_tables, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return rc;
    }
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        tables_exist = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);

    // Drop and create tables only if they don't exist
    if (!tables_exist) {
        const char *sql_drop_tables = "DROP TABLE IF EXISTS users; "
                                      "DROP TABLE IF EXISTS projects; "
                                      "DROP TABLE IF EXISTS tickets; "
                                      "DROP TABLE IF EXISTS user_projects;";

        char *errmsg;
        rc = sqlite3_exec(db, sql_drop_tables, 0, 0, &errmsg);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error dropping tables: %s\n", errmsg);
            sqlite3_free(errmsg);
            sqlite3_close(db);
            return rc;
        }

        // Create new tables
        const char *sql_users = "CREATE TABLE IF NOT EXISTS users ("
                                "id INTEGER PRIMARY KEY, "
                                "username TEXT, "
                                "password TEXT, "
                                "email TEXT, "
                                "register_date TEXT, "
                                "role TEXT);";

        const char *sql_projects = "CREATE TABLE IF NOT EXISTS projects ("
                                   "id INTEGER PRIMARY KEY, "
                                   "title TEXT, "
                                   "description TEXT, "
                                   "start_date TEXT, "
                                   "deadline TEXT, "
                                   "register_date TEXT, "
                                   "leader_id INTEGER, "
                                   "FOREIGN KEY(leader_id) REFERENCES users(id));";

        const char *sql_tickets = "CREATE TABLE IF NOT EXISTS tickets ("
                                  "id INTEGER PRIMARY KEY, "
                                  "title TEXT, "
                                  "description TEXT, "
                                  "priority TEXT, "
                                  "status TEXT, "
                                  "owner_id INTEGER, "
                                  "project_id INTEGER, "
                                  "start_date TEXT, "
                                  "deadline TEXT, "
                                  "register_date TEXT, "
                                  "FOREIGN KEY(owner_id) REFERENCES users(id), "
                                  "FOREIGN KEY(project_id) REFERENCES projects(id));";

        const char *sql_user_projects = "CREATE TABLE IF NOT EXISTS user_projects ("
                                        "user_id INTEGER, "
                                        "project_id INTEGER, "
                                        "FOREIGN KEY(user_id) REFERENCES users(id), "
                                        "FOREIGN KEY(project_id) REFERENCES projects(id), "
                                        "PRIMARY KEY(user_id, project_id));";

        rc = sqlite3_exec(db, sql_users, 0, 0, &errmsg);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error creating users table: %s\n", errmsg);
            sqlite3_free(errmsg);
            sqlite3_close(db);
            return rc;
        }

        rc = sqlite3_exec(db, sql_projects, 0, 0, &errmsg);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error creating projects table: %s\n", errmsg);
            sqlite3_free(errmsg);
            sqlite3_close(db);
            return rc;
        }

        rc = sqlite3_exec(db, sql_tickets, 0, 0, &errmsg);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error creating tickets table: %s\n", errmsg);
            sqlite3_free(errmsg);
            sqlite3_close(db);
            return rc;
        }

        rc = sqlite3_exec(db, sql_user_projects, 0, 0, &errmsg);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error creating user_projects table: %s\n", errmsg);
            sqlite3_free(errmsg);
            sqlite3_close(db);
            return rc;
        }
    }

    sqlite3_close(db);
    return SQLITE_OK;
}


int validate_user_credentials(const char *username, const char *password) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int user_id = -1;

    // Open the database
    if (sqlite3_open("app.db", &db) != SQLITE_OK) {
        g_warning("Cannot open database: %s", sqlite3_errmsg(db));
        return -1;
    }

    // Prepare the SQL query
    const char *sql = "SELECT id, password FROM users WHERE username = ?";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        g_warning("Failed to prepare statement: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    // Bind the username to the query
    if (sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC) != SQLITE_OK) {
        g_warning("Failed to bind username: %s", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return -1;
    }

    // Execute the query
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *db_password = (const char *)sqlite3_column_text(stmt, 1);
        if (g_strcmp0(password, db_password) == 0) {
            user_id = sqlite3_column_int(stmt, 0);
        } else {
            g_warning("Password mismatch for user: %s", username);
        }
    } else {
        g_warning("No user found with username: %s", username);
    }

    // Clean up
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return user_id;
}

Project* fetch_all_projects(int* num_projects) {
    sqlite3 *db;
    int rc;
    sqlite3_stmt *stmt;
    Project *projects = NULL;

    rc = sqlite3_open("app.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    const char *sql = "SELECT id, title, description, start_date, deadline, register_date, leader_id FROM projects;";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }

    int count = 0;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        count++;
    }

    *num_projects = count;
    projects = malloc(count * sizeof(Project));

    sqlite3_reset(stmt);

    count = 0;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        projects[count].id = sqlite3_column_int(stmt, 0);
        strncpy(projects[count].title, (const char*)sqlite3_column_text(stmt, 1), sizeof(projects[count].title) - 1);
        strncpy(projects[count].description, (const char*)sqlite3_column_text(stmt, 2), sizeof(projects[count].description) - 1);
        strncpy(projects[count].start_date, (const char*)sqlite3_column_text(stmt, 3), sizeof(projects[count].start_date) - 1);
        strncpy(projects[count].deadline, (const char*)sqlite3_column_text(stmt, 4), sizeof(projects[count].deadline) - 1);
        strncpy(projects[count].register_date, (const char*)sqlite3_column_text(stmt, 5), sizeof(projects[count].register_date) - 1);
        projects[count].leader_id = sqlite3_column_int(stmt, 6);
        count++;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return projects;
}

void free_projects(Project* projects, int num_projects) {
    free(projects);
}

Ticket* fetch_tickets_by_project_id(int project_id, int *num_tickets) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;

    *num_tickets = 0;
    rc = sqlite3_open("app.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    const char *sql = "SELECT id, title, description, priority, status, owner_id, project_id, start_date, deadline, register_date FROM tickets WHERE project_id = ?";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch tickets: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }

    sqlite3_bind_int(stmt, 1, project_id);

    Ticket *tickets = NULL;
    int capacity = 10;
    tickets = malloc(capacity * sizeof(Ticket));
    if (!tickets) {
        fprintf(stderr, "Out of memory\n");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return NULL;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        if (*num_tickets >= capacity) {
            capacity *= 2;
            tickets = realloc(tickets, capacity * sizeof(Ticket));
            if (!tickets) {
                fprintf(stderr, "Out of memory\n");
                sqlite3_finalize(stmt);
                sqlite3_close(db);
                return NULL;
            }
        }

        tickets[*num_tickets].id = sqlite3_column_int(stmt, 0);
        strncpy(tickets[*num_tickets].title, (const char*)sqlite3_column_text(stmt, 1), sizeof(tickets[*num_tickets].title));
        strncpy(tickets[*num_tickets].description, (const char*)sqlite3_column_text(stmt, 2), sizeof(tickets[*num_tickets].description));
        strncpy(tickets[*num_tickets].priority, (const char*)sqlite3_column_text(stmt, 3), sizeof(tickets[*num_tickets].priority));
        strncpy(tickets[*num_tickets].status, (const char*)sqlite3_column_text(stmt, 4), sizeof(tickets[*num_tickets].status));
        tickets[*num_tickets].owner_id = sqlite3_column_int(stmt, 5);
        tickets[*num_tickets].project_id = sqlite3_column_int(stmt, 6);
        strncpy(tickets[*num_tickets].start_date, (const char*)sqlite3_column_text(stmt, 7), sizeof(tickets[*num_tickets].start_date));
        strncpy(tickets[*num_tickets].deadline, (const char*)sqlite3_column_text(stmt, 8), sizeof(tickets[*num_tickets].deadline));
        strncpy(tickets[*num_tickets].register_date, (const char*)sqlite3_column_text(stmt, 9), sizeof(tickets[*num_tickets].register_date));

        (*num_tickets)++;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return tickets;
}

void free_tickets(Ticket *tickets) {
    free(tickets);
}

int get_leader_id_by_name(const char *leader_name) {
    int leader_id = -1;
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    const char *sql = "SELECT id FROM leaders WHERE name = ?";

    // Open the database connection (assuming `db_open` is a function that opens the database)
    rc = sqlite3_open("app.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    }

    // Prepare the SQL statement
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        // Bind the leader name to the SQL statement
        sqlite3_bind_text(stmt, 1, leader_name, -1, SQLITE_STATIC);

        // Execute the statement and fetch the result
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            leader_id = sqlite3_column_int(stmt, 0);
        }
    }

    // Finalize the statement and close the database connection
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return leader_id;
}

