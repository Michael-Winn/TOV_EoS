reset
set style line 11 lc rgb '#808080' lt 1
set border 3 back ls 11
set style line 12 lc rgb '#808080' lt 0 lw 1
set grid back ls 12
set tics nomirror

#set multiplot

set xlabel "R[km]"
set ylabel "M [Solar Mass]"
set y2label "n_0 [fm^{-3}]"
set title "Mass of the star function the radius R"

set key top left
set title "Extremum values of M(R) curves"

#plot 'all_out73.txt' u 1:2:3 "%lf %lf %lf" w filledcu, \
     'all_out16.txt' u 1:2:3 lt -1 w filledcu  

#plot 'all_out73.txt' u 1:2 with filledcurves notitle,\
     'all_out16.txt' u 1:2 with filledcurves notitle

set style fill transparent solid 0.5 # fill style 

#plot 'extreme.csv' u 1:2:3 with filledcurves lc rgb "purple"  ,\
     'all_out73.txt' u 1:2 w l lc 6 ,\
     'extreme.csv' u 1:3 w l lc 6 ,\
     'all_out16.txt' u 1:2 w l lc 3  
set xrange[11.5:20]
set yrange[0.1:0.7]
f(x) = a*exp(-b*x)
#g(x) = e*x*x*x +f*x*x +g*x + h 
g(x) = d*x*x +e*x + f

fit [11.5:20] [0.1:0.7] f(x) "all_out16.txt" using 1:2 via a,b
fit g(x) "all_out73.txt" using 1:2 via d,e,f


plot '+' using 1:(f($1)):(g($1)) with filledcurves closed,\
     '+' using 1:(f($1)) ,\
     '+' using 1:(g($1)) ,\
     'all_out73.txt' u 1:2 w l lc 6 ,\
     'all_out16.txt' u 1:2 w l lc 3  

#plot 'all_out73.txt' u 1:2:3 with filledcurves lc 6 ,\
     'all_out16.txt' u 1:2:3 with filledcurves lc 3  