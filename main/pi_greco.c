#include"random.h"
#include<math.h>
#include<time.h>
#include<malloc.h>
#include<stdio.h>
#include<stdlib.h>

double pi_greco(int n)
{
    double *r=(double*)malloc(2*n*sizeof(double));
    ranlxd(r,2*n);
    int i;
    double pi=0;
    for(i=0;i<n;i++)
        if(r[i]*r[i]+r[n+i]*r[n+i]<1)
            pi+=1;
    return 4*pi/n;
}

int main(int argc,char* argv[]){
    rlxd_init(2,time(NULL));
    printf("\nπ = %lf\n\n",pi_greco(atoi(argv[1])));
    return 0;
}
