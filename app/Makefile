CXX = g++
CXXFLAGS = -Wall

SRC_DIR = ./src
BUILD_DIR = ./build
OBJS = main.o System.o SystemConfig.o

$(BUILD_DIR)/main: $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJS): %.o: $(SRC_DIR)/%.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $^ -o $@

clean:
	rm -f $(BUILD_DIR)/main $(OBJS)