// NodeFlipFlop.hpp
#ifndef __NODEFLIPFLOP_HPP__
#define __NODEFLIPFLOP_HPP__

#include "Node.hpp"
#include "NodeComplex.hpp"
#include "NodeLogics.hpp"
#include "NodeValue.hpp"

/*
Node NodeSRFlipFlopNor
{
    nor1 = NodeNor
    nor2 = NodeNor
    val1 = NodeValue<bool>
    val2 = NodeValue<bool>

    enty(1) -> nor1(1)
    enty(2) -> nor1(1)
    nor1(1) -> val1(1), exit(1)
    nor2(1) -> val2(1), exit(2)
    val1(1) -> nor2(2)
    val2(1) -> nor1(2)
}
*/

/*
           _     _
  R  S  Qp Qp Q  Q
 ------------------
  1  0  0  0  0  1
  1  0  0  1  0  1
  0  1  0  1  0  0 <-


*/


//-----------------------------------------------------------
// NOR型SRフリップフロップ
// 最初は内部状態(val1, val2の値)は不定と想定すべき。
// 即ち、使い始めに一発リセットを打ち込む必要がある。
// In-Ports  : R, S
// Out-Ports : Q, R-bar
class NodeSRFlipFlopNor : public NodeComplex
{
public:
    // //-------------------------------------------------------
    // NodeSRFlipFlopNor(void)
    // {
    //     auto& gb = getGraphBuilder();

    //     auto enty = getEntryNode();
    //     auto exit = getExitNode();
    //     auto nor1 = gb.createNode<NodeNor>("nor1");
    //     auto nor2 = gb.createNode<NodeNor>("nor2");
    //     auto val1 = gb.createNode<NodeValue<bool>>("val1");
    //     auto val2 = gb.createNode<NodeValue<bool>>("val2");

    //     gb.outto(Port(enty, 1), Ports{ Port(nor1, 1) });                // R
    //     gb.outto(Port(enty, 2), Ports{ Port(nor2, 1) });                // S
    //     gb.outto(Port(nor1, 1), Ports{ Port(val1, 1), Port(exit, 1) }); // Q
    //     gb.outto(Port(nor2, 1), Ports{ Port(val2, 1), Port(exit, 2) }); // Q-inv.
    //     gb.outto(Port(val1, 1), Ports{ Port(nor2, 2) });
    //     gb.outto(Port(val2, 1), Ports{ Port(nor1, 2) });

    //     commit();
    // }

    //-------------------------------------------------------
    NodeSRFlipFlopNor(void)
    {
        auto& gb = getGraphBuilder();

        auto enty = getEntryNode();
        auto exit = getExitNode();
        auto nor1 = gb.createNode<NodeNor>("nor1");
        auto nor2 = gb.createNode<NodeNor>("nor2");

        gb.outto(Port(enty, 1), Ports{ Port(nor1, 1) });                // R
        gb.outto(Port(enty, 2), Ports{ Port(nor2, 1) });                // S
        gb.outto(Port(nor1, 1), Ports{ Port(nor2, 2), Port(exit, 1) }); // Q
        gb.outto(Port(nor2, 1), Ports{ Port(nor1, 2), Port(exit, 2) }); // Q-inv.

        commit();
    }

};

#endif


