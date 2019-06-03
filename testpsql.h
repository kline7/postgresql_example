#ifndef _TESTPSQL_H
#define _TESTPSQL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <libpq-fe.h>

#define CONNINFO "dbname = postgres host = localhost port = 5432 user = skline"
#define BUFFER_SIZE 1024

extern PGconn * dbconn;

static void exit_clean(PGconn *dbconn);
PGconn *connect_database();
void create_person_table(PGconn *dbconn);
void insert_person_table(PGconn *dbconn, char * first_name, char * date_of_birth);
void show_person_table(PGconn *dbconn);

#endif /* _TESTPSQL_H */
