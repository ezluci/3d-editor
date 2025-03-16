# Compiler and flags
CXX = g++
CXXFLAGS = -c -O2 -std=c++17 -Wall -Wextra
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system
# LDFLAGS = -L/usr/lib/sfml-2.6.2 -lsfml-graphics -lsfml-window -lsfml-system
# export LD_LIBRARY_PATH := /usr/lib/sfml-2.6.2:$(LD_LIBRARY_PATH)

# Source files and object files
SOURCES = src/main.cpp src/camera.cpp src/clock.cpp src/points_math.cpp src/save_and_load.cpp src/shape.cpp src/ui/button.cpp src/ui/text_box.cpp src/shapes/cube.cpp src/shapes/pyramid.cpp src/shapes/sphere.cpp src/shapes/cone.cpp src/shapes/cylinder.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = main

# Default target
all: $(EXECUTABLE)

# Link object files to create the executable
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

# Clean up build files
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
