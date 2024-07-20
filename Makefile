SRC_DIR = ./src
BUILD_DIR = ./build
APP_DIR = ./apps
INC_DIR = ./include

CXX = g++
CXXFLAGS = -Wall -I$(INC_DIR)

CONSOLE_EXE = main
GUI_EXE = gui

SRCS = $(shell find $(SRC_DIR) -name '*.cpp')
OBJS = $(SRCS:%.cpp=$(BUILD_DIR)/%.o)

all: $(CONSOLE_EXE) $(GUI_EXE)

$(CONSOLE_EXE): $(APP_DIR)/console/main.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(GUI_EXE): $(APP_DIR)/gui/gui.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $^ -o $@

clean:
	rm -rf $(BUILD_DIR) $(CONSOLE_EXE) $(GUI_EXE)