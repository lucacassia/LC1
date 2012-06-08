#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<time.h>
#include"libutil.h"
#include"random.h"

#define N 32

double M = 1;
double W = 1;
double D = 3;

double V(double x){
    return M*W*W*x*x/2;
}

double dS(double *x,double y,int i){
    return M*((x[i]-y)*(x[(i+1)%N]+x[(i-1+N)%N])+(y*y-x[i]*x[i]))+V(y)-V(x[i]);
}

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

double correlation(double* x,int dt){
    double odt = 0;
    int i; for(i = 0; i < N; i++)
        odt += x[i]*x[(i+dt)%N];
    return odt/N;
}

double autoCorrelation(int i,int k,int n,double* data){
    double mean,var,Rk;
    mean = var = Rk = 0;
    int t; for(t = 0; t < n-k; t++){
        double tmp = data[N*t+i];
        Rk += tmp*data[N*(t+k)+i];
        mean += tmp;
        var += tmp*tmp;
    }
    mean /= n-k;
    var = var/(n-k)-mean*mean;
    return (Rk/(n-k)-mean*mean)/var;
}

int main(int argc,char* argv[]){
    /*init ranlux*/
    rlxd_init(2,time(NULL));
    /*init parameters*/
    int cycles = 1e6; if(argc == 2) cycles = atoi(argv[1]);
    int width = 100; if(argc == 3) width = atoi(argv[2]);
    int bin = cycles/width;
    int i,j,k;
    /*alloc data array for autocorrelation purposes*/
    double* data = malloc(N*width*bin*sizeof(double));
    /*alloc data array for clusterization*/
    double* dtcl = malloc(N*bin*sizeof(double));
    /*init variables*/
    double x[N],c[N],var[N],tmp;
    for(i = 0; i < N; i++){
        x[i] = 0;
        c[i] = var[i] = 0;
    }

    /*action*/
    double S = action(x);
    FILE* f = fopen("action.dat","w");
    for(i = 0; i < 1e3; i++)
        fprintf(f,"%d\t%lf\n",(i+1),S += metropolis(x));
    fclose(f);

    /*metropolis loop*/
    for(i = 0; i < bin; i++)
        for(j = 0; j < width; j++){
            metropolis(x);
            for(k = 0; k < N; k++){
                tmp = correlation(x,k);
                data[(i*width+j)*N+k] = tmp;
                dtcl[k*bin+i] += tmp/width;
            }
            loading(i*width+j,bin*width);
        }

    /*autocorrelation*/
    f = fopen("autocorrelation.dat","w");
    for(i = 0; i < 30; i++)
        fprintf(f,"%d\t%lf\n",i,autoCorrelation(1,i,bin*width,data));
    fclose(f);
    free(data);

    /*correlation*/
    f = fopen("correlation.dat","w");
    for(k = 0; k < N; k++){
        for(i = 0; i < bin; i++){
            c[k] += tmp = dtcl[k*bin+i];
            var[k] += tmp*tmp;
        }
        c[k] /= bin;
        var[k] = var[k]/bin-c[k]*c[k];
        fprintf(f,"%d\t%lf\t%lf\n",k,fabs(c[k]),var[k]);
    }
    fclose(f);

    /*deltaE & W*/
    double dE = 0, W = 0;
    for(k = 2; k < 5; k++){
        dE += tmp = acosh((c[k+1]+c[k-1])/(2*c[k]));
        W += sqrt(c[k]*exp(tmp*N/2.0)/(2*cosh(tmp*(N/2.0-k))));
    }
    dE /= 3;
    W /= 3;

    /*cluster generation*/
    double mcl[6];
    for(i = 1; i < 6; i++){
        mcl[i] = 0;
        for(j = 0; j < bin; j++)
            mcl[i] += dtcl[i*bin+j];
        mcl[i] /= bin;
        for(j = 0; j < bin; j++)
            dtcl[i*bin+j] = mcl[i]+(mcl[i]-dtcl[i*bin+j])/(bin-1);
    }

    /*compute mean values*/
    double dEm = 0, Wm = 0;
    for(k = 2; k < 5; k++){
        dEm += tmp = acosh((mcl[k+1]+mcl[k-1])/(2*mcl[k]));
        Wm += sqrt(mcl[k]*exp(tmp*N/2.0)/(2*cosh(tmp*(N/2.0-k))));
    }

    /*compute variance*/
    double var_dE = 0, var_W = 0;
    for(j = 0; j < bin; j++){
        double tmp1 = 0, tmp2 = 0;
        for(k = 2; k < 5; k++){
            tmp1 += tmp = acosh((dtcl[(k+1)*bin+j]+dtcl[(k-1)*bin+j])/(2*dtcl[k*bin+j]));
            tmp2 += sqrt(c[k]*exp(tmp*N/2.0)/(2*cosh(tmp*(N/2.0-k))));
        }
        var_dE += (tmp1-dEm)*(tmp1-dEm)/9;
        var_W += (tmp2-Wm)*(tmp2-Wm)/9;
    }
    free(dtcl);
    var_dE = (var_dE*(bin-1))/bin;
    var_W = (var_W*(bin-1))/bin;

#ifdef HISTOGRAM
    /*plot histogram of dE*/
    f = fopen("histogram.dat","a");
    fprintf(f,"%lf\n",dE);
    fclose(f);
    binning();
#endif
    binning();
    printf("\n\n dE  = %14.10lf\n\n sigma = %14.10lf\n\n W  = %14.10lf\n\n sigma = %14.10lf\n\n",dE,sqrt(var_dE),W,sqrt(var_W));

    return 0;
}
