// NodeAdder.hpp
#ifndef __NODEADDER_HPP__
#define __NODEADDER_HPP__

#include "GraphBuilder.hpp"
#include "NodeLogics.hpp"
#include "NodeValue.hpp"


//-----------------------------------------------------------
// half Adder
// In-port  : A, B
// Out0port : S, C
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
// In-port  : A, B, C-in
// Out0port : S, C-out
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


//-----------------------------------------------------------
// 4 bits addr S, C = A + B
// In-port  : A0, A1, A2, A3, B0, B1, B2, B3
// Out0port : S0, S1, S2, S3, C-out
// where
//      C-out
//              true  : carry over
//              false : carry not over
class NodeAddr4bit : public NodeComplex
{
public:
    //-------------------------------------------------------
    NodeAddr4bit(void)
    {
        auto& gb = getGraphBuilder();

        // ノード生成
        std::vector<QuasiNode> adders;
        for(int i=0;i<4;i++){
            auto adder = gb.createNode<NodeFullAdder>();
            adders.push_back(adder);
        }

        auto const0 = gb.createNode<NodeValue<bool>>();

        // ノード間接続
        gb.outto(Port(const0, 1), Ports{Port(adders[0], 3)});
        gb.outto(Port(adders[0], 2), Ports{Port(adders[1], 3)});
        gb.outto(Port(adders[1], 2), Ports{Port(adders[2], 3)});
        gb.outto(Port(adders[2], 2), Ports{Port(adders[3], 3)});

        // 外部入出力定義
        setInPortss(Ports{ Port(adders[0], 1) },    // A0
                    Ports{ Port(adders[1], 1) },    // A1
                    Ports{ Port(adders[2], 1) },    // A2
                    Ports{ Port(adders[3], 1) },    // A3

                    Ports{ Port(adders[0], 2) },    // B0
                    Ports{ Port(adders[1], 2) },    // B1
                    Ports{ Port(adders[2], 2) },    // B2
                    Ports{ Port(adders[3], 2) } );  // B3

        setOutPorts(Port(adders[0], 1),             // S0
                    Port(adders[1], 1),             // S1
                    Port(adders[2], 1),             // S2
                    Port(adders[3], 1),             // S3
                    Port(adders[3], 2));            // C-out

        // ノード初期値設定
        auto nConst = static_cast<NodeValue<bool>*>(const0.getNode());
        nConst->setValue(false);

        commit();
    }
};


//-----------------------------------------------------------
// 4 bits add/sub S, C = A +/- B
// In-port  : A0, A1, A2, A3, B0, B1, B2, B3, ADD/SUB
// Out0port : S0, S1, S2, S3, C-out
// where
//      ADD/SUB
//              true  : SUB
//              false : ADD
//      C-out (in case of ADD)
//              true  : carry over
//              false : carry not over
//      C-out (in case of SUB)
//              true  : result of sub is positive
//              false : result of sub is negative
class NodeAddSub4bit : public NodeComplex
{
public:
    //-------------------------------------------------------
    NodeAddSub4bit(void)
    {
        auto& gb = getGraphBuilder();

        // 全加算器ノード生成
        std::vector<QuasiNode> adders;
        for(int i=0;i<4;i++){
            auto adder = gb.createNode<NodeFullAdder>();
            adders.push_back(adder);
        }

        // EXORノード生成
        std::vector<QuasiNode> exors;
        for(int i=0;i<4;i++){
            auto exor = gb.createNode<NodeExor>();
            exors.push_back(exor);
        }

        // ノード間接続
        gb.outto(Port(adders[0], 2), Ports{Port(adders[1], 3)});
        gb.outto(Port(adders[1], 2), Ports{Port(adders[2], 3)});
        gb.outto(Port(adders[2], 2), Ports{Port(adders[3], 3)});

        gb.outto(Port(exors[0], 1), Ports{Port(adders[0], 2)});
        gb.outto(Port(exors[1], 1), Ports{Port(adders[1], 2)});
        gb.outto(Port(exors[2], 1), Ports{Port(adders[2], 2)});
        gb.outto(Port(exors[3], 1), Ports{Port(adders[3], 2)});


        // 外部入出力定義
        setInPortss(Ports{ Port(adders[0], 1) },    // A0
                    Ports{ Port(adders[1], 1) },    // A1
                    Ports{ Port(adders[2], 1) },    // A2
                    Ports{ Port(adders[3], 1) },    // A3

                    Ports{ Port(exors[0], 1) },     // B0
                    Ports{ Port(exors[1], 1) },     // B1
                    Ports{ Port(exors[2], 1) },     // B2
                    Ports{ Port(exors[3], 1) },     // B3

                    Ports{                          // ADD/SUB
                            Port(exors[0], 2),
                            Port(exors[1], 2),
                            Port(exors[2], 2),
                            Port(exors[3], 2),
                            Port(adders[0], 3) } );

        setOutPorts(Port(adders[0], 1),             // S0
                    Port(adders[1], 1),             // S1
                    Port(adders[2], 1),             // S2
                    Port(adders[3], 1),             // S3
                    Port(adders[3], 2));            // C-out

        commit();

    }
};

#endif


