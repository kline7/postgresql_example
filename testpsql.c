/* Spencer Kline 06/03/2019
 * This is a simple program to connect to a DB,
 * Insert some data, query some data, update some data, and remove some data, drop a table, show data in the table,
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

int create_person_table(PGconn *dbconn){
  PGresult * response = PQexec(dbconn, "SET client_min_messages = error;");

  response = PQexec(dbconn, "CREATE TABLE IF NOT EXISTS person(id BIGSERIAL NOT NULL PRIMARY KEY, first_name VARCHAR(50) NOT NULL, date_of_birth DATE NOT NULL);");

  if(PQresultStatus(response) != PGRES_COMMAND_OK){
    fprintf(stderr, "ERROR: CREATE TABLE person failed. %s\n", PQerrorMessage(dbconn));
    PQclear(response);
    exit_clean(dbconn);
    return -1;
  }else{
    PQclear(response);
    printf("\nperson table created or exists.\n");
    return 1;
  }
}

int insert_person_table(PGconn *dbconn, char * first_name, char * date_of_birth){
  PGresult * response = NULL;
  char buffer[BUFFER_SIZE];
  int buffer_len = snprintf(buffer, sizeof(buffer), "INSERT INTO person( first_name, date_of_birth) VALUES(%s, %s);", PQescapeLiteral(dbconn, first_name, (size_t)strlen(first_name)), PQescapeLiteral(dbconn, date_of_birth, (size_t)strlen(date_of_birth)));

  if(buffer_len > BUFFER_SIZE){
    fprintf(stderr, "ERROR: buffer_size is too small for insert.\n");
    return -2;
  }

  //Assert that the table exists
  create_person_table(dbconn);

  response = PQexec(dbconn, buffer);

  if(PQresultStatus(response) != PGRES_COMMAND_OK){
    fprintf(stderr, "ERROR: INSERT INTO person command failed. %s\n", PQerrorMessage(dbconn));
    PQclear(response);
    exit_clean(dbconn);
    return -1;
  }else{
    printf("\n person added. \n");
  }

  PQclear(response);
  return 1;
}

int show_person_table(PGconn *dbconn){
  PGresult * response = NULL;
  int nFields, nRows;
  response = PQexec(dbconn, "SELECT * FROM person;");

  if(PQresultStatus(response) != PGRES_TUPLES_OK){
    fprintf(stderr, "ERROR: show person command failed. %s\n", PQerrorMessage(dbconn));
    PQclear(response);
    exit_clean(dbconn);
    return -1;    
  }

  nFields = PQnfields(response);
  nRows = PQntuples(response);

  if(nFields < 0 || nRows < 0){
    fprintf(stderr, "ERROR: show person command failed no rows or fields.\n");
    PQclear(response);
    return -2;
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
  return 1;
}

int delete_person_byId(PGconn *dbconn, int id){
  PGresult * response = NULL;
  char buffer[BUFFER_SIZE];
  int buffer_len;
  
  if(id < 0 || id > INT32_MAX ){
    fprintf(stderr, "ERROR: id given is not within range.\n");
    return -2;
  }

  buffer_len = snprintf(buffer, sizeof(buffer), "DELETE FROM person WHERE id = %u;", id);
  if(buffer_len > BUFFER_SIZE){
    fprintf(stderr, "ERROR: buffer_size is too small for delete.\n");
    return -2;
  }

  response = PQexec(dbconn, buffer);

  if(PQresultStatus(response) != PGRES_COMMAND_OK){
    fprintf(stderr, "ERROR: DELETE FROM person command failed. %s\n", PQerrorMessage(dbconn));
    PQclear(response);
    exit_clean(dbconn);
    return -1;
  }else{
    printf("\n person %u DELETED. \n", id);
  }

  PQclear(response);  
  return 1;
}

int alter_person_byId(PGconn *dbconn, int id, char * first_name, char * date_of_birth){
  PGresult * response = PQexec(dbconn, "SET client_min_messages = error;");
  char buffer[BUFFER_SIZE];
  int buffer_len;
  
  if(id < 0 || id > INT32_MAX ){
    fprintf(stderr, "ERROR: id given is not within range.\n");
    return -2;
  }

  if(first_name == NULL){
    fprintf(stderr, "ERROR: No first name given for update.\n");
    return -2;
  }

  if(date_of_birth == NULL){
    fprintf(stderr, "ERROR: No date of birth given for update.\n");
    return -2;   
  }

  buffer_len = snprintf(buffer, sizeof(buffer), "UPDATE person SET first_name = %s, date_of_birth = %s WHERE id = %u;", PQescapeLiteral(dbconn, first_name, (size_t)strlen(first_name)), PQescapeLiteral(dbconn, date_of_birth, (size_t)strlen(date_of_birth)), id);
  if(buffer_len > BUFFER_SIZE){
    fprintf(stderr, "ERROR: buffer_size is too small for alter.\n");
    return -2;
  }

  response = PQexec(dbconn, buffer);

  if(PQresultStatus(response) != PGRES_COMMAND_OK){
    fprintf(stderr, "ERROR: alter person by id command failed. %s\n", PQerrorMessage(dbconn));
    PQclear(response);
    exit_clean(dbconn);
    return -1;
  }else{
    printf("\n person %u altered. \n", id);
  }
  PQclear(response);
  return 1;
}

int drop_person_table(PGconn *dbconn){
  PGresult * response = NULL;

  response = PQexec(dbconn, "DROP TABLE person");

  if(PQresultStatus(response) != PGRES_COMMAND_OK){
    fprintf(stderr, "ERROR: DROP TABLE person command failed. %s\n", PQerrorMessage(dbconn));
    PQclear(response);
    exit_clean(dbconn);
    return -1;
  }else{
    printf("\n person table DELETED.\n");
  }

  PQclear(response);  
  return 1;
}

struct person select_person_byId(PGconn *dbconn, int id){
  PGresult * response = NULL;
  char buffer[BUFFER_SIZE];
  int buffer_len;
  int nFields, nRows;
  struct person ret;
  
  if(id < 0 || id > INT32_MAX ){
    fprintf(stderr, "ERROR: id given is not within range.\n");
    ret.first_name[0] = '\0';
    return ret;
  }

  buffer_len = snprintf(buffer, sizeof(buffer), "SELECT * FROM person WHERE id = %u;", id);
  if(buffer_len > BUFFER_SIZE){
    fprintf(stderr, "ERROR: buffer_size is too small for select by id.\n");
    exit(-2);
  }

  response = PQexec(dbconn, buffer);

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
    ret.first_name[0] ='\0';
    return ret;
  }

  strcpy(ret.first_name,PQgetvalue(response,0,1));
  strcpy(ret.date_of_birth,PQgetvalue(response,0,2));

  return ret;
}

int main(){
  struct person resp;
  printf("\nConnecting to db...\n");
  connect_database();
  create_person_table(dbconn);
  insert_person_table(dbconn, "Jim", "1965-02-10");
  show_person_table(dbconn);
  resp = select_person_byId(dbconn, 1);
  printf("Person name: %s d-o-b: %s\n", resp.first_name, resp.date_of_birth);
  exit_clean(dbconn);
  return 0;
}
