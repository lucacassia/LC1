#include<stdio.h>
#include<stdlib.h>
#include"gauss5.h"

double function1(double x){ return log(1+x);}
double primitive1(double x){ return -x+log(1+x)+x*log(1+x);}
double function2(double x){ return pow(x,9) - pow(x,7) + 3.0;}
double primitive2(double x){ return pow(x,10)/10.0 - pow(x,8)/8.0 + 3.0*x;}

int main(int argc, char *argv[])
{
    double a = 1, b = 2, I1 = 0, I2 = 0;
    int i, n = 10;
    if(argc>1)
        n = atoi(argv[1]);
    for(i = 0; i < n; i++){
        I1 += gauss5(function1,a+i*(b-a)/n,a+(i+1)*(b-a)/n);
        I2 += gauss5(function2,a+i*(b-a)/n,a+(i+1)*(b-a)/n);
    }
    printf("\nintervals = %d\n",n);
    printf("\nintegral over [%f,%f] of log(1+x) = %e\testimated = %e\terror = %e\n", a, b, primitive1(b)-primitive1(a), I1, fabs(primitive1(b)-primitive1(a)-I1)/(primitive1(b)-primitive1(a)));
    printf("\nintegral over [%f,%f] of x^9-x^7+3 = %e\testimated = %e\terror = %e\n\n", a, b, primitive2(b)-primitive2(a), I2, fabs(primitive2(b)-primitive2(a)-I2)/(primitive2(b)-primitive2(a)));
    return 0;
}
