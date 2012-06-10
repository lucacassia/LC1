#include"random.h"
#include<math.h>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include"libutil.h"

#define PI 3.141592653589793238462643383279502884197169399375105820974944592308

#define SIGMA 10000
#define MU 310

double r[64];

double pi_greco(double n){
    long unsigned int i,j,pi;
    for(pi = i = 0; i < n; i++){
        ranlxd(r,64);
        for(j = 0; j < 32; j++)
            if(hypot(r[j],r[32+j]) < 1)
                pi++;
    }
    return 4.0*pi/floor(n)/32.0;
}

//Log-normal
double deltaProb(double x, double y){
    return x * exp( (log(x*y)-2*MU)*log(x/y)/(2*SIGMA*SIGMA) ) / y;
}

double metropolis(double x){
    double y;
    ranlxd(r,64);
    int i, count;
    for(count = i = 0; i < 32 && count < 5; i++){
        y = fabs( x + 1000 * (2 * r[i] - 1) );
        if(r[32+i] < deltaProb(x,y)){
            x = y;
            count++;
        }
    }
    return x;
}

int main(int argc,char* argv[]){
    long unsigned int i, sweeps = 1000;
    double x = MU;
    rlxd_init(2,time(NULL));
    FILE *f = fopen("pi.dat","a");
    for(i = 0; i < 500; i++)
        x = metropolis(x);
    for(i = 0; i < sweeps; i++){
        loading(i,sweeps);
        x = metropolis(x);
        fprintf(f,"%14.10e\t%14.10e\n",x,fabs(4*atan(1)-pi_greco(x)));
    }
    fclose(f);
    return 0;
}
