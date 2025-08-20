CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
INCLUDES = -Iinclude -IstrategyAlg

SRC = $(wildcard src/*.cpp) $(wildcard src/algorithms/*.cpp)
OBJ = $(SRC:.cpp=.o)

TARGET = server

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
