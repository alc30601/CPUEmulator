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

        gb.outto<bool>(Port(enty, 1), Ports{ Port(nor1, 1) });                // R
        gb.outto<bool>(Port(enty, 2), Ports{ Port(nor2, 1) });                // S
        gb.outto<bool>(Port(nor1, 1), Ports{ Port(nor2, 2), Port(exit, 1) }); // Q
        gb.outto<bool>(Port(nor2, 1), Ports{ Port(nor1, 2), Port(exit, 2) }); // Q-inv.

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

        gb.outto<bool>(Port(enty, 1), Ports{ Port(nand1, 1) });                 // R
        gb.outto<bool>(Port(enty, 2), Ports{ Port(nand2, 1) });                 // S
        gb.outto<bool>(Port(nand1, 1), Ports{ Port(nand2, 2), Port(exit, 1) }); // Q
        gb.outto<bool>(Port(nand2, 1), Ports{ Port(nand1, 2), Port(exit, 2) }); // Q-inv.

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

        gb.outto<bool>(Port(enty,  1), Ports{ Port(nand1, 1), Port(nand2, 1) });    // E
        gb.outto<bool>(Port(enty,  2), Ports{ Port(nand1, 2) });    // S
        gb.outto<bool>(Port(enty,  3), Ports{ Port(nand2, 2) });    // R
        gb.outto<bool>(Port(nand1, 1), Ports{ Port(srff,  1) });    // A
        gb.outto<bool>(Port(nand2, 1), Ports{ Port(srff,  2) });    // B
        gb.outto<bool>(Port(srff,  1), Ports{ Port(exit,  1) });    // Q
        gb.outto<bool>(Port(srff,  2), Ports{ Port(exit,  2) });    // Q-inv.

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

        gb.outto<bool>(Port(enty,  1), Ports{ Port(gsrl, 1) }); // E
        gb.outto<bool>(Port(enty,  2), Ports{ Port(gsrl, 2), Port(not1, 1) });  // D->S
        gb.outto<bool>(Port(not1,  1), Ports{ Port(gsrl, 3) }); // R
        gb.outto<bool>(Port(gsrl,  1), Ports{ Port(exit, 1) }); // Q
        gb.outto<bool>(Port(gsrl,  2), Ports{ Port(exit, 2) }); // Q-inv.

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

        gb.outto<bool>(Port(enty,  1), Ports{ Port(u1, 1), Port(u2, 1) }); // CK
        gb.outto<bool>(Port(enty,  2), Ports{ Port(u1, 2) });              // J
        gb.outto<bool>(Port(enty,  3), Ports{ Port(u2, 2) });              // K
        gb.outto<bool>(Port(u1, 1),    Ports{ Port(u3, 1) });              // A
        gb.outto<bool>(Port(u2, 1),    Ports{ Port(u4, 1) });              // B
        gb.outto<bool>(Port(u3, 1),    Ports{ Port(u2, 3), Port(u4, 2), Port(exit, 1) }); // Q
        gb.outto<bool>(Port(u4, 1),    Ports{ Port(u1, 3), Port(u3, 2), Port(exit, 2) }); // Q-inv

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

        gb.outto<bool>(Port(enty,  1), Ports{ Port(jkff, 1) }); // CK
        gb.outto<bool>(Port(enty,  2), Ports{ Port(jkff, 2), Port(not1, 1) });    // D -> J
        gb.outto<bool>(Port(not1,  1), Ports{ Port(jkff, 3) }); // D-inv -> K
        gb.outto<bool>(Port(jkff,  1), Ports{ Port(exit, 1) }); // Q
        gb.outto<bool>(Port(jkff,  2), Ports{ Port(exit, 2) }); // Q-inv

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

        gb.outto<bool>(Port(enty,  1), Ports{ Port(u1, 1), Port(u2, 1), Port(u9, 1)   }); // CK
        gb.outto<bool>(Port(enty,  2), Ports{ Port(u1, 2)                             }); // J
        gb.outto<bool>(Port(enty,  3), Ports{ Port(u2, 2)                             }); // K
        gb.outto<bool>(Port(u1,    1), Ports{ Port(u3, 1)                             });
        gb.outto<bool>(Port(u2,    1), Ports{ Port(u4, 1)                             });
        gb.outto<bool>(Port(u3,    1), Ports{ Port(u4, 2), Port(u5, 2)                });
        gb.outto<bool>(Port(u4,    1), Ports{ Port(u3, 2), Port(u6, 2)                });
        gb.outto<bool>(Port(u9,    1), Ports{ Port(u5, 1), Port(u6, 1)                }); // CK-inv
        gb.outto<bool>(Port(u5,    1), Ports{ Port(u7, 1)                             });
        gb.outto<bool>(Port(u6,    1), Ports{ Port(u8, 1)                             });
        gb.outto<bool>(Port(u7,    1), Ports{ Port(u8, 2), Port(u2, 3), Port(exit, 1) }); // Q
        gb.outto<bool>(Port(u8,    1), Ports{ Port(u7, 2), Port(u1, 3), Port(exit, 2) }); // Q-inv

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

        gb.outto<bool>(Port(enty, 1), Ports{ Port(jkff, 1)                }); // CK
        gb.outto<bool>(Port(enty, 2), Ports{ Port(jkff, 2), Port(nnot, 1) }); // D
        gb.outto<bool>(Port(nnot, 1), Ports{ Port(jkff, 3)                }); // K-> JK-FF
        gb.outto<bool>(Port(jkff, 1), Ports{ Port(exit, 1)                }); // Q
        gb.outto<bool>(Port(jkff, 2), Ports{ Port(exit, 2)                }); // Q-inv

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

        gb.outto<bool>(Port(plup, 1), Ports{ Port(jkff, 2), Port(jkff, 3) });
        gb.outto<bool>(Port(enty, 1), Ports{ Port(jkff, 1)                }); // T
        gb.outto<bool>(Port(jkff, 1), Ports{ Port(exit, 1)                }); // Q
        gb.outto<bool>(Port(jkff, 2), Ports{ Port(exit, 2)                }); // Q-inv

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

        gb.outto<bool>(Port(enty, 1), Ports{ Port(u2, 2), Port(u3, 2) }               ); // CK
        gb.outto<bool>(Port(enty, 2), Ports{ Port(u4, 2) }                            ); // D
        gb.outto<bool>(Port(u1, 1)  , Ports{ Port(u2, 1) }                            ); // A
        gb.outto<bool>(Port(u2, 1)  , Ports{ Port(u1, 2), Port(u3, 1), Port(u5, 1) }  ); // X
        gb.outto<bool>(Port(u3, 1)  , Ports{ Port(u4, 1), Port(u6, 2) }               ); // Y
        gb.outto<bool>(Port(u4, 1)  , Ports{ Port(u3, 3), Port(u1, 1) }               ); // B
        gb.outto<bool>(Port(u5, 1)  , Ports{ Port(u6, 1), Port(exit, 1) }             ); // Q
        gb.outto<bool>(Port(u6, 1)  , Ports{ Port(u5, 2), Port(exit, 2) }             ); // Q-inv
 
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

        gb.outto<bool>(Port(enty, 1), Ports{ Port(dff, 1) }                   ); // CK
        gb.outto<bool>(Port(enty, 2), Ports{ Port(and1, 2) }                  ); // J
        gb.outto<bool>(Port(enty, 3), Ports{ Port(not1, 1) }                  ); // K
        gb.outto<bool>(Port(not1, 1), Ports{ Port(and2, 1) }                  );
        gb.outto<bool>(Port(and1, 1), Ports{ Port(or1, 1) }                   );
        gb.outto<bool>(Port(and2, 1), Ports{ Port(or1, 2) }                   );
        gb.outto<bool>(Port(or1, 1), Ports{ Port(dff, 2) }                    ); // D
        gb.outto<bool>(Port(dff, 1), Ports{ Port(and2, 2), Port(exit, 1) }    ); // Q
        gb.outto<bool>(Port(dff, 2), Ports{ Port(and1, 1), Port(exit, 2) }    ); // Q-inv

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

        gb.outto<bool>(Port(enty, 1), Ports{ Port(dff, 1) }                   ); // T
        gb.outto<bool>(Port(dff, 1),  Ports{ Port(exit, 1) }                  ); // Q
        gb.outto<bool>(Port(dff, 2),  Ports{ Port(dff, 2), Port(exit, 2) }    ); // Q-inv

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

        gb.outto<bool>(Port(enty, 1), Ports{ Port(u2, 3), Port(u4, 3), Port(u6, 3)    }); // RST
        gb.outto<bool>(Port(enty, 2), Ports{ Port(u2, 2), Port(u3, 2)                 }); // CK
        gb.outto<bool>(Port(enty, 3), Ports{ Port(u4, 2)                              }); // D
        gb.outto<bool>(Port(u1, 1),   Ports{ Port(u2, 1)                              }); // A
        gb.outto<bool>(Port(u2, 1),   Ports{ Port(u1, 2), Port(u3, 1), Port(u5, 1)    }); // X
        gb.outto<bool>(Port(u3, 1),   Ports{ Port(u4, 1), Port(u6, 2)                 }); // Y
        gb.outto<bool>(Port(u4, 1),   Ports{ Port(u3, 3), Port(u1, 1)                 }); // B
        gb.outto<bool>(Port(u5, 1),   Ports{ Port(u6, 1), Port(exit, 1)               }); // Q
        gb.outto<bool>(Port(u6, 1),   Ports{ Port(u5, 2), Port(exit, 2)               }); // Q-inv
 
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

        gb.outto<bool>(Port(enty, 1), Ports{ Port(dff, 1) }                   ); // RST
        gb.outto<bool>(Port(enty, 2), Ports{ Port(dff, 2) }                   ); // CK
        gb.outto<bool>(Port(enty, 3), Ports{ Port(and1, 2) }                  ); // J
        gb.outto<bool>(Port(enty, 4), Ports{ Port(not1, 1) }                  ); // K
        gb.outto<bool>(Port(not1, 1), Ports{ Port(and2, 1) }                  );
        gb.outto<bool>(Port(and1, 1), Ports{ Port(or1, 1) }                   );
        gb.outto<bool>(Port(and2, 1), Ports{ Port(or1, 2) }                   );
        gb.outto<bool>(Port(or1, 1), Ports{ Port(dff, 3) }                    ); // D
        gb.outto<bool>(Port(dff, 1), Ports{ Port(and2, 2), Port(exit, 1) }    ); // Q
        gb.outto<bool>(Port(dff, 2), Ports{ Port(and1, 1), Port(exit, 2) }    ); // Q-inv

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

        gb.outto<bool>(Port(enty, 1), Ports{ Port(dff, 1) }                   ); // RST
        gb.outto<bool>(Port(enty, 2), Ports{ Port(dff, 2) }                   ); // T
        gb.outto<bool>(Port(dff, 1),  Ports{ Port(exit, 1) }                  ); // Q
        gb.outto<bool>(Port(dff, 2),  Ports{ Port(dff, 3), Port(exit, 2) }    ); // Q-inv

        commit();
    }
};




#endif


