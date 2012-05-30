#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include"integral.h"

double function(double x){ return log(1+x);}
double primitive(double x){ return -x+log(1+x)+x*log(1+x);}

int main(int argc, char *argv[])
{
    double a = 1, b = 2, S, I = primitive(b) - primitive(a);
    unsigned i, n = 10;
    if(argc > 1) n = atoi(argv[1]);
    rlxd_init(2,time(NULL));
    FILE *f = fopen("data.dat","a");

    for(S = i = 0; i < n; i++)
        S += newton_cotes1(function, a+i*(b-a)/n, a+(i+1)*(b-a)/n);
    fprintf(f,"%d\t%e", n, fabs(I-S));

    for(S = i = 0; i < n; i++)
        S += newton_cotes2(function, a+i*(b-a)/n, a+(i+1)*(b-a)/n);
    fprintf(f,"\t%e", fabs(I-S));

    for(S = i = 0; i < n; i++)
        S += gauss5(function, a+i*(b-a)/n, a+(i+1)*(b-a)/n);
    fprintf(f,"\t%e", fabs(I-S));

    S = monte_carlo(function, a, b, n);
    fprintf(f,"\t%e\n", fabs(I-S));

    return 0;
}
