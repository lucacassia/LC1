#ifndef DERIVE
#define DERIVE

#include<math.h>

double Derive(double (*func)(double),double x,int n,double h){
    if(n < 1) return func(x);
    return (Derive(func,x+h/2,n-1,h/2) - Derive(func,x-h/2,n-1,h/2)) / h;
}

double Derivez(double (*func)(double),double x,int n){
    if(n < 1) return func(x);
    double tmp, h = 1;
    do{
        tmp = (Derivez(func,x+h/2,n-1) - Derivez(func,x-h/2,n-1)) / h;
        h /= 2;
    }while(fabs((Derivez(func,x+h/2,n-1) - Derivez(func,x-h/2,n-1)) / h - tmp) > 1e-6);
    return tmp;
}

#endif
