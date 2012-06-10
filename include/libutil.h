#ifndef LIBUTIL_H
#define LIBUTIL_H

#include<stdio.h>
#include<math.h>

void loading(int i,int n){
    static char symb[4] = {'-','\\','|','/'};
    if(i == 0 || ( floor(i*100.0/n) != floor((i+1)*100.0/n) ) ){
        fflush(stdout);
        printf("\r %c Computing...\t%3d%% ", symb[((int)((i+1)*100.0/n))%4], ((int)((i+1)*100.0/n)));
    }
}

void binning(){
    unsigned int i, count = 0, freq[125];
    double min = 0.93, max = 0.99, width = (max-min)/125, tmp;
    for(i = 0; i < 125; i++)
        freq[i] = 0;
    FILE* f = fopen("histogram.dat","r");
    while(fscanf(f, "%lf\n", &tmp) == 1){
        count++;
        for(i = 0; i < 125; i++)
            if(tmp > min+i*width && tmp <= min+(i+1)*width)
                freq[i]++;
    }
    fclose(f);
    f = fopen("bin.dat","w");
    for(i = 0; i < 125; i++)
        fprintf(f, "%14.10e\t%14.10e\n", min+(i+0.5)*width, freq[i]/width/count);
    fclose(f);
}

#endif
