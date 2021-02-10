#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>

#include "../include/list.h"
#include "../include/sql.h"

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}

void fromMySQL(int volRow, const char username[], const char password[], const char database[], const char query[])
{
  MYSQL *con = mysql_init(NULL);
  int i;
  float arrList[COL];

  if (con == NULL)
  {
      fprintf(stderr, "mysql_init() failed\n");
      exit(1);
  }

  if (mysql_real_connect(con, "localhost", username, password,
          database, 0, NULL, 0) == NULL)
  {
      finish_with_error(con);
  }

  if (mysql_query(con, query))
  {
      finish_with_error(con);
  }

  MYSQL_RES *result = mysql_store_result(con);

  if (result == NULL)
  {
      finish_with_error(con);
  }

  int num_fields = mysql_num_fields(result);

  MYSQL_ROW row;
  float first[] = {57.20, 57.20, 57.20, 57.20, 1, 2658.75, 64.0};
  insertFirst(first);
  insertFirst(first);
  while ((row = mysql_fetch_row(result)))
  {
    for(i=1; i < num_fields; i++){ arrList[i-1] = atof(row[i]);}
    if(arrList[volRow] != 0) 
    {
      insertFirst(arrList);
    }
  }
  float last[] = {57.70, 57.70, 57.70, 57.70, 1, 2658.75, 64.0};
  insertFirst(last);
  insertFirst(last);
  mysql_free_result(result);
  mysql_close(con);
}