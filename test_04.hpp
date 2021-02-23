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
// ０入力、２出力ノード
class Test04NodeS2 : public Node
{
    bool _value1;
    bool _value2;

public:

    //-------------------------------------------------------
    void setValue(bool value1, bool value2)
    {
        _value1 = value1;
        _value2 = value2;
    }

    //-------------------------------------------------------
    void setEdge(Edge* edge1, Edge* edge2)
    {
        _outEdges.resize(2);
        _outEdges.at(0) = edge1;
        _outEdges.at(1) = edge2;

    }

    //-------------------------------------------------------
    void execute(void)
    {
        Node::execute();

        std::cout << "Test04NodeS2::execute()" <<std::endl;
        std::cout << "input value is : " << _value1 << " , " << _value2 << std::endl;
        _outEdges.at(0)->setValue(_value1);
        _outEdges.at(1)->setValue(_value2);
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
// 2入力１入つ力のノードのテストをするベース関数
// テンプレートとして試験対象ノードを設定する。
template <class T>
void test_2in1out(void)
{
    // ノード生成
    T* nodeOp(new T);
    Test04NodeS2* n1(new Test04NodeS2);
    Test04NodeE* n2(new Test04NodeE);
    std::vector<Node*> nodes = {n1, n2, nodeOp};

    // エッジ生成
    Edge* e0(new Edge(true));
    Edge* e1(new Edge(true));
    Edge* e2(new Edge(true));
    std::vector<Edge*> edges = {e0, e1, e2};

    // ノードにエッジを紐付ける
    std::vector<Edge*> inEdges = {e0, e1};
    nodeOp->addInEdges(inEdges);

    std::vector<Edge*> outEdges = {e2};
    nodeOp->addOutEdges(outEdges);

    n1->setEdge(e0, e1);
    n2->setEdge(e2);

    // エッジにノードを紐付ける
    e0->addOutNode(nodeOp);
    e1->addOutNode(nodeOp);
    e2->addOutNode(n2);

    // 実行
    Executor exe(n1, nodes, edges);

    n1->setValue(false, false);
    exe.step();

    n1->setValue(false, true);
    exe.step();

    n1->setValue(true, false);
    exe.step();

    n1->setValue(true, true);
    exe.step();

}
//-----------------------------------------------------------
void test04(void)
{
    test_04_01();
    test_2in1out<NodeAnd>();
    test_2in1out<NodeOr>();

    test_2in1out<NodeNor>();

}


#endif
