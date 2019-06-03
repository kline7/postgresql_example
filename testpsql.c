/* Spencer Kline
 * This is a simple program to connect to a DB,
 * Insert some data, query some data, update some data? and remove some data
 * and disconnect from the DB
 */
#include "testpsql.h"

PGconn * dbconn = NULL;

static void exit_clean(PGconn * dbconn){
  PQfinish(dbconn);
}

PGconn *connect_database(){
  dbconn = PQconnectdb(CONNINFO);
  
  if(PQstatus(dbconn) != CONNECTION_OK){
    fprintf(stderr, "ERROR: connect db failed %s\n", PQerrorMessage(dbconn));
    exit_clean(dbconn);
    exit(-1);
  }else{
    printf("\nSucc Connect\n");
  }
  
  return dbconn;
}

void create_person_table(PGconn *dbconn){
  PGresult * response = PQexec(dbconn, "SET client_min_messages = error;");

  response = PQexec(dbconn, "CREATE TABLE IF NOT EXISTS person(id BIGSERIAL NOT NULL PRIMARY KEY, first_name VARCHAR(50) NOT NULL, date_of_birth DATE NOT NULL);");

  if(PQresultStatus(response) != PGRES_COMMAND_OK){
    fprintf(stderr, "ERROR: CREATE TABLE person failed. %s\n", PQerrorMessage(dbconn));
    PQclear(response);
    exit_clean(dbconn);
    exit(-1);
  }else{
    PQclear(response);
    printf("\nperson table created or exists.\n");
    return;
  }
}

void insert_person_table(PGconn *dbconn, char * first_name, char * date_of_birth){
  PGresult * response = NULL;
  char buffer[BUFFER_SIZE];
  int buffer_len = snprintf(buffer, sizeof(buffer), "INSERT INTO person( first_name, date_of_birth) VALUES(%s, %s);", PQescapeLiteral(dbconn, first_name, (size_t)strlen(first_name)), PQescapeLiteral(dbconn, date_of_birth, (size_t)strlen(date_of_birth)));

  if(buffer_len > BUFFER_SIZE){
    fprintf(stderr, "ERROR: buffer_size is too small for insert.\n");
    exit(-2);
  }

  //Assert that the table exists
  create_person_table(dbconn);

  response = PQexec(dbconn, buffer);

  if(PQresultStatus(response) != PGRES_COMMAND_OK){
    fprintf(stderr, "ERROR: INSERT INTO person command failed. %s\n", PQerrorMessage(dbconn));
    PQclear(response);
    exit_clean(dbconn);
    exit(-1);
  }else{
    printf("\n person added. \n");
  }

  PQclear(response);
}

void show_person_table(PGconn *dbconn){
  PGresult * response = NULL;
  int nFields, nRows;
  response = PQexec(dbconn, "SELECT * FROM person;");

  if(PQresultStatus(response) != PGRES_TUPLES_OK){
    fprintf(stderr, "ERROR: show person command failed. %s\n", PQerrorMessage(dbconn));
    PQclear(response);
    exit_clean(dbconn);
    exit(-1);    
  }

  nFields = PQnfields(response);
  nRows = PQntuples(response);

  if(nFields < 0 || nRows < 0){
    fprintf(stderr, "ERROR: show person command failed no rows or fields.\n");
    PQclear(response);
  }

  printf("\n \t\tperson table\n\n");

  for(int i = 0; i < nFields; i++){
    printf("%-15s", PQfname(response, i));
  }
  printf("\n\n");
  for(int r =0; r < nRows; r++){
    for(int f = 0; f < nFields; f++){
      printf("%-15s", PQgetvalue(response, r, f));
    }
    printf("\n");
  }
  PQclear(response);
  return;
}

void delete_person_byId(PGconn *dbconn, int id){
  PGresult * response = NULL;
  char buffer[BUFFER_SIZE];
  int buffer_len;
  
  if(id < 0 || id > INT32_MAX ){
    fprintf(stderr, "ERROR: id given is not within range.\n");
    return;
  }

  buffer_len = snprintf(buffer, sizeof(buffer), "DELETE FROM person WHERE id = %u;", id);
  if(buffer_len > BUFFER_SIZE){
    fprintf(stderr, "ERROR: buffer_size is too small for delete.\n");
    exit(-2);
  }

  response = PQexec(dbconn, buffer);

  if(PQresultStatus(response) != PGRES_COMMAND_OK){
    fprintf(stderr, "ERROR: DELETE FROM person command failed. %s\n", PQerrorMessage(dbconn));
    PQclear(response);
    exit_clean(dbconn);
    exit(-1);
  }else{
    printf("\n person %u DELETED. \n", id);
  }

  PQclear(response);  
  return;
}

int main(){
  printf("\nConnecting to db...\n");
  connect_database();
  create_person_table(dbconn);
  insert_person_table(dbconn, "Tim Cook", "1965-02-10");
  show_person_table(dbconn);
  delete_person_byId(dbconn, 5);
  show_person_table(dbconn);
  exit_clean(dbconn);
  return 0;
}
