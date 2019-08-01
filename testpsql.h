// Spencer Kline 06/03/2019
#ifndef _TESTPSQL_H
#define _TESTPSQL_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <postgresql/libpq-fe.h>

#define CONNINFO "dbname = postgres host = localhost port = 5432 user = skline"
#define BUFFER_SIZE 1024

extern PGconn * dbconn;

struct person {
    char first_name[50];
    char date_of_birth[256];
};

static void exit_clean(PGconn *dbconn);
PGconn *connect_database();
// create person table that has pk id, first name, and date of birth
int create_person_table(PGconn *dbconn);
// insert new person into person table with first name and date of birth (doesn't check if duplicate)
int insert_person_table(PGconn *dbconn, char * first_name, char * date_of_birth);
// displays the person table
int show_person_table(PGconn *dbconn);
// alter an existing person in the table by id
int alter_person_byId(PGconn *dbconn, int id, char * first_name, char * date_of_birth);
// delete an existing person in the table
int delete_person_byId(PGconn *dbconn, int id);
// drop the person table
int drop_person_table(PGconn *dbconn);
// select person by id
struct person select_person_byId(PGconn *dbconn, int id);
// find and print duplicate rows
int show_person_dups(PGconn *dbconn);
// delete all duplicate rows
int delete_person_dups(PGconn *dbconn);

#endif /* _TESTPSQL_H */
