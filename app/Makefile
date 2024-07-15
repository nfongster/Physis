CXX = g++
CXXFLAGS = -Wall

OBJS = main.o System.o SystemConfig.o

main: $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJS): %.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

clean:
	rm -f main $(OBJS)