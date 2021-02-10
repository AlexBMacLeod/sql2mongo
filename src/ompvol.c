#include <mysql/mysql.h>
#include <mongoc/mongoc.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <pthread.h>


#include "../include/list.h"
#include "../include/interpolate.h"
#include "../include/sql.h"
#include "../include/bars.h"

int COL = 7;
int ROW = 25;
int THREAD = 4;

//omp_set_num_threads(4);

struct args {
    struct node* list;
    float *arr;
};

void finish_with_error(MYSQL *con);
void mongoUp(float *const restrict tmpArr, mongoc_collection_t *collection);
void storeNumsMongo(int volRow, int Freq, mongoc_collection_t *collection);

int
main (int argc, char *argv[])
{
   char sqlQuery[] = "select * from testjointhree limit 5000";
   char sqlDatabase[] = "pipelines";
   char sqlPassword[] = "imacnerd";
   char sqlUsername[] = "root";
   int volRow = 4;
   int Freq = 250;
   const char *uri_string = "mongodb://localhost:27017";
   mongoc_uri_t *uri;
   mongoc_client_t *client;
   mongoc_database_t *database;
   mongoc_collection_t *collection;
   bson_error_t error;
   mongoc_init ();
   uri = mongoc_uri_new_with_error (uri_string, &error);
   if (!uri) {
      fprintf (stderr,
               "failed to parse URI: %s\n"
               "error message:       %s\n",
               uri_string,
               error.message);
      return EXIT_FAILURE;
   }
   client = mongoc_client_new_from_uri (uri);
   if (!client) {
      return EXIT_FAILURE;
   }
   mongoc_client_set_appname (client, "connect-example");
   database = mongoc_client_get_database (client, "db_name");
   collection = mongoc_client_get_collection (client, "db_name", "pytest");
   fromMySQL(volRow, sqlUsername, sqlPassword, sqlDatabase, sqlQuery);
   launchThreadRepairers(volRow);
   storeNumsMongo( volRow, Freq, collection);
   mongoc_collection_destroy (collection);
   mongoc_database_destroy (database);
   mongoc_uri_destroy (uri);
   mongoc_client_destroy (client);
   mongoc_cleanup ();
   deleteList();
}

void mongoUp(float *const restrict tmpArr, mongoc_collection_t *collection)
{
   bson_t *insert, child;
   uint32_t i,j;
   insert = bson_new();
   char buf[16];
   const char *key;
   bson_error_t error;
   size_t keylen;
   for(j = 0; j < ROW; j++)
   {
      keylen = bson_uint32_to_string (j, &key, buf, sizeof buf);
      BSON_APPEND_ARRAY_BEGIN (insert, key, &child);
      for (i = 0; i < COL; ++i) 
      {
         keylen = bson_uint32_to_string (i, &key, buf, sizeof buf);
         bson_append_double (&child, key, (int) keylen, tmpArr[i+COL*j]);
      }
      bson_append_array_end (insert, &child);
   }

   if (!mongoc_collection_insert_one (collection, insert, NULL, NULL, &error)) 
   {
      fprintf (stderr, "%s\n", error.message);
   }
   bson_destroy (insert);
}



void storeNumsMongo(int volRow, int Freq, mongoc_collection_t *collection)
{
   struct node *plist = last;
   int j=0;
   struct args arg[THREAD];
   for(int i=0; i<THREAD; i++)
   {
    arg[i].arr = malloc(sizeof(float)*ROW*COL);
   }
   //memset(tmpArr, 0.0, ROW*COL*sizeof(*tmpArr));
   while(plist!=NULL) //to go through the linked list
   {
       for(int i=0; i<THREAD;i++)
       {
           if(plist!=NULL)
           {
               arg[i].list = plist;
               plist=plist->prev;
               j++;
           }
       }
        #pragma omp parallel for
        for(int i=0;i<j;i++)
        {   
            if(plist!=NULL){
            secondsMatrix(arg[i].list, arg[i].arr, Freq, volRow);
            }
        }
        for(int i=0; i<j;i++) if(plist!=NULL) mongoUp(arg[i].arr, collection);
        j=0;
   }
   for(int i=0;i<THREAD;i++) free(arg[i].arr);
}