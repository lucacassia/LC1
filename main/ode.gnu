set border linewidth 1.5
set style line 1 lc rgb '#bf0d23' lt 1 lw 3 # --- red
set grid
set title "RK error"
set xlabel "t"
set ylabel "error"
set xrange[0:100]
set term postscript enhanced color landscape lw 1 "Verdana,10"
set output 'ode.eps'
plot 'ode.dat' using ($7):(sqrt(($1-$4)*($1-$4)+($2-$5)*($2-$5)+($3-$6)*($3-$6))) w l ls 1 notitle
