// test_01.hpp
// 入力ノードと出力ノードの２つ
// その間に2本のエッジ
// 
// +---------+              +---------+
// | MyNode1 |--- edge1 --->| MyNode2 |
// |         |--- edge2 --->|         |
// +---------+              +---------+


#ifndef __TEST_01_HPP__
#define __TEST_01_HPP__

#include "Edge.hpp"
#include "Node.hpp"
#include "Executor.hpp"


//-----------------------------------------------------------
class Test01MyNode1 : public Node
{
    Edge* _edge1;
    Edge* _edge2;

public:
    void setEdge(Edge* edge1, Edge* edge2)
    {
        _edge1 = edge1;
        _edge2 = edge2;

        addOutEdge(edge1);
        addOutEdge(edge2);
    }

    void execute(void)
    {
        std::cout << "Test01MyNode1::execute()" <<std::endl;

        Node::execute();

        _edge1->setValue(3);
        _edge2->setValue('B');
    }
};

//-----------------------------------------------------------
class Test01MyNode2 : public Node
{
    Edge* _edge1;
    Edge* _edge2;

public:
    void setEdge(Edge* edge1, Edge* edge2)
    {
        _edge1 = edge1;
        _edge2 = edge2;

        addInEdge(edge1);
        addInEdge(edge2);
    }

    void execute(void)
    {
        std::cout << "Test01MyNode2::execute()" <<std::endl;

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
void test01(void)
{
    Test01MyNode1* n1(new Test01MyNode1);
    Test01MyNode2* n2(new Test01MyNode2);
    Edge* edge1(new Edge(0));
    Edge* edge2(new Edge('A'));

    n1->setEdge(edge1, edge2);
    n2->setEdge(edge1, edge2);

    edge1->addOutNode(n2);
    edge2->addOutNode(n2);

    std::vector<Node*> nodes = {n1, n2};
    std::vector<Edge*> edges = {edge1, edge2};

    Executor exe(n1, nodes, edges);
    exe.step();

}


#endif
