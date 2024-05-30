#include <sqlite3.h>
#include "../include/database.h"

int initialize_database() {
    sqlite3 *db;
    int rc;

    rc = sqlite3_open("app.db", &db);
    if (rc != SQLITE_OK) {
        return rc;
    }

    const char *sql_users = "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, username TEXT, password TEXT);";
    const char *sql_tickets = "CREATE TABLE IF NOT EXISTS tickets (id INTEGER PRIMARY KEY, title TEXT, status TEXT, user_id INTEGER, FOREIGN KEY(user_id) REFERENCES users(id));";

    char *errmsg;
    rc = sqlite3_exec(db, sql_users, 0, 0, &errmsg);
    if (rc != SQLITE_OK) {
        sqlite3_free(errmsg);
        sqlite3_close(db);
        return rc;
    }

    rc = sqlite3_exec(db, sql_tickets, 0, 0, &errmsg);
    if (rc != SQLITE_OK) {
        sqlite3_free(errmsg);
        sqlite3_close(db);
        return rc;
    }

    sqlite3_close(db);
    return SQLITE_OK;
}
