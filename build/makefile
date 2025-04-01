# Compiler and flags
CXX = g++
CXXFLAGS = -pedantic-errors -std=c++17

################## BEGIN LIBRARIES ##################

# FTXUI
LIB_FTXUI_INCLUDE = -I../dependencies/FTXUI/include
LIB_FTXUI_LINK_FLAGS = -lftxui-component -lftxui-dom -lftxui-screen
LIB_FTXUI_LINK = -L../dependencies/FTXUI/lib $(LIB_FTXUI_LINK_FLAGS)
LIB_FTXUI_LINK_MACOS = -L../dependencies/FTXUI_macOS/lib $(LIB_FTXUI_LINK_FLAGS)
LIB_FTXUI_LINK_WINDOWS = -L../dependencies/FTXUI_windows/lib $(LIB_FTXUI_LINK_FLAGS)

################## ALL LIBRARIES ##################
LIBS_ALL_LINK = $(LIB_FTXUI_LINK)
LIBS_ALL_LINK_MACOS = $(LIB_FTXUI_LINK_MACOS)
LIBS_ALL_LINK_WINDOWS = $(LIB_FTXUI_LINK_WINDOWS)

LIBS_ALL_INCLUDE = $(LIB_FTXUI_INCLUDE)
################## END LIBRARIES ##################

# Source files
SRCS = $(shell find ../src -type f -name '*.cpp')
OBJS = $(SRCS:.cpp=.o)

# Executable
TARGET = main
TARGET_MACOS = main.out
TARGET_WINDOWS = main.exe

# Build targets
all: $(TARGET)
all_macos: $(TARGET_MACOS)
all_windows: $(TARGET_WINDOWS)

# Build and run
run: all
	./$(TARGET)
run_macos: all_macos
	./$(TARGET_MACOS)
run_windows: all_windows
	./$(TARGET_WINDOWS)

# Build executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBS_ALL_LINK) -o $(TARGET)
$(TARGET_MACOS): $(OBJS)
	rm -f main 2> /dev/null || true
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBS_ALL_LINK_MACOS) -o $(TARGET_MACOS)
	mv $(TARGET_MACOS) main
$(TARGET_WINDOWS): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBS_ALL_LINK_WINDOWS) -o $(TARGET_WINDOWS)

# Build object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(LIBS_ALL_INCLUDE) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJS) $(TARGET) $(TARGET_MACOS) $(TARGET_WINDOWS) 2> /dev/null || true

.PHONY: all all_macos all_windows clean