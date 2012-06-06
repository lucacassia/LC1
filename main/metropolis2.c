#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<time.h>
#include"libutil.h"
#include"random.h"
#include"plot.h"

#define N 32
//#define PLOT

double M = 1;
double W = 1;
double D = 3;

double V(double x){
    return M*W*W*x*x/2;
}

double dS(double *x,double y,int i){
    return M*((x[i]-y)*(x[(i+1)%N]+x[(i-1+N)%N])+(y*y-x[i]*x[i]))+V(y)-V(x[i]);
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

int main(int argc,char* argv[]){
#ifdef CALC
    /*set input/output to be unbuffered*/
    setvbuf(stdout, NULL, _IONBF, 0);
    /*init ranlux*/
    rlxd_init(2,time(NULL));
    /*init stuff*/
    int cycles = 1e6; if(argc == 2) cycles = atoi(argv[1]);
    int width = 100; if(argc == 3) width = atoi(argv[2]);
    int bin = cycles/width;
    int i,j,k;
    /*init mem*/
    double* dtcl = malloc(N*bin*sizeof(double));
    /*init variables*/
    double x[N],c[N],var[N],tmp;
    for(i = 0; i < N; i++){
        x[i] = 100;
        c[i] = var[i] = 0;
    }

    /*action*/
    for(i = 0; i < 1000; i++)
        metropolis(x);

    /*metropolis loop*/
    for(i = 0; i < bin; i++)
        for(j = 0; j < width; j++){
            metropolis(x);
            for(k = 0; k < N; k++)
                dtcl[k*bin+i] += correlation(x,k)/width;
            loading(i*width+j,bin*width);
        }

    /*correlation*/
    for(k = 0; k < N; k++){
        for(i = 0; i < bin; i++){
            c[k] += tmp = dtcl[k*bin+i];
            var[k] += tmp*tmp;
        }
        c[k] /= bin;
        var[k] = var[k]/bin-c[k]*c[k];
    }

    /*ΔE & W*/
    double dE = 0, W = 0;;
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
    var_dE = (var_dE*(bin-1))/bin;
    var_W = (var_W*(bin-1))/bin;
    free(dtcl);

#ifdef PLOT
    FILE* f;
    /*plot ΔE*/
    f = fopen("dE.dat","a");
    fprintf(f,"%lf\n",dE);
    fclose(f);
    fit();

    /*plot W*/
    f = fopen("W.dat","a");
    fprintf(f,"%lf\n",W);
    fclose(f);
#endif
#ifdef VAR
    FILE* g = fopen("var_dE.dat","a");
    fprintf(g,"%d\t%e\n",bin*width,var_dE);
    fclose(g);
    plot_var();
    printf("\n\n ΔE  = %lf\n\n σ = %e\n\n W  = %lf\n\n σ = %e\n\n",dE,sqrt(var_dE),W,sqrt(var_W));
#endif

#endif

    fit();

    return 0;
}
