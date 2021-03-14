// NodeLogics.hpp

#ifndef __NODELOGICS_HPP__
#define __NODELOGICS_HPP__

#include <iostream>
#include <memory>

#include "NodeBase.hpp"
#include "GraphBuilder.hpp"


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


//-----------------------------------------------------------
// NOR
// 2入力1出力ノード
// データ型はともにbool
#define NodeNor Node21_11<NodeOr, NodeNot>

//-----------------------------------------------------------
// NAND
// 2入力1出力ノード
// データ型はともにbool
#define NodeNand Node21_11<NodeAnd, NodeNot>


//-----------------------------------------------------------
// Exclusive OR
class NodeExor : public NodeComplex
{
public:
    NodeExor(void)
    {
        auto& gb = getGraphBuilder();
        auto not1 = gb.createNode<NodeNot>();
        auto not2 = gb.createNode<NodeNot>();
        auto and1 = gb.createNode<NodeAnd>();
        auto and2 = gb.createNode<NodeAnd>();
        auto or1 = gb.createNode<NodeOr>();
        
        gb.outto(Port(not1, 1), Port(and1, 1));
        gb.outto(Port(not2, 1), Port(and2, 1));
        gb.outto(Port(and1, 1), Port(or1, 1));
        gb.outto(Port(and2, 1), Port(or1, 2));

    }
};


#endif


