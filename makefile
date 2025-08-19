CXX=g++
CXXFLAGS=-std=c++17 -Wall -Wextra -Iinclude -IstrategyAlg
SRC=src/*.cpp src/algorithms/*.cpp
OUT=server

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)
