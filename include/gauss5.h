#ifndef GAUSS_5
#define GAUSS_5

#include<math.h>

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

#endif
