#ifndef INTEGRAL
#define INTEGRAL

#include <math.h>
#include "random.h"

double newton_cotes1(double(*function)(double),double a,double b)
{
    return (b-a)*(function(a)+function(b))/2.0;
}

double newton_cotes2(double(*function)(double),double a,double b)
{
    return (b-a)*(function(a) + 4.0*function((a+b)/2.0) + function(b))/6.0;
}

double gauss5(double(*function)(double),double a,double b)
{
    double zero[5] = {0.0000000000000000,
                     -0.5384693101056831,
                      0.5384693101056831,
                     -0.9061798459386640,
                      0.9061798459386640 };
    double weight[5] = { 0.5688888888888889,
                         0.4786286704993665,
                         0.4786286704993665,
                         0.2369268850561891,
                         0.2369268850561891 };
    double integral = 0;
    int i;
    for(i = 0; i < 5; i++)
        integral += weight[i] * function( (b-a)*zero[i]/2.0 + (a+b)/2.0 );
    return (b-a)*integral/2.0;
}

double monte_carlo(double (*function)(double),double a,double b,unsigned n)
{
    unsigned i,j;
    double integral = 0;
    double r[32];
    for(i = 0; i < n; i++)
        for(j = 0; j < 32; j++){
            ranlxd(r,32);
            integral += function(a+r[j]*(b-a));
        }
    return integral/32.0/n;
}

#endif
