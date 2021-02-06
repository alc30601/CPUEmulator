// main.cpp
//
// Remote git repository is below.
// https://github.com/alc30601/DataFlowGraph.git

#include <iostream>
#include <memory>

#include "Edge.hpp"
#include "Node.hpp"


//-----------------------------------------------------------
class MyNode1 : public Node
{
    std::shared_ptr<Edge> _edge;

public:
    void setEdge(std::shared_ptr<Edge> edge)
    {
        _edge = edge;
    }

    void execute(void)
    {
        _edge->setValue(3);
    }
};

//-----------------------------------------------------------
class MyNode2 : public Node
{
    std::shared_ptr<Edge> _edge;

public:
    void setEdge(std::shared_ptr<Edge> edge)
    {
        _edge = edge;
    }

    void execute(void)
    {
        int a;
        a = std::any_cast<int>(_edge->getValue());
        std::cout << "flowed value is : " << a << std::endl;
    }
};


//-----------------------------------------------------------
int main(void)
{
    std::cout << "-- DataFlowGraph -- " << std::endl;

    MyNode1 n1;
    MyNode2 n2;
    std::shared_ptr<Edge> edge(new Edge(0));

    n1.setEdge(edge);
    n2.setEdge(edge);

    n1.execute();
    n2.execute();

    return 0;
}

