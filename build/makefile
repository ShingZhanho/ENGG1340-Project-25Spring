# Compiler and flags
CXX = g++
CXXFLAGS = -pedantic-errors -std=c++17

################## BEGIN LIBRARIES ##################
# ncurses
LIB_NCURSES_LINK = -lncurses

# FTXUI
LIB_FTXUI_INCLUDE = -I../dependencies/FTXUI/include
LIB_FTXUI_LINK = -L../dependencies/FTXUI/lib -lftxui-component -lftxui-dom -lftxui-screen

################## ALL LIBRARIES ##################
LIBS_ALL_LINK = $(LIB_NCURSES_LINK) $(LIB_FTXUI_LINK)
LIBS_ALL_INCLUDE = $(LIB_FTXUI_INCLUDE)
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
	$(CXX) $(CXXFLAGS) $(LIBS_ALL_INCLUDE) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean