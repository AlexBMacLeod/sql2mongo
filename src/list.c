#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "../include/list.h"


struct node *head = NULL;
struct node *last = NULL;
struct node *current = NULL;

void deleteList()
{
   /* deref head_ref to get the real head */
   struct node* cur;
   struct node* next;

   cur = head;
   while (cur != NULL) 
   {
       next = cur->next;
       free(cur);
       cur = next;
   }
   
   /* deref head_ref to affect the real head back
      in the caller. */
}

void insertFirst(float data[COL]) 
{
   //create a link
   struct node *link = (struct node*) malloc(sizeof(struct node));
   memcpy(link->row, data, sizeof(float)*COL);
    link->next = NULL;
    link->prev = NULL;	
   if(isEmpty()) {
      //make it the last link
      head = link;
      last = link;
   } else {
      //update first prev link
      link->next = head;
      head->prev = link;
      head = link;
   }
}

bool isEmpty(void) {
   return head == NULL;
}