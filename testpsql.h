#ifndef _TESTPSQL_H
#define _TESTPSQL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <libpq-fe.h>

#define CONNINFO "dbname = test host = localhost port = 5432 user = skline"

extern PGconn * dbconn;

static void exit_clean(PGconn *conn);
PGconn *connect_database();

#endif /* _TESTPSQL_H */
