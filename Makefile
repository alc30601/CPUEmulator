# makefile

HEADERS = 	Edge.hpp \
			Node.hpp \
			Executor.hpp \
			NodeSubSystem.hpp \
			NodeBase.hpp \
			NodeLogics.hpp \
			NodeArithmetic.hpp \
			test_01.hpp \
			test_02.hpp \
			test_03.hpp \
			test_04.hpp \
			test_05.hpp \



SOURCE = 	main.cpp

exe : $(SOURCE) $(HEADERS)
	g++ -std=c++17 $(SOURCE) -o dataflowgraph

clean :
	rm dataflowgraph
