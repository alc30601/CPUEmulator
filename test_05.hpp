// test_05.hpp
// モジュール化ノードの評価(NodeSubSystem)
// 
// +-------+      +--------------+      +-------+
// |       |      | NodeComplex  |      |       |
// |       |      |   +------+   |      |       |
// |       |----->|-->|      |-->|----->|       |
// | NodeS |----->|-->|      |-->|----->| NodeE |
// |       |      |   +------+   |      |       |
// |       |      | NodeInternal |      |       |
// +-------+      +--------------+      +-------+


#ifndef __TEST_05_HPP__
#define __TEST_05_HPP__

#include <iostream>
#include <any>

#include "Node.hpp"
#include "Edge.hpp"
#include "Executor.hpp"
#include "NodeSubSystem.hpp"


//-----------------------------------------------------------
class NodeS : public Node
{
    bool _value1;
    bool _value2;

public:

    //-------------------------------------------------------
    void setValues(bool v1, bool v2)
    {
        _value1 = v1;
        _value2 = v2;
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

        std::cout << "NodeS::execute()" << std::endl;
        std::cout << "input value is : " << _value1 << " , " << _value2 << std::endl;

        _outEdges.at(0)->setValue(_value1);
        _outEdges.at(1)->setValue(_value2);
    }
};


//-----------------------------------------------------------
class NodeE : public Node
{
    bool _value1;
    bool _value2;

public:
    //-------------------------------------------------------
    void setEdge(Edge* edge1, Edge* edge2)
    {
        _inEdges.resize(2);
        _inEdges.at(0) = edge1;
        _inEdges.at(1) = edge2;
    }

    //-------------------------------------------------------
    void execute(void)
    {
        Node::execute();

        _value1 = std::any_cast<bool>(_inEdges.at(0)->getValue());
        _value2 = std::any_cast<bool>(_inEdges.at(1)->getValue());
        std::cout << "NodeE::execute()" <<std::endl;
        std::cout << "output value is : " << _value1 << " , " << _value2 << std::endl;
    }
};


//-----------------------------------------------------------
// 複合ノード内の内部ノード
class NodeInternal : public Node
{
public:
    //-------------------------------------------------------
    void execute(void)
    {
        Node::execute();

        // 入力エッジの値をそのまま出力エッジに出力する。
        bool value1 = std::any_cast<bool>(_inEdges.at(0)->getValue());
        bool value2 = std::any_cast<bool>(_inEdges.at(1)->getValue());
        _outEdges.at(0)->setValue(value1);
        _outEdges.at(1)->setValue(value2);

        std::cout << "NodeInternal::execute()" << std::endl;
        std::cout << "the value is : " << value1 << " , " << value2 << std::endl;
    }    
};

//-----------------------------------------------------------
// 複合ノード
// ２入力、２出力、内部ノード１
class NodeComplex : public NodeSubSystem
{
    NodeInternal* _internalNode;

public:
    //-------------------------------------------------------
    NodeComplex(void)
    {
        _internalNode = new NodeInternal();
    }

    //-------------------------------------------------------
    void construct(void)
    {
        // 複合ノードの入口/出口ノードの内側のエッジを取得
        std::vector<Edge*> inEdges = getInDualEdges();
        std::vector<Edge*> outEdges = getOutDualEdges();

        // 双対エッジを内部のノードに紐付ける。
        _internalNode->addInEdges(inEdges);
        _internalNode->addOutEdges(outEdges);

        // 複合ノードの入り口ノードの内側双対エッジに内部ノードを紐付ける
        for(auto edge : inEdges){
            edge->addOutNode(_internalNode);
        }

    }

};


//-----------------------------------------------------------
void test(void)
{
    // ノード生成
    NodeS* nS(new NodeS);
    NodeE* nE(new NodeE);
    NodeComplex* nC(new NodeComplex);

    // エッジ生成
    Edge* e11(new Edge(true));
    Edge* e12(new Edge(true));
    Edge* e21(new Edge(true));
    Edge* e22(new Edge(true));

    // ノードにエッジを紐付ける。
    nS->setEdge(e11, e12);
    nE->setEdge(e21, e22);

    std::vector<Edge*> inEdges = {e11, e12};
    std::vector<Edge*> outEdges = {e21, e22};
    nC->setEdge(inEdges, outEdges);

    // エッジにノードを紐付ける。
    e11->addOutNode(nC);
    e12->addOutNode(nC);
    e21->addOutNode(nE);
    e22->addOutNode(nE);

    // 複合ノードの内部結線を設定する。
    nC->construct();

    // 実行
    std::vector<Node*> nodes = {nS, nE, nC};
    std::vector<Edge*> edges = {e11, e12, e21, e22};

    Executor exe(nS, nodes, edges);

    nS->setValues(false, false);
    exe.step();

    nS->setValues(false, true);
    exe.step();

    nS->setValues(true, false);
    exe.step();

    nS->setValues(true, true);
    exe.step();

}

#endif


