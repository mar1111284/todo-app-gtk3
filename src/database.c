#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../include/database.h"

int initialize_database() {
    sqlite3 *db;
    int rc;

    rc = sqlite3_open("app.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    // Drop existing tables if they exist
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

    // Insert fake data
    const char *sql_insert_users = "INSERT INTO users (username, password, email, register_date, role) VALUES "
                                   "('user1', 'pass1', 'user1@example.com', '2023-01-01', 'admin'), "
                                   "('user2', 'pass2', 'user2@example.com', '2023-01-02', 'user');";

    rc = sqlite3_exec(db, sql_insert_users, 0, 0, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error inserting into users table: %s\n", errmsg);
        sqlite3_free(errmsg);
        sqlite3_close(db);
        return rc;
    }

    const char *sql_insert_projects = "INSERT INTO projects (title, description, start_date, deadline, register_date, leader_id) VALUES "
                                      "('Project App', 'Description of Project 1', '2023-01-01', '2023-12-31', '2023-01-01', 1), "
                                      "('Project 2', 'Description of Project 2', '2023-02-01', '2023-11-30', '2023-02-01', 2);";

    rc = sqlite3_exec(db, sql_insert_projects, 0, 0, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error inserting into projects table: %s\n", errmsg);
        sqlite3_free(errmsg);
        sqlite3_close(db);
        return rc;
    }

    const char *sql_insert_tickets = "INSERT INTO tickets (title, description, priority, status, owner_id, project_id, start_date, deadline, register_date) VALUES "
                                     "('Ticket 1', 'Description of Ticket 1', 'High', 'TODO', 1, 1, '2023-01-01', '2023-06-30', '2023-01-01'), "
                                     "('Ticket 2', 'Description of Ticket 2', 'Medium', 'In Progress', 2, 2, '2023-02-01', '2023-07-31', '2023-02-01');";

    rc = sqlite3_exec(db, sql_insert_tickets, 0, 0, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error inserting into tickets table: %s\n", errmsg);
        sqlite3_free(errmsg);
        sqlite3_close(db);
        return rc;
    }

    const char *sql_insert_user_projects = "INSERT INTO user_projects (user_id, project_id) VALUES "
                                           "(1, 1), "
                                           "(1, 2), "
                                           "(2, 1);";

    rc = sqlite3_exec(db, sql_insert_user_projects, 0, 0, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error inserting into user_projects table: %s\n", errmsg);
        sqlite3_free(errmsg);
        sqlite3_close(db);
        return rc;
    }

    sqlite3_close(db);
    return SQLITE_OK;
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
