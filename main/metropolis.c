#include<stdlib.h>
#include<math.h>
#include<time.h>
#include"random.h"
#include"metro1.h"
#include"metro2.h"
#include"metro3.h"
#include"libutil.h"

#define N 32
#define Pi 3.14159265
#define POINTS 100
#define BIN_W 100

double M=1;
double W=1;
double D=3;
double x[N],o[N],tmp[N],var[N];

double V(double x){ return M*W*W*x*x/2; }

double lagrangian(double x1,double x2){ return M*(x2-x1)*(x2-x1)/2+(V(x1)+V(x2))/2; }

//double dS(double *x,double y,int i){ return lagrangian(y,x[(i+1)%N])+lagrangian(x[(N+i-1)%N],y)-lagrangian(x[i],x[(i+1)%N])-lagrangian(x[(N+i-1)%N],x[i]); }

double dS(double *x,double y,int i){ return M*((x[i]-y)*(x[(i+1)%N]+x[(N+i-1)%N])+(y*y-x[i]*x[i]))+V(y)-V(x[i]); }

double action(const double *x,int n){
    double s=0;
    int i;for(i=0;i<n-1;i++)
        s+=lagrangian(x[i],x[i+1]);
    return s;
}

double metropolis(double *x){
    double DS = 0, y, ds, r[2*N];
    ranlxd(r,2*N);
    int i; for(i=0;i<N;i++){
        y = x[i]+D*(2*r[i]-1);
        ds = dS(x,y,i);
        if(r[N+i]<exp(-ds)){
            x[i] = y;
            DS += ds;
        }
    }
    return DS;
}

void ooo(double* x,double* o){
    int dt; for(dt=0;dt<N;dt++){
        double odt=0;
        int i; for(i=0;i<N;i++)
            odt+=x[i]*x[(i+dt)%N];
        o[dt]+=odt/N;
    }
}

int main(int argc,char* argv[]){
    /*init cycles*/
    int cycles=1e6; if(argc==2) cycles=atoi(argv[1]);
    /*init state*/
    int i,dt;for(i=0;i<N;i++){x[i]=o[i]=tmp[i]=var[i]=0;}
    /*init ranlux*/
    rlxd_init(2,time(NULL));

    /*action*/
    double S=action(x,N);
    FILE *f3 = fopen("metro3.dat","w");
    for(i=0;i<1000;i++){
        S+=metropolis(x);
        fprintf(f3,"%d\t%lf\n",(i+1),S);
    }
    fclose(f3);

    /*metropolis loop*/
    for(i=0;i<cycles;i++){
        metropolis(x);
        ooo(x,tmp);
        if((i+1)%BIN_W==0)
            for(dt=0;dt<N;dt++){
                o[dt]+=tmp[dt]/BIN_W;
                var[dt]+=tmp[dt]*tmp[dt]/BIN_W/BIN_W;
                tmp[dt]=0;
            }
        loading(i,cycles);
    }

    /*print data to file*/
    FILE *f1 = fopen("metro1.dat","w");
    FILE *f2 = fopen("metro2.dat","w");
    for(i=0;i<N;i++){
        o[i]/=cycles/BIN_W;
        var[i]=var[i]/(cycles/BIN_W)-o[i]*o[i];
        fprintf(f1,"%d\t%lf\n",i,x[i]);
        fprintf(f2,"%d\t%lf\t%lf\n",i,fabs(o[i]),var[i]);
    }
    fclose(f1);
    fclose(f2);

    /*plot data from file*/
    metro1();metro2();metro3();
    system("rm metro1.dat");
//    system("rm metro2.dat");
    system("rm metro3.dat");
    printf("\n");
    return 0;
}
