#ifndef METRO3_H
#define METRO3_H

#include <stdio.h>

void metro3(void){
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
	fprintf(pipe, "set output 'metro3.eps'\n");
	fprintf(pipe, "plot 'metro3.dat' with linespoints ls 1\n");
	fclose(pipe);
}

#endif
