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
// create person table that has pk id, first name, and date of birth
void create_person_table(PGconn *dbconn);
// insert new person into person table with first name and date of birth (doesn't check if duplicate)
void insert_person_table(PGconn *dbconn, char * first_name, char * date_of_birth);
// displays the person table
void show_person_table(PGconn *dbconn);
// alter an existing person in the table by id
void alter_person_table(PGconn *dbconn, int id, char * first_name, char * date_of_birth);
// delete an existing person in the table
void delete_person_byId(PGconn *dbconn, int id);
// drop the person table
void drop_person_table(PGconn *dbconn);


#endif /* _TESTPSQL_H */
