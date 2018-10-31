######################################################################
### @file makefile
### @author Tristan Fountain CS5001
### @brief Makefile for Homework 1 - Linear Learning
######################################################################

.PHONY: all clean

CXX = /usr/bin/g++
CXXFLAGS = -g -Wall -W -pedantic-errors -std=c++11

SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.h)

OBJECTS = $(SOURCES:%.cpp=%.o)
LIBRARY_FLAGS = `libpng-config --ldflags`

default: driver

%.o: %.cpp
	@echo "Compiling $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

driver: $(OBJECTS)
	@echo "Building $@..."
	@$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(LIBRARY_FLAGS)
	@echo "Success!"
	@echo ""

clean:
	-@rm -f core
	-@rm -f driver
	-@rm -f depend
	-@rm -f $(OBJECTS)

# Automatically generate dependencies and include them in makefile
depend: $(SOURCES) $(HEADERS)
	@echo "Generating dependencies"
	@$(CXX) -MM *.cpp > $@


-include depend