CXX = g++
CXXFLAGS = -Wall

OBJS = main.o System.o SystemConfig.o

main: $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

main.o: main.cpp System.h SystemConfig.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

System.o: System.cpp System.h SystemConfig.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

SystemConfig.o: SystemConfig.cpp SystemConfig.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f main $(OBJS)