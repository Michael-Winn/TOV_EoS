reset
set style line 11 lc rgb '#808080' lt 1
set border 3 back ls 11
set style line 12 lc rgb '#808080' lt 0 lw 1
set grid back ls 12
#set tics nomirror

#set multiplot

set xlabel "R[km]"
set ylabel "M [Solar Mass]"
set y2label "n_0 [fm^{-3}]"
set title "Mass of the star function the radius R"

#set key top left
unset key
set title "Extremum values of M(R) curves"

#plot 'all_out73.txt' u 1:2:3 "%lf %lf %lf" w filledcu, \
     'all_out16.txt' u 1:2:3 lt -1 w filledcu  

#plot 'all_out73.txt' u 1:2 with filledcurves notitle,\
     'all_out16.txt' u 1:2 with filledcurves notitle
