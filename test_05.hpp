// test_05.hpp
// モジュール化ノードの評価(NodeSub)
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


//-----------------------------------------------------------
class NodeS : public Node
{
    bool _value1;
    bool _value2;

public:

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
        std::cout << "input value is : " << _value1 << " , " << _value2 << std::endl;
    }
};


//-----------------------------------------------------------
// 複合ノード内の内部ノード
class NodeInternal : public Node
{

};

//-----------------------------------------------------------
// 複合ノード
class NodeComplex : public NodeSub
{
    NodeInternal* _internalNode;

public:
    //-------------------------------------------------------
    NodeComplex(void)
    {

    }


};


//-----------------------------------------------------------
void test(void)
{
    // ノード生成
    NodeS* n1(new NodeS);
    NodeE* n2(new NodeE);
    NodeComplex* nc(new NodeComplex);

    // エッジ生成
    Edge* e11(new Edge(true));
    Edge* e12(new Edge(true));
    Edge* e21(new Edge(true));
    Edge* e22(new Edge(true));

    // ノードにエッジを紐付ける。
    n1->setEdge(e11, e12);
    n2->setEdge(e21, e22);

    std::vector<Edge*> inEdges = {e11, e12};
    std::vector<Edge*> outEdges = {e21, e22};
    nc->setEdge(inEdges, outEdges);

    // エッジにノードを紐付ける。
    e11->addOutNode(nc);
    e12->addOutNode(nc);
    e21->addOutNode(n2);
    e22->addOutNode(n2);

    // 実行
    std::vector<Node*> nodes = {n1, n2, nc};
    std::vector<Edge*> edges = {e11, e12, e21, e22};

    Executor exe(n1, nodes, edges);
    exe.step();

}

#endif


