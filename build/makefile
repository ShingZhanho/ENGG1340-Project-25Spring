## Libraries
# FTXUI
LIB_FTXUI_INCLUDE = -I../dependencies/FTXUI/include
LIB_FTXUI_LINK = -L../dependencies/FTXUI/lib -lftxui-component -lftxui-dom -lftxui-screen

# ==== ALL LIBS ====
LIBS_INCLUDE = $(LIB_FTXUI_INCLUDE)
LIBS_LINK = $(LIB_FTXUI_LINK)

# Compiler and flags
CXX = g++
CXXFLAGS = -pedantic-errors -std=c++17

# Source files
SRCS = ../src/main.cpp
OBJS = $(SRCS:.cpp=.o)

# Executable
TARGET = main

# Build
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBS_LINK) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(LIBS_INCLUDE) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean