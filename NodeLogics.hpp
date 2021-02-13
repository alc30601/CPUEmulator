// NodeLogics.hpp

#ifndef __NODELOGICS_HPP__
#define __NODELOGICS_HPP__

#include <iostream>
#include <memory>

#include "NodeBases.hpp"


//-----------------------------------------------------------
// NOT
// 1入力1出力ノード
// データ型はともにbool
class NodeNot : public Node1In1Out<bool>
{
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
    bool calculate(bool p1, bool p2)
    {
        bool result = p1 | p2;
        return result;
    }
};

//-----------------------------------------------------------
// ★★★
// NORをORとNOTの組合せで構成する。
// ノードの結合を行う。
// 複合ノードは外部から見れば１つのノードとして振る舞う。
// 内部的に複数のノードを組合せる。
// 実行は外部のExecutorとは別に内部にExecutorを持ったほうが良いかもしれない。
class NodeNor : public Node
{
    std::shared_ptr<NodeOr> _nodeOr;
    std::shared_ptr<NodeNot> _nodeNot;
    std::shared_ptr<Edge> _edge;

public:
    NodeNor(void)
    {
        _nodeOr = std::shared_ptr<NodeOr>(new NodeOr());
        _nodeNot = std::shared_ptr<NodeNot>(new NodeNot());
        _edge = std::shared_ptr<Edge>(new Edge(true));


    }
};

#endif


