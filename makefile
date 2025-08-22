CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -O0
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

# Run with valgrind memcheck
valgrind_memcheck: $(TARGET)
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./$(TARGET)

# Run with valgrind helgrind 
valgrind_helgrind: $(TARGET)
	valgrind --tool=helgrind ./$(TARGET)

# Run with valgrind helgrind with suppressions
valgrind_helgrind_supp: $(TARGET)
	valgrind --tool=helgrind --suppressions=helgrind.supp ./$(TARGET)


# Run with valgrind callgrind (profiling)
valgrind_callgrind: $(TARGET)
	valgrind --tool=callgrind ./$(TARGET)
