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
#include <cassert>
#include <tuple>

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
void testOp2to1(std::vector<std::vector<bool>>& input, std::vector<bool>& expected)
{
    auto ret = test_2to1_template<T, bool>();
    auto exe = std::get<0>(ret);
    auto nEntry = static_cast<NodeTestEntry<bool>*>(std::get<1>(ret).getNode());
    auto nExit = static_cast<NodeTestExit<bool>*>(std::get<2>(ret).getNode());

    for(int i=0;i<input.size();i++){
        nEntry->setValues(input[i]);
        exe->step();
        auto values = nExit->getValues();
        assert(expected[i] == values[0]);
    }
}


//-----------------------------------------------------------
void test04(void)
{
    std::cout << "-- test NOT -- " << std::endl;
    test_04_01();

    std::vector<std::vector<bool>> testVector{{false,false},{false,true}, {true,false},{true,true}};

    std::cout << "-- test AND -- " << std::endl;
    std::vector<bool> expectedAND{false, false, false, true};
    testOp2to1<NodeAnd>(testVector, expectedAND);

    std::cout << "-- test OR -- " << std::endl;
    std::vector<bool> expectedOR{false, true, true, true};
    testOp2to1<NodeOr>(testVector, expectedOR);

    std::cout << "-- test NOR -- " << std::endl;
    std::vector<bool> expectedNOR{true, false, false, false};
    testOp2to1<NodeNor>(testVector, expectedNOR);

    std::cout << "-- test NAND -- " << std::endl;
    std::vector<bool> expectedNAND{true, true, true, false};
    testOp2to1<NodeNand>(testVector, expectedNAND);

    std::cout << "-- test EXOR -- " << std::endl;
    std::vector<bool> expectedEXOR{false, true, true, false};
    testOp2to1<NodeExor>(testVector, expectedEXOR);

}


#endif
