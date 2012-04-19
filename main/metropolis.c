#include<stdlib.h>
#include<math.h>
#include<time.h>
#include"random.h"
#include"metro0.h"
#include"metro1.h"
#include"metro2.h"
#include"metro3.h"
#include"libutil.h"

#define N 32
#define BIN_W 100

double M = 1;
double W = 1;
double D = 3;
double x[N],o[N],tmp[N],var[N];
double *data;

double V(double x){ return M*W*W*x*x/2; }

double dS(double *x,double y,int i){ return M*((x[i]-y)*(x[(i+1)%N]+x[(N+i-1)%N])+(y*y-x[i]*x[i]))+V(y)-V(x[i]); }

double action(const double *x){
    double s = 0;
    int i; for(i = 0; i < N-1; i++)
        s += M*(x[i+1]-x[i])*(x[i+1]-x[i])/2+(V(x[i])+V(x[i+1]))/2;
    return s;
}

double metropolis(double *x){
    double DS = 0, y, ds, r[2*N];
    ranlxd(r,2*N);
    int i; for(i = 0; i < N; i++){
        y = x[i]+D*(2*r[i]-1);
        ds = dS(x,y,i);
        if(r[N+i] < exp(-ds)){
            x[i] = y;
            DS += ds;
        }
    }
    return DS;
}

void ooo(double* x,double* o,double* data){
    int dt; for(dt = 0; dt < N; dt++){
        double odt = 0;
        int i; for(i = 0; i < N; i++)
            odt += x[i]*x[(i+dt)%N];
        o[dt] += odt/N;
        data[dt] = odt/N;
    }
}

double autoCorrelation(int i,int k,int n){
    double mean,var,Rk; mean = var = Rk = 0;
    int t; for(t = 0; t < n-k; t++){
        Rk += data[N*t+i]*data[N*(t+k)+i];
        mean += data[N*t+i];
        var += data[N*t+i]*data[N*t+i];
    }
    mean /= n-k;
    var = var/(n-k)-mean*mean;
    return (Rk/(n-k)-mean*mean)/var;
}

int main(int argc,char* argv[]){
    /*init cycles*/
    int cycles = 1e6; if(argc == 2) cycles = atoi(argv[1]);
    /*init mem*/
    data = (double*)malloc(N*cycles*sizeof(double));
    /*init variables*/
    int i; for(i = 0; i < N; i++){
        x[i] = 100; /*hot action*/
        o[i] = tmp[i] = var[i] = 0;
    }

    /*init ranlux*/
    rlxd_init(2,time(NULL));

    /*action*/
    double S = action(x);
    FILE *f0 = fopen("metro0.dat","w");
    for(i = 0; i < 1000; i++)
        fprintf(f0,"%d\t%lf\n",(i+1),S += metropolis(x));
    fclose(f0); metro0(); system("rm metro0.dat");

    /*metropolis loop*/
    for(i = 0; i < cycles; i++){
        metropolis(x);
        ooo(x,tmp,&data[i*N]);
        if((i+1)%BIN_W == 0){
            int dt; for(dt = 0; dt < N; dt++){
                o[dt] += tmp[dt]/BIN_W;
                var[dt] += tmp[dt]*tmp[dt]/BIN_W/BIN_W;
                tmp[dt] = 0;
            }
        }
        loading(i,cycles);
    }

    /*print data to file and plot*/
    FILE *f1 = fopen("metro1.dat","w");
    FILE *f2 = fopen("metro2.dat","w");
    for(i = 0; i < N; i++){
        o[i] /= cycles/BIN_W;
        var[i] = var[i]/(cycles/BIN_W)-o[i]*o[i];
        fprintf(f1,"%d\t%lf\n",i,x[i]);
        fprintf(f2,"%d\t%lf\t%lf\n",i,fabs(o[i]),var[i]);
    }
    fclose(f1);metro1(); system("rm metro1.dat");
    fclose(f2);metro2(); system("rm metro2.dat");

    /*autocorrelation*/
    FILE *f3 = fopen("metro3.dat","w");
    for(i = 0; i < 30; i++)
        fprintf(f3,"%d\t%lf\n",i,autoCorrelation(1,i,cycles));
    fclose(f3); metro3(); system("rm metro3.dat");

    /*delta E*/
    double dE = 0;
    for(i = 1; i < 4; i++)
        dE += acosh((o[i+1]+o[i-1])/(2*o[i]));
    printf("\n\n dE = %lf\n\n",dE/=3);

    free(data);
    return 0;
}
