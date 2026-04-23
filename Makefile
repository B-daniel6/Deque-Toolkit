CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = deque_test
SRCS = main.cpp deque.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $(SRCS)

clean:
	rm -f $(TARGET)
