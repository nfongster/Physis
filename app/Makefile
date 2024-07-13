CXX = g++
CXXFLAGS = -Wall

OBJS = main.o System.o SystemConfig.o

main: $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

System.o: System.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

SystemConfig.o: SystemConfig.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f main $(OBJS)