
CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -pthread
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -pthread

TARGET = mandelbrot
SRC = main.cpp ComplexPlane.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
