CXX = g++
CXXFLAGS = -Wall

OBJS = main.o System.o SystemConfig.o

main: $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

main.o: main.cpp System.h SystemConfig.h
	$(CXX) $(CXXFLAGS) -c main.cpp -o $@

System.o: System.cpp System.h SystemConfig.h
	$(CXX) $(CXXFLAGS) -c System.cpp -o $@

SystemConfig.o: SystemConfig.cpp SystemConfig.h
	$(CXX) $(CXXFLAGS) -c SystemConfig.cpp -o $@

clean:
	rm -f main $(OBJS)