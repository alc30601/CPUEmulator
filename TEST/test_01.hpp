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

#include <cassert>
#include <tuple>

#include "Edge.hpp"
#include "Node.hpp"
#include "Executor.hpp"
#include "GraphBuilder.hpp"


//-----------------------------------------------------------
class Test01MyNode1 : public Node
{
    int _a;
    char _b;

public:

    //-------------------------------------------------------
    void execute(void)
    {
        Node::execute();

        auto outEdges = getOutEdges();
        outEdges[0]->setValue(_a);
        outEdges[1]->setValue(_b);
    }

    //-------------------------------------------------------
    void setValue(int a, char b)
    {
        _a = a;
        _b = b;
    }
};

//-----------------------------------------------------------
class Test01MyNode2 : public Node
{
    int _a;
    char _b;

public:

    //-------------------------------------------------------
    void execute(void)
    {
        Node::execute();

        auto inEdges = getInEdges();
        _a = std::any_cast<int>(inEdges[0]->getValue());
        _b = std::any_cast<char>(inEdges[1]->getValue());
        std::cout << "flowed value 1 is : " << _a << std::endl;
        std::cout << "flowed value 2 is : " << _b << std::endl;
    }

    //-------------------------------------------------------
    std::tuple<int, char> getValue(void)
    {
        auto ret = std::make_tuple(_a, _b);
        return ret;
    }

};

//-----------------------------------------------------------
void test01(void)
{
    GraphBuilder gb;
    auto n0 = gb.createNode<Test01MyNode1>();
    auto n1 = gb.createNode<Test01MyNode2>();
    gb.outto(Port(n0, 1), Port(n1,1));
    gb.outto(Port(n0, 2), Port(n1,2));

    auto nodes = gb.getNodes();
    auto edges = gb.getEdges();
    auto nEntry = n0.getNode();
    // Executor* exe(new Executor(nEntry, nodes, edges));
    Executor* exe = getExecutor(nEntry, nodes, edges);

    auto node_n0 = static_cast<Test01MyNode1*>(n0.getNode());
    auto node_n1 = static_cast<Test01MyNode2*>(n1.getNode());

    int a0, a1;
    char b0, b1;
    std::tuple<int, char> ret;

    a0 = 3;
    b0 = 'B';
    node_n0->setValue(a0, b0);
    exe->step();
    ret = node_n1->getValue();
    a1 = std::get<0>(ret);
    b1 = std::get<1>(ret);
    assert(a0 == a1);
    assert(b0 == b1);

    a0 = 4;
    b0 = 'C';
    node_n0->setValue(a0, b0);
    exe->step();
    ret = node_n1->getValue();
    a1 = std::get<0>(ret);
    b1 = std::get<1>(ret);
    assert(a0 == a1);
    assert(b0 == b1);
}


#endif
