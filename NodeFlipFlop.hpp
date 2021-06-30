// NodeFlipFlop.hpp
// 
// SR Flip Flop (NOR)
// SR Flip Flop (NAND)
// Gated SR Latch (Synchronous SR Flip Flop)
// Gated D Latch
// JK Flip Flop
// D Flip Flop
// JK Flip Flop (Master-Slave)
// D Flip Flop (Master-Slave)
// T Flip Flop (Master-Slave)
// D Flip Flop (Edge-Trigger)
// JK Flip Flop (Edge-Trigger)
// D Flip Flop (Edge-Trigger/Asynchronous reset)
// JK Flip Flop (Edge-Trigger/Asynchronous reset)
// T Flip Flop (Edge-Trigger/Asynchronous reset)

#ifndef __NODEFLIPFLOP_HPP__
#define __NODEFLIPFLOP_HPP__

#include "Node.hpp"
#include "NodeComplex.hpp"
#include "NodeLogics.hpp"
#include "NodeValue.hpp"


//-----------------------------------------------------------
// SR Flip Flop (NOR)
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
        auto nor1 = gb.createNode<NodeNor>("nor1 in SRFlipFlopNor");
        auto nor2 = gb.createNode<NodeNor>("nor2 in SRFlipFlopNor");

        gb.outto(Port(enty, 1), Ports{ Port(nor1, 1) }, typeid(bool));                // R
        gb.outto(Port(enty, 2), Ports{ Port(nor2, 1) }, typeid(bool));                // S
        gb.outto(Port(nor1, 1), Ports{ Port(nor2, 2), Port(exit, 1) }, typeid(bool)); // Q
        gb.outto(Port(nor2, 1), Ports{ Port(nor1, 2), Port(exit, 2) }, typeid(bool)); // Q-inv.

        commit();
    }

};


//-----------------------------------------------------------
// SR Flip Flop (NAND)
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
        auto nand1 = gb.createNode<NodeNand>("nand1 in SRFlipFlopNand");
        auto nand2 = gb.createNode<NodeNand>("nand2 in SRFlipFlopNand");

        gb.outto(Port(enty, 1), Ports{ Port(nand1, 1) }, typeid(bool));                 // R
        gb.outto(Port(enty, 2), Ports{ Port(nand2, 1) }, typeid(bool));                 // S
        gb.outto(Port(nand1, 1), Ports{ Port(nand2, 2), Port(exit, 1) }, typeid(bool)); // Q
        gb.outto(Port(nand2, 1), Ports{ Port(nand1, 2), Port(exit, 2) }, typeid(bool)); // Q-inv.

        commit();
    }

};


//-----------------------------------------------------------
// Gated SR Latch (Synchronous SR Flip Flop)
// In-Ports  : E, S, R
// Out-Ports : Q, Q-inv
class NodeGatedSRLatch : public NodeComplex
{
public:
    //-------------------------------------------------------
    NodeGatedSRLatch(void)
    {
        auto& gb = getGraphBuilder();

        auto enty  = getEntryNode();
        auto exit  = getExitNode();
        auto nand1 = gb.createNode<NodeNand>("nand1 in GatedSRLatch");
        auto nand2 = gb.createNode<NodeNand>("nand2 in GatedSRLatch");
        auto srff  = gb.createNode<NodeSRFlipFlopNand>("srff(nand) in GatedSRLatch");

        gb.outto(Port(enty,  1), Ports{ Port(nand1, 1), Port(nand2, 1) }, typeid(bool));    // E
        gb.outto(Port(enty,  2), Ports{ Port(nand1, 2) }, typeid(bool));    // S
        gb.outto(Port(enty,  3), Ports{ Port(nand2, 2) }, typeid(bool));    // R
        gb.outto(Port(nand1, 1), Ports{ Port(srff,  1) }, typeid(bool));    // A
        gb.outto(Port(nand2, 1), Ports{ Port(srff,  2) }, typeid(bool));    // B
        gb.outto(Port(srff,  1), Ports{ Port(exit,  1) }, typeid(bool));    // Q
        gb.outto(Port(srff,  2), Ports{ Port(exit,  2) }, typeid(bool));    // Q-inv.

        commit();
    }
};


//-----------------------------------------------------------
// Gated D Latch
// In-Ports  : E, D
// Out-Ports : Q, Q-inv
class NodeGatedDLatch : public NodeComplex
{
public:
    //-------------------------------------------------------
    NodeGatedDLatch(void)
    {
        auto& gb = getGraphBuilder();

        auto enty  = getEntryNode();
        auto exit  = getExitNode();
        auto not1 = gb.createNode<NodeNot>("not1 in GatedDLatch");
        auto gsrl  = gb.createNode<NodeGatedSRLatch>("g-sr latch in GatedDLatch");

        gb.outto(Port(enty,  1), Ports{ Port(gsrl, 1) }, typeid(bool)); // E
        gb.outto(Port(enty,  2), Ports{ Port(gsrl, 2), Port(not1, 1) }, typeid(bool));  // D->S
        gb.outto(Port(not1,  1), Ports{ Port(gsrl, 3) }, typeid(bool)); // R
        gb.outto(Port(gsrl,  1), Ports{ Port(exit, 1) }, typeid(bool)); // Q
        gb.outto(Port(gsrl,  2), Ports{ Port(exit, 2) }, typeid(bool)); // Q-inv.

        commit();
    }
};


//-----------------------------------------------------------
// JK Flip Flop
// In-Ports  : CK, J, K
// Out-Ports : Q, Q-inv
class NodeJKFlipFlop : public NodeComplex
{
public:
    //-------------------------------------------------------
    NodeJKFlipFlop(void)
    {
        auto& gb = getGraphBuilder();

        auto enty  = getEntryNode();
        auto exit  = getExitNode();

        auto u1 = gb.createNode<Node3Nand>("U1 in JK-FF");
        auto u2 = gb.createNode<Node3Nand>("U2 in JK-FF");
        auto u3 = gb.createNode<NodeNand>("U3 in JK-FF");
        auto u4 = gb.createNode<NodeNand>("U4 in JK-FF");

        gb.outto(Port(enty,  1), Ports{ Port(u1, 1), Port(u2, 1) }, typeid(bool)); // CK
        gb.outto(Port(enty,  2), Ports{ Port(u1, 2) },  typeid(bool));             // J
        gb.outto(Port(enty,  3), Ports{ Port(u2, 2) },  typeid(bool));             // K
        gb.outto(Port(u1, 1),    Ports{ Port(u3,  1) }, typeid(bool));             // A
        gb.outto(Port(u2, 1),    Ports{ Port(u4,  1) }, typeid(bool));             // B
        gb.outto(Port(u3, 1),    Ports{ Port(u2, 3), Port(u4, 2), Port(exit, 1) }, typeid(bool)); // Q
        gb.outto(Port(u4, 1),    Ports{ Port(u1, 3), Port(u3, 2), Port(exit, 2) }, typeid(bool)); // Q-inv

        commit();
    }
};


//-----------------------------------------------------------
// D Flip Flop
// In-Ports  : CK, D
// Out-Ports : Q, Q-inv
class NodeDFlipFlop : public NodeComplex
{
public:
    //-------------------------------------------------------
    NodeDFlipFlop(void)
    {
        auto& gb = getGraphBuilder();
        auto enty  = getEntryNode();
        auto exit  = getExitNode();

        auto not1 = gb.createNode<NodeNot>("not1 in DFlipFlop");
        auto jkff  = gb.createNode<NodeJKFlipFlop>("jkff in DFlipFlop");

        gb.outto(Port(enty,  1), Ports{ Port(jkff, 1) }, typeid(bool)); // CK
        gb.outto(Port(enty,  2), Ports{ Port(jkff, 2), Port(not1, 1) }, typeid(bool));    // D -> J
        gb.outto(Port(not1,  1), Ports{ Port(jkff, 3) }, typeid(bool)); // D-inv -> K
        gb.outto(Port(jkff,  1), Ports{ Port(exit, 1) }, typeid(bool)); // Q
        gb.outto(Port(jkff,  2), Ports{ Port(exit, 2) }, typeid(bool)); // Q-inv

        commit();
    }
};


//-----------------------------------------------------------
// JK Flip Flop (Master-Slave)
// In-Ports  : CK, J, K
// Out-Ports : Q, Q-inv
class NodeJKFlipFlopMasterSlave : public NodeComplex
{
public:
    //-------------------------------------------------------
    NodeJKFlipFlopMasterSlave(void)
    {
        auto& gb = getGraphBuilder();
        auto enty  = getEntryNode();
        auto exit  = getExitNode();

        auto u1 = gb.createNode<Node3Nand>("U1 in JK-FF");
        auto u2 = gb.createNode<Node3Nand>("U2 in JK-FF");
        auto u3 = gb.createNode<NodeNand>("U3 in JK-FF");
        auto u4 = gb.createNode<NodeNand>("U4 in JK-FF");
        auto u5 = gb.createNode<NodeNand>("U5 in JK-FF");
        auto u6 = gb.createNode<NodeNand>("U6 in JK-FF");
        auto u7 = gb.createNode<NodeNand>("U7 in JK-FF");
        auto u8 = gb.createNode<NodeNand>("U8 in JK-FF");
        auto u9 = gb.createNode<NodeNot>("U9(NOT) in JK-FF");

        gb.outto(Port(enty,  1), Ports{ Port(u1, 1), Port(u2, 1), Port(u9, 1)   }, typeid(bool)); // CK
        gb.outto(Port(enty,  2), Ports{ Port(u1, 2)                             }, typeid(bool)); // J
        gb.outto(Port(enty,  3), Ports{ Port(u2, 2)                             }, typeid(bool)); // K
        gb.outto(Port(u1,    1), Ports{ Port(u3, 1)                             }, typeid(bool));
        gb.outto(Port(u2,    1), Ports{ Port(u4, 1)                             }, typeid(bool));
        gb.outto(Port(u3,    1), Ports{ Port(u4, 2), Port(u5, 2)                }, typeid(bool));
        gb.outto(Port(u4,    1), Ports{ Port(u3, 2), Port(u6, 2)                }, typeid(bool));
        gb.outto(Port(u9,    1), Ports{ Port(u5, 1), Port(u6, 1)                }, typeid(bool)); // CK-inv
        gb.outto(Port(u5,    1), Ports{ Port(u7, 1)                             }, typeid(bool));
        gb.outto(Port(u6,    1), Ports{ Port(u8, 1)                             }, typeid(bool));
        gb.outto(Port(u7,    1), Ports{ Port(u8, 2), Port(u2, 3), Port(exit, 1) }, typeid(bool)); // Q
        gb.outto(Port(u8,    1), Ports{ Port(u7, 2), Port(u1, 3), Port(exit, 2) }, typeid(bool)); // Q-inv

        commit();
    }
};


//-----------------------------------------------------------
// D Flip Flop (Master-Slave)
// In-Ports  : CK, D
// Out-Ports : Q, Q-inv
class NodeDFlipFlopMasterSlave : public NodeComplex
{
public:
    //-------------------------------------------------------
    NodeDFlipFlopMasterSlave(void)
    {
        auto& gb = getGraphBuilder();
        auto enty  = getEntryNode();
        auto exit  = getExitNode();

        auto jkff = gb.createNode<NodeJKFlipFlopMasterSlave>("JK-FF in D-FF");
        auto nnot = gb.createNode<NodeNot>("NOT in D-FF");

        gb.outto(Port(enty, 1), Ports{ Port(jkff, 1)                }, typeid(bool)); // CK
        gb.outto(Port(enty, 2), Ports{ Port(jkff, 2), Port(nnot, 1) }, typeid(bool)); // D
        gb.outto(Port(nnot, 1), Ports{ Port(jkff, 3)                }, typeid(bool)); // K-> JK-FF
        gb.outto(Port(jkff, 1), Ports{ Port(exit, 1)                }, typeid(bool)); // Q
        gb.outto(Port(jkff, 2), Ports{ Port(exit, 2)                }, typeid(bool)); // Q-inv

        commit();
    }
};



//-----------------------------------------------------------
// T Flip Flop (Master-Slave)
// In-Ports  : T
// Out-Ports : Q, Q-inv
class NodeTFlipFlopMasterSlave : public NodeComplex
{
public:
    //-------------------------------------------------------
    NodeTFlipFlopMasterSlave(void)
    {
        auto& gb = getGraphBuilder();
        auto enty  = getEntryNode();
        auto exit  = getExitNode();

        auto jkff = gb.createNode<NodeJKFlipFlopMasterSlave>("JK-FF in D-FF");
        auto plup = gb.createNode<NodeValue<bool>>("Pull Up");

        gb.outto(Port(plup, 1), Ports{ Port(jkff, 2), Port(jkff, 3) }, typeid(bool));
        gb.outto(Port(enty, 1), Ports{ Port(jkff, 1)                }, typeid(bool)); // T
        gb.outto(Port(jkff, 1), Ports{ Port(exit, 1)                }, typeid(bool)); // Q
        gb.outto(Port(jkff, 2), Ports{ Port(exit, 2)                }, typeid(bool)); // Q-inv

        auto nValue = static_cast<NodeValue<bool>*>(plup.getNode());
        nValue->setValue(T);    // Pull UP

        commit();
    }
};




//-----------------------------------------------------------
// D Flip Flop (Edge-Trigger)
// In-Ports  : CK, D
// Out-Ports : Q, Q-inv
class NodeDFlipFlopEdgeTrigger : public NodeComplex
{
public:
    //-------------------------------------------------------
    NodeDFlipFlopEdgeTrigger(void)
    {
        auto& gb = getGraphBuilder();
        auto enty  = getEntryNode();
        auto exit  = getExitNode();

        auto u1 = gb.createNode<NodeNand>("U1 in D-FF(EdgeTrigger)");
        auto u2 = gb.createNode<NodeNand>("U2 in D-FF(EdgeTrigger)");
        auto u3 = gb.createNode<Node3Nand>("U3 in D-FF(EdgeTrigger)");
        auto u4 = gb.createNode<NodeNand>("U4 in D-FF(EdgeTrigger)");
        auto u5 = gb.createNode<NodeNand>("U5 in D-FF(EdgeTrigger)");
        auto u6 = gb.createNode<NodeNand>("U6 in D-FF(EdgeTrigger)");

        gb.outto(Port(enty, 1), Ports{ Port(u2, 2), Port(u3, 2) }               , typeid(bool)); // CK
        gb.outto(Port(enty, 2), Ports{ Port(u4, 2) }                            , typeid(bool)); // D
        gb.outto(Port(u1, 1)  , Ports{ Port(u2, 1) }                            , typeid(bool)); // A
        gb.outto(Port(u2, 1)  , Ports{ Port(u1, 2), Port(u3, 1), Port(u5, 1) }  , typeid(bool)); // X
        gb.outto(Port(u3, 1)  , Ports{ Port(u4, 1), Port(u6, 2) }               , typeid(bool)); // Y
        gb.outto(Port(u4, 1)  , Ports{ Port(u3, 3), Port(u1, 1) }               , typeid(bool)); // B
        gb.outto(Port(u5, 1)  , Ports{ Port(u6, 1), Port(exit, 1) }             , typeid(bool)); // Q
        gb.outto(Port(u6, 1)  , Ports{ Port(u5, 2), Port(exit, 2) }             , typeid(bool)); // Q-inv
 
        commit();
    }
};




//-----------------------------------------------------------
// JK Flip Flop (Edge-Trigger)
// In-Ports  : CK, J, K
// Out-Ports : Q, Q-inv
class NodeJKFlipFlopEdgeTrigger : public NodeComplex
{
public:
    //-------------------------------------------------------
    NodeJKFlipFlopEdgeTrigger(void)
    {
        auto& gb = getGraphBuilder();
        auto enty  = getEntryNode();
        auto exit  = getExitNode();

        auto dff = gb.createNode<NodeDFlipFlopEdgeTrigger>("D-FF(EdgeTrigger) in JKFF(EdgeTrigger)");
        auto not1 = gb.createNode<NodeNot>("NOT in JKFF(EdgeTrigger)");
        auto and1 = gb.createNode<NodeAnd>("AND1 in JKFF(EdgeTrigger)");
        auto and2 = gb.createNode<NodeAnd>("AND2 in JKFF(EdgeTrigger)");
        auto or1 = gb.createNode<NodeOr>("OR in JKFF(EdgeTrigger)");

        gb.outto(Port(enty, 1), Ports{ Port(dff, 1) }                   , typeid(bool)); // CK
        gb.outto(Port(enty, 2), Ports{ Port(and1, 2) }                  , typeid(bool)); // J
        gb.outto(Port(enty, 3), Ports{ Port(not1, 1) }                  , typeid(bool)); // K
        gb.outto(Port(not1, 1), Ports{ Port(and2, 1) }                  , typeid(bool));
        gb.outto(Port(and1, 1), Ports{ Port(or1, 1) }                   , typeid(bool));
        gb.outto(Port(and2, 1), Ports{ Port(or1, 2) }                   , typeid(bool));
        gb.outto(Port(or1, 1), Ports{ Port(dff, 2) }                    , typeid(bool)); // D
        gb.outto(Port(dff, 1), Ports{ Port(and2, 2), Port(exit, 1) }    , typeid(bool)); // Q
        gb.outto(Port(dff, 2), Ports{ Port(and1, 1), Port(exit, 2) }    , typeid(bool)); // Q-inv

        commit();
    }

};


//-----------------------------------------------------------
// T Flip Flop (Edge-Trigger)
// In-Ports  : T
// Out-Ports : Q, Q-inv
class NodeTFlipFlopEdgeTrigger : public NodeComplex
{
public:
    //-------------------------------------------------------
    NodeTFlipFlopEdgeTrigger(void)
    {
        auto& gb = getGraphBuilder();
        auto enty  = getEntryNode();
        auto exit  = getExitNode();

        auto dff = gb.createNode<NodeDFlipFlopEdgeTrigger>("D-FF(EdgeTrigger) in T-FF(EdgeTrigger)");

        gb.outto(Port(enty, 1), Ports{ Port(dff, 1) }                   , typeid(bool)); // T
        gb.outto(Port(dff, 1),  Ports{ Port(exit, 1) }                  , typeid(bool)); // Q
        gb.outto(Port(dff, 2),  Ports{ Port(dff, 2), Port(exit, 2) }    , typeid(bool)); // Q-inv

        commit();

    }
};



//-----------------------------------------------------------
// D Flip Flop (Edge-Trigger/Asynchronous reset)
// In-Ports  : RST, CK, D
// Out-Ports : Q, Q-inv
class NodeDFlipFlopEdgeTriggerAsyncReset : public NodeComplex
{
public:
    //-------------------------------------------------------
    NodeDFlipFlopEdgeTriggerAsyncReset(void)
    {
        auto& gb = getGraphBuilder();
        auto enty  = getEntryNode();
        auto exit  = getExitNode();

        auto u1 = gb.createNode<NodeNand>("U1 in D-FF(EdgeTriggerAsyncReset)");
        auto u2 = gb.createNode<Node3Nand>("U2 in D-FF(EdgeTriggerAsyncReset)");
        auto u3 = gb.createNode<Node3Nand>("U3 in D-FF(EdgeTriggerAsyncReset)");
        auto u4 = gb.createNode<Node3Nand>("U4 in D-FF(EdgeTriggerAsyncReset)");
        auto u5 = gb.createNode<NodeNand>("U5 in D-FF(EdgeTriggerAsyncReset)");
        auto u6 = gb.createNode<Node3Nand>("U6 in D-FF(EdgeTriggerAsyncReset)");

        gb.outto(Port(enty, 1), Ports{ Port(u2, 3), Port(u4, 3), Port(u6, 3)    }, typeid(bool)); // RST
        gb.outto(Port(enty, 2), Ports{ Port(u2, 2), Port(u3, 2)                 }, typeid(bool)); // CK
        gb.outto(Port(enty, 3), Ports{ Port(u4, 2)                              }, typeid(bool)); // D
        gb.outto(Port(u1, 1),   Ports{ Port(u2, 1)                              }, typeid(bool)); // A
        gb.outto(Port(u2, 1),   Ports{ Port(u1, 2), Port(u3, 1), Port(u5, 1)    }, typeid(bool)); // X
        gb.outto(Port(u3, 1),   Ports{ Port(u4, 1), Port(u6, 2)                 }, typeid(bool)); // Y
        gb.outto(Port(u4, 1),   Ports{ Port(u3, 3), Port(u1, 1)                 }, typeid(bool)); // B
        gb.outto(Port(u5, 1),   Ports{ Port(u6, 1), Port(exit, 1)               }, typeid(bool)); // Q
        gb.outto(Port(u6, 1),   Ports{ Port(u5, 2), Port(exit, 2)               }, typeid(bool)); // Q-inv
 
        commit();

    }
};


//-----------------------------------------------------------
// JK Flip Flop (Edge-Trigger/Asynchronous reset)
// In-Ports  : RST, CK, J, K
// Out-Ports : Q, Q-inv
class NodeJKFlipFlopEdgeTriggerAsyncReset : public NodeComplex
{
public:
    //-------------------------------------------------------
    NodeJKFlipFlopEdgeTriggerAsyncReset(void)
    {
        auto& gb = getGraphBuilder();
        auto enty  = getEntryNode();
        auto exit  = getExitNode();

        auto dff = gb.createNode<NodeDFlipFlopEdgeTriggerAsyncReset>("D-FF in JKFF");
        auto not1 = gb.createNode<NodeNot>("NOT in JKFF");
        auto and1 = gb.createNode<NodeAnd>("AND1 in JKFF");
        auto and2 = gb.createNode<NodeAnd>("AND2 in JKFF");
        auto or1 = gb.createNode<NodeOr>("OR in JKFF ");

        gb.outto(Port(enty, 1), Ports{ Port(dff, 1) }                   , typeid(bool)); // RST
        gb.outto(Port(enty, 2), Ports{ Port(dff, 2) }                   , typeid(bool)); // CK
        gb.outto(Port(enty, 3), Ports{ Port(and1, 2) }                  , typeid(bool)); // J
        gb.outto(Port(enty, 4), Ports{ Port(not1, 1) }                  , typeid(bool)); // K
        gb.outto(Port(not1, 1), Ports{ Port(and2, 1) }                  , typeid(bool));
        gb.outto(Port(and1, 1), Ports{ Port(or1, 1) }                   , typeid(bool));
        gb.outto(Port(and2, 1), Ports{ Port(or1, 2) }                   , typeid(bool));
        gb.outto(Port(or1, 1), Ports{ Port(dff, 3) }                    , typeid(bool)); // D
        gb.outto(Port(dff, 1), Ports{ Port(and2, 2), Port(exit, 1) }    , typeid(bool)); // Q
        gb.outto(Port(dff, 2), Ports{ Port(and1, 1), Port(exit, 2) }    , typeid(bool)); // Q-inv

        commit();
    }
};



//-----------------------------------------------------------
// T Flip Flop (Edge-Trigger/Asynchronous reset)
// In-Ports  : RST, T
// Out-Ports : Q, Q-inv
class NodeTFlipFlopEdgeTriggerAsyncReset : public NodeComplex
{
public:
    //-------------------------------------------------------
    NodeTFlipFlopEdgeTriggerAsyncReset(void)
    {
        auto& gb = getGraphBuilder();
        auto enty  = getEntryNode();
        auto exit  = getExitNode();

        auto dff = gb.createNode<NodeDFlipFlopEdgeTriggerAsyncReset>("D-FF in JKFF");

        gb.outto(Port(enty, 1), Ports{ Port(dff, 1) }                   , typeid(bool)); // RST
        gb.outto(Port(enty, 2), Ports{ Port(dff, 2) }                   , typeid(bool)); // T
        gb.outto(Port(dff, 1),  Ports{ Port(exit, 1) }                  , typeid(bool)); // Q
        gb.outto(Port(dff, 2),  Ports{ Port(dff, 3), Port(exit, 2) }    , typeid(bool)); // Q-inv

        commit();
    }
};




#endif


