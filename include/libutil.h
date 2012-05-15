#ifndef LIBUTIL_H
#define LIBUTIL_H

#include<stdio.h>

void loading(int i,int n){
    static char symb[4]={'-','\\','|','/'};
    if(((int)(i*100.0/n))!=((int)((i+1)*100.0/n))||i==0){
//        fflush(stdout);
        printf("\r %c Computing...\t%3d%% ",symb[((int)((i+1)*100.0/n))%4],((int)((i+1)*100.0/n)));
    }
}

#endif
