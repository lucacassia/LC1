#include<stdlib.h>
#include<math.h>
#include<time.h>
#include"random.h"
#include"metro0.h"
#include"metro1.h"
#include"metro2.h"
#include"metro3.h"
#include"libutil.h"
#include"jackknife.h"

#define N 32
#define BIN_W 100

double M = 1;
double W = 1;
double D = 3;

double V(double x){ return M*W*W*x*x/2; }

double dS(double *x,double y,int i){ return M*((x[i]-y)*(x[(i+1)%N]+x[(N+i-1)%N])+(y*y-x[i]*x[i]))+V(y)-V(x[i]); }

double action(double *x){
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
        double odt = o[dt] = 0;
        int i; for(i = 0; i < N; i++)
            odt += x[i]*x[(i+dt)%N];
        odt /= N;
        o[dt] += odt;
        data[dt] = odt;
    }
}

double autoCorrelation(int i,int k,int n,double* data){
    double mean,var,Rk;
    mean = var = Rk = 0;
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
    int bin = cycles/BIN_W;
    /*init mem*/
    double* data = (double*)malloc(N*cycles*sizeof(double));
    double* dtcl = (double*)malloc(N*bin*sizeof(double));
    if(data==NULL || data==NULL)printf("\nmalloc fail!\n");
    /*init variables*/
    double x[N],o[N],var[N];
    int i,j;
    for(i = 0; i < N; i++){
        x[i] = 100; /*hot action*/
        o[i] = var[i] = 0;
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
    double vtmp[N];
    for(i = 0; i < bin; i++)
        for(j = 0; j < BIN_W; j++){
            metropolis(x);
            ooo(x,vtmp,&data[(i*BIN_W+j)*N]);
            int dt; for(dt = 0; dt < N; dt++){
                double temp = vtmp[dt]/BIN_W;
                o[dt] += temp;
                var[dt] += temp*temp;
                dtcl[dt*bin+i] = temp;
            }
            loading(i*BIN_W+j,cycles);
        }

    /*print data to file and plot*/
    FILE *f1 = fopen("metro1.dat","w");
    FILE *f2 = fopen("metro2.dat","w");
    for(i = 0; i < N; i++){
        o[i] /= bin;
        var[i] = var[i]/bin-o[i]*o[i];
        fprintf(f1,"%d\t%lf\n",i,x[i]);
        fprintf(f2,"%d\t%lf\t%lf\n",i,fabs(o[i]),var[i]);
    }
    fclose(f1); metro1(); system("rm metro1.dat");
    fclose(f2); metro2(); system("rm metro2.dat");

    /*autocorrelation*/
    FILE *f3 = fopen("metro3.dat","w");
    for(i = 0; i < 30; i++)
        fprintf(f3,"%d\t%lf\n",i,autoCorrelation(1,i,cycles,data));
    fclose(f3); metro3(); system("rm metro3.dat");

    /*delta E*/
    double dE = 0;
    for(i = 2; i < 5; i++)
        dE += acosh((o[i+1]+o[i-1])/(2*o[i]));
    printf("\n\n dE = %lf\n",dE/=3);

    /*jackknife*/
    double mcl[N],dEm = 0,var_dE = 0;
    for(i = 1; i < 6; i++)
        mcl[i] = cluster(&dtcl[i*bin],bin);
    for(i = 2; i < 5; i++)
        dEm += acosh((mcl[i+1]+mcl[i-1])/(2*mcl[i]));
    dEm /= 3;
    printf("\n\n dEm = %lf\n",dEm);
    for(j = 0; j < bin; i++){
        dtcl[j] = 0;
        for(i = 2; i < 5; i++)
            dtcl[j] += acosh((dtcl[(i+1)*bin+j]+dtcl[(i-1)*bin+j])/(2*dtcl[i*bin+j]));
        dtcl[j] /= 3;
    }
    for(j = 0; j < bin; j++)
        var_dE += (dtcl[j]-dEm)*(dtcl[j]-dEm);
    var_dE*=(bin-1)/bin;
    printf("\n var = %lf\n\n",var_dE);

    free(data);
    free(dtcl);
    return 0;
}
