// test_04.hpp
//
// Notノード評価
//  NodeS ---> Not ---> NodeE
// Orノード評価
//  NodeS ---> And ---> NodeE
// Andノード評価
//  NodeS ---> And ---> NodeE

#ifndef __TEST_04_HPP__
#define __TEST_04_HPP__

#include <string>
#include <memory>
#include <vector>

#include "Edge.hpp"
#include "Node.hpp"
#include "Executor.hpp"
#include "NodeLogics.hpp"

#include "NodeTestBase.hpp"


//-----------------------------------------------------------
// 0入力、1出力ノード
class Test04NodeS : public Node
{
    bool _value;

public:

    //-------------------------------------------------------
    void setValue(bool value)
    {
        _value = value;
    }

    //-------------------------------------------------------
    void setEdge(Edge* edge)
    {
        _outEdges.resize(1);
        _outEdges.at(0) = edge;

    }

    //-------------------------------------------------------
    void execute(void)
    {
        Node::execute();

        std::cout << "Test04NodeS::execute()" <<std::endl;
        std::cout << "input value is : " << _value << std::endl;
        _outEdges.at(0)->setValue(_value);
    }
};

//-----------------------------------------------------------
// １入力、０出力ノード
class Test04NodeE : public Node
{
public:
    //-------------------------------------------------------
    void setEdge(Edge* edge)
    {
        _inEdges.resize(1);
        _inEdges.at(0) = edge;
    }

    //-------------------------------------------------------
    void execute(void)
    {
        Node::execute();

        std::cout << "Test04NodeE::execute()" <<std::endl;
        bool a;
        a = std::any_cast<bool>(_inEdges.at(0)->getValue());
        std::cout << "flowed value is : " << a << std::endl;
    }
};



//-----------------------------------------------------------
// NOTテスト
void test_04_01(void)
{
    // ノード生成
    NodeNot* nodeNot(new NodeNot);
    Test04NodeS* n1(new Test04NodeS);
    Test04NodeE* n2(new Test04NodeE);
    std::vector<Node*> nodes = {n1, n2, nodeNot};

    // エッジ生成
    Edge* e0(new Edge(true));
    Edge* e1(new Edge(true));
    std::vector<Edge*> edges = {e0, e1};

    // ノードにエッジを紐付ける
    std::vector<Edge*> inEdges = {e0};
    std::vector<Edge*> outEdges = {e1};
    nodeNot->addInEdges(inEdges);
    nodeNot->addOutEdges(outEdges);
    n1->setEdge(e0);
    n2->setEdge(e1);

    // エッジにノードを紐付ける
    e0->addOutNode(nodeNot);
    e0->addOutNode(n2);

    // 実行
    Executor exe(n1, nodes, edges);

    n1->setValue(false);
    exe.step();

    n1->setValue(true);
    exe.step();

}



//-----------------------------------------------------------
template<typename T>
void testOp2to1(void)
{
    std::tuple<Executor*, NodeTestEntry<bool>*> ret = test_2to1<T, bool>();
    auto exe = std::get<0>(ret);
    auto nEntry = std::get<1>(ret);

    nEntry->setValues(std::vector<bool>{false, false});
    exe->step();

    nEntry->setValues(std::vector<bool>{false, true});
    exe->step();

    nEntry->setValues(std::vector<bool>{true, false});
    exe->step();

    nEntry->setValues(std::vector<bool>{true, true});
    exe->step();

}

void test04_01(void)
{
    testOp2to1<NodeAnd>();
}

//-----------------------------------------------------------
void test04(void)
{
    std::cout << "-- test NOT -- " << std::endl;
    test_04_01();

    std::cout << "-- test AND -- " << std::endl;
    testOp2to1<NodeAnd>();

    std::cout << "-- test OR -- " << std::endl;
    testOp2to1<NodeOr>();

    std::cout << "-- test NOR -- " << std::endl;
    testOp2to1<NodeNor>();

    std::cout << "-- test NAND -- " << std::endl;
    testOp2to1<NodeNand>();

}


#endif
