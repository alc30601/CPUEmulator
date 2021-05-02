// NodeFlipFlop.hpp
#ifndef __NODEFLIPFLOP_HPP__
#define __NODEFLIPFLOP_HPP__

#include "Node.hpp"
#include "NodeComplex.hpp"
#include "NodeLogics.hpp"
#include "NodeValue.hpp"


//-----------------------------------------------------------
// NOR型SRフリップフロップ
// 最初は内部状態(val1, val2の値)は不定と想定すべき。
// 即ち、使い始めに一発リセットを打ち込む必要がある。
// In-Ports  : R, S
// Out-Ports : Q, Q-inv
class NodeSRFlipFlopNor : public NodeComplex
{
public:
    //-------------------------------------------------------
    NodeSRFlipFlopNor(void)
    {
        auto& gb = getGraphBuilder();

        auto enty = getEntryNode();
        auto exit = getExitNode();
        auto nor1 = gb.createNode<NodeNor>("nor1");
        auto nor2 = gb.createNode<NodeNor>("nor2");

        gb.outto(Port(enty, 1), Ports{ Port(nor1, 1) }, typeid(bool));                // R
        gb.outto(Port(enty, 2), Ports{ Port(nor2, 1) }, typeid(bool));                // S
        gb.outto(Port(nor1, 1), Ports{ Port(nor2, 2), Port(exit, 1) }, typeid(bool)); // Q
        gb.outto(Port(nor2, 1), Ports{ Port(nor1, 2), Port(exit, 2) }, typeid(bool)); // Q-inv.

        commit();
    }

};


//-----------------------------------------------------------
// NAND型SRフリップフロップ
// In-Ports  : S-inv, R-inv
// Out-Ports : Q, Q-inv
class NodeSRFlipFlopNand : public NodeComplex
{
public:
    //-------------------------------------------------------
    NodeSRFlipFlopNand(void)
    {
        auto& gb = getGraphBuilder();

        auto enty = getEntryNode();
        auto exit = getExitNode();
        auto nand1 = gb.createNode<NodeNand>("nand1");
        auto nand2 = gb.createNode<NodeNand>("nand2");

        gb.outto(Port(enty, 1), Ports{ Port(nand1, 1) }, typeid(bool));                 // R
        gb.outto(Port(enty, 2), Ports{ Port(nand2, 1) }, typeid(bool));                 // S
        gb.outto(Port(nand1, 1), Ports{ Port(nand2, 2), Port(exit, 1) }, typeid(bool)); // Q
        gb.outto(Port(nand2, 1), Ports{ Port(nand1, 2), Port(exit, 2) }, typeid(bool)); // Q-inv.

        commit();
    }

};


#endif


