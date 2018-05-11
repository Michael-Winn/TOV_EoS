IDIR =../code
CXX = g++ -std=c++11
CFLAGS = -I$(IDIR)

DEPS = build_star.h
OBJ = build_star.o build_star_funct.o

%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)

build_star: $(OBJ)
	$(CXX) -o star build_star.cxx build_star_funct.cc $(CFLAGS)


