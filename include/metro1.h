#ifndef METRO1_H
#define METRO1_H

#include <stdio.h>

void metro1(void){
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
	fprintf(pipe, "set output 'metro1.eps'\n");
	fprintf(pipe, "plot 'metro1.dat' with linespoints ls 1\n");
	fclose(pipe);
}

#endif
