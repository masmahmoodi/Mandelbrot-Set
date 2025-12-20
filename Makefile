CXX = g++
CXXFLAGS = -std=c++17 -O2
LIBS = -lsfml-graphics -lsfml-window -lsfml-system -pthread

all: mandelbrot

mandelbrot: main.o ComplexPlane.o
	$(CXX) $(CXXFLAGS) main.o ComplexPlane.o -o mandelbrot $(LIBS)

main.o: main.cpp ComplexPlane.h
	$(CXX) $(CXXFLAGS) -c main.cpp

ComplexPlane.o: ComplexPlane.cpp ComplexPlane.h
	$(CXX) $(CXXFLAGS) -c ComplexPlane.cpp

clean:
	rm -f *.o mandelbrot
