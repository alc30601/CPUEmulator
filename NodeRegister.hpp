// NodeRegister.hpp
// register
// 資料にはAレジスタ 、Bレジスタの２つがあり、Bレジスタは出力に
// トライステート・バッファをかませている。
// シミュレータ上でトライステート・バッファのハイインピーダンス状態の出力を
// 作り出すことは難しい。代わりにマルチプレクサで代用することとする。
// トライステート・バッファの用途は同一バスに異なる信号を流すことなので
// マルチプレクサで入力信号を選択することで代用可能と考える。
// よって、レジスタの実装としてはAもBも１つとし、上位でBレジスタの出力に
// トライステート・バッファ(マルチプレクサ)をかませることとする。
#ifndef __NODEREGISTER_HPP__
#define __NODEREGISTER_HPP__

#include "LogicDefines.hpp"
#include "NodeComplex.hpp"
#include "Node4bitRegister.hpp"
#include "NodeMultiplexer.hpp"
#include "NodeUtils.hpp"


//-----------------------------------------------------------
// <部品ノード>
// 4Input - 1Output
// 
// A--[NOR ]
// B--[   1]--+
//            +--[AND ]
//            +--[   0]----Z
// C--[NOR ]--+
// D--[   2]
// 
// In-Ports  : A, B, C, D
// Out-Ports : Z
class NodeZeroDetector : public NodeComplex
{
public:
    //-------------------------------------------------------
    NodeZeroDetector(void)
    {
        auto& gb = getGraphBuilder();
        auto enty  = getEntryNode();
        auto exit  = getExitNode();

        auto nor1 = gb.createNode<NodeNor>("nor1 in Node3Selector");
        auto nor2 = gb.createNode<NodeNor>("nor2 in Node3Selector");
        auto and0 = gb.createNode<NodeAnd>("and0 in Node3Selector");

        gb.outto<bool>(Port(enty, 1), Ports{ Port(nor1, 1) }); // A
        gb.outto<bool>(Port(enty, 2), Ports{ Port(nor1, 2) }); // B
        gb.outto<bool>(Port(enty, 3), Ports{ Port(nor2, 1) }); // C
        gb.outto<bool>(Port(enty, 4), Ports{ Port(nor2, 2) }); // D

        gb.outto<bool>(Port(nor1, 1), Ports{ Port(and0, 1) });
        gb.outto<bool>(Port(nor2, 1), Ports{ Port(and0, 2) });

        gb.outto<bool>(Port(and0, 1), Ports{ Port(exit, 1) }); // Z

        commit();
    }
};


//-----------------------------------------------------------
// <部品ノード>
// 2Input/2Selector - 1Output
// 
// ==[NAND1]--+
// ==[NAND2]--+--[NAND0]--
// 
// In-Ports  : S0, S1, D0, D1
// Out-Ports : Q
class Node2Selector : public NodeComplex
{
public:
    //-------------------------------------------------------
    Node2Selector(void)
    {
        auto& gb = getGraphBuilder();
        auto enty  = getEntryNode();
        auto exit  = getExitNode();

        auto nand0 = gb.createNode<NodeNand>("nand0 in Node3Selector");
        auto nand1 = gb.createNode<NodeNand>("nand1 in Node3Selector");
        auto nand2 = gb.createNode<NodeNand>("nand2 in Node3Selector");

        gb.outto<bool>(Port(enty, 1), Ports{ Port(nand1, 1) }); // S0
        gb.outto<bool>(Port(enty, 2), Ports{ Port(nand2, 1) }); // S1

        gb.outto<bool>(Port(enty, 3), Ports{ Port(nand1, 2) }); // D0
        gb.outto<bool>(Port(enty, 4), Ports{ Port(nand2, 2) }); // D1

        gb.outto<bool>(Port(nand1, 1), Ports{ Port(nand0, 1) });
        gb.outto<bool>(Port(nand2, 1), Ports{ Port(nand0, 2) });

        gb.outto<bool>(Port(nand0, 1), Ports{ Port(exit, 1) }); // Q

        commit();
    }
};


//-----------------------------------------------------------
// <部品ノード>
// Input/3Selector - 1Output
// 
// ==[NAND1]--+
// ==[NAND2]--+--[NAND0]--
// ==[NAND3]--+
// 
// In-Ports  : S0, S1, S2, D0, D1, D2
// Out-Ports : Q
class Node3Selector : public NodeComplex
{
public:
    //-------------------------------------------------------
    Node3Selector(void)
    {
        auto& gb = getGraphBuilder();
        auto enty  = getEntryNode();
        auto exit  = getExitNode();

        auto nand0 = gb.createNode<NodeNand>("nand0 in Node3Selector");
        auto nand1 = gb.createNode<NodeNand>("nand1 in Node3Selector");
        auto nand2 = gb.createNode<NodeNand>("nand2 in Node3Selector");
        auto nand3 = gb.createNode<NodeNand>("nand3 in Node3Selector");

        gb.outto<bool>(Port(enty, 1), Ports{ Port(nand1, 1) }); // S0
        gb.outto<bool>(Port(enty, 2), Ports{ Port(nand2, 1) }); // S1
        gb.outto<bool>(Port(enty, 3), Ports{ Port(nand3, 1) }); // S2

        gb.outto<bool>(Port(enty, 4), Ports{ Port(nand1, 2) }); // D0
        gb.outto<bool>(Port(enty, 5), Ports{ Port(nand2, 2) }); // D1
        gb.outto<bool>(Port(enty, 6), Ports{ Port(nand3, 2) }); // D2

        gb.outto<bool>(Port(nand1, 1), Ports{ Port(nand0, 1) });
        gb.outto<bool>(Port(nand2, 1), Ports{ Port(nand0, 2) });
        gb.outto<bool>(Port(nand3, 1), Ports{ Port(nand0, 3) });

        gb.outto<bool>(Port(nand0, 1), Ports{ Port(exit, 1) }); // Q

        commit();
    }
};

//-----------------------------------------------------------
// <部品ノード>
// UP/LD Selector
// 
// UP--+--[NOT1]--+--[AND ]
//     |          +--[   1]----HOLD
//     +-------------[AND ]
// LD--+--[NOT2]--+--[   2]----UP_EN
//     +-----------------------LD
// 
// In-Ports  : UP, LD
// Out-Ports : HOLD, UP_EN, LD
class NodeUpLdSelector : public NodeComplex
{
public:
    //-------------------------------------------------------
    NodeUpLdSelector(void)
    {
        auto& gb = getGraphBuilder();
        auto enty  = getEntryNode();
        auto exit  = getExitNode();

        auto not1 = gb.createNode<NodeNot>("not1 in NodeUpLdSelector");
        auto not2 = gb.createNode<NodeNot>("not2 in NodeUpLdSelector");
        auto and1 = gb.createNode<NodeAnd>("and1 in NodeUpLdSelector");
        auto and2 = gb.createNode<NodeAnd>("and2 in NodeUpLdSelector");

        gb.outto<bool>(Port(enty, 1), Ports{ Port(not1, 1), Port(and2, 1) }); // UP
        gb.outto<bool>(Port(enty, 2), Ports{ Port(not2, 1), Port(exit, 3) }); // LD
        gb.outto<bool>(Port(not1, 1), Ports{ Port(and1, 1)                });
        gb.outto<bool>(Port(not2, 1), Ports{ Port(and1, 2), Port(and2, 2) });
        gb.outto<bool>(Port(and1, 1), Ports{ Port(exit, 1)                }); // HOLD
        gb.outto<bool>(Port(and2, 1), Ports{ Port(exit, 2)                }); // UP_EN

        commit();
    }
};






//-----------------------------------------------------------
// Register
// In-Ports  : RST, CK, LD, D0, D1, D2, D3
// Out-Ports : Q0, Q1, Q2, Q3
class NodeRegister : public NodeComplex
{
public:
    //-------------------------------------------------------
    NodeRegister(void)
    {
        auto& gb = getGraphBuilder();
        auto enty  = getEntryNode();
        auto exit  = getExitNode();

        auto reg0 = gb.createNode<Node4bitRegister>("4bit reg in Register");
        auto mul0 = gb.createNode<NodeMultiplexer>("MUX0 in Register");
        auto mul1 = gb.createNode<NodeMultiplexer>("MUX1 in Register");
        auto mul2 = gb.createNode<NodeMultiplexer>("MUX2 in Register");
        auto mul3 = gb.createNode<NodeMultiplexer>("MUX3 in Register");

        gb.outto<bool>(Port(enty, 1), Ports{ Port(reg0, 1) }); // RST
        gb.outto<bool>(Port(enty, 2), Ports{ Port(reg0, 2) }); // CK

        gb.outto<bool>(Port(enty, 3), Ports{ Port(mul0, 1), Port(mul1, 1), Port(mul2, 1), Port(mul3, 1), }); // LD

        gb.outto<bool>(Port(enty, 4), Ports{ Port(mul0, 3) }); // D0
        gb.outto<bool>(Port(enty, 5), Ports{ Port(mul1, 3) }); // D1
        gb.outto<bool>(Port(enty, 6), Ports{ Port(mul2, 3) }); // D2
        gb.outto<bool>(Port(enty, 7), Ports{ Port(mul3, 3) }); // D3

        gb.outto<bool>(Port(mul0, 1), Ports{ Port(reg0, 3) }); // D0
        gb.outto<bool>(Port(mul1, 1), Ports{ Port(reg0, 4) }); // D1
        gb.outto<bool>(Port(mul2, 1), Ports{ Port(reg0, 5) }); // D2
        gb.outto<bool>(Port(mul3, 1), Ports{ Port(reg0, 6) }); // D3

        gb.outto<bool>(Port(reg0, 1), Ports{ Port(mul0, 2), Port(exit, 1) }); // Q0
        gb.outto<bool>(Port(reg0, 2), Ports{ Port(mul1, 2), Port(exit, 2) }); // Q1
        gb.outto<bool>(Port(reg0, 3), Ports{ Port(mul2, 2), Port(exit, 3) }); // Q2
        gb.outto<bool>(Port(reg0, 4), Ports{ Port(mul3, 2), Port(exit, 4) }); // Q3

        commit();
    }
};





//-----------------------------------------------------------
// PC (Program Counter)
// In-Ports  : RST, CK, LD, UP, D0, D1, D2, D3
// Out-Ports : Q0, Q1, Q2, Q3
class NodePC : public NodeComplex
{
public:
    //-------------------------------------------------------
    NodePC(void)
    {
        auto& gb = getGraphBuilder();
        auto enty  = getEntryNode();
        auto exit  = getExitNode();

        auto reg4 = gb.createNode<Node4bitRegister>("4bit register in PC");
        auto had1 = gb.createNode<NodeHalfAdder>("Half Addr1 in PC");
        auto had2 = gb.createNode<NodeHalfAdder>("Half Addr2 in PC");
        auto exor = gb.createNode<NodeExor>("Exor in PC");
        auto not1 = gb.createNode<NodeNot>("Not in PC");

        auto upld = gb.createNode<NodeUpLdSelector>("UPLD selector in PC");
        auto sel0 = gb.createNode<Node3Selector>("3Selector0 selector in PC");
        auto sel1 = gb.createNode<Node3Selector>("3Selector1 selector in PC");
        auto sel2 = gb.createNode<Node3Selector>("3Selector2 selector in PC");
        auto sel3 = gb.createNode<Node3Selector>("3Selector3 selector in PC");

        gb.outto<bool>(Port(enty, 1), Ports{ Port(reg4, 1) }); // RST
        gb.outto<bool>(Port(enty, 2), Ports{ Port(reg4, 2) }); // CK
        gb.outto<bool>(Port(enty, 3), Ports{ Port(upld, 2) }); // LD
        gb.outto<bool>(Port(enty, 4), Ports{ Port(upld, 1) }); // UP

        gb.outto<bool>(Port(upld, 1), Ports{ Port(sel0, 1), Port(sel1, 1), Port(sel2, 1), Port(sel3, 1) }); // HOLD
        gb.outto<bool>(Port(upld, 2), Ports{ Port(sel0, 2), Port(sel1, 2), Port(sel2, 2), Port(sel3, 2) }); // UP_EN
        gb.outto<bool>(Port(upld, 3), Ports{ Port(sel0, 3), Port(sel1, 3), Port(sel2, 3), Port(sel3, 3) }); // LD

        gb.outto<bool>(Port(enty, 5), Ports{ Port(sel0, 6) }); // D0
        gb.outto<bool>(Port(enty, 6), Ports{ Port(sel1, 6) }); // D1
        gb.outto<bool>(Port(enty, 7), Ports{ Port(sel2, 6) }); // D2
        gb.outto<bool>(Port(enty, 8), Ports{ Port(sel3, 6) }); // D3

        gb.outto<bool>(Port(sel0, 1), Ports{ Port(reg4, 3) }); // D0 to 4bit reg
        gb.outto<bool>(Port(sel1, 1), Ports{ Port(reg4, 4) }); // D1 to 4bit reg
        gb.outto<bool>(Port(sel2, 1), Ports{ Port(reg4, 5) }); // D2 to 4bit reg
        gb.outto<bool>(Port(sel3, 1), Ports{ Port(reg4, 6) }); // D3 to 4bit reg

        gb.outto<bool>(Port(not1, 1), Ports{ Port(sel0, 5) });
        gb.outto<bool>(Port(had1, 1), Ports{ Port(sel1, 5) });
        gb.outto<bool>(Port(had2, 1), Ports{ Port(sel2, 5) });
        gb.outto<bool>(Port(exor, 1), Ports{ Port(sel3, 5) });

        gb.outto<bool>(Port(had1, 2), Ports{ Port(had2, 2) });
        gb.outto<bool>(Port(had2, 2), Ports{ Port(exor, 2) });

        gb.outto<bool>(Port(reg4, 1), Ports{ Port(exit, 1), Port(sel0, 4), Port(had1, 2), Port(not1, 1) }); // Q0
        gb.outto<bool>(Port(reg4, 2), Ports{ Port(exit, 2), Port(sel1, 4), Port(had1, 1) }); // Q1
        gb.outto<bool>(Port(reg4, 3), Ports{ Port(exit, 3), Port(sel2, 4), Port(had2, 1) }); // Q2
        gb.outto<bool>(Port(reg4, 4), Ports{ Port(exit, 4), Port(sel3, 4), Port(exor, 1) }); // Q3

        commit();
    }
};


//-----------------------------------------------------------
// 出力ポートはこのエミュレータ上はレジスタと同じ。
using NodeOutPort = NodeRegister;

//-----------------------------------------------------------
// 入力ポートはこのエミュレータ上は単に入力値を出力値に送るだけのもの
using NodeInPort = NodeTransparent;


//-----------------------------------------------------------
// ALU
// 但し、OE(Output Enabler)は設けない。本来は出力にトライステート・バッファを設けて
// OEで出力制御をしているが、本エミュレータでは、上位でバスを共有するところで
// MUXによる入力選択を行うようにする。
// 
//             1    2   3   4   5      6      7   8   9   10  11  12  13  14
// In-Ports  : RST, CK, LD, AS, MUX_A, MUX_B, A0, A1, A2, A3, B0, B1, B2, B3
//             1   2   3   4   5  6
// Out-Ports : Q0, Q1, Q2, Q3, C, Z
class NodeALU : public NodeComplex
{
public:
    //-------------------------------------------------------
    NodeALU(void)
    {
        auto& gb = getGraphBuilder();
        auto enty  = getEntryNode();
        auto exit  = getExitNode();

        auto reg4 = gb.createNode<NodeAddSub4bit>("4bit Add/Sub in ALU");

        auto andA0 = gb.createNode<NodeAnd>("and in ALU");
        auto andB0 = gb.createNode<NodeAnd>("and in ALU");
        auto andA1 = gb.createNode<NodeAnd>("and in ALU");
        auto andB1 = gb.createNode<NodeAnd>("and in ALU");
        auto andA2 = gb.createNode<NodeAnd>("and in ALU");
        auto andB2 = gb.createNode<NodeAnd>("and in ALU");
        auto andA3 = gb.createNode<NodeAnd>("and in ALU");
        auto andB3 = gb.createNode<NodeAnd>("and in ALU");

        auto zero0 = gb.createNode<NodeZeroDetector>("NodeZeroDetector in ALU");
        auto mulc = gb.createNode<NodeMultiplexer>("MUX in ALU");
        auto mulz = gb.createNode<NodeMultiplexer>("MUX in ALU");

        auto dffc = gb.createNode<NodeDFlipFlopEdgeTriggerAsyncReset>("DFlipFlop in ALU");
        auto dffz = gb.createNode<NodeDFlipFlopEdgeTriggerAsyncReset>("DFlipFlop in ALU");

        gb.outto<bool>(Port(enty, 4), Ports{ Port(reg4, 9) }); // AS->Add/Sub

        gb.outto<bool>(Port(enty,  5), Ports{ Port(andA0, 2), Port(andA1, 2), Port(andA2, 2), Port(andA3, 2), }); // MUX_A
        gb.outto<bool>(Port(enty,  7), Ports{ Port(andA0, 1) }); // A0
        gb.outto<bool>(Port(enty,  8), Ports{ Port(andA1, 1) }); // A1
        gb.outto<bool>(Port(enty,  9), Ports{ Port(andA2, 1) }); // A2
        gb.outto<bool>(Port(enty, 10), Ports{ Port(andA3, 1) }); // A3

        gb.outto<bool>(Port(enty,  6), Ports{ Port(andB0, 2), Port(andB1, 2), Port(andB2, 2), Port(andB3, 2), }); // MUX_B
        gb.outto<bool>(Port(enty, 11), Ports{ Port(andB0, 1) }); // B0
        gb.outto<bool>(Port(enty, 12), Ports{ Port(andB1, 1) }); // B1
        gb.outto<bool>(Port(enty, 13), Ports{ Port(andB2, 1) }); // B2
        gb.outto<bool>(Port(enty, 14), Ports{ Port(andB3, 1) }); // B3

        gb.outto<bool>(Port(andA0, 1), Ports{ Port(reg4, 1) }); // A0->Add/Sub
        gb.outto<bool>(Port(andA1, 1), Ports{ Port(reg4, 2) }); // A1->Add/Sub
        gb.outto<bool>(Port(andA2, 1), Ports{ Port(reg4, 3) }); // A2->Add/Sub
        gb.outto<bool>(Port(andA3, 1), Ports{ Port(reg4, 4) }); // A3->Add/Sub

        gb.outto<bool>(Port(andB0, 1), Ports{ Port(reg4, 5) }); // B0->Add/Sub
        gb.outto<bool>(Port(andB1, 1), Ports{ Port(reg4, 6) }); // B1->Add/Sub
        gb.outto<bool>(Port(andB2, 1), Ports{ Port(reg4, 7) }); // B2->Add/Sub
        gb.outto<bool>(Port(andB3, 1), Ports{ Port(reg4, 8) }); // B3->Add/Sub


        gb.outto<bool>(Port(enty, 1), Ports{ Port(dffz, 1), Port(dffc, 1) }); // RST
        gb.outto<bool>(Port(enty, 2), Ports{ Port(dffz, 2), Port(dffc, 2) }); // CK
        gb.outto<bool>(Port(enty, 3), Ports{ Port(mulz, 1), Port(mulc, 1) }); // LD

        gb.outto<bool>(Port(reg4, 1), Ports{ Port(zero0, 1), Port(exit, 1) }); // S0
        gb.outto<bool>(Port(reg4, 2), Ports{ Port(zero0, 2), Port(exit, 2) }); // S1
        gb.outto<bool>(Port(reg4, 3), Ports{ Port(zero0, 3), Port(exit, 3) }); // S2
        gb.outto<bool>(Port(reg4, 4), Ports{ Port(zero0, 4), Port(exit, 4) }); // S3

        gb.outto<bool>(Port(zero0, 1), Ports{ Port(mulz, 3) }); // Z
        gb.outto<bool>(Port(reg4, 5), Ports{ Port(mulc, 3) });  // C

        gb.outto<bool>(Port(mulc, 1), Ports{ Port(dffc, 3) }); // C->D-FF
        gb.outto<bool>(Port(mulz, 1), Ports{ Port(dffz, 3) }); // Z->D-FF

        gb.outto<bool>(Port(dffc, 1), Ports{ Port(mulc, 2), Port(exit, 5) }); // C
        gb.outto<bool>(Port(dffz, 1), Ports{ Port(mulz, 2), Port(exit, 6) }); // Z

        commit();
    }
};


//-----------------------------------------------------------
// STATE MACHINE
// 本エミュレータではクロックとリセット回路はStateMachineには含めない。
// クロック、リセットは通常のNodeとしての実現が難しいため、疑似Nodeとして
// 別で設け、それらの出力を本StateMachineに入力することとする。
//             1    2   3     4
// In-Ports  : RST, CK, HALT, PC_LD
//             1    2   3      4
// Out-Ports : RST, CK, PC_UP, LD_EN
class NodeStateMachine : public NodeComplex
{
public:
    //-------------------------------------------------------
    NodeStateMachine(void)
    {
        auto& gb = getGraphBuilder();
        auto enty  = getEntryNode();
        auto exit  = getExitNode();

        auto dff1 = gb.createNode<NodeDFlipFlopEdgeTriggerAsyncReset>("DFF1 in SM");
        auto dff2 = gb.createNode<NodeDFlipFlopEdgeTriggerAsyncReset>("DFF2 in SM");
        auto nor1 = gb.createNode<NodeNor>("nor1 in SM");
        auto nor2 = gb.createNode<NodeNor>("nor2 in SM");
        auto and1 = gb.createNode<NodeAnd>("and1 in SM");

        gb.outto<bool>(Port(enty, 1), Ports{ Port(dff1, 1), Port(dff2, 1), Port(exit, 1) }); // RST
        gb.outto<bool>(Port(enty, 2), Ports{ Port(and1, 1) }); // CK
        gb.outto<bool>(Port(enty, 3), Ports{ Port(nor1, 1) }); // HALT
        gb.outto<bool>(Port(enty, 4), Ports{ Port(nor2, 1) }); // PC_LD

        gb.outto<bool>(Port(and1, 1), Ports{ Port(dff1, 2), Port(dff2, 2) , Port(exit, 2) }); // CK
        gb.outto<bool>(Port(nor1, 1), Ports{ Port(dff1, 3) }); // D -> DFF1
        gb.outto<bool>(Port(nor2, 1), Ports{ Port(dff2, 3) }); // D -> DFF2

        gb.outto<bool>(Port(dff1, 2), Ports{ Port(and1, 2) }); // HALT-inv
        gb.outto<bool>(Port(dff2, 1), Ports{ Port(nor1, 2), Port(nor2, 2), Port(exit, 3) }); // PC_UP
        gb.outto<bool>(Port(dff2, 2), Ports{ Port(exit, 4) }); // LD_EN

        commit();
    }
};


//-----------------------------------------------------------
// ID (Instruction Decorder)
//             1   2   3   4   5  6  7
// In-Ports  : D0, D1, D2, D3, C, Z, LD_EN
//             
//             1       2      3     4       5       6       7          8          9         10        11        12      13   
// Out-Ports : ROM_OE, PC_LD, HALT, ALU_LD, ALU_OE, ALU_AS, ALU_MUX_A, ALU_MUX_B, A_REG_LD, B_REG_LD, B_REG_OE, OUT_LD, IN_OE
// 
class NodeID : public NodeComplex
{
public:
    //-------------------------------------------------------
    NodeID(void)
    {
        auto& gb = getGraphBuilder();
        auto enty  = getEntryNode();
        auto exit  = getExitNode();

        auto not01 = gb.createNode<NodeNot>("not1 in ID");
        auto not02 = gb.createNode<NodeNot>("not2 in ID");
        auto not03 = gb.createNode<NodeNot>("not3 in ID");
        auto not04 = gb.createNode<NodeNot>("not4 in ID");
        auto not05 = gb.createNode<NodeNot>("not5 in ID");
        auto not06 = gb.createNode<NodeNot>("not6 in ID");
        auto not07 = gb.createNode<NodeNot>("not7 in ID");

        auto nor01 = gb.createNode<NodeNor>("nor01 in ID");
        auto nor02 = gb.createNode<NodeNor>("nor02 in ID");
        auto nor03 = gb.createNode<NodeNor>("nor03 in ID");
        auto nor04 = gb.createNode<NodeNor>("nor04 in ID");
        auto nor05 = gb.createNode<NodeNor>("nor05 in ID");
        auto nor06 = gb.createNode<NodeNor>("nor06 in ID");
        auto nor07 = gb.createNode<NodeNor>("nor07 in ID");
        auto nor08 = gb.createNode<NodeNor>("nor08 in ID");
        auto nor09 = gb.createNode<NodeNor>("nor09 in ID");
        auto nor10 = gb.createNode<NodeNor>("nor10 in ID");
        auto nor11 = gb.createNode<NodeNor>("nor11 in ID");
        auto nor12 = gb.createNode<NodeNor>("nor12 in ID");
        auto nor13 = gb.createNode<NodeNor>("nor13 in ID");
        auto nor14 = gb.createNode<NodeNor>("nor14 in ID");
        auto nor15 = gb.createNode<NodeNor>("nor15 in ID");
        auto nor16 = gb.createNode<NodeNor>("nor16 in ID");
        auto nor17 = gb.createNode<NodeNor>("nor17 in ID");
        auto nor18 = gb.createNode<NodeNor>("nor18 in ID");
        auto nor19 = gb.createNode<NodeNor>("nor19 in ID");
        auto nor20 = gb.createNode<NodeNor>("nor20 in ID");
        auto nor21 = gb.createNode<NodeNor>("nor21 in ID");

        auto nand01 = gb.createNode<NodeNand>("nand01 in ID");
        auto nand02 = gb.createNode<NodeNand>("nand02 in ID");
        auto nand03 = gb.createNode<NodeNand>("nand03 in ID");
        auto nand04 = gb.createNode<NodeNand>("nand04 in ID");
        auto nand05 = gb.createNode<NodeNand>("nand05 in ID");
        auto nand06 = gb.createNode<NodeNand>("nand06 in ID");
        auto nand07 = gb.createNode<NodeNand>("nand07 in ID");
        auto nand08 = gb.createNode<NodeNand>("nand08 in ID");
        auto nand09 = gb.createNode<NodeNand>("nand09 in ID");
        auto nand10 = gb.createNode<NodeNand>("nand10 in ID");
        auto nand11 = gb.createNode<NodeNand>("nand11 in ID");
        auto nand12 = gb.createNode<NodeNand>("nand12 in ID");
        auto nand13 = gb.createNode<NodeNand>("nand13 in ID");
        auto nand14 = gb.createNode<NodeNand>("nand14 in ID");
        auto nand15 = gb.createNode<NodeNand>("nand15 in ID");

        auto and01 = gb.createNode<NodeAnd>("and01 in ID");
        auto and02 = gb.createNode<NodeAnd>("and02 in ID");
        auto and03 = gb.createNode<NodeAnd>("and03 in ID");
        auto and04 = gb.createNode<NodeAnd>("and04 in ID");
        auto and05 = gb.createNode<NodeAnd>("and05 in ID");

        auto or01  = gb.createNode<NodeOr>("or01 in ID");
        auto or02  = gb.createNode<NodeOr>("or02 in ID");

        gb.outto<bool>(Port(enty,   1), Ports{ Port(not04,   1), Port(nor01,  3), Port(nand02, 2), Port(nand05, 2), Port(nand10, 2), Port(nor19,  2), Port(nor21, 2) }); // D0
        gb.outto<bool>(Port(enty,   2), Ports{ Port(not03,   1), Port(nand02, 1), Port(nor07,  3), Port(nor09,  3), Port(nand05, 1), Port(nand10, 1), Port(nor19, 1), Port(nor20, 1)  }); // D1
        gb.outto<bool>(Port(enty,   3), Ports{ Port(not02,   1), Port(nor01,  2), Port(nor05,  2), Port(nor06,  2), Port(nor09,  2), Port(nand08, 2), Port(nor16, 2) }); // D2
        gb.outto<bool>(Port(enty,   4), Ports{ Port(not01,   1), Port(nor01,  1), Port(nor02,  1), Port(nand01, 1), Port(nor05,  1), Port(nor06,  1), Port(nor07, 1), Port(nor11, 1), Port(nor12, 1), Port(nand06, 1) }); // D3

        gb.outto<bool>(Port(enty,   5), Ports{ Port(or01,    2) }); // C
        gb.outto<bool>(Port(enty,   6), Ports{ Port(or02,    2) }); // Z
        gb.outto<bool>(Port(enty,   7), Ports{ Port(and01,   2),  Port(and02, 2),  Port(and03, 2), Port(and05,  2) }); // LD_EN

        gb.outto<bool>(Port(not04,  1), Ports{ Port(nor05,   3), Port(nor11,  3), Port(nand07, 2), Port(nor18, 2),  Port(nor20, 2) }); // D0-inv
        gb.outto<bool>(Port(not03,  1), Ports{ Port(nor06,   3), Port(nand07, 1), Port(nor18,  1), Port(nor21, 1) }); // D1-inv
        gb.outto<bool>(Port(not02,  1), Ports{ Port(nor02,   2), Port(nand01, 2), Port(nor07,  2), Port(nor11, 2),  Port(nor12, 2), Port(nand04, 2), Port(nand06, 2), Port(nor17, 2) }); // D2-inv
        gb.outto<bool>(Port(not01,  1), Ports{ Port(nor09,   1), Port(nand04, 1), Port(nand08, 1), Port(nor16, 1),  Port(nor17, 1) }); // D3-inv

        gb.outto<bool>(Port(nor01,  1), Ports{ Port(nor04,   1) });
        gb.outto<bool>(Port(nor02,  1), Ports{ Port(nor04,   2) });
        gb.outto<bool>(Port(nand01, 1), Ports{ Port(nor03,   1) });
        gb.outto<bool>(Port(nand02, 1), Ports{ Port(nor03,   2) });
        gb.outto<bool>(Port(nor03,  1), Ports{ Port(nor04,   3), Port(not06, 1), Port(exit, 13), Port(nor10, 2) }); // IN_OE
        gb.outto<bool>(Port(nor04,  1), Ports{ Port(not05,   1) });
        gb.outto<bool>(Port(not05,  1), Ports{ Port(and01,   1) });
        gb.outto<bool>(Port(and01,  1), Ports{ Port(exit,    9) }); // A_REG_LD
        gb.outto<bool>(Port(not06,  1), Ports{ Port(exit,    5) }); // ALU_OE

        gb.outto<bool>(Port(nor05,  1), Ports{ Port(and02,   1) });
        gb.outto<bool>(Port(and02,  1), Ports{ Port(exit,   10) }); // B_REG_LD

        gb.outto<bool>(Port(nor06,  1), Ports{ Port(nor08,   1) });
        gb.outto<bool>(Port(nor07,  1), Ports{ Port(nor08,   2) });
        gb.outto<bool>(Port(nor09,  1), Ports{ Port(nor10,   1) });
        gb.outto<bool>(Port(nor08,  1), Ports{ Port(nand03,  1) });
        gb.outto<bool>(Port(nor10,  1), Ports{ Port(nand03,  2) });
        gb.outto<bool>(Port(nand03, 1), Ports{ Port(not07,   1), Port(exit, 11) }); // B_REG_OE
        gb.outto<bool>(Port(not07,  1), Ports{ Port(exit,    1) }); // ROM_OE

        gb.outto<bool>(Port(nor11,  1),  Ports{ Port(exit,   6) }); // ALU_AS

        gb.outto<bool>(Port(nor12,  1),  Ports{ Port(and03,  1) });
        gb.outto<bool>(Port(and03,  1),  Ports{ Port(exit,   4) }); // ALU_LD

        gb.outto<bool>(Port(nand04, 1), Ports{ Port(nor13,   1) });
        gb.outto<bool>(Port(nand05, 1), Ports{ Port(nor13,   2) });
        gb.outto<bool>(Port(nand06, 1), Ports{ Port(nor14,   1) });
        gb.outto<bool>(Port(nand07, 1), Ports{ Port(nor14,   2) });
        gb.outto<bool>(Port(nor13,  1), Ports{ Port(nor15,   1) });
        gb.outto<bool>(Port(nor14,  1), Ports{ Port(nor15,   2) });
        gb.outto<bool>(Port(nor15,  1), Ports{ Port(exit,    8) , Port(nand09, 2)}); // ALU_MUX_B

        gb.outto<bool>(Port(nand08, 1), Ports{ Port(nand09,  1) });
        gb.outto<bool>(Port(nand09, 1), Ports{ Port(exit,    7) }); // ALU_MUX_A

        gb.outto<bool>(Port(nor16,  1), Ports{ Port(and04,   1) });
        gb.outto<bool>(Port(nand10, 1), Ports{ Port(and04,   2) });
        gb.outto<bool>(Port(and04,  1), Ports{ Port(and05,   1) });
        gb.outto<bool>(Port(and05,  1), Ports{ Port(exit,   12) }); // OUT_LD

        gb.outto<bool>(Port(nor17,  1),  Ports{ Port(nand11, 1), Port(nand12, 2), Port(nand13, 2), Port(nand14, 2)  });
        gb.outto<bool>(Port(nor18,  1),  Ports{ Port(nand11, 2) });
        gb.outto<bool>(Port(nand11, 1), Ports{ Port(exit,    3) }); // HALT

        gb.outto<bool>(Port(nor19,  1), Ports{ Port(nand12,  1) });
        gb.outto<bool>(Port(nor20,  1), Ports{ Port(nand13,  1) });
        gb.outto<bool>(Port(nor21,  1), Ports{ Port(nand14,  1) });
        gb.outto<bool>(Port(nand13, 1), Ports{ Port(or01,    1) });
        gb.outto<bool>(Port(nand14, 1), Ports{ Port(or02,    1) });
        gb.outto<bool>(Port(nand12, 1), Ports{ Port(nand15,  1) });
        gb.outto<bool>(Port(or01,   1), Ports{ Port(nand15,  2) });
        gb.outto<bool>(Port(or02,   1), Ports{ Port(nand15,  3) });
        gb.outto<bool>(Port(nand15, 1), Ports{ Port(exit,    2) }); // PC_LD

        commit();
    }
};


#endif

/*
IN_OEの出力
D0,D1,D2,D3が1,1,0,1の時のみ、1となる。
資料のオペコードは1011とあり、
オペコードのビットの並びは
D3,D2,D1,D0
であることがわかる。

 D
0 1 2 3   NAND(D0,D1) NAND(NOT(D2),D3) NOR
0 0 0 0 | 1           1                0
0 0 0 1 | 1           0                0
0 0 1 0 | 1           1                0
0 0 1 1 | 1           1                0
0 1 0 0 | 1           1                0
0 1 0 1 | 1           0                0
0 1 1 0 | 1           1                0
0 1 1 1 | 1           1                0
1 0 0 0 | 1           1                0
1 0 0 1 | 1           0                0
1 0 1 0 | 1           1                0
1 0 1 1 | 1           1                0
1 1 0 0 | 0           1                0
1 1 0 1 | 0           0                1
1 1 1 0 | 0           1                0
1 1 1 1 | 0           1                0


*/