#include<stdio.h>
#include<stdlib.h>
#include"derive.h"

double function(double x){
    return tan(x);
}

int main(int argc, char *argv[]){
    int n = 1;
    if(argc > 1) n = atoi(argv[1]);
    double x = 4*atan(1);
    printf("\n(D^%d)function(%e) = %e,\t%e\n\n",n,x,Derivez(function,x,n),Derive(function,x,n,1e-6));
    return 0;
}
