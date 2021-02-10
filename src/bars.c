#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../include/list.h"
#include "../include/bars.h"


void secondsMatrix(struct node *tmp, float *tmpArr, int Freq, int volRow)
{
    memset(tmpArr, 0.0, ROW*COL*sizeof(*tmpArr));
    int j=0;
    while(j < ROW && tmp!=NULL) //this j makes a document of second bars
    {
        if(tmp->row[volRow]+tmpArr[volRow +COL*j]<Freq)
        {
            for(int i = 0; i<COL; i++)
            {
                if(i==volRow) tmpArr[volRow +COL*j] += tmp->row[volRow];
                else tmpArr[i+COL*j] += (tmp->row[volRow]*tmp->row[i])/Freq;
            }
            tmp = tmp->next;
        }
    
        else if(tmp->row[volRow]>Freq)
        {
            float tmpScaler = tmp->row[volRow];
            while(tmpScaler>Freq && j<ROW)
            {
                if(tmpArr[volRow +COL*j]>0)
                {
                    float remainder = Freq-tmpArr[volRow +COL*j];
                    for(int i=0; i<COL;i++)
                    {
                        if(i==volRow) tmpArr[volRow +COL*j] += remainder;
                        else tmpArr[i+COL*j] += (remainder*tmp->row[i])/Freq;
                    }
                    tmpScaler -= remainder;
                }
                else
                {
                    for(int i=0; i<COL;i++)
                    {
                        if(i==volRow) tmpArr[volRow +COL*j] =Freq;
                        else tmpArr[i+COL*j] = tmp->row[i];
                    }
                    tmpScaler-=Freq;
                }
                j++;
        }
        if(tmpScaler>0 && j<ROW)
        {
            for(int i=0; i<COL;i++)
            {
                if(i==volRow) tmpArr[volRow +COL*j] =tmpScaler;
                else tmpArr[i+COL*j] = (tmp->row[i]*tmpScaler)/Freq;               
            }
        }
        tmp = tmp->next;
        }
        
        else if(tmp->row[volRow]+tmpArr[volRow +COL*j]>Freq)
        {
            float tmpScaler = tmp->row[volRow];
            while((tmpScaler+tmpArr[volRow +COL*j])>Freq && j<ROW)
            {
                if(tmpArr[volRow +COL*j]>0)
                {
                    float remainder = Freq-tmpArr[volRow +COL*j];
                    for(int i=0; i<COL;i++)
                    {
                        if(i==volRow) tmpArr[volRow +COL*j] += remainder;
                        else tmpArr[i+COL*j] += (remainder*tmp->row[i])/Freq;
                    }
                    tmpScaler -= remainder;
                }
                else
                {
                    for(int i=0; i<COL;i++)
                    {
                        if(i==volRow) tmpArr[volRow +COL*j] =Freq;
                        else tmpArr[i+COL*j] = tmp->row[i];
                    }
                    tmpScaler-=Freq;
                }
                j++;
            }
            if(tmpScaler>0 && j<ROW)
            {
                for(int i=0; i<COL;i++)
                {
                    if(i==volRow) tmpArr[volRow +COL*j] =tmpScaler;
                    else tmpArr[i+COL*j] = (tmp->row[i]*tmpScaler)/Freq;               
                }
            }
            tmp=tmp->next;
        }
        else
        {
            for(int i=0; i<COL; i++)
            {
                float scaler = tmp->row[volRow];
                if(i==volRow) tmpArr[volRow +COL*j] += scaler;
                else tmpArr[i+COL*j] += (tmp->row[i]*scaler)/Freq;
            }
            j++;
            tmp=tmp->next;
        }
    }
    if(j<ROW)
    {
        if(tmpArr[volRow +COL*j]!=0.0)
        {
            for(int i=0;i<COL;i++)
            {
                float sc = Freq/tmpArr[volRow +COL*j];
                if(i==volRow) tmpArr[volRow +COL*j]=tmpArr[volRow +COL*j];
                else tmpArr[i+COL*j] = (tmpArr[i+COL*j])*sc;
            }
            j++;
        }
        while(j<ROW)
        {
            for (int i = 0; i < COL; ++i) 
            { //This i makes a second bar
                tmpArr[i+COL*j]=0.0;
            }
            j++;
        }
    }
}