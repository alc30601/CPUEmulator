// main.cpp
//
// Remote git repository is below.
// https://github.com/alc30601/DataFlowGraph.git

#include <iostream>
#include <memory>

#include "Edge.hpp"
#include "Node.hpp"
#include "Executor.hpp"


//-----------------------------------------------------------
class MyNode1 : public Node
{
    std::shared_ptr<Edge> _edge1;
    std::shared_ptr<Edge> _edge2;

public:
    void setEdge(std::shared_ptr<Edge> edge1, std::shared_ptr<Edge> edge2)
    {
        _edge1 = edge1;
        _edge2 = edge2;

        addOutEdge(edge1);
        addOutEdge(edge2);
    }

    void execute(void)
    {
        std::cout << "MyNode1::execute()" <<std::endl;

        Node::execute();

        _edge1->setValue(3);
        _edge2->setValue('B');
    }
};

//-----------------------------------------------------------
class MyNode2 : public Node
{
    std::shared_ptr<Edge> _edge1;
    std::shared_ptr<Edge> _edge2;

public:
    void setEdge(std::shared_ptr<Edge> edge1, std::shared_ptr<Edge> edge2)
    {
        _edge1 = edge1;
        _edge2 = edge2;

        addInEdge(edge1);
        addInEdge(edge2);
    }

    void execute(void)
    {
        std::cout << "MyNode2::execute()" <<std::endl;

        Node::execute();
        int a;
        char b;
        a = std::any_cast<int>(_edge1->getValue());
        b = std::any_cast<char>(_edge2->getValue());
        std::cout << "flowed value 1 is : " << a << std::endl;
        std::cout << "flowed value 2 is : " << b << std::endl;
    }
};


//-----------------------------------------------------------
int main(void)
{
    std::cout << "-- DataFlowGraph -- " << std::endl;

    std::shared_ptr<MyNode1> n1(new MyNode1);
    std::shared_ptr<MyNode2> n2(new MyNode2);
    std::shared_ptr<Edge> edge1(new Edge(0));
    std::shared_ptr<Edge> edge2(new Edge('A'));

    n1->setEdge(edge1, edge2);
    n2->setEdge(edge1, edge2);

    edge1->addOutNode(n2);
    edge2->addOutNode(n2);

    Executor exe(n1);
    exe.step();

    return 0;
}

