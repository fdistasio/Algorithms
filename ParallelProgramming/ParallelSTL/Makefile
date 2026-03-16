CXX                = g++ -std=c++20
OPTFLAGS	   = -O3 -march=native
CXXFLAGS          += -Wall 
INCLUDES	   = -I. -I ./include
LIBS               = -pthread -ltbb
SOURCES            = $(wildcard src/*.cpp)
TARGET             = primes

.PHONY: all clean cleanall 

%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) $(INCLUDES) -c $< -o $@

primes: $(SOURCES:src/%.cpp=%.o)
	$(CXX) $(OPTFLAGS) -o $@ $^ $(LIBS)

all: $(TARGET)

clean: 
	-rm -fr *.o *~
cleanall: clean
	-rm -fr $(TARGET)

