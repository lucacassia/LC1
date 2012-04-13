#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include"monte_carlo.h"

double gaussian(double x,double s,double m)
{
    return exp(-(x-m)*(x-m)/(2*s*s));
}

double function(double x)
{
    int i,n=3;
    double momentum=gaussian(x,1,0);
    for(i=0;i<n;i++)
        momentum*=x;
    return momentum;
}

int main(int argc,char* argv[])
{
    rlxd_init(2,time(NULL));
    printf("\nintegral over [0,1] of f(x) = %lf\n\n",monte_carlo(function,atoi(argv[1]),0.0,1.0));
    return 0;
}

