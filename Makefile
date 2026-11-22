CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = streetlight_app
SRC = Source_Code/main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

demo: $(TARGET)
	./$(TARGET) < Tests/sample_demo_input.txt

clean:
	rm -f $(TARGET) *.o

.PHONY: all run demo clean
