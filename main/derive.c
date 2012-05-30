#include<stdio.h>
#include"derive.h"

double function(double x){
    return exp(x);
}

int main(){
    int i;
    double x = 0;
    for(i = 0; i < 5; i++)
        printf("\n(D^%d)[F(%f)]: errors\t%14.10e,\t%14.10e\n\n",i,x,fabs(Derivez(function,x,i)-1),fabs(Derive(function,x,i,1e-6)-1));
    return 0;
}
