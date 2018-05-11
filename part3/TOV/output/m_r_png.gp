reset
set terminal png size 1280,720 enhanced font "Helvetica,20"
set output 'm_r.png'
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
set title "All M(R) for al 325 EOS"
set xrange[8:20]
set yrange [0.15:0.65]

plot for [i=0:324] 'all_out'.i.'.txt' using 1:2 title ''.i ,\
                   'all_out73.txt' u 1:2 w l notitle,\
                   'all_out16.txt' u 1:2 w l notitle ,\
                   'all_out_sly.txt' u 1:2 w l ,\
                   'all_out_DH.txt' u 1:2 w l 

#plot 'all_out73.txt' u 1:2:3 "%lf %lf %lf" w filledcu, \
     'all_out16.txt' u 1:2:3 lt -1 w filledcu  

#plot 'all_out73.txt' u 1:2 with filledcurves notitle,\
     'all_out16.txt' u 1:2 with filledcurves notitle
