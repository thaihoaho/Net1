# Makefile
CXX = g++
CXXFLAGS = -Wall -g

# Source files
SOURCES = main.cpp include/command.cpp include/feature.cpp include/request.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = myProgram

build: $(SOURCES)
	$(CXX) -o SOURCES EXECUTABLE -lws2_32

run: $(EXECUTABLE)
	.\$(EXECUTABLE)

bar: build
	.\$(EXECUTABLE)