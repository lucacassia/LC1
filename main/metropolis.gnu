reset
set term postscript enhanced color landscape lw 1 "Verdana,10"

#ACTION
set border linewidth 1.5
set style line 1 lc rgb '#bf0d23' lt 1 lw 1 pt 7 ps 0.5 # --- red
set grid
set title "Metropolis"
set xlabel "i"
set ylabel "S"
unset key
set log x
set output 'action.eps'
plot 'action.dat' with linespoints ls 1

#CONFIGURATION
set border linewidth 1.5
set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 7 ps 1.5 # --- blue
set grid
set title "Metropolis"
set xlabel "i"
set ylabel "x[i]"
unset key
set output 'configuration.eps'
plot 'configuration.dat' with linespoints ls 1

#CORRELATION
set border linewidth 1.5
set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 7 ps 1.5 # --- indigo
set grid
set title "Metropolis"
set xlabel "{/Symbol D}t"
set ylabel "c[{/Symbol D}t]"
unset key
set xrange[0:32]
set output 'correlation.eps'
plot 'correlation.dat' with linespoints ls 1

#AUTOCORRELATION
set border linewidth 1.5
set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 7 ps 1.5 # --- indigo
set grid
set title "Metropolis"
set xlabel "k"
set ylabel "R[k]"
unset key
set output 'autocorrelation.eps'
plot 'autocorrelation.dat' with linespoints ls 1

#HISTOGRAM
set border linewidth 1.5
set style line 1 lc rgb '#0060ad' lt 1 lw 3 pt 7 ps 0.5 # --- indigo
set grid
set title "Metropolis"
set tics out nomirror
set xlabel "{/Symbol D}E"
set ylabel "Frequency"
set term postscript enhanced color landscape lw 1 "Verdana,10"
set output 'histogram.eps'
f(x)=exp(-0.5*((x-m)/s)**2)/(2.50662827*s)
m=0.962
s=5e-3
pi=3.14159265
fit f(x) 'bin.dat' using 1:2 via s,m
n=125\t#number of intervals
max=0.99\t#max value
min=0.93\t#min value
width=(max-min)/n\t#interval width
hist(x,width)=width*floor(x/width)+width/2.0
set xrange [min:max]
set yrange [0:]
set xtics min,(max-min)/5,max
set boxwidth width
set style fill solid 0.5\t#fillstyle
set tics out nomirror
ti = sprintf("Gaussian Fit:\\n{/Symbol m} = %%f; {/Symbol s} = %%f", m, s)
plot 'histogram.dat' u (hist($1,width)):(1.0) smooth freq w boxes lc rgb '#87b2d7' title 'Binned data',f(x) w l ls 1 title ti

#VARIANCE
set border linewidth 1.5
set grid
set title "Metropolis"
set xlabel "sweep number"
set ylabel "{/Symbol s^2}"
set output 'variance.eps'
set log xy
plot 'var_dE.dat' pt 6 ps 1.5 lc rgb '#000022' title 'variance'

