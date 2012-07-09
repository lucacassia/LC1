set border linewidth 1.5
set grid
set title "Pi MC"
set xlabel "N"
set ylabel "var/sigma"
set term postscript enhanced color landscape lw 1 "Verdana,10"
set output 'pi.eps'
f(x) = 1/x
set sample 1000
set xrange[1:100000000]
set log xy
set style line 1 lt 1 lc rgb '#000000' lw 3
plot 'pi.dat' pt 6 ps 0.1 lc rgb '#000000' title 'MC error', f(x) ls 1 title '1/N'
