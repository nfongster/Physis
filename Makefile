SRC_DIR = ./src
BUILD_DIR = ./build
APP_DIR = ./apps
INC_DIR = ./include
EXTERNALS_DIR = /usr/local/bin

GLFW_INC = $(EXTERNALS_DIR)/glfw/include
GLAD_INC = $(EXTERNALS_DIR)/glad/include

GLFW_LIB = $(EXTERNALS_DIR)/glfw/build/src/libglfw3.a

CXX = g++
CXXFLAGS = -Wall -I$(INC_DIR) -I$(GLFW_INC) -I$(GLAD_INC)
LDFLAGS = -Lglfw3 -LGL -LX11 -Lpthread -LXrandr -LXi -Ldl

CONSOLE_EXE = main
GUI_EXE = gui

SRCS = $(shell find $(SRC_DIR) -name '*.cpp')
OBJS = $(SRCS:%.cpp=$(BUILD_DIR)/%.o)

all: $(CONSOLE_EXE) $(GUI_EXE)

$(CONSOLE_EXE): $(APP_DIR)/console/main.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(GUI_EXE): $(APP_DIR)/gui/gui.cpp $(APP_DIR)/gui/glad.c $(OBJS) $(GLFW_LIB)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $^ -o $@

clean:
	rm -rf $(BUILD_DIR) $(CONSOLE_EXE) $(GUI_EXE)