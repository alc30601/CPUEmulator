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
//             1   2   3   4
// Out-Ports : O0, O1, O2, O3
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
        gb.outto(Port(sm, 1), Ports{ Port(pc, 1), Port(rega, 1), Port(regb, 1), Port(alu, 1), Port(outp, 1)  }, typeid(bool)); // RST
        gb.outto(Port(sm, 2), Ports{ Port(pc, 2), Port(rega, 2), Port(regb, 2), Port(alu, 2), Port(outp, 2)  }, typeid(bool)); // CK
        gb.outto(Port(sm, 3), Ports{ Port(pc, 4) }, typeid(bool)); // PC_UP
        gb.outto(Port(sm, 4), Ports{ Port(id, 7) }, typeid(bool)); // LD_EN
 
        // From ID
        // B_REG_OEの出力は使用しない。ROM_OEを使用する。
        // IN_OEの出力は使用しない。ALU_OEを使用する。
        gb.outto(Port(id,  1), Ports{ Port(mux1, 1) }, typeid(bool)); // ROM_OE 
        gb.outto(Port(id,  2), Ports{ Port(sm,   4), Port(pc, 3) }, typeid(bool)); // PC_LD
        gb.outto(Port(id,  3), Ports{ Port(sm,   3) }, typeid(bool)); // HALT
        gb.outto(Port(id,  4), Ports{ Port(alu,  3) }, typeid(bool)); // ALU_LD
        gb.outto(Port(id,  5), Ports{ Port(mux2, 1) }, typeid(bool)); // ALU_OE
        gb.outto(Port(id,  6), Ports{ Port(alu,  4) }, typeid(bool)); // ALU_AS
        gb.outto(Port(id,  7), Ports{ Port(alu,  5) }, typeid(bool)); // ALU_MUX_A
        gb.outto(Port(id,  8), Ports{ Port(alu,  6) }, typeid(bool)); // ALU_MUX_B
        gb.outto(Port(id,  9), Ports{ Port(rega, 3) }, typeid(bool)); // A_REG_LD
        gb.outto(Port(id, 10), Ports{ Port(regb, 3) }, typeid(bool)); // B_REG_LD
        gb.outto(Port(id, 12), Ports{ Port(outp, 3) }, typeid(bool)); // OUT_LD

        // From ROM
        gb.outto(Port(rom, 1), Ports{ Port(mux1, 6) }, typeid(bool)); // D0 in Bus1
        gb.outto(Port(rom, 2), Ports{ Port(mux1, 7) }, typeid(bool)); // D1 in Bus1
        gb.outto(Port(rom, 3), Ports{ Port(mux1, 8) }, typeid(bool)); // D2 in Bus1
        gb.outto(Port(rom, 4), Ports{ Port(mux1, 9) }, typeid(bool)); // D3 in Bus1
        gb.outto(Port(rom, 5), Ports{ Port(id,   1) }, typeid(bool)); // D4
        gb.outto(Port(rom, 6), Ports{ Port(id,   2) }, typeid(bool)); // D5
        gb.outto(Port(rom, 7), Ports{ Port(id,   3) }, typeid(bool)); // D6
        gb.outto(Port(rom, 8), Ports{ Port(id,   4) }, typeid(bool)); // D7

        // From In Port
        gb.outto(Port(inp, 1), Ports{ Port(mux2,   2) }, typeid(bool)); // Q0 in Bus2
        gb.outto(Port(inp, 2), Ports{ Port(mux2,   3) }, typeid(bool)); // Q1 in Bus2
        gb.outto(Port(inp, 3), Ports{ Port(mux2,   4) }, typeid(bool)); // Q2 in Bus2
        gb.outto(Port(inp, 4), Ports{ Port(mux2,   5) }, typeid(bool)); // Q3 in Bus2

        // From Out Port
        gb.outto(Port(outp, 1), Ports{ Port(exit,  1) }, typeid(bool)); // Q0
        gb.outto(Port(outp, 2), Ports{ Port(exit,  2) }, typeid(bool)); // Q1
        gb.outto(Port(outp, 3), Ports{ Port(exit,  3) }, typeid(bool)); // Q2
        gb.outto(Port(outp, 4), Ports{ Port(exit,  4) }, typeid(bool)); // Q3

        // From PC
        gb.outto(Port(pc, 1), Ports{ Port(rom,  1) }, typeid(bool)); // A0
        gb.outto(Port(pc, 2), Ports{ Port(rom,  2) }, typeid(bool)); // A1
        gb.outto(Port(pc, 3), Ports{ Port(rom,  3) }, typeid(bool)); // A2
        gb.outto(Port(pc, 4), Ports{ Port(rom,  4) }, typeid(bool)); // A3

        // From ALU
        gb.outto(Port(alu, 1), Ports{ Port(mux2,  6) }, typeid(bool)); // Q0
        gb.outto(Port(alu, 2), Ports{ Port(mux2,  7) }, typeid(bool)); // Q1
        gb.outto(Port(alu, 3), Ports{ Port(mux2,  8) }, typeid(bool)); // Q2
        gb.outto(Port(alu, 4), Ports{ Port(mux2,  9) }, typeid(bool)); // Q3
        gb.outto(Port(alu, 5), Ports{ Port(id,    5) }, typeid(bool)); // C
        gb.outto(Port(alu, 6), Ports{ Port(id,    6) }, typeid(bool)); // Z

        // From A Register
        gb.outto(Port(rega, 1), Ports{ Port(alu,   7) }, typeid(bool)); // A0
        gb.outto(Port(rega, 2), Ports{ Port(alu,   8) }, typeid(bool)); // A1
        gb.outto(Port(rega, 3), Ports{ Port(alu,   9) }, typeid(bool)); // A2
        gb.outto(Port(rega, 4), Ports{ Port(alu,  10) }, typeid(bool)); // A3

        // From B Register
        gb.outto(Port(regb, 1), Ports{ Port(mux1,  2) }, typeid(bool)); // B0
        gb.outto(Port(regb, 2), Ports{ Port(mux1,  3) }, typeid(bool)); // B1
        gb.outto(Port(regb, 3), Ports{ Port(mux1,  4) }, typeid(bool)); // B2
        gb.outto(Port(regb, 4), Ports{ Port(mux1,  5) }, typeid(bool)); // B3

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



