# Compiler
CXX = g++
CXXFLAGS = -std=c++20 -O2 -Wall -Wextra -Iinclude -pthread

# Files
SRC = main.cpp channel.cpp pipeline.cpp
OBJ = main.o channel.o pipeline.o
TARGET = pipeline

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

# Compile
main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

channel.o: channel.cpp
	$(CXX) $(CXXFLAGS) -c channel.cpp

pipeline.o: pipeline.cpp
	$(CXX) $(CXXFLAGS) -c pipeline.cpp

# Run targets
run1: $(TARGET)
	./$(TARGET) -n 1000 -q 1

run2: $(TARGET)
	./$(TARGET) -n 1000 -q 50

run3: $(TARGET)
	./$(TARGET) -n 10000 -q 50

run4: $(TARGET)
	./$(TARGET) -n 100000 -q 1000

# Clean
clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean run1 run2 run3 run4