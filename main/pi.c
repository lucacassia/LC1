#include"random.h"
#include<math.h>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include"libutil.h"

#define SIGMA 1000
#define MU 310

double pi_greco(int n)
{
    double pi, *r = (double*)malloc(2*n*sizeof(double));
    int i;
    ranlxd(r,2*n);
    for(pi = i = 0; i < n; i++)
        if(hypot(r[i],r[n+i]) < 1)
            pi += 1;
    free(r);
    return 4.0*pi/n;
}

//Log-normal
double pdf(double x, double y){
    return x * exp( (log(x*y)-2*MU)*log(x/y)/(2*SIGMA*SIGMA) ) / y;
}

double metropolis(double x){
    double r[32], y;
    ranlxd(r,64);
    unsigned int i, count;
    for(count = i = 0; i < 32 && count < 5; i++){
        y = fabs( x + 10000 * (2 * r[i] - 1) );
        if(r[32+i] < pdf(x,y)){
            x = y;
            count++;
        }
    }
    return x;
}

int main(int argc,char* argv[]){
    unsigned int i, sweeps = 10000;
    double x = MU;
    rlxd_init(2,time(NULL));
    FILE *f = fopen("pi.dat","a");
    for(i = 0; i < 500; i++)
        x = metropolis(x);
    for(i = 0; i < sweeps; i++){
        loading(i,sweeps);
        x = metropolis(x);
        fprintf(f,"%d\t%14.10e\n",(int)floor(x),fabs(4*atan(1)-pi_greco((int)floor(x))));
    }
    fclose(f);
    return 0;
}
