#!/bin/bash 
cd ../EOS_maker/NSEoS/source/apps/stats
make
./stats
cd EOS
gnuplot EOS_png.gp
xdg-open EOS.png
cd ../../../../../../TOV
g++ -std=c++11 build_star.cxx build_star_funct.cc build_star.h -o star
./star
cd output
gnuplot m_r_png.gp
xdg-open m_r.png