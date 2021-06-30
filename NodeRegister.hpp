// NodeRegister.hpp
// 4bits register
// 4bits Asynchronous Counter
// 4bits Synchronous Counter
// 4bits shift register

#ifndef __NODEREGISTER_HPP__
#define __NODEREGISTER_HPP__

#include "NodeComplex.hpp"
#include "NodeFlipFlop.hpp"


//-----------------------------------------------------------
// 4 bit register
// In-Ports  : RST, CK, D0, D1, D2, D3
// Out-Ports : Q0, Q1, Q2, Q3
class Node4bitRegister : public NodeComplex
{
public:
    //-------------------------------------------------------
    Node4bitRegister(void)
    {
        auto& gb = getGraphBuilder();
        auto enty  = getEntryNode();
        auto exit  = getExitNode();

        auto dff0 = gb.createNode<NodeDFlipFlopEdgeTriggerAsyncReset>("D-FF0 in 4bit Register");
        auto dff1 = gb.createNode<NodeDFlipFlopEdgeTriggerAsyncReset>("D-FF1 in 4bit Register");
        auto dff2 = gb.createNode<NodeDFlipFlopEdgeTriggerAsyncReset>("D-FF2 in 4bit Register");
        auto dff3 = gb.createNode<NodeDFlipFlopEdgeTriggerAsyncReset>("D-FF3 in 4bit Register");


        gb.outto(Port(enty, 1), Ports{ Port(dff0, 1), Port(dff1, 1), Port(dff2, 1), Port(dff3, 1) }, typeid(bool)); // RST
        gb.outto(Port(enty, 2), Ports{ Port(dff0, 2), Port(dff1, 2), Port(dff2, 2), Port(dff3, 2) }, typeid(bool)); // CK

        gb.outto(Port(enty, 3), Ports{ Port(dff0, 3) }, typeid(bool)); // D0
        gb.outto(Port(enty, 4), Ports{ Port(dff1, 3) }, typeid(bool)); // D1
        gb.outto(Port(enty, 5), Ports{ Port(dff2, 3) }, typeid(bool)); // D2
        gb.outto(Port(enty, 6), Ports{ Port(dff3, 3) }, typeid(bool)); // D3

        gb.outto(Port(dff0, 1), Ports{ Port(exit, 1) }, typeid(bool)); // Q0
        gb.outto(Port(dff1, 1), Ports{ Port(exit, 2) }, typeid(bool)); // Q1
        gb.outto(Port(dff2, 1), Ports{ Port(exit, 3) }, typeid(bool)); // Q2
        gb.outto(Port(dff3, 1), Ports{ Port(exit, 4) }, typeid(bool)); // Q3

        commit();

    }
};


#endif

