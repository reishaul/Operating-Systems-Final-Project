#reishaul1@gmail.com
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -O0
INCLUDES = -Iinclude -IstrategyAlg

#wildcard is a function that returns all files matching a pattern
SRC = $(wildcard src/*.cpp) $(wildcard src/algorithms/*.cpp)
OBJ = $(SRC:.cpp=.o)

TARGET = server

# Build the target
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ $(LDFLAGS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

#clean all kind of coverage files and object files except the important ones
clean:
	rm -f $(OBJ) $(TARGET)
	rm -rf coverage_report
	rm -f src/*.gcda src/*.gcno src/algorithms/*.gcda src/algorithms/*.gcno
	rm -f *.gcda *.gcno coverage*.info
	@# Keep important .gcov files: Graph.cpp.gcov MST.cpp.gcov MaxFlow.cpp.gcov main.cpp.gcov Hamilton.cpp.gcov server.cpp.gcov MaxClique.cpp.gcov Pipeline.cpp.gcov
	@find . -name "*.gcov" ! -name "Graph.cpp.gcov" ! -name "MST.cpp.gcov" ! -name "MaxFlow.cpp.gcov" ! -name "main.cpp.gcov" ! -name "Hamilton.cpp.gcov" ! -name "server.cpp.gcov" ! -name "MaxClique.cpp.gcov" ! -name "Pipeline.cpp.gcov" -delete

# Clean all kind of coverage files and object files
clean-all:
	rm -f $(OBJ) $(TARGET)
	rm -rf coverage_report
	rm -f src/*.gcda src/*.gcno src/algorithms/*.gcda src/algorithms/*.gcno
	rm -f *.gcda *.gcno *.gcov coverage*.info

# Run with valgrind memcheck
valgrind_memcheck: $(TARGET)
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./$(TARGET)

# Run with valgrind helgrind 
valgrind_helgrind: $(TARGET)
	valgrind --tool=helgrind ./$(TARGET)


# Run with valgrind callgrind (profiling)
valgrind_callgrind: $(TARGET)
	valgrind --tool=callgrind ./$(TARGET)


# For Code Coverage

# Build with coverage flags
coverage-build: clean
	$(MAKE) CXXFLAGS="$(CXXFLAGS) -fprofile-arcs -ftest-coverage" LDFLAGS="-lgcov --coverage" all

# Generate coverage report 
coverage-report: 
	@echo "Generating coverage report with gcov..."
	@for cpp_file in $(SRC); do \
		gcov $$cpp_file -o $$(dirname $$cpp_file); \
	done
	@echo "Coverage files (.gcov) generated for all source files"
	@echo "Use 'ls *.gcov' to see all coverage files"


