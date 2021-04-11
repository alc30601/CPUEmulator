// NodeFlipFlop.hpp
#ifndef __NODEFLIPFLOP_HPP__
#define __NODEFLIPFLOP_HPP__

#include "Node.hpp"
#include "NodeComplex.hpp"
#include "NodeLogics.hpp"
#include "NodeValue.hpp"


//-----------------------------------------------------------
// NOR型SRフリップフロップ
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
        auto val1 = gb.createNode<NodeValue<bool>>("val1");
        auto val2 = gb.createNode<NodeValue<bool>>("val2");

        gb.outto(Port(enty, 1), Ports{ Port(nor1, 1) });
        gb.outto(Port(enty, 2), Ports{ Port(nor2, 1) });

        gb.outto(Port(nor1, 1), Ports{ Port(val1, 1), Port(exit, 1) });
        gb.outto(Port(nor2, 1), Ports{ Port(val2, 1), Port(exit, 2) });
        gb.outto(Port(val1, 1), Ports{ Port(nor2, 2) });
        gb.outto(Port(val2, 1), Ports{ Port(nor1, 2) });

        commit();
    }
};

#endif


