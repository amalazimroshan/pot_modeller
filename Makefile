# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -std=c++11

# Include directories
INCLUDES = -I.

# Libraries (SDL2 in this case)
LIBS = -lSDL2

# Source files
SRCS = Main.cpp Renderer.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable name
TARGET = main

# Default target
all: $(TARGET)

# Rule to link object files and create the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

# Rule to compile .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean rule to remove the generated files
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets (not real files)
.PHONY: all clean


