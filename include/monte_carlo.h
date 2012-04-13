#ifndef MONTE_CARLO
#define MONTE_CARLO

#include"randx.h"
#include<stdlib.h>

double monte_carlo(double (*function)(double),unsigned n,double a,double b)
{
    unsigned i;
    double integral=0;
    for(i=0;i<n;i++){
        integral+=function(a+randx()*(b-a));
    }
    return integral/n;
}

#endif
