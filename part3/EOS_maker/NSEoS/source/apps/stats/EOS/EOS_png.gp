reset
set terminal png size 1280,720 enhanced font "Helvetica,20"
set output 'EOS.png'
#set multiplot
set xlabel "n_0 [g/cm^{3}]"
set ylabel "Pressure [MeV/fm^3] "

set xtics 
set style line 11 lc rgb '#808080' lt 1
set border 3 back ls 11
set style line 12 lc rgb '#808080' lt 0 lw 1
set grid back ls 12
set format y "10^{%L}"
set format x "10^{%L}"
set tics nomirror

set logscale x
set logscale y


set key top left
set title "All EOS with valide parameters sets up to 2*n_{sat}"




plot for [i=0:324] 'eos_'.i.'_.out' using 2:3 notitle
