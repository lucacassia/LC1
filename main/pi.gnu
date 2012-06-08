set border linewidth 1.5
set grid
set title "Pi MC"
set xlabel "N"
set ylabel "error"
set term postscript enhanced color landscape lw 1 "Verdana,10"
set output 'pi.eps'
f(x) = 1/sqrt(x)
set sample 1000
set xrange[1:1000000]
set log xy
set style line 1 lt 1 lc rgb '#000000' lw 3
plot 'pi.dat' pt 6 ps 0.1 lc rgb '#000000' title 'MC error', f(x) ls 1 title '1/sqrt(N)'
