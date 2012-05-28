set term postscript enhanced color landscape lw 1 "Verdana,10"
set output 'integral.eps'

set style function lines
set size 1.0, 1.0
set origin 0.0, 0.0
set multiplot
set grid
unset key
set format x "10^{%L}"
set format y "10^{%L}"

#  Plot 1
set size 0.5,0.5
set origin 0.0,0.5
set title "Newton-Cotes 1"
set logscale xy
set xlabel "Intervals"
set ylabel "error"
set xrange [10:1100]
plot 'data.dat' using 1:2 w l lt 1 lw 1 lc 1 title 'NC1'

#  Plot 2
set size 0.5,0.5
set origin 0.0,0.0
set title "Newton-Cotes 2"
set logscale xy
set xlabel "Intervals"
set ylabel "error"
set xrange [10:1100]
plot 'data.dat' using 1:3 w l lt 1 lw 1 lc 2 title 'NC2'

#  Plot 3
set size 0.5,0.5
set origin 0.5,0.5
set title "Gaussian 5"
set logscale xy
set xlabel "Intervals"
set ylabel "error"
set xrange [10:1100]
plot 'data.dat' using 1:4 w l lt 1 lw 1 lc 3 title 'G5'

#  Plot 4
set origin 0.5,0.0
set title "Monte Carlo"
set logscale xy
set xlabel "Intervals"
set ylabel "error"
set xrange [10:1100]
plot 'data.dat' using 1:5 w l lt 1 lw 1 lc 4 title 'MC'

unset multiplot
