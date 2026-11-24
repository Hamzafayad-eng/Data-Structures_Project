CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = streetlight_app
SRC = Source_Code/main.cpp Source_Code/stack_repair_history.cpp Source_Code/queue_faults_waiting.cpp Source_Code/linked_list_pole_inventory.cpp Source_Code/bst_pole_id_index.cpp
HEADERS = Source_Code/repair_desk.h

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
