#include<stdio.h>
#include"derive.h"

double funzione(double x){
    return tan(x);
}

int main(){
    int n=1;
    double x=PI;
    printf("\n(d%d)func(%lf) = %lf\n\n",n,x,Derivez(funzione,x,n));
    return 0;
}
