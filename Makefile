# makefile

HEADERS = 	\
			Edge.hpp \
			Node.hpp \
			Executor.hpp \
			NodeSubSystem.hpp \
			NodeBase.hpp \
			NodeLogics.hpp \
			NodeArithmetic.hpp \

SOURCE = 	main.cpp

exe : $(SOURCE) $(HEADERS)
	g++ -std=c++17  -I. -I./TEST $(SOURCE) -o dataflowgraph

clean :
	rm dataflowgraph
