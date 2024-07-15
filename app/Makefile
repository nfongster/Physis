CXX = g++
CXXFLAGS = -Wall

BUILD_DIR = ./build
OBJS = main.o System.o SystemConfig.o

$(BUILD_DIR)/main: $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# TODO: Get object files into build folder
$(OBJS): %.o: %.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $^ -o $@

clean:
	rm -f $(BUILD_DIR)/main $(OBJS)