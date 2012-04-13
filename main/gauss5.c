#include<stdio.h>
#include"gauss5.h"

double function(double x)
{
    return exp(x);
}

int main()
{
    printf("\nintegral over [-1,1] of f(x) = %lf\n\n",gauss5ab(function,-1,1));
    return 0;
}
