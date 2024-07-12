CXX = g++
CXXFLAGS = -Wall

OBJS = main.o System.o SystemConfig.o

main: $(OBJS)
	$(CXX) $(CXXFLAGS) -o main main.o System.o SystemConfig.o

main.o: main.cpp System.h SystemConfig.h
System.o: System.cpp System.h SystemConfig.h
SystemConfig.o: SystemConfig.cpp SystemConfig.h

clean:
	rm -f main $(OBJS)