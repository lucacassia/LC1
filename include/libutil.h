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
    int i, n = 125;
    double min = 0.93, max = 0.99, width = (max-min)/n, tmp;
    int* freq = malloc(n*sizeof(int));
    for(i = 0; i < n; i++)
        freq[i] = 0;
    FILE* f = fopen("histogram.dat","r");
    while(fscanf(f, "%lf\n", &tmp))
        for(i = 0; i < n; i++)
            if(tmp > min+i*width && tmp <= min+(i+1)*width)
                freq[i]++;
    fclose(f);
    f = fopen("bin.dat","w");
    for(i = 0; i < n; i++)
//        if(freq[i] > 10)
            fprintf(f, "%lf\t%d\n", min+(i+0.5)*width, freq[i]);
    fclose(f);
    free(freq);
}

#endif
