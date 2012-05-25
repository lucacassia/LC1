#ifndef PLOT_H
#define PLOT_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void plot_action(void){
	FILE *pipe = popen("gnuplot -persist","w");
	fprintf(pipe, "reset\n");
	fprintf(pipe, "set border linewidth 1.5\n");
	fprintf(pipe, "set style line 1 lc rgb '#bf0d23' lt 1 lw 1 pt 7 ps 0.5 # --- red\n");
	fprintf(pipe, "set grid\n");
	fprintf(pipe, "set title \"Metropolis\"\n");
	fprintf(pipe, "set xlabel \"i\"\n");
	fprintf(pipe, "set ylabel \"S\"\n");
	fprintf(pipe, "unset key\n");
	fprintf(pipe, "set log x\n");
	fprintf(pipe, "set term postscript enhanced color landscape lw 1 \"Verdana,10\"\n");
	fprintf(pipe, "set output 'action.eps'\n");
	fprintf(pipe, "plot 'action.dat' with linespoints ls 1\n");
	fclose(pipe);
    system("rm action.dat");
}

void plot_configuration(void){
	FILE *pipe = popen("gnuplot -persist","w");
	fprintf(pipe, "reset\n");
	fprintf(pipe, "set border linewidth 1.5\n");
	fprintf(pipe, "set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 7 ps 1.5 # --- blue\n");
	fprintf(pipe, "set grid\n");
	fprintf(pipe, "set title \"Metropolis\"\n");
	fprintf(pipe, "set xlabel \"i\"\n");
	fprintf(pipe, "set ylabel \"x[i]\"\n");
	fprintf(pipe, "unset key\n");
	fprintf(pipe, "set term postscript enhanced color landscape lw 1 \"Verdana,10\"\n");
	fprintf(pipe, "set output 'configuration.eps'\n");
	fprintf(pipe, "plot 'configuration.dat' with linespoints ls 1\n");
	fclose(pipe);
    system("rm configuration.dat");
}

void plot_correlation(void){
	FILE *pipe = popen("gnuplot -persist","w");
	fprintf(pipe, "reset\n");
	fprintf(pipe, "set border linewidth 1.5\n");
	fprintf(pipe, "set style line 1 lc rgb '#228f26' lt 1 lw 2 pt 7 ps 1.5 # --- green\n");
	fprintf(pipe, "set grid\n");
	fprintf(pipe, "set title \"Metropolis\"\n");
	fprintf(pipe, "set xlabel \"{/Symbol D}t\"\n");
	fprintf(pipe, "set ylabel \"c[{/Symbol D}t]\"\n");
	fprintf(pipe, "unset key\n");
	fprintf(pipe, "set term postscript enhanced color landscape lw 1 \"Verdana,10\"\n");
	fprintf(pipe, "set output 'correlation.eps'\n");
	fprintf(pipe, "plot 'correlation.dat' with linespoints ls 1\n");
	fclose(pipe);
    system("rm correlation.dat");
}

void plot_autocorrelation(void){
	FILE *pipe = popen("gnuplot -persist","w");
	fprintf(pipe, "reset\n");
	fprintf(pipe, "set border linewidth 1.5\n");
	fprintf(pipe, "set style line 1 lc rgb '#ffbb00' lt 1 lw 2 pt 7 ps 1.5 # --- orange\n");
	fprintf(pipe, "set grid\n");
	fprintf(pipe, "set title \"Metropolis\"\n");
	fprintf(pipe, "set xlabel \"k\"\n");
	fprintf(pipe, "set ylabel \"R[k]\"\n");
	fprintf(pipe, "unset key\n");
	fprintf(pipe, "set term postscript enhanced color landscape lw 1 \"Verdana,10\"\n");
	fprintf(pipe, "set output 'autocorrelation.eps'\n");
	fprintf(pipe, "plot 'autocorrelation.dat' with linespoints ls 1\n");
	fclose(pipe);
    system("rm autocorrelation.dat");
}

void binning(){
    int i,n=125;
    double min=0.93,max=0.99,width=(max-min)/n,tmp;
    int* freq = malloc(n*sizeof(int));
    for(i=0;i<n;i++)
        freq[i]=0;
    FILE* f=fopen("dE.dat","r");
    while(fscanf(f,"%lf\n",&tmp)==1)
        for(i=0;i<n;i++)
            if(tmp>min+i*width && tmp<=min+(i+1)*width)
                freq[i]++;
    fclose(f);
    f=fopen("bin.dat","w");
    for(i=0;i<n;i++)
//        if(freq[i]>10)
            fprintf(f,"%lf\t%d\n",min+(i+0.5)*width,freq[i]);
    fclose(f);
    free(freq);
}

void fit(){
    binning();
	FILE *pipe = popen("gnuplot -persist","w");
	fprintf(pipe, "reset\n");
	fprintf(pipe, "set border linewidth 1.5\n");
	fprintf(pipe, "set style line 1 lc rgb '#aa5500' lt 1 lw 3 pt 7 ps 0.5 # --- red\n");
	fprintf(pipe, "set grid\n");
	fprintf(pipe, "set title \"Metropolis\"\n");
	fprintf(pipe, "set tics out nomirror\n");
	fprintf(pipe, "set xlabel \"{/Symbol D}E\"\n");
	fprintf(pipe, "set ylabel \"Frequency\"\n");
	fprintf(pipe, "set term postscript enhanced color landscape lw 1 \"Verdana,10\"\n");
	fprintf(pipe, "set output 'histogram.eps'\n");
	fprintf(pipe, "f(x)=exp(-0.5*((x-m)/s)**2)/(2.50662827*s)\n");
	fprintf(pipe, "m=0.962\n");
	fprintf(pipe, "s=5e-3\n");
	fprintf(pipe, "pi=3.14159265\n");
	fprintf(pipe, "fit f(x) 'bin.dat' using 1:2 via s,m\n");
	fprintf(pipe, "n=125\t#number of intervals\n");
	fprintf(pipe, "max=0.99\t#max value\n");
	fprintf(pipe, "min=0.93\t#min value\n");
	fprintf(pipe, "width=(max-min)/n\t#interval width\n");
	fprintf(pipe, "hist(x,width)=width*floor(x/width)+width/2.0\n");
	fprintf(pipe, "set xrange [min:max]\n");
	fprintf(pipe, "set yrange [0:]\n");
	fprintf(pipe, "set xtics min,(max-min)/5,max\n");
	fprintf(pipe, "set boxwidth width\n");
	fprintf(pipe, "set style fill solid 0.5\t#fillstyle\n");
	fprintf(pipe, "set tics out nomirror\n");
	fprintf(pipe, "ti = sprintf(\"Gaussian Fit:\\n{/Symbol m} = %%f; {/Symbol s} = %%f\", m, s)\n");
	fprintf(pipe, "plot 'dE.dat' u (hist($1,width)):(1.0) smooth freq w boxes lc rgb '#ddcccc' title 'Binned data',f(x) w l ls 1 title ti\n");
	fclose(pipe);
    system("rm fit.log");
    system("rm bin.dat");
}

void plot_var(){
	FILE *pipe = popen("gnuplot -persist","w");
	fprintf(pipe, "reset\n");
	fprintf(pipe, "set border linewidth 1.5\n");
	fprintf(pipe, "set grid\n");
	fprintf(pipe, "set title \"Metropolis\"\n");
	fprintf(pipe, "set xlabel \"sweep number\"\n");
	fprintf(pipe, "set ylabel \"{/Symbol s^2}\"\n");
	fprintf(pipe, "set term postscript enhanced color landscape lw 1 \"Verdana,10\"\n");
	fprintf(pipe, "set output 'variance.eps'\n");
	fprintf(pipe, "set log xy\n");
	fprintf(pipe, "plot 'var_dE.dat' pt 6 ps 1.5 lc rgb '#000022' title 'variance'\n");
	fclose(pipe);
}

#endif
