reset
set term postscript enhanced color landscape lw 1 "Verdana,10"
set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 7 ps 1.5 # --- indigo
set style line 2 lc rgb '#0060ad' lt 1 lw 1 pt 7 ps 0.5 # --- indigo
set style line 3 lc rgb '#bf0d23' lt 1 lw 1 pt 7 ps 0.5 # --- red
set border linewidth 1.5
set grid
unset key
set title "Metropolis"

#ACTION
set xlabel "i"
set ylabel "S"
set log x
set output 'action.eps'
plot 'action.dat' with linespoints ls 2
unset logscale

#CORRELATION
set xlabel "{/Symbol D}t"
set ylabel "c[{/Symbol D}t]"
set output 'correlation.eps'
plot 'correlation.dat' with linespoints ls 1

#AUTOCORRELATION
set xlabel "k"
set ylabel "R[k]"
set output 'autocorrelation.eps'
plot 'autocorrelation.dat' with linespoints ls 1

#VARIANCE
set log xy
set xlabel "sweep number"
set ylabel "{/Symbol s^2}"
set output 'variance.eps'
plot 'variance.dat' pt 6 ps 1.5 lc rgb '#0060ad' title 'variance'
unset logscale

#HISTOGRAM
set key
set tics out nomirror
set xlabel "{/Symbol D}E"
set ylabel "Frequency"
set output 'histogram.eps'
f(x)=exp(-0.5*((x-m)/s)**2)/(2.50662827*s)
m=0.962
s=5e-3
pi=3.14159265
fit f(x) 'bin.dat' using 1:2 via s,m
n=125
max=0.99
min=0.93
width=(max-min)/n
hist(x,width)=width*floor(x/width)+width/2.0
set xrange [min:max]
set yrange [0:]
set xtics min,(max-min)/5,max
set boxwidth width
set style fill solid 0.5
set tics out nomirror
ti = sprintf("Gaussian Fit:\n{/Symbol m} = %f; {/Symbol s} = %f", m, s)
plot 'histogram.dat' u (hist($1,width)):(1.0) smooth freq w boxes lc rgb '#87b2d7' title 'Binned data', f(x) w l ls 1 title ti

