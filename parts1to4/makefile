CXX = g++
CXXFLAGS = -g -Wall
TARGET = euler
SRCS = Graph.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET) -v 4 -e 4 -s 3

clean:
	rm -f $(TARGET) $(OBJS) \
	      gmon.out gprof_report.txt valgrind_report.txt \
	      callgrind.out.*

.PHONY: all clean run
