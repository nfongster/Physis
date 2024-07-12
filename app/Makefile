CXX=g++
CXXFLAGS=-Wall
main: main.o System.o SystemConfig.o
	$(CXX) $(CXXFLAGS) -o main main.o System.o SystemConfig.o

main.o: main.cpp System.h SystemConfig.h
System.o: System.cpp System.h SystemConfig.h
SystemConfig.o: SystemConfig.cpp SystemConfig.h

clean:
		rm -f main main.o System.o SystemConfig.o