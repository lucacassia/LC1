#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<time.h>
#include"libutil.h"
#include"random.h"

#define N 32

double M = 1.0f;
double W = 1.0f;
double D = 3.0f;
double S = 0.0f;

double V(double x){
    return M*W*W*x*x/2;
}

double action(double *x){
    double s = 0.0f;
    int i; for(i = 0; i < N-1; i++)
        s += M*(x[i+1]-x[i])*(x[i+1]-x[i])/2+(V(x[i])+V(x[i+1]))/2;
    return s;
}

double newS(double *x,double y,int i){
    double tmp[N]; int j;
    for(j = 0; j < N; j++)
        if(i == j)
            tmp[j] = y;
        else
            tmp[j] = x[j];
    return action(tmp);
}

void metropolis(double *x){
    double y, news, r[2*N];
    ranlxd(r,2*N);
    int i; for(i = 0; i < N; i++){
        y = x[i]+D*(2.0f*r[i]-1.0f);
        news = newS(x,y,i);
        if(r[N+i] < exp(-news+S)){
            x[i] = y;
            S = news;
        }
    }
}

double correlation(double* x,int dt){
    double odt = 0.0f;
    int i; for(i = 0; i < N; i++)
        odt += x[i]*x[(i+dt)%N];
    return odt/N;
}

double autoCorrelation(int i,int k,int n,double* data){
    double mean,var,Rk;
    mean = var = Rk = 0.0f;
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
        x[i] = 100.0f;
        c[i] = var[i] = 0.0f;
    }

    /*action*/
    S = action(x);
    FILE* f = fopen("action.dat","w");
    for(i = 0; i < 1e3; i++){
        metropolis(x);
        fprintf(f,"%d\t%lf\n",(i+1), S);
    }
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
        dE += tmp = acosh((c[k+1]+c[k-1])/(2.0f*c[k]));
        W += sqrt(c[k]*exp(tmp*N/2.0f)/(2.0f*cosh(tmp*(N/2.0f-k))));
    }
    dE /= 3.0f;
    W /= 3.0f;

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
    double dEm = 0.0f, Wm = 0.0f;
    for(k = 2; k < 5; k++){
        dEm += tmp = acosh((mcl[k+1]+mcl[k-1])/(2.0f*mcl[k]));
        Wm += sqrt(mcl[k]*exp(tmp*N/2.0f)/(2.0f*cosh(tmp*(N/2.0f-k))));
    }

    /*compute variance*/
    double var_dE = 0.0f, var_W = 0.0f;
    for(j = 0; j < bin; j++){
        double tmp1 = 0.0f, tmp2 = 0.0f;
        for(k = 2; k < 5; k++){
            tmp1 += tmp = acosh((dtcl[(k+1)*bin+j]+dtcl[(k-1)*bin+j])/(2.0f*dtcl[k*bin+j]));
            tmp2 += sqrt(c[k]*exp(tmp*N/2.0f)/(2.0f*cosh(tmp*(N/2.0f-k))));
        }
        var_dE += (tmp1-dEm)*(tmp1-dEm)/9.0f;
        var_W += (tmp2-Wm)*(tmp2-Wm)/9.0f;
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

    printf("\n\n dE  = %14.10lf\n\n sigma = %14.10lf\n\n W  = %14.10lf\n\n sigma = %14.10lf\n\n",dE,sqrt(var_dE),W,sqrt(var_W));

    return 0;
}
