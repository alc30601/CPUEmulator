// NodeLogics.hpp

#ifndef __NODELOGICS_HPP__
#define __NODELOGICS_HPP__

#include <iostream>
#include <memory>

#include "Node.hpp"
#include "NodeBase.hpp"


//-----------------------------------------------------------
// NOT
// 1入力1出力ノード
// データ型はともにbool
class NodeNot : public Node1In1Out<bool>
{
    //-------------------------------------------------------
    bool calculate(bool p)
    {
        bool result = !p;
        return result;
    }
};


//-----------------------------------------------------------
// AND
// 2入力1出力ノード
// データ型はともにbool
class NodeAnd : public Node2In1Out<bool>
{
    //-------------------------------------------------------
    bool calculate(bool p1, bool p2)
    {
        bool result = p1 & p2;
        return result;
    }
};

//-----------------------------------------------------------
// OR
// 2入力1出力ノード
// データ型はともにbool
class NodeOr : public Node2In1Out<bool>
{
    //-------------------------------------------------------
    bool calculate(bool p1, bool p2)
    {
        bool result = p1 | p2;
        return result;
    }
};

// //-----------------------------------------------------------
// // ★★★
// // NORをORとNOTの組合せで構成する。
// // ノードの結合を行う。
// // 複合ノードは外部から見れば１つのノードとして振る舞う。
// // 内部的に複数のノードを組合せる。
// // 実行は外部のExecutorとは別に内部にExecutorを持ったほうが良いかもしれない。
// class NodeNor : public Node
// {
//     std::shared_ptr<NodeOr> _nodeOr;
//     std::shared_ptr<NodeNot> _nodeNot;
//     std::shared_ptr<Edge> _edge;

// public:

//     //-------------------------------------------------------
//     // 本来、ここで生成した内部ノードにエッジを紐付けたいが
//     // まだ、本ノード生成時点で外部エッジを指定されていないためできない。
//     // setEdge()が呼ばれた時点でまとめて紐付けを行う。
//     NodeNor(void)
//     {
//         // ノード生成
//         _nodeOr = std::shared_ptr<NodeOr>(new NodeOr());
//         _nodeNot = std::shared_ptr<NodeNot>(new NodeNot());

//         // エッジ生成
//         _edge = std::shared_ptr<Edge>(new Edge(true));

//         // エッジにノードを紐付ける
//         _edge->addOutNode(_nodeNot);
//     }

//     //-------------------------------------------------------
//     // 外部エッジの紐付けは本ノードの担当外。
//     // 自身の持つノードに対するエッジの紐付けのみを行う。
//     void setEdge(std::shared_ptr<Edge> inEdge1, std::shared_ptr<Edge> inEdge2, std::shared_ptr<Edge> outEdge)
//     {
//         _inEdges.resize(2);
//         _inEdges.at(0) = inEdge1;
//         _inEdges.at(1) = inEdge2;

//         _outEdges.resize(1);
//         _outEdges.at(0) = outEdge;

//         // ノードにエッジを紐付ける
//         _nodeOr->setEdge(inEdge1, inEdge2, _edge);
//         _nodeNot->setEdge(_edge, outEdge);
//     }

//     //-------------------------------------------------------
//     void execute(void)
//     {
//         Node::execute();


//     }
// };

#endif


