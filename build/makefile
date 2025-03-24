# Compiler and flags
CXX = g++
CXXFLAGS = -pedantic-errors -std=c++17

################## BEGIN LIBRARIES ##################
# ncurses
LIB_NCURSES_LINK = -lncurses

################## ALL LIBRARIES ##################
LIBS_ALL_LINK = $(LIB_NCURSES_LINK)
################## END LIBRARIES ##################

# Source files
SRCS = $(wildcard ../src/*.cpp)
OBJS = $(SRCS:.cpp=.o)

# Executable
TARGET = main

# Build
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBS_ALL_LINK) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean