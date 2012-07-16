#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include"integral.h"

double function(double x){ return pow(x,9)-pow(x,7)+3;}
double primitive(double x){ return pow(x,10)/10.0-pow(x,8)/8.0+3*x;}

int main(int argc, char *argv[])
{
    double a = 1, b = 2, S, I = primitive(b) - primitive(a);
    unsigned i, n = 10;
    if(argc > 1) n = atoi(argv[1]);
    rlxd_init(2,time(NULL));
    FILE *f1 = fopen("integral1.dat","a");
    FILE *f2 = fopen("integral2.dat","a");
    FILE *f3 = fopen("integral3.dat","a");
    FILE *f4 = fopen("integral4.dat","a");

    for(S = i = 0; i < n; i++)
        S += newton_cotes1(function, a+i*(b-a)/n, a+(i+1)*(b-a)/n);
    fprintf(f1,"%d\t%e\n", n, fabs(I-S));

    for(S = i = 0; i < n; i++)
        S += newton_cotes2(function, a+i*(b-a)/n, a+(i+1)*(b-a)/n);
    fprintf(f2,"%d\t%e\n", n, fabs(I-S));

    for(S = i = 0; i < n; i++)
        S += gauss5(function, a+i*(b-a)/n, a+(i+1)*(b-a)/n);
    fprintf(f3,"%d\t%e\n", n, fabs(I-S));

    S = monte_carlo(function, a, b, n);
    fprintf(f4,"%d\t%e\n", n, fabs(I-monte_carlo(function, a, b, n)));

    fclose(f1);
    fclose(f2);
    fclose(f3);
    fclose(f4);

    return 0;
}
