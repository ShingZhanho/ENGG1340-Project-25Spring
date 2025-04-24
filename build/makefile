# Compiler and flags
CXX = g++
CXXFLAGS = -pedantic-errors -std=c++17 -g

################## BEGIN LIBRARIES ##################

# Core Game Headers
LIB_CORE_INCLUDE = -I../include

# FTXUI
LIB_FTXUI_INCLUDE = -I../dependencies/FTXUI/include
LIB_FTXUI_LINK_FLAGS = -lftxui-component -lftxui-dom -lftxui-screen
LIB_FTXUI_LINK = -L../dependencies/FTXUI/lib $(LIB_FTXUI_LINK_FLAGS)
LIB_FTXUI_LINK_MACOS = -L../dependencies/FTXUI_macOS/lib $(LIB_FTXUI_LINK_FLAGS)
LIB_FTXUI_LINK_WINDOWS = -L../dependencies/FTXUI_windows/lib $(LIB_FTXUI_LINK_FLAGS)

# pthread
LIB_PTHREAD_LINK = -lpthread

################## ALL LIBRARIES ##################
LIBS_ALL_LINK = $(LIB_FTXUI_LINK) $(LIB_PTHREAD_LINK)
LIBS_ALL_LINK_MACOS = $(LIB_FTXUI_LINK_MACOS)
LIBS_ALL_LINK_WINDOWS = $(LIB_FTXUI_LINK_WINDOWS)

LIBS_ALL_INCLUDE = $(LIB_FTXUI_INCLUDE) $(LIB_CORE_INCLUDE)
################## END LIBRARIES ##################

# Source files
SRCS = $(shell find ../src -type f -name '*.cpp')
OBJS_TARGET_DIR = objs
OBJS = $(patsubst ../src/%.cpp,$(OBJS_TARGET_DIR)/%.o,$(SRCS))

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
	./$(TARGET)
run_windows: all_windows
	./$(TARGET_WINDOWS)

# Build executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBS_ALL_LINK) -o $(TARGET)
	$(COPY_RESOURCES)
$(TARGET_MACOS): $(OBJS)
	rm -f main 2> /dev/null || true
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBS_ALL_LINK_MACOS) -o $(TARGET_MACOS)
	mv $(TARGET_MACOS) main
	$(COPY_RESOURCES)
$(TARGET_WINDOWS): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBS_ALL_LINK_WINDOWS) -o $(TARGET_WINDOWS)
	$(COPY_RESOURCES)

# Build object files
$(OBJS_TARGET_DIR)/%.o: ../src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(LIBS_ALL_INCLUDE) -c $< -o $@

# Copy game resources to output
COPY_RESOURCES = mkdir -p ./res && rsync -av --delete ../res ./

# Clean up
clean:
	rm -rf $(OBJS) $(TARGET) $(TARGET_MACOS) $(TARGET_WINDOWS) $(OBJS_TARGET_DIR) ./res ./game.log 2> /dev/null || true

.PHONY: all all_macos all_windows clean