set border linewidth 1.5
set grid
set title "MC-MH"
set xlabel "N"
set ylabel "pdf"
set term postscript enhanced color landscape lw 1 "Verdana,10"
set output 'pdf.eps'
#set log x
bin(x,width)=width*floor(x/width)
#plot 'pi.dat' using (bin($1,exp(floor(log($1))))):(1.0) smooth freq with boxes lw 3 lc rgb '#ff0000' notitle
plot 'pi.dat' using (bin($1,1000)):(1.0) smooth freq with boxes lw 3 lc rgb '#ff0000' notitle
