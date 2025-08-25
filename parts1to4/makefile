CXX = g++
CXXFLAGS = -g -Wall
TARGET = euler
SRCS = graph.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)

# Coverage flags
COVERAGE_FLAGS = -fprofile-arcs -ftest-coverage
COVERAGE_LIBS = -lgcov

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET) -v 4 -e 4 -s 3

# Coverage build
coverage: clean
	$(MAKE) CXXFLAGS="$(CXXFLAGS) $(COVERAGE_FLAGS)" LDFLAGS="$(COVERAGE_LIBS)" $(TARGET)

coverage-run: coverage
	./$(TARGET) -v 4 -e 4 -s 3
	-./$(TARGET) -v 5 -e 6 -s 42
	./$(TARGET) -v 3 -e 3 -s 1
	-./$(TARGET) -v 3 -e -3 -s 1
	-./$(TARGET) -v 3 -r 3 -s 1
	./$(TARGET) -v 4 -e 0 -s 1
	-./$(TARGET) -v 4 -r 0 -s 1
	-./$(TARGET) -v 0 -e 5 -s 1
	-./$(TARGET) -v 5 -e -1 -s 1
	./$(TARGET) -v 3 -e 3 -s 10
	-./$(TARGET) --invalid

# Generate coverage report
coverage-report:
	@if [ ! -f graph.gcda ]; then \
		echo "No coverage data found. Run 'make coverage-run' first."; \
		exit 1; \
	fi
	@echo "Generating coverage report with gcov..."
	gcov graph.cpp
	gcov main.cpp
	@echo "Coverage files (.gcov) generated"
	@echo "Use 'ls *.gcov' to see all coverage files"

clean:
	rm -f $(TARGET) $(OBJS) \
	      gmon.out gprof_report.txt valgrind_report.txt \
	      callgrind.out.* \
	      *.gcda *.gcno coverage.info \
	      $(filter-out main.cpp.gcov graph.cpp.gcov, $(wildcard *.gcov))
	rm -rf coverage_report

.PHONY: all clean run coverage-run coverage-report
