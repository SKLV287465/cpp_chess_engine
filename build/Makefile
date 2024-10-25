# Variables
CXX = g++
CXXFLAGS = -Iinclude -Wall -std=c++20
LDFLAGS =
SRC = ../src/engine/Board.cpp ../src/test/Test.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = my_program

# Rules
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean