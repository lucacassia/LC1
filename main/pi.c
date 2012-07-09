#include"random.h"
#include"libutil.h"
#include<math.h>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>

#define N 1000

double SIGMA = 3.0;
double MU = 7.0;
double r[96];

double pi_quarti(double n){
    unsigned long int i,j,k;
    for(k = i = 0; 32*i < n+32; i++){
        ranlxd(&r[32],64);
        for(j = 0; j < 32 && 32*i+j < n; j++)
            if(hypot(r[32+j],r[64+j]) < 1)
                k++;
    }
    return k / n;
}

int main(){
    unsigned long int i,j;
    double n, tmp, var;
    rlxd_init(2,time(NULL));
    FILE *f = fopen("pi.dat","w");
    for(i = 0; i < N; i++){
        gauss_dble(r, 32);
        for(j = 0; j < 32; j++){
            n = floor(exp(MU + SIGMA * r[j]));
            if(n > 25){
                tmp = pi_quarti(n);
                var = n*pow(1.0-tmp,2.0)/(n-1.0);
                fprintf(f,"%g\t%10.10e\n", n, pow(1.0-tmp/atanl(1),2.0)/var);
            }
        }
        loading(i,N);
    }
    printf("\n");
    fclose(f);
    return 0;
}

