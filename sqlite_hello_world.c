#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

static int
callback(void *unused, int argc, char **argv, char **azColName)
{
    int i;

    for (i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return (0);
}


static int
sql_execute(sqlite3 *db, char *sqlCmd, char *opString)
{
    int ret;
    char *errMsg = NULL;

    ret = sqlite3_exec(db, sqlCmd, callback, 0, &errMsg);

    if (ret != SQLITE_OK) {
        fprintf(stdout, "%s failed: %s\n", opString, errMsg);
        sqlite3_free(errMsg);
    } else {
        fprintf(stdout, "%s succeded\n", opString);
    }
    return (ret);
}

static int
sqldb_view(sqlite3 *db)
{
    char *sqlCmd;

    sqlCmd = "SELECT * FROM ACCOUNT"; 
    sql_execute(db, sqlCmd, "Select all");

    return (0);
}

static int
sqldb_delete_data(sqlite3 *db, int id)
{
    char sqlCmd[1024];

    snprintf(sqlCmd, sizeof (sqlCmd), 
             "DELETE from ACCOUNT where ID = %d", id);
    sql_execute(db, sqlCmd, "Data delete");

    return (0);
}

static int
sqldb_update_age(sqlite3 *db, int id, int age)
{
    char sqlCmd[1024];

    snprintf(sqlCmd, sizeof (sqlCmd), 
             "UPDATE ACCOUNT set AGE = %d where ID = %d",
             age, id);
    sql_execute(db, sqlCmd, "Data update");

    return (0);
}

static int
sqldb_insert(sqlite3 *db, int id, char *name, int age, char *addr)
{
    char sqlCmd[1024];

    snprintf(sqlCmd, sizeof (sqlCmd), 
             "INSERT INTO ACCOUNT (ID, NAME, AGE, ADDRESS) " \
             "VALUES (%d, \'%s\', %d, \'%s\');", id, name, age, addr);
    sql_execute(db, sqlCmd, "Data insert");

    return (0);
}

static int
sqldb_create(sqlite3 *db)
{
    char *sqlCmd;

    sqlCmd = "CREATE TABLE ACCOUNT(" \
             "ID       INT PRIMARY KEY NOT NULL," \
             "NAME     TEXT            NOT NULL," \
             "AGE      INT             NOT NULL," \
             "ADDRESS  CHAR(50));";
    sql_execute(db, sqlCmd, "Create DB");

    return (0);
}

static sqlite3 *
sqldb_open(char *filename)
{
    int ret;
    sqlite3 *db = NULL;
    ret = sqlite3_open(filename, &db);

    if (ret) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    } else {
        fprintf(stderr, "Opened database successfully\n");
    }
    return db;
}

int
main(int argc, char *argv[])
{
    sqlite3 *db;
    char *errMsg = NULL;
    char *sql = NULL;
    int ret;

    /* Open DB */
    db = sqldb_open(":memory:");

    /* Create table */
    sqldb_create(db);

    /* Insert into table */
    sqldb_insert(db, 1, "Sherlock", 34, "221B Baker Street");
    sqldb_insert(db, 2, "Abby", 23, "123 Sesame Street");

    /* Update data */
    sqldb_update_age(db, 1, 39);

    /* View */
    sqldb_view(db);

    /* Update data */
    sqldb_delete_data(db, 2);


    /* Close DB */
    sqlite3_close(db);
}
