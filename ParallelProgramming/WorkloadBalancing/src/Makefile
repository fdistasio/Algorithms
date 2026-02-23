CXX = g++
CXXFLAGS = -std=c++20 -O2 -pthread
TARGET = adaptive_quadrature

SRC = main.cpp integrator.cpp strategies.cpp

OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run 1: sequential
run1: $(TARGET)
	./$(TARGET) 10000 0.2 0

# Run 2: block-cyclic
run2: $(TARGET)
	./$(TARGET) 10000 0.2 1

# Run 3: work-sharing
run3: $(TARGET)
	./$(TARGET) 10000 0.2 2

clean:
	rm -f $(TARGET) $(OBJ) *.out