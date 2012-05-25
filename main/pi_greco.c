#include"random.h"
#include<math.h>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>

double pi_greco(int n)
{
    double pi, *r = (double*)malloc(2*n*sizeof(double));
    int i;
    ranlxd(r,2*n);
    for(pi = i = 0; i < n; i++)
        if(hypot(r[i],r[n+i]) < 1)
            pi += 1;
    free(r);
    return 4.0*pi/n;
}

int main(int argc,char* argv[]){
    rlxd_init(2,time(NULL));
    printf("\nπ = %lf\n\n",pi_greco(atoi(argv[1])));
    return 0;
}
