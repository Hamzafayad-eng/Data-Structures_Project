CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = streetlight_app
SRC = Source_Code/main.cpp Source_Code/stack.cpp Source_Code/queue.cpp Source_Code/linked_list.cpp Source_Code/bst.cpp
HEADERS = Source_Code/stack.h Source_Code/queue.h Source_Code/linked_list.h Source_Code/bst.h

all: $(TARGET)

$(TARGET): $(SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

demo: $(TARGET)
	./$(TARGET) < Tests/sample_demo_input.txt

clean:
	rm -f $(TARGET) *.o

.PHONY: all run demo clean
