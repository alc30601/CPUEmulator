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
//                                  |-> Probe
//             1   2   3   4   5    | ...
// Out-Ports : O0, O1, O2, O3, HALT | ...
// 
// Probe list
//  1 : RST
//  2 : CK
//  3 : Address 0
//  4 : Address 1
//  5 : Address 2
//  6 : Address 3
// 
//  7 : A-REG LD
//  8 : B-REG LD
//  9 : ROM OE
// 10 : ALU A/S
// 11 : ALU OE
// 12 : ALU LD
// 13 : ALU MUX A
// 14 : ALU MUX B
// 15 : OUT LD
// 16 : HALT
// 
// 17 : ALU-Q0
// 18 : ALU-Q1
// 19 : ALU-Q2
// 20 : ALU-Q3
// 
// 21 : A-REG0
// 22 : A-REG1
// 23 : A-REG2
// 24 : A-REG3
// 
// 25 : B-REG0
// 26 : B-REG1
// 27 : B-REG2
// 28 : B-REG3
// 
// 
class NodeCPU : public NodeComplex
{
    //-------------------------------------------------------
    NodeROM* _rom; // ROMは外部からデータを更新することがあるため、自分で保持しておく。

public:

    static const int PROBE = 5; // Probeの開始はPROBE+1から。

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
        gb.outto<bool>(Port(enty, 1), Ports{ Port(sm, 1) }); // RST
        gb.outto<bool>(Port(enty, 2), Ports{ Port(sm, 2) }); // CK

        gb.outto<bool>(Port(enty, 3), Ports{ Port(inp, 1) }); // I0
        gb.outto<bool>(Port(enty, 4), Ports{ Port(inp, 2) }); // I0
        gb.outto<bool>(Port(enty, 5), Ports{ Port(inp, 3) }); // I0
        gb.outto<bool>(Port(enty, 6), Ports{ Port(inp, 4) }); // I0

        // OUTPUT
        gb.outto<bool>(Port(outp, 1), Ports{ Port(exit, 1) }); // O0
        gb.outto<bool>(Port(outp, 2), Ports{ Port(exit, 2) }); // O1
        gb.outto<bool>(Port(outp, 3), Ports{ Port(exit, 3) }); // O2
        gb.outto<bool>(Port(outp, 4), Ports{ Port(exit, 4) }); // O3

        // From STATE MACHINE
        gb.outto<bool>(Port(sm, 1), Ports{ Port(pc, 1), Port(rega, 1), Port(regb, 1), Port(alu, 1), Port(outp, 1), Port(exit, PROBE+1) }); // RST
        gb.outto<bool>(Port(sm, 2), Ports{ Port(pc, 2), Port(rega, 2), Port(regb, 2), Port(alu, 2), Port(outp, 2), Port(exit, PROBE+2)  }); // CK
        gb.outto<bool>(Port(sm, 3), Ports{ Port(pc, 4) }); // PC_UP
        gb.outto<bool>(Port(sm, 4), Ports{ Port(id, 7) }); // LD_EN
 
        // From ID
        // B_REG_OEの出力は使用しない。ROM_OEを使用する。
        // B_REG_OEとROM_OEは相反。ROM_OEだけを使い、ROM_OEの逆をB_REG_OEと考える。
        // IN_OEの出力は使用しない。ALU_OEを使用する。
        // ALU_OEとIN_OEは相反。ALU_OEだけを使い、ALU_OEの逆をIN_OEと考える。
        gb.outto<bool>(Port(id,  1), Ports{ Port(mux1, 1), Port(exit, PROBE+9) }); // ROM_OE / B_REG_OE-inv
        gb.outto<bool>(Port(id,  2), Ports{ Port(sm,   4), Port(pc, 3) }); // PC_LD
        gb.outto<bool>(Port(id,  3), Ports{ Port(sm,   3), Port(exit, 5), Port(exit, PROBE+16) }); // HALT
        gb.outto<bool>(Port(id,  4), Ports{ Port(alu,  3), Port(exit, PROBE+12) }); // ALU_LD
        gb.outto<bool>(Port(id,  5), Ports{ Port(mux2, 1), Port(exit, PROBE+11) }); // ALU_OE / IN_OE-inv
        gb.outto<bool>(Port(id,  6), Ports{ Port(alu,  4), Port(exit, PROBE+10) }); // ALU_AS
        gb.outto<bool>(Port(id,  7), Ports{ Port(alu,  5), Port(exit, PROBE+13) }); // ALU_MUX_A
        gb.outto<bool>(Port(id,  8), Ports{ Port(alu,  6), Port(exit, PROBE+14) }); // ALU_MUX_B
        gb.outto<bool>(Port(id,  9), Ports{ Port(rega, 3), Port(exit, PROBE+7) }); // A_REG_LD
        gb.outto<bool>(Port(id, 10), Ports{ Port(regb, 3), Port(exit, PROBE+8) }); // B_REG_LD
        gb.outto<bool>(Port(id, 12), Ports{ Port(outp, 3), Port(exit, PROBE+15) }); // OUT_LD

        // From ROM
        gb.outto<bool>(Port(rom, 1), Ports{ Port(mux1, 6) }); // D7 in Bus1
        gb.outto<bool>(Port(rom, 2), Ports{ Port(mux1, 7) }); // D6 in Bus1
        gb.outto<bool>(Port(rom, 3), Ports{ Port(mux1, 8) }); // D5 in Bus1
        gb.outto<bool>(Port(rom, 4), Ports{ Port(mux1, 9) }); // D4 in Bus1
        gb.outto<bool>(Port(rom, 5), Ports{ Port(id,   1) }); // Instruction 0
        gb.outto<bool>(Port(rom, 6), Ports{ Port(id,   2) }); // Instruction 1
        gb.outto<bool>(Port(rom, 7), Ports{ Port(id,   3) }); // Instruction 2
        gb.outto<bool>(Port(rom, 8), Ports{ Port(id,   4) }); // Instruction 3

        // From In Port
        gb.outto<bool>(Port(inp, 1), Ports{ Port(mux2,   2) }); // Q0 in Bus2
        gb.outto<bool>(Port(inp, 2), Ports{ Port(mux2,   3) }); // Q1 in Bus2
        gb.outto<bool>(Port(inp, 3), Ports{ Port(mux2,   4) }); // Q2 in Bus2
        gb.outto<bool>(Port(inp, 4), Ports{ Port(mux2,   5) }); // Q3 in Bus2

        // From PC
        gb.outto<bool>(Port(pc, 1), Ports{ Port(rom,  1), Port(exit, PROBE+3) }); // Address 0
        gb.outto<bool>(Port(pc, 2), Ports{ Port(rom,  2), Port(exit, PROBE+4) }); // Address 1
        gb.outto<bool>(Port(pc, 3), Ports{ Port(rom,  3), Port(exit, PROBE+5) }); // Address 2
        gb.outto<bool>(Port(pc, 4), Ports{ Port(rom,  4), Port(exit, PROBE+6) }); // Address 3

        // From ALU
        gb.outto<bool>(Port(alu, 1), Ports{ Port(mux2,  6), Port(exit, PROBE+17) }); // Q0
        gb.outto<bool>(Port(alu, 2), Ports{ Port(mux2,  7), Port(exit, PROBE+18) }); // Q1
        gb.outto<bool>(Port(alu, 3), Ports{ Port(mux2,  8), Port(exit, PROBE+19) }); // Q2
        gb.outto<bool>(Port(alu, 4), Ports{ Port(mux2,  9), Port(exit, PROBE+20) }); // Q3
        gb.outto<bool>(Port(alu, 5), Ports{ Port(id,    5) }); // C
        gb.outto<bool>(Port(alu, 6), Ports{ Port(id,    6) }); // Z

        // From A Register
        gb.outto<bool>(Port(rega, 1), Ports{ Port(alu,   7), Port(exit, PROBE+21) }); // A-REG 0
        gb.outto<bool>(Port(rega, 2), Ports{ Port(alu,   8), Port(exit, PROBE+22) }); // A-REG 1
        gb.outto<bool>(Port(rega, 3), Ports{ Port(alu,   9), Port(exit, PROBE+23) }); // A-REG 2
        gb.outto<bool>(Port(rega, 4), Ports{ Port(alu,  10), Port(exit, PROBE+24) }); // A-REG 3

        // From B Register
        gb.outto<bool>(Port(regb, 1), Ports{ Port(mux1,  2), Port(exit, PROBE+25) }); // B-REG 0
        gb.outto<bool>(Port(regb, 2), Ports{ Port(mux1,  3), Port(exit, PROBE+26) }); // B-REG 1
        gb.outto<bool>(Port(regb, 3), Ports{ Port(mux1,  4), Port(exit, PROBE+27) }); // B-REG 2
        gb.outto<bool>(Port(regb, 4), Ports{ Port(mux1,  5), Port(exit, PROBE+28) }); // B-REG 3

        // From MUX1(multiplex ROM and B-REG)
        gb.outto<bool>(Port(mux1, 1), Ports{ Port(alu,  11) }); // B0
        gb.outto<bool>(Port(mux1, 2), Ports{ Port(alu,  12) }); // B1
        gb.outto<bool>(Port(mux1, 3), Ports{ Port(alu,  13) }); // B2
        gb.outto<bool>(Port(mux1, 4), Ports{ Port(alu,  14) }); // B3

        // From MUX2(multiplex IN port and ALU)
        gb.outto<bool>(Port(mux2, 1), Ports{ Port(rega, 4), Port(regb, 4), Port(outp, 4), Port(pc, 5) }); // ALU/IN-0
        gb.outto<bool>(Port(mux2, 2), Ports{ Port(rega, 5), Port(regb, 5), Port(outp, 5), Port(pc, 6) }); // ALU/IN-1
        gb.outto<bool>(Port(mux2, 3), Ports{ Port(rega, 6), Port(regb, 6), Port(outp, 6), Port(pc, 7) }); // ALU/IN-2
        gb.outto<bool>(Port(mux2, 4), Ports{ Port(rega, 7), Port(regb, 7), Port(outp, 7), Port(pc, 8) }); // ALU/IN-3

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



