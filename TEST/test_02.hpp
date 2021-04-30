// test_02.hpp
// 入力ノード２つと出力ノード１つ
// 各入力ノードから1本ずつエッジが出力ノードに
// トリガーをかけるために開始ノードを設ける(MyNode0)
//
//               e01    +---------+
// +--------+  +----->  | MyNode1 |---+            +---------+
// |MyNode0 |--+        +---------+   +--- e13 --->|         |
// |        |--+                                   | MyNode3 |
// +--------+  +----->  +---------+   +--- e23 --->|         |
//               e02    | MyNode2 |---+            +---------+
//                      +---------+


#ifndef __TEST_02_HPP__
#define __TEST_02_HPP__

#include <vector>

#include "Edge.hpp"
#include "Node.hpp"
#include "Executor.hpp"
#include "GraphBuilder.hpp"


//-----------------------------------------------------------
class Test02MyNode0 : public Node
{
    std::string _a;
    double _b;

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
    void setValue(std::string a, double b)
    {
        _a = a;
        _b = b;
    }
};

//-----------------------------------------------------------
class Test02MyNode1 : public Node
{
public:
    //-------------------------------------------------------
    void execute(void)
    {
        Node::execute();

        auto inEdges = getInEdges();
        auto outEdges = getOutEdges();
        auto value = inEdges[0]->getValue();
        outEdges[0]->setValue(value);
    }
};

//-----------------------------------------------------------
class Test02MyNode2 : public Node
{
public:
    //-------------------------------------------------------
    void execute(void)
    {
        Node::execute();

        auto inEdges = getInEdges();
        auto outEdges = getOutEdges();
        auto value = inEdges[0]->getValue();
        outEdges[0]->setValue(value);
    }
};

//-----------------------------------------------------------
class Test02MyNode3 : public Node
{
    std::string _a;
    double _b;

public:
    //-------------------------------------------------------
    void execute(void)
    {
        Node::execute();

        auto inEdges = getInEdges();
        _a = std::any_cast<std::string>(inEdges[0]->getValue());
        _b = std::any_cast<double>(inEdges[1]->getValue());
        std::cout << "flowed value 1 is : " << _a << std::endl;
        std::cout << "flowed value 2 is : " << _b << std::endl;
    }

    //-------------------------------------------------------
    std::tuple<std::string, double> getValue(void)
    {
        auto ret = std::make_tuple(_a, _b);
        return ret;
    }
};


//-----------------------------------------------------------
void test02(void)
{
    GraphBuilder gb;
    auto n0 = gb.createNode<Test02MyNode0>();
    auto n1 = gb.createNode<Test02MyNode1>();
    auto n2 = gb.createNode<Test02MyNode2>();
    auto n3 = gb.createNode<Test02MyNode3>();

    gb.outto(Port(n0, 1), Port(n1, 1));
    gb.outto(Port(n0, 2), Port(n2, 1));
    gb.outto(Port(n1, 1), Port(n3, 1));
    gb.outto(Port(n2, 1), Port(n3, 2));

    auto nodes = gb.getNodes();
    auto edges = gb.getEdges();
    auto nEntry = n0.getNode();
    // Executor* exe(new Executor(nEntry, nodes, edges));
    Executor* exe = getExecutor(nEntry, nodes, edges);

    auto node_n0 = static_cast<Test02MyNode0*>(n0.getNode());
    auto node_n3 = static_cast<Test02MyNode3*>(n3.getNode());

    std::string a0, a1;
    double b0, b1;
    std::tuple<std::string, double> ret;

    a0 = "Hello";
    b0 = 3.141592;
    node_n0->setValue(a0, b0);
    exe->step();
    ret = node_n3->getValue();
    a1 = std::get<0>(ret);
    b1 = std::get<1>(ret);
    assert(a0 == a1);
    assert(b0 == b1);

    a0 = "Bye";
    b0 = 1.414;
    node_n0->setValue(a0, b0);
    exe->step();
    ret = node_n3->getValue();
    a1 = std::get<0>(ret);
    b1 = std::get<1>(ret);
    assert(a0 == a1);
    assert(b0 == b1);

}




#endif
