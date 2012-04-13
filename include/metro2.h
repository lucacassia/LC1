#ifndef METRO2_H
#define METRO2_H

#include <stdio.h>

void metro2(void){
	FILE *pipe = popen("gnuplot -persist","w");
	fprintf(pipe, "reset\n");
	fprintf(pipe, "set border linewidth 1.5\n");
	fprintf(pipe, "set style line 1 lc rgb '#228f26' lt 1 lw 2 pt 7 ps 1.5 # --- green\n");
	fprintf(pipe, "set grid\n");
	fprintf(pipe, "set title \"Metropolis\"\n");
	fprintf(pipe, "set xlabel \"i\"\n");
	fprintf(pipe, "set ylabel \"o[i]\"\n");
	fprintf(pipe, "unset key\n");
	fprintf(pipe, "set log y\n");
	fprintf(pipe, "set term postscript enhanced color landscape lw 1 \"Verdana,10\"\n");
	fprintf(pipe, "set output 'metro2.eps'\n");
	fprintf(pipe, "plot 'metro2.dat' with linespoints ls 1\n");
	fclose(pipe);
}

#endif
