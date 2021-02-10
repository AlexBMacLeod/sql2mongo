#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../include/list.h"
#include "../include/interpolate.h"

void repairer(struct node *tmp, float first, float end, int count, int colNum)
{
  int cnt = 1;
  if(first==0) first = end;
  for(int i = 0; i<(int)count; i++)
  {
    tmp->row[colNum] = first+(float)cnt*(end-first)/(float)count;
    cnt ++;
    tmp = tmp->prev;
  }
}
//Are there never any bz?
void repairCols(int colNum)
{
  float first, end;
  int count = 0;
  int switches;
  struct node *tail = last;
  struct node *plist = last;
  first = 0.0;
  switches = 0;
  while(plist != NULL)
  {
    if(plist->row[colNum]<.001)
    {
      switches = 1;
      count ++;
      plist = plist->prev;
    }
    else
    {
      if(switches == 1)
      {
        end = plist->row[colNum];
        repairer(tail, first, end, count, colNum);
        count = 0;
        switches = 0;
        tail = plist;
        first = plist->row[colNum];
        plist = plist->prev;
      }
      else
      {
        tail = plist;
        first = plist->row[colNum];
        plist = plist->prev;
      }
    }
  }
  if(switches == 1) 
  {
      repairer(tail, first, first, count, colNum);
  }
}

void launchThreadRepairers(int volRow)
{
  int startCol = volRow+1;
  #pragma omp parallel for
  for (int i=startCol; i < COL; i++)
  {
      repairCols(i);
  }
}