// NodeCPU.hpp
#ifndef __NODECPU_HPP__
#define __NODECPU_HPP__

#include "NodeComplex.hpp"
#include "NodeRegister.hpp"
#include "NodeMultiplexer.hpp"
#include "NodePseudoParts.hpp"


//-----------------------------------------------------------
// CPU全体。
// A/Bレジスタ, ALU, ID(Instruction Decorder), PC(Program Counter), State machine
// ROM, Out Port, In Port から成る。
// Reset信号、クロックは外部から供給する。
// I/Oとして4bitの入出力も外部から入力、外部へ出力する。
// 
// 内部バスは4bit幅で４系統(下記)
// Bus1: 入力 ROM(D0-D3) / B REG
//       出力 ALU
// Bus2: 入力 ALU / IN PORT
//       出力 OUT PORT / PC / B REG / A REG
// Bus3: 入力 ROM(D4-D7)
//       出力 ID
// Bus4: 入力 PC
//       出力 ROM
// ここでBus1, Bus2は入力が複数あるため、4bitのMUXを挿入する。
// 実機ではトライステート・バッファを使用するがエミュレータではMUXを挟む。
// 
//             1    2   3   4   5   6
// In-Ports  : RST, CK, I0, I1, I2, I3
//                             |-> Probe
//             1   2   3   4   | 5   6   ...
// Out-Ports : O0, O1, O2, O3, | P1, P2, ...
//                             | RST CK  ...
// 
//  7 : Address 0
//  8 : Address 1
//  9 : Address 2
// 10 : Address 3
// 
// 11 : A-REG LD
// 12 : B-REG LD
// 13 : ROM OE
// 14 : ALU A/S
// 15 : ALU OE
// 16 : ALU LD
// 17 : ALU MUX A
// 18 : ALU MUX B
// 19 : OUT LD
// 20 : HALT
// 
// 21 : ALU-Q0
// 22 : ALU-Q1
// 23 : ALU-Q2
// 24 : ALU-Q3
// 
// 25 : A-REG0
// 26 : A-REG1
// 27 : A-REG2
// 28 : A-REG3
// 
// 29 : B-REG0
// 30 : B-REG1
// 31 : B-REG2
// 32 : B-REG3
// 
// 
class NodeCPU : public NodeComplex
{
    //-------------------------------------------------------
    NodeROM* _rom; // ROMは外部からデータを更新することがあるため、自分で保持しておく。

public:
    //-------------------------------------------------------
    NodeCPU(void)
    {
        auto& gb = getGraphBuilder();
        auto enty  = getEntryNode();
        auto exit  = getExitNode();

        auto rega = gb.createNode<NodeRegister>("Reg A");
        auto regb = gb.createNode<NodeRegister>("Reg B");
        auto alu  = gb.createNode<NodeALU>("ALU");
        auto id   = gb.createNode<NodeID>("ID");
        auto pc   = gb.createNode<NodePC>("PC");
        auto sm   = gb.createNode<NodeStateMachine>("StateMachine");
        auto outp = gb.createNode<NodeOutPort>("OUT PORT");
        auto inp  = gb.createNode<NodeInPort>("IN PORT");
        auto rom  = gb.createNode<NodeROM>("ROM");
        auto mux1  = gb.createNode<Node4bitMultiplexer>("MUX1"); // For Bus1
        auto mux2  = gb.createNode<Node4bitMultiplexer>("MUX2"); // For Bus2

        _rom = dynamic_cast<NodeROM*>(rom.getNode());


        // INPUT
        gb.outto(Port(enty, 1), Ports{ Port(sm, 1) }, typeid(bool)); // RST
        gb.outto(Port(enty, 2), Ports{ Port(sm, 2) }, typeid(bool)); // CK

        gb.outto(Port(enty, 3), Ports{ Port(inp, 1) }, typeid(bool)); // I0
        gb.outto(Port(enty, 4), Ports{ Port(inp, 2) }, typeid(bool)); // I0
        gb.outto(Port(enty, 5), Ports{ Port(inp, 3) }, typeid(bool)); // I0
        gb.outto(Port(enty, 6), Ports{ Port(inp, 4) }, typeid(bool)); // I0

        // OUTPUT
        gb.outto(Port(outp, 1), Ports{ Port(exit, 1) }, typeid(bool)); // O0
        gb.outto(Port(outp, 2), Ports{ Port(exit, 2) }, typeid(bool)); // O1
        gb.outto(Port(outp, 3), Ports{ Port(exit, 3) }, typeid(bool)); // O2
        gb.outto(Port(outp, 4), Ports{ Port(exit, 4) }, typeid(bool)); // O3

        // From STATE MACHINE
        gb.outto(Port(sm, 1), Ports{ Port(pc, 1), Port(rega, 1), Port(regb, 1), Port(alu, 1), Port(outp, 1), Port(exit, 5) }, typeid(bool)); // RST
        gb.outto(Port(sm, 2), Ports{ Port(pc, 2), Port(rega, 2), Port(regb, 2), Port(alu, 2), Port(outp, 2), Port(exit, 6)  }, typeid(bool)); // CK
        gb.outto(Port(sm, 3), Ports{ Port(pc, 4) }, typeid(bool)); // PC_UP
        gb.outto(Port(sm, 4), Ports{ Port(id, 7) }, typeid(bool)); // LD_EN
 
        // From ID
        // B_REG_OEの出力は使用しない。ROM_OEを使用する。
        // B_REG_OEとROM_OEは相反。ROM_OEだけを使い、ROM_OEの逆をB_REG_OEと考える。
        // IN_OEの出力は使用しない。ALU_OEを使用する。
        // ALU_OEとIN_OEは相反。ALU_OEだけを使い、ALU_OEの逆をIN_OEと考える。
        gb.outto(Port(id,  1), Ports{ Port(mux1, 1), Port(exit, 13) }, typeid(bool)); // ROM_OE / B_REG_OE-inv
        gb.outto(Port(id,  2), Ports{ Port(sm,   4), Port(pc, 3) },    typeid(bool)); // PC_LD
        gb.outto(Port(id,  3), Ports{ Port(sm,   3), Port(exit, 20) }, typeid(bool)); // HALT
        gb.outto(Port(id,  4), Ports{ Port(alu,  3), Port(exit, 16) }, typeid(bool)); // ALU_LD
        gb.outto(Port(id,  5), Ports{ Port(mux2, 1), Port(exit, 15) }, typeid(bool)); // ALU_OE / IN_OE-inv
        gb.outto(Port(id,  6), Ports{ Port(alu,  4), Port(exit, 14) }, typeid(bool)); // ALU_AS
        gb.outto(Port(id,  7), Ports{ Port(alu,  5), Port(exit, 17) }, typeid(bool)); // ALU_MUX_A
        gb.outto(Port(id,  8), Ports{ Port(alu,  6), Port(exit, 18) }, typeid(bool)); // ALU_MUX_B
        gb.outto(Port(id,  9), Ports{ Port(rega, 3), Port(exit, 11) }, typeid(bool)); // A_REG_LD
        gb.outto(Port(id, 10), Ports{ Port(regb, 3), Port(exit, 12) }, typeid(bool)); // B_REG_LD
        gb.outto(Port(id, 12), Ports{ Port(outp, 3), Port(exit, 19) }, typeid(bool)); // OUT_LD

        // From ROM
        gb.outto(Port(rom, 1), Ports{ Port(mux1, 6) }, typeid(bool)); // D7 in Bus1
        gb.outto(Port(rom, 2), Ports{ Port(mux1, 7) }, typeid(bool)); // D6 in Bus1
        gb.outto(Port(rom, 3), Ports{ Port(mux1, 8) }, typeid(bool)); // D5 in Bus1
        gb.outto(Port(rom, 4), Ports{ Port(mux1, 9) }, typeid(bool)); // D4 in Bus1
        gb.outto(Port(rom, 5), Ports{ Port(id,   1) }, typeid(bool)); // Instruction 0
        gb.outto(Port(rom, 6), Ports{ Port(id,   2) }, typeid(bool)); // Instruction 1
        gb.outto(Port(rom, 7), Ports{ Port(id,   3) }, typeid(bool)); // Instruction 2
        gb.outto(Port(rom, 8), Ports{ Port(id,   4) }, typeid(bool)); // Instruction 3

        // From In Port
        gb.outto(Port(inp, 1), Ports{ Port(mux2,   2) }, typeid(bool)); // Q0 in Bus2
        gb.outto(Port(inp, 2), Ports{ Port(mux2,   3) }, typeid(bool)); // Q1 in Bus2
        gb.outto(Port(inp, 3), Ports{ Port(mux2,   4) }, typeid(bool)); // Q2 in Bus2
        gb.outto(Port(inp, 4), Ports{ Port(mux2,   5) }, typeid(bool)); // Q3 in Bus2

        // From PC
        gb.outto(Port(pc, 1), Ports{ Port(rom,  1), Port(exit,  7) }, typeid(bool)); // Address 0
        gb.outto(Port(pc, 2), Ports{ Port(rom,  2), Port(exit,  8) }, typeid(bool)); // Address 1
        gb.outto(Port(pc, 3), Ports{ Port(rom,  3), Port(exit,  9) }, typeid(bool)); // Address 2
        gb.outto(Port(pc, 4), Ports{ Port(rom,  4), Port(exit, 10) }, typeid(bool)); // Address 3

        // From ALU
        gb.outto(Port(alu, 1), Ports{ Port(mux2,  6), Port(exit, 21) }, typeid(bool)); // Q0
        gb.outto(Port(alu, 2), Ports{ Port(mux2,  7), Port(exit, 22) }, typeid(bool)); // Q1
        gb.outto(Port(alu, 3), Ports{ Port(mux2,  8), Port(exit, 23) }, typeid(bool)); // Q2
        gb.outto(Port(alu, 4), Ports{ Port(mux2,  9), Port(exit, 24) }, typeid(bool)); // Q3
        gb.outto(Port(alu, 5), Ports{ Port(id,    5) }, typeid(bool)); // C
        gb.outto(Port(alu, 6), Ports{ Port(id,    6) }, typeid(bool)); // Z

        // From A Register
        gb.outto(Port(rega, 1), Ports{ Port(alu,   7), Port(exit, 25) }, typeid(bool)); // A-REG 0
        gb.outto(Port(rega, 2), Ports{ Port(alu,   8), Port(exit, 26) }, typeid(bool)); // A-REG 1
        gb.outto(Port(rega, 3), Ports{ Port(alu,   9), Port(exit, 27) }, typeid(bool)); // A-REG 2
        gb.outto(Port(rega, 4), Ports{ Port(alu,  10), Port(exit, 28) }, typeid(bool)); // A-REG 3

        // From B Register
        gb.outto(Port(regb, 1), Ports{ Port(mux1,  2), Port(exit, 29) }, typeid(bool)); // B-REG 0
        gb.outto(Port(regb, 2), Ports{ Port(mux1,  3), Port(exit, 30) }, typeid(bool)); // B-REG 1
        gb.outto(Port(regb, 3), Ports{ Port(mux1,  4), Port(exit, 31) }, typeid(bool)); // B-REG 2
        gb.outto(Port(regb, 4), Ports{ Port(mux1,  5), Port(exit, 32) }, typeid(bool)); // B-REG 3

        // From MUX1(multiplex ROM and B-REG)
        gb.outto(Port(mux1, 1), Ports{ Port(alu,  11) }, typeid(bool)); // B0
        gb.outto(Port(mux1, 2), Ports{ Port(alu,  12) }, typeid(bool)); // B1
        gb.outto(Port(mux1, 3), Ports{ Port(alu,  13) }, typeid(bool)); // B2
        gb.outto(Port(mux1, 4), Ports{ Port(alu,  14) }, typeid(bool)); // B3

        // From MUX2(multiplex IN port and ALU)
        gb.outto(Port(mux2, 1), Ports{ Port(rega, 4), Port(regb, 4), Port(outp, 4), Port(pc, 5) }, typeid(bool)); // ALU/IN-0
        gb.outto(Port(mux2, 2), Ports{ Port(rega, 5), Port(regb, 5), Port(outp, 5), Port(pc, 6) }, typeid(bool)); // ALU/IN-1
        gb.outto(Port(mux2, 3), Ports{ Port(rega, 6), Port(regb, 6), Port(outp, 6), Port(pc, 7) }, typeid(bool)); // ALU/IN-2
        gb.outto(Port(mux2, 4), Ports{ Port(rega, 7), Port(regb, 7), Port(outp, 7), Port(pc, 8) }, typeid(bool)); // ALU/IN-3

        commit();
    }

    //-------------------------------------------------------
    // ROMのデータを更新する。
    void setROMData(const bool (&rom)[16][8])
    {
        _rom->setROMData(rom);
    }

};



#endif



