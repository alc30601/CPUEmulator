# makefile

HEADERS = 	Edge.hpp \
			Node.hpp \


SOURCE = 	main.cpp

exe : $(SOURCE) $(HEADERS)
	g++ -std=c++17 $(SOURCE) -o dataflowgraph

clean :
	rm dataflowgraph
