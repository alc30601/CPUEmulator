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

//-----------------------------------------------------------
// 4 bit Multiplexer
//            1    2   3   4   5   6   7   8   9
// In-port  : sel, A0, A1, A2, A3, B0, B1, B2, B3
//            1   2   3   4
// Out0port : Y0, Y1, Y2, Y3
class Node4bitMultiplexer : public NodeComplex
{
public:
    //-------------------------------------------------------
    Node4bitMultiplexer(void)
    {
        auto& gb = getGraphBuilder();
        auto enty = getEntryNode();
        auto exit = getExitNode();

        auto mux0 = gb.createNode<NodeMultiplexer>("MUX0");
        auto mux1 = gb.createNode<NodeMultiplexer>("MUX1");
        auto mux2 = gb.createNode<NodeMultiplexer>("MUX2");
        auto mux3 = gb.createNode<NodeMultiplexer>("MUX3");

        gb.outto(Port(enty, 1), Ports{ Port(mux0, 1), Port(mux1, 1), Port(mux2, 1), Port(mux3, 1) }, typeid(bool)); // sel

        gb.outto(Port(enty, 2), Ports{ Port(mux0, 2) }, typeid(bool)); // A0
        gb.outto(Port(enty, 3), Ports{ Port(mux1, 2) }, typeid(bool)); // A1
        gb.outto(Port(enty, 4), Ports{ Port(mux2, 2) }, typeid(bool)); // A2
        gb.outto(Port(enty, 5), Ports{ Port(mux3, 2) }, typeid(bool)); // A3

        gb.outto(Port(enty, 6), Ports{ Port(mux0, 3) }, typeid(bool)); // B0
        gb.outto(Port(enty, 7), Ports{ Port(mux1, 3) }, typeid(bool)); // B1
        gb.outto(Port(enty, 8), Ports{ Port(mux2, 3) }, typeid(bool)); // B2
        gb.outto(Port(enty, 9), Ports{ Port(mux3, 3) }, typeid(bool)); // B3

        gb.outto(Port(mux0, 1), Ports{ Port(exit, 1) }, typeid(bool)); // Y0
        gb.outto(Port(mux1, 1), Ports{ Port(exit, 2) }, typeid(bool)); // Y1
        gb.outto(Port(mux2, 1), Ports{ Port(exit, 3) }, typeid(bool)); // Y2
        gb.outto(Port(mux3, 1), Ports{ Port(exit, 4) }, typeid(bool)); // Y3

        commit();
    }
};


#endif

