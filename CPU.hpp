// CPU.hpp
#ifndef __CPU_HPP__
#define __CPU_HPP__

#include <iostream>
#include <sstream>
#include <iomanip>

#include "NodeCPU.hpp"
#include "GraphBuilder.hpp"
#include "GraphFlatten.hpp"




//-----------------------------------------------------------
static const bool rom1[16][8] = {
    //   Data         Instruction
    //   D0 D1 D2 D3  D4 D5 D6 D7
        {F, F, F, F,  F, F, F, F}, // [ 0] LD  A 0
        {T, F, T, F,  T, F, F, F}, // [ 1] LD  B 5
        {F, F, F, F,  F, F, T, F}, // [ 2] ADD A B
        {F, F, F, F,  F, F, F, T}, // [ 3] OUT A
        {F, F, F, F,  F, T, F, F}, // [ 4] LD  A B
        {T, F, F, F,  T, T, T, F}, // [ 5] SUB A 1
        {F, F, F, T,  F, T, T, T}, // [ 6] JNZ 08
        {F, F, F, F,  T, T, T, T}, // [ 7] HALT
        {F, F, F, F,  T, T, F, F}, // [ 8] LD  B A
        {F, F, F, F,  T, T, F, T}, // [ 9] IN  A
        {F, T, F, F,  F, F, T, T}, // [10] JMP 2
        {F, F, F, F,  T, T, T, T}, // [11] HALT
        {F, F, F, F,  T, T, T, T}, // [12] HALT
        {F, F, F, F,  T, T, T, T}, // [13] HALT
        {F, F, F, F,  T, T, T, T}, // [14] HALT
        {F, F, F, F,  T, T, T, T}, // [15] HALT

    };

//-----------------------------------------------------------
static const bool rom2[16][8] = {
    //   Data         Instruction
    //   D0 D1 D2 D3  D4 D5 D6 D7
        {F, F, F, F,  F, F, F, F}, // [ 0] LD  A 0
        {T, T, F, F,  T, F, F, F}, // [ 1] LD  B 3
        {F, F, T, F,  F, T, T, F}, // [ 2] ADD A 4
        {F, F, F, F,  F, F, F, T}, // [ 3] OUT A
        {F, F, F, F,  F, T, F, F}, // [ 4] LD  A B
        {T, F, F, F,  T, T, T, F}, // [ 5] SUB A 1
        {F, F, F, F,  T, T, F, F}, // [ 6] LD  B A
        {F, F, F, F,  T, T, F, T}, // [ 7] IN  A
        {F, T, F, F,  F, T, T, T}, // [ 8] JNZ 2
        {F, F, F, F,  T, T, T, T}, // [ 9] HALT
        {F, F, F, F,  T, T, T, T}, // [10] HALT
        {F, F, F, F,  T, T, T, T}, // [11] HALT
        {F, F, F, F,  T, T, T, T}, // [12] HALT
        {F, F, F, F,  T, T, T, T}, // [13] HALT
        {F, F, F, F,  T, T, T, T}, // [14] HALT
        {F, F, F, F,  T, T, T, T}, // [15] HALT
};

//-----------------------------------------------------------
static const bool rom3[16][8] = {
    //   Data         Instruction
    //   D0 D1 D2 D3  D4 D5 D6 D7
        {T, F, F, T,  F, F, F, F}, // [ 0] LD  A 9
        {F, F, F, F,  T, F, F, F}, // [ 1] LD  B 0
        {F, T, F, F,  T, T, T, F}, // [ 2] SUB A 2
        {F, T, F, T,  T, F, T, T}, // [ 3] JNC 10
        {F, F, F, F,  F, F, F, T}, // [ 4] OUT A
        {F, F, F, F,  F, T, F, F}, // [ 5] LD  A B
        {T, F, F, F,  F, T, T, F}, // [ 6] ADD A 1
        {F, F, F, F,  T, T, F, F}, // [ 7] LD  B A
        {F, F, F, F,  T, T, F, T}, // [ 8] IN  A
        {F, T, F, F,  F, F, T, T}, // [ 9] JMP 2
        {F, F, F, F,  T, T, F, T}, // [10] IN  A
        {F, F, F, F,  T, F, F, T}, // [11] OUT B
        {F, F, F, F,  T, T, T, T}, // [12] HALT
        {F, F, F, F,  T, T, T, T}, // [13] HALT
        {F, F, F, F,  T, T, T, T}, // [14] HALT
        {F, F, F, F,  T, T, T, T}, // [15] HALT
};
 


//-----------------------------------------------------------
template <typename T>
void printVector(std::vector<T>& v, std::string name)
{
    std::cout << name;
    for(auto d : v){
        std::cout << d << " ";
    }
    std::cout << std::endl;

}


//-----------------------------------------------------------
void CPU(void)
{
    std::cout << "-- Start - CPU -- " << std::endl;

    //---------------------------
    // CPU基盤の構築

#define NUM_OUTPORT 28 // Number of Probe

    GraphBuilder gb;
    auto rc  = gb.createNode<NodeRstClk>("RESET/Clock");
    auto cpu = gb.createNode<NodeCPU>("CPU");
    auto dh  = gb.createNode<NodeOutput<bool, 1>>("Detect HALT");

    // 値出力用ノード。ここでは出力ポート、アドレス、Aレジスタ、Bレジスタの値を出力する。
    auto d_o = gb.createNode<NodeBit2Digit<4>>("OUTPORT");
    auto d_a = gb.createNode<NodeBit2Digit<4>>("ADDRESS");
    auto dra = gb.createNode<NodeBit2Digit<4>>("REG-A");
    auto drb = gb.createNode<NodeBit2Digit<4>>("REG-B");
    auto out = gb.createNode<NodeOutput<int, 4>>("Digit Data"); // OUT, ADDRESS, REG-A, REG-B 

    // 信号線プローブ用ノード
    auto probe = gb.createNode<NodeOutput<bool, NUM_OUTPORT>>("PROBE");

    NodeRstClk*          nodeRC  = dynamic_cast<NodeRstClk*>(rc.getNode());
    NodeCPU*             nodeCPU = dynamic_cast<NodeCPU*>(cpu.getNode());
    NodeOutput<int, 4>*  nodeOut = dynamic_cast<NodeOutput<int,4>*>(out.getNode());
    NodeOutput<bool, 1>* nodeDH  = dynamic_cast<NodeOutput<bool,1>*>(dh.getNode());
    NodeOutput<bool, NUM_OUTPORT>* nodeProbe = dynamic_cast<NodeOutput<bool,NUM_OUTPORT>*>(probe.getNode());

    // リセット、クロックの接続
    gb.outto<bool>(Port(rc, 1), Ports{ Port(cpu, 1) }); // RST
    gb.outto<bool>(Port(rc, 2), Ports{ Port(cpu, 2) }); // CK


    // CPUのOUTポートをCPUのINポートに接続
    gb.outto<bool>(Port(cpu, 1), Ports{ Port(cpu, 3), Port(d_o, 1) }); // D0
    gb.outto<bool>(Port(cpu, 2), Ports{ Port(cpu, 4), Port(d_o, 2) }); // D1
    gb.outto<bool>(Port(cpu, 3), Ports{ Port(cpu, 5), Port(d_o, 3) }); // D2
    gb.outto<bool>(Port(cpu, 4), Ports{ Port(cpu, 6), Port(d_o, 4) }); // D3

    gb.outto<bool>(Port(cpu, 5), Ports{ Port(dh, 1) }); // HALT

    gb.outto<int>(Port(d_a, 1), Ports{ Port(out, 1) }); // ADDRESS
    gb.outto<int>(Port(dra, 1), Ports{ Port(out, 2) }); // REG-A
    gb.outto<int>(Port(drb, 1), Ports{ Port(out, 3) }); // REG-B
    gb.outto<int>(Port(d_o, 1), Ports{ Port(out, 4) }); // OUTPORT

    // PROBE

    gb.outto<bool>(Port(cpu, nodeCPU->PROBE+1), Ports{ Port(probe, 1) }); // RST
    gb.outto<bool>(Port(cpu, nodeCPU->PROBE+2), Ports{ Port(probe, 2) }); // CK

    gb.outto<bool>(Port(cpu, nodeCPU->PROBE+3), Ports{ Port(probe, 3), Port(d_a, 1) }); // Address 0
    gb.outto<bool>(Port(cpu, nodeCPU->PROBE+4), Ports{ Port(probe, 4), Port(d_a, 2) }); // Address 1
    gb.outto<bool>(Port(cpu, nodeCPU->PROBE+5), Ports{ Port(probe, 5), Port(d_a, 3) }); // Address 2
    gb.outto<bool>(Port(cpu, nodeCPU->PROBE+6), Ports{ Port(probe, 6), Port(d_a, 4) }); // Address 2

    gb.outto<bool>(Port(cpu, nodeCPU->PROBE+7), Ports{ Port(probe, 7) }); // A-REG LD
    gb.outto<bool>(Port(cpu, nodeCPU->PROBE+8), Ports{ Port(probe, 8) }); // B-REG LD
    gb.outto<bool>(Port(cpu, nodeCPU->PROBE+9), Ports{ Port(probe, 9) }); // ROM OE
    gb.outto<bool>(Port(cpu, nodeCPU->PROBE+10), Ports{ Port(probe,10) }); // ALU A/S
    gb.outto<bool>(Port(cpu, nodeCPU->PROBE+11), Ports{ Port(probe,11) }); // ALU OE
    gb.outto<bool>(Port(cpu, nodeCPU->PROBE+12), Ports{ Port(probe,12) }); // ALU LD
    gb.outto<bool>(Port(cpu, nodeCPU->PROBE+13), Ports{ Port(probe,13) }); // ALU MUX A
    gb.outto<bool>(Port(cpu, nodeCPU->PROBE+14), Ports{ Port(probe,14) }); // ALU MUX B
    gb.outto<bool>(Port(cpu, nodeCPU->PROBE+15), Ports{ Port(probe,15) }); // OUT LD
    gb.outto<bool>(Port(cpu, nodeCPU->PROBE+16), Ports{ Port(probe,16) }); // HALT

    gb.outto<bool>(Port(cpu, nodeCPU->PROBE+17), Ports{ Port(probe,17) }); // ALU-Q0
    gb.outto<bool>(Port(cpu, nodeCPU->PROBE+18), Ports{ Port(probe,18) }); // ALU-Q1
    gb.outto<bool>(Port(cpu, nodeCPU->PROBE+19), Ports{ Port(probe,19) }); // ALU-Q2
    gb.outto<bool>(Port(cpu, nodeCPU->PROBE+20), Ports{ Port(probe,20) }); // ALU-Q3

    gb.outto<bool>(Port(cpu, nodeCPU->PROBE+21), Ports{ Port(probe,21), Port(dra, 1) }); // A-REG0
    gb.outto<bool>(Port(cpu, nodeCPU->PROBE+22), Ports{ Port(probe,22), Port(dra, 2) }); // A-REG1
    gb.outto<bool>(Port(cpu, nodeCPU->PROBE+23), Ports{ Port(probe,23), Port(dra, 3) }); // A-REG2
    gb.outto<bool>(Port(cpu, nodeCPU->PROBE+24), Ports{ Port(probe,24), Port(dra, 4) }); // A-REG3

    gb.outto<bool>(Port(cpu, nodeCPU->PROBE+25), Ports{ Port(probe,25), Port(drb, 1) }); // B-REG0
    gb.outto<bool>(Port(cpu, nodeCPU->PROBE+26), Ports{ Port(probe,26), Port(drb, 2) }); // B-REG1
    gb.outto<bool>(Port(cpu, nodeCPU->PROBE+27), Ports{ Port(probe,27), Port(drb, 3) }); // B-REG2
    gb.outto<bool>(Port(cpu, nodeCPU->PROBE+28), Ports{ Port(probe,28), Port(drb, 4) }); // B-REG3


    //---------------------------
    // グラフ平坦化を行い、全ノード、エッジを取得
    std::vector<Node*>& nodes = gb.getNodes();
    std::vector<Edge*>& edges = gb.getEdges();
    std::pair<std::vector<Node*>, std::vector<Edge*>> p = graphFlatten(nodes, edges);
    nodes = p.first;
    edges = p.second;

    auto exe = createExecutor();
    exe->setStartNode(static_cast<Node*>(nodeRC));
    exe->addNodes(nodes);
    exe->addEdges(edges);


    //---------------------------
    // Node一覧表示
    std::cout << "List of Nodes" << std::endl;
    for(auto n : nodes){
        // std::string& name = n->getName();
        // std::cout << name << std::endl;
        std::cout << typeid(*n).name() << std::endl;
    }
    std::cout << std::endl;


    //---------------------------
    // CPU実行

    // ROMデータを設定
    nodeCPU->setROMData(rom3);

    // RESETは初期状態としてRESET状態にしておく。
    nodeRC->setReset(F);

    exe->step(); // リセット状態で一度実行

     nodeRC->setReset(T); // リセット解除

    std::cout << "   ADDR,REG-A,REG-B,OUTPUT" << std::endl;
    for(int i=0;;i++){

        // クロックを刻み、Executor実行
        nodeRC->doClock(); 
        exe->step();

        // PROBE出力
        // auto probes = nodeProbe->output();
        // printVector(probes, "PROVE: ");

        // OUTPORT出力
        auto output_data = nodeOut->output();
        std::string name = "[" + std::to_string(i) + "] ";
        printVector(output_data, name);

        // HALTを検出したら、終了する。
        auto halts = nodeDH->output();
        bool halt = halts[0];
        if(halt == F){
            std::cout << "--- PROGRAM HALT ---" << std::endl;;
            break;
        }

    }
}


#endif

