# makefile

HEADERS = 	Edge.hpp \
			Node.hpp \


SOURCE = 	main.cpp

exe : $(SOURCE) $(HEADERS)
	g++ $(SOURCE) -o dataflowgraph

clean :
	rm dataflowgraph
