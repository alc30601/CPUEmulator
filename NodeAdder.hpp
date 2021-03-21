// NodeAdder.hpp
#ifndef __NODEADDER_HPP__
#define __NODEADDER_HPP__

#include "GraphBuilder.hpp"
#include "NodeLogics.hpp"

//-----------------------------------------------------------
// half Adder
class NodeHalfAdder : public NodeComplex
{
public:
    //-------------------------------------------------------
    NodeHalfAdder(void)
    {
        auto& gb = getGraphBuilder();
        auto exor1 = gb.createNode<NodeExor>();
        auto and1  = gb.createNode<NodeAnd>();

        setInPortss(Ports{Port(exor1, 1), Port(and1, 1)}, 
                    Ports{Port(exor1, 2), Port(and1, 2)});
        setOutPorts(Port(exor1, 1), 
                    Port(and1, 1));

        commit();
    }
};

//-----------------------------------------------------------
// Full Adder
class NodeFullAdder : public NodeComplex
{
public:
    //-------------------------------------------------------
    NodeFullAdder(void)
    {
        auto& gb = getGraphBuilder();
        auto half1 = gb.createNode<NodeHalfAdder>();
        auto half2 = gb.createNode<NodeHalfAdder>();
        auto or1   = gb.createNode<NodeOr>();

        gb.outto(Port(half1, 1), Ports{ Port(half2, 1) });
        gb.outto(Port(half1, 2), Ports{ Port(or1,   2) });
        gb.outto(Port(half2, 2), Ports{ Port(or1,   1) });

        setInPortss(Ports{Port(half1, 1)}, 
                    Ports{Port(half1, 2)},
                    Ports{Port(half2, 2)});
        setOutPorts(Port(half2, 1), 
                    Port(or1, 1));

        commit();
    }
};

#endif


