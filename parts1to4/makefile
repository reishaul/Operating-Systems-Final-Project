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
	./$(TARGET) -v 5 -e 6 -s 42
	./$(TARGET) -v 3 -e 3 -s 1

# Generate coverage report
coverage-report:
	@if [ ! -f graph.gcda ]; then \
		echo "No coverage data found. Run 'make coverage-run' first."; \
		exit 1; \
	fi
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory coverage_report
	@echo "Coverage report generated in coverage_report/index.html"

clean:
	rm -f $(TARGET) $(OBJS) \
	      gmon.out gprof_report.txt valgrind_report.txt \
	      callgrind.out.* \
		  *.gcda *.gcno *.gcov coverage.info
	rm -rf coverage_report

.PHONY: all clean run coverage-run coverage-report
