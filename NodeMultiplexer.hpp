// NodeMultiplexer.hpp
#ifndef __NODEMULTIPLEXER_HPP__
#define __NODEMULTIPLEXER_HPP__

#include "GraphBuilder.hpp"
#include "NodeLogics.hpp"


//-----------------------------------------------------------
// Multiplexer
// In-port  : sel, A, B
// Out0port : Y
class NodeMultiplexer : public NodeComplex
{
public:
    //-------------------------------------------------------
    NodeMultiplexer(void)
    {
        auto& gb = getGraphBuilder();
        auto enty = getEntryNode();
        auto exit = getExitNode();
        
        auto not1 = gb.createNode<NodeNot>();
        auto and1  = gb.createNode<NodeAnd>();
        auto and2  = gb.createNode<NodeAnd>();
        auto or1  = gb.createNode<NodeOr>();

        gb.outto(Port(not1, 1), Ports{Port(and1, 2)}, typeid(bool));
        gb.outto(Port(and1, 1), Ports{Port(or1, 1)}, typeid(bool));
        gb.outto(Port(and2, 1), Ports{Port(or1, 2)}, typeid(bool));

        gb.outto(Port(enty, 1), Ports{ Port(not1, 1), Port(and2, 2)}, typeid(bool));
        gb.outto(Port(enty, 2), Ports{ Port(and1, 1) }, typeid(bool));
        gb.outto(Port(enty, 3), Ports{ Port(and2, 1) }, typeid(bool));

        gb.outto(Port(or1, 1), Ports{ Port(exit, 1) }, typeid(bool));

       commit();
    }
};

//-----------------------------------------------------------
// DeMultiplexer
// In-port  : sel, A
// Out0port : Y1, Y2
class NodeDeMultiplexer : public NodeComplex
{
public:
    //-------------------------------------------------------
    NodeDeMultiplexer(void)
    {
        auto& gb = getGraphBuilder();
        auto enty = getEntryNode();
        auto exit = getExitNode();

        auto not1 = gb.createNode<NodeNot>();
        auto and1  = gb.createNode<NodeAnd>();
        auto and2  = gb.createNode<NodeAnd>();

        gb.outto(Port(not1, 1), Ports{Port(and1, 2)}, typeid(bool));

        gb.outto(Port(enty, 1), Ports{ Port(not1, 1), Port(and2, 2) }, typeid(bool));
        gb.outto(Port(enty, 2), Ports{ Port(and1, 1), Port(and2, 1) }, typeid(bool));
        gb.outto(Port(and1, 1), Ports{ Port(exit, 1) }, typeid(bool));
        gb.outto(Port(and2, 1), Ports{ Port(exit, 2) }, typeid(bool));

        commit();

    }
};


#endif

