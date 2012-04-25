#ifndef PLOT_H
#define PLOT_H

#include <stdio.h>

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
}

void plot_correlation(void){
	FILE *pipe = popen("gnuplot -persist","w");
	fprintf(pipe, "reset\n");
	fprintf(pipe, "set border linewidth 1.5\n");
	fprintf(pipe, "set style line 1 lc rgb '#228f26' lt 1 lw 2 pt 7 ps 1.5 # --- green\n");
	fprintf(pipe, "set grid\n");
	fprintf(pipe, "set title \"Metropolis\"\n");
	fprintf(pipe, "set xlabel \"dt\"\n");
	fprintf(pipe, "set ylabel \"c[dt]\"\n");
	fprintf(pipe, "unset key\n");
	fprintf(pipe, "set term postscript enhanced color landscape lw 1 \"Verdana,10\"\n");
	fprintf(pipe, "set output 'correlation.eps'\n");
	fprintf(pipe, "plot 'correlation.dat' with linespoints ls 1\n");
	fclose(pipe);
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
}

#endif
