// NodeRegister.hpp
// 4bits register
// 4bits Asynchronous Counter
// 4bits Synchronous Counter
// 4bits shift register

#ifndef __NODE4BITREGISTER_HPP__
#define __NODE4BITREGISTER_HPP__

#include "NodeComplex.hpp"
#include "NodeFlipFlop.hpp"
#include "NodeAdder.hpp"


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

//-----------------------------------------------------------
// 4bits Asynchronous Counter
// In-Ports  : RST, CK
// Out-Ports : Q0, Q1, Q2, Q3
class Node4bitAsyncCounter : public NodeComplex
{
public:
    //-------------------------------------------------------
    Node4bitAsyncCounter(void)
    {
        auto& gb = getGraphBuilder();
        auto enty  = getEntryNode();
        auto exit  = getExitNode();

        auto tff0 = gb.createNode<NodeTFlipFlopEdgeTriggerAsyncReset>("T-FF0 in 4bit Async Counter");
        auto tff1 = gb.createNode<NodeTFlipFlopEdgeTriggerAsyncReset>("T-FF1 in 4bit Async Counter");
        auto tff2 = gb.createNode<NodeTFlipFlopEdgeTriggerAsyncReset>("T-FF2 in 4bit Async Counter");
        auto tff3 = gb.createNode<NodeTFlipFlopEdgeTriggerAsyncReset>("T-FF3 in 4bit Async Counter");

        gb.outto(Port(enty, 1), Ports{ Port(tff0, 1), Port(tff1, 1), Port(tff2, 1), Port(tff3, 1) }, typeid(bool)); // RST

        gb.outto(Port(enty, 2), Ports{ Port(tff0, 2) }, typeid(bool)); // CK
        gb.outto(Port(tff0, 2), Ports{ Port(tff1, 2) }, typeid(bool));
        gb.outto(Port(tff1, 2), Ports{ Port(tff2, 2) }, typeid(bool));
        gb.outto(Port(tff2, 2), Ports{ Port(tff3, 2) }, typeid(bool));

        gb.outto(Port(tff0, 1), Ports{ Port(exit, 1) }, typeid(bool)); // Q0
        gb.outto(Port(tff1, 1), Ports{ Port(exit, 2) }, typeid(bool)); // Q1
        gb.outto(Port(tff2, 1), Ports{ Port(exit, 3) }, typeid(bool)); // Q2
        gb.outto(Port(tff3, 1), Ports{ Port(exit, 4) }, typeid(bool)); // Q3

        commit();
    }
};



//-----------------------------------------------------------
// 4bits Synchronous Counter
// In-Ports  : RST, CK
// Out-Ports : Q0, Q1, Q2, Q3
class Node4bitSyncCounter : public NodeComplex
{
public:
    //-------------------------------------------------------
    Node4bitSyncCounter(void)
    {
        auto& gb = getGraphBuilder();
        auto enty  = getEntryNode();
        auto exit  = getExitNode();

        auto reg4 = gb.createNode<Node4bitRegister>("4bit register in 4bit Sync Counter");
        auto not1 = gb.createNode<NodeNot>("Not in 4bit Sync Counter");
        auto had1 = gb.createNode<NodeHalfAdder>("Half Addr1 in 4bit Sync Counter");
        auto had2 = gb.createNode<NodeHalfAdder>("Half Addr2 in 4bit Sync Counter");
        auto exor = gb.createNode<NodeExor>("Exor in 4bit Async Counter");

        gb.outto(Port(enty, 1), Ports{ Port(reg4, 1) }, typeid(bool)); // RST
        gb.outto(Port(enty, 2), Ports{ Port(reg4, 2) }, typeid(bool)); // CK

        gb.outto(Port(reg4, 1), Ports{ Port(not1, 1), Port(had1, 1), Port(exit, 1) }, typeid(bool)); // Q0
        gb.outto(Port(reg4, 2), Ports{ Port(had1, 2), Port(exit, 2) }, typeid(bool)); // Q1
        gb.outto(Port(reg4, 3), Ports{ Port(had2, 2), Port(exit, 3) }, typeid(bool)); // Q2
        gb.outto(Port(reg4, 4), Ports{ Port(exor, 2), Port(exit, 4) }, typeid(bool)); // Q3

        gb.outto(Port(not1, 1), Ports{ Port(reg4, 3) }, typeid(bool)); // D0
        gb.outto(Port(had1, 1), Ports{ Port(reg4, 4) }, typeid(bool)); // D1
        gb.outto(Port(had2, 1), Ports{ Port(reg4, 5) }, typeid(bool)); // D2
        gb.outto(Port(exor, 1), Ports{ Port(reg4, 6) }, typeid(bool)); // D3

        gb.outto(Port(had1, 2), Ports{ Port(had2, 1) }, typeid(bool));
        gb.outto(Port(had2, 2), Ports{ Port(exor, 1) }, typeid(bool));

        commit();
    }
};


//-----------------------------------------------------------
// 4bits shift register
// In-Ports  : RST, CK, D
// Out-Ports : Q0, Q1, Q2, Q3
class Node4bitShiftRegister : public NodeComplex
{
public:
    //-------------------------------------------------------
    Node4bitShiftRegister(void)
    {
        auto& gb = getGraphBuilder();
        auto enty  = getEntryNode();
        auto exit  = getExitNode();

        auto dff0 = gb.createNode<NodeDFlipFlopEdgeTriggerAsyncReset>("D-FF0 in 4bit Shift Register");
        auto dff1 = gb.createNode<NodeDFlipFlopEdgeTriggerAsyncReset>("D-FF1 in 4bit Shift Register");
        auto dff2 = gb.createNode<NodeDFlipFlopEdgeTriggerAsyncReset>("D-FF2 in 4bit Shift Register");
        auto dff3 = gb.createNode<NodeDFlipFlopEdgeTriggerAsyncReset>("D-FF3 in 4bit Shift Register");

        gb.outto(Port(enty, 1), Ports{ Port(dff0, 1), Port(dff1, 1), Port(dff2, 1), Port(dff3, 1) }, typeid(bool)); // RST
        gb.outto(Port(enty, 2), Ports{ Port(dff0, 2), Port(dff1, 2), Port(dff2, 2), Port(dff3, 2) }, typeid(bool)); // CK

        gb.outto(Port(enty, 3), Ports{ Port(dff0, 3) }, typeid(bool)); // D

        gb.outto(Port(dff0, 1), Ports{ Port(dff1, 3), Port(exit, 1) }, typeid(bool)); // Q0
        gb.outto(Port(dff1, 1), Ports{ Port(dff2, 3), Port(exit, 2) }, typeid(bool)); // Q1
        gb.outto(Port(dff2, 1), Ports{ Port(dff3, 3), Port(exit, 3) }, typeid(bool)); // Q2
        gb.outto(Port(dff3, 1), Ports{                Port(exit, 4) }, typeid(bool)); // Q3

        commit();
    }
};


#endif

