SRC_DIR = ./src
BUILD_DIR = ./build
APP_DIR = ./apps
INC_DIR = ./include
CONSOLE_DIR = $(APP_DIR)/console

CXX = g++
CXXFLAGS = -Wall -I$(INC_DIR)

EXE_NAME = main

SRCS = $(shell find $(SRC_DIR) $(APP_DIR) -name '*.cpp')
OBJS = $(SRCS:%.cpp=$(BUILD_DIR)/%.o)

$(EXE_NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $^ -o $@

clean:
	rm -rf $(BUILD_DIR) $(EXE_NAME)