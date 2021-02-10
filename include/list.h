#pragma once

extern int COL;
extern int ROW;
extern int THREAD;

struct node {
    struct node* next; // Pointer to next node in DLL
    struct node* prev; // Pointer to previous node in DLL
    float row[7];
};

extern struct node *head;
extern struct node *last;
extern struct node *current;

void insertFirst(float data[COL]);
bool isEmpty();
void deleteList();