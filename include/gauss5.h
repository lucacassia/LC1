#ifndef GAUSS_5
#define GAUSS_5

#include<math.h>

double gauss5(double (*function)(double))
{
    double zero[5]={0,
                    sqrt(5.0-2.0*sqrt(10.0/7.0))/3.0,
                    -sqrt(5.0-2.0*sqrt(10.0/7.0))/3.0,
                    sqrt(5.0+2.0*sqrt(10.0/7.0))/3.0,
                    -sqrt(5.0+2.0*sqrt(10.0/7.0))/3.0 };
    double weight[5] = { 128.0/255.0,
                        (322.0+13.0*sqrt(70))/900.0,
                        (322.0+13.0*sqrt(70))/900.0,
                        (322.0-13.0*sqrt(70))/900.0,
                        (322.0-13.0*sqrt(70))/900.0 };
    double integral=0;
    int i;
    for(i=0;i<5;i++)
        integral+=function(zero[i])*weight[i];
    return integral;
}

double gauss5ab(double(*function)(double),double a,double b)
{
    double zero[5]={0,
                    sqrt(5.0-2.0*sqrt(10.0/7.0))/3.0,
                    -sqrt(5.0-2.0*sqrt(10.0/7.0))/3.0,
                    sqrt(5.0+2.0*sqrt(10.0/7.0))/3.0,
                    -sqrt(5.0+2.0*sqrt(10.0/7.0))/3.0 };
    double weight[5] = { 128.0/255.0,
                        (322.0+13.0*sqrt(70))/900.0,
                        (322.0+13.0*sqrt(70))/900.0,
                        (322.0-13.0*sqrt(70))/900.0,
                        (322.0-13.0*sqrt(70))/900.0 };
    double integral=0;
    int i;
    for(i=0;i<5;i++)
        integral+=function((b-a)*zero[i]/2+(a+b)/2)*weight[i];
    return (b-a)*integral/2;
}

#endif
