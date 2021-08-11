// CPU.hpp
#ifndef __CPU_HPP__
#define __CPU_HPP__

#include <iostream>
#include "NodeCPU.hpp"
#include "GraphBuilder.hpp"

/*
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

---
            ADDRESS                       ALU      A-REG    B-REG
LD A 0000
PROVE: 1 1  0 0 0 0  1 0 1 0 1 0 0 1 0 1  0 0 0 0  0 0 0 0  0 0 0 0 
PROVE: 1 0  0 0 0 0  1 0 1 0 1 0 0 1 0 1  0 0 0 0  0 0 0 0  0 0 0 0 
PROVE: 1 1  0 0 0 0  0 0 1 0 1 0 0 1 0 1  0 0 0 0  0 0 0 0  0 0 0 0 
PROVE: 1 0  0 0 0 0  0 0 1 0 1 0 0 1 0 1  0 0 0 0  0 0 0 0  0 0 0 0 

LD B 0101
PROVE: 1 1  1 0 0 0  0 1 1 0 1 0 0 1 0 1  1 0 1 0  0 0 0 0  0 0 0 0 
PROVE: 1 0  1 0 0 0  0 1 1 0 1 0 0 1 0 1  1 0 1 0  0 0 0 0  0 0 0 0 
PROVE: 1 1  1 0 0 0  0 0 1 0 1 0 0 1 0 1  1 0 1 0  0 0 0 0  1 0 1 0 
PROVE: 1 0  1 0 0 0  0 0 1 0 1 0 0 1 0 1  1 0 1 0  0 0 0 0  1 0 1 0 

ADD A B
PROVE: 1 1  0 1 0 0  1 0 0 0 1 1 1 1 0 1  1 0 1 0  0 0 0 0  1 0 1 0 
PROVE: 1 0  0 1 0 0  1 0 0 0 1 1 1 1 0 1  1 0 1 0  0 0 0 0  1 0 1 0 
PROVE: 1 1  0 1 0 0  0 0 0 0 1 0 1 1 0 1  0 1 0 1  1 0 1 0  1 0 1 0 
PROVE: 1 0  0 1 0 0  0 0 0 0 1 0 1 1 0 1  0 1 0 1  1 0 1 0  1 0 1 0 

PROVE: 1 0  1 1 1 0  0 0 1 0 1 0 0 1 0 0  0 0 0 0  0 0 0 0  1 0 0 0 
*/




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

#define NUM_OUTPORT (32-4) // 4 means outport. so number of probe must be minus 4

    GraphBuilder gb;
    auto rc  = gb.createNode<NodeRstClk>("RESET/Clock");
    auto cpu = gb.createNode<NodeCPU>("CPU");
    auto b2d = gb.createNode<NodeBit2Digit<4>>("B -> D");
    auto out = gb.createNode<NodeOutput<int, 1>>("OUT PORT");
    auto dh  = gb.createNode<NodeOutput<bool, 1>>("Detect HALT");

    auto probe = gb.createNode<NodeOutput<bool, NUM_OUTPORT>>("PROBE");

    NodeRstClk*         nodeRC  = dynamic_cast<NodeRstClk*>(rc.getNode());
    NodeCPU*            nodeCPU = dynamic_cast<NodeCPU*>(cpu.getNode());
    NodeOutput<int, 1>* nodeOut = dynamic_cast<NodeOutput<int,1>*>(out.getNode());
    NodeOutput<bool, 1>* nodeDH = dynamic_cast<NodeOutput<bool,1>*>(dh.getNode());
    NodeOutput<bool, NUM_OUTPORT>* nodeProbe = dynamic_cast<NodeOutput<bool,NUM_OUTPORT>*>(probe.getNode());

    // リセット、クロックの接続
    gb.outto(Port(rc, 1), Ports{ Port(cpu, 1) }, typeid(bool)); // RST
    gb.outto(Port(rc, 2), Ports{ Port(cpu, 2) }, typeid(bool)); // CK

    // CPUのOUTポートをCPUのINポートに接続
    gb.outto(Port(cpu, 1), Ports{ Port(cpu, 3), Port(b2d, 1) }, typeid(bool)); // D0
    gb.outto(Port(cpu, 2), Ports{ Port(cpu, 4), Port(b2d, 2) }, typeid(bool)); // D1
    gb.outto(Port(cpu, 3), Ports{ Port(cpu, 5), Port(b2d, 3) }, typeid(bool)); // D2
    gb.outto(Port(cpu, 4), Ports{ Port(cpu, 6), Port(b2d, 4) }, typeid(bool)); // D3

    gb.outto(Port(b2d, 1), Ports{ Port(out, 1) }, typeid(int)); // OUTPORT

    // PROBE
    gb.outto(Port(cpu, 5), Ports{ Port(probe, 1) }, typeid(int)); // RST
    gb.outto(Port(cpu, 6), Ports{ Port(probe, 2) }, typeid(int)); // CK

    gb.outto(Port(cpu, 7), Ports{ Port(probe, 3) }, typeid(int)); // Address 0
    gb.outto(Port(cpu, 8), Ports{ Port(probe, 4) }, typeid(int)); // Address 1
    gb.outto(Port(cpu, 9), Ports{ Port(probe, 5) }, typeid(int)); // Address 2
    gb.outto(Port(cpu,10), Ports{ Port(probe, 6) }, typeid(int)); // Address 2

    gb.outto(Port(cpu,11), Ports{ Port(probe, 7) }, typeid(int)); // A-REG LD
    gb.outto(Port(cpu,12), Ports{ Port(probe, 8) }, typeid(int)); // B-REG LD
    gb.outto(Port(cpu,13), Ports{ Port(probe, 9) }, typeid(int)); // ROM OE
    gb.outto(Port(cpu,14), Ports{ Port(probe,10) }, typeid(int)); // ALU A/S
    gb.outto(Port(cpu,15), Ports{ Port(probe,11) }, typeid(int)); // ALU OE
    gb.outto(Port(cpu,16), Ports{ Port(probe,12) }, typeid(int)); // ALU LD
    gb.outto(Port(cpu,17), Ports{ Port(probe,13) }, typeid(int)); // ALU MUX A
    gb.outto(Port(cpu,18), Ports{ Port(probe,14) }, typeid(int)); // ALU MUX B
    gb.outto(Port(cpu,19), Ports{ Port(probe,15) }, typeid(int)); // OUT LD
    gb.outto(Port(cpu,20), Ports{ Port(probe,16), Port(dh, 1) }, typeid(int)); // HALT

    gb.outto(Port(cpu,21), Ports{ Port(probe,17) }, typeid(int)); // ALU-Q0
    gb.outto(Port(cpu,22), Ports{ Port(probe,18) }, typeid(int)); // ALU-Q1
    gb.outto(Port(cpu,23), Ports{ Port(probe,19) }, typeid(int)); // ALU-Q2
    gb.outto(Port(cpu,24), Ports{ Port(probe,20) }, typeid(int)); // ALU-Q3

    gb.outto(Port(cpu,25), Ports{ Port(probe,21) }, typeid(int)); // A-REG0
    gb.outto(Port(cpu,26), Ports{ Port(probe,22) }, typeid(int)); // A-REG1
    gb.outto(Port(cpu,27), Ports{ Port(probe,23) }, typeid(int)); // A-REG2
    gb.outto(Port(cpu,28), Ports{ Port(probe,24) }, typeid(int)); // A-REG3

    gb.outto(Port(cpu,29), Ports{ Port(probe,25) }, typeid(int)); // B-REG0
    gb.outto(Port(cpu,30), Ports{ Port(probe,26) }, typeid(int)); // B-REG1
    gb.outto(Port(cpu,31), Ports{ Port(probe,27) }, typeid(int)); // B-REG2
    gb.outto(Port(cpu,32), Ports{ Port(probe,28) }, typeid(int)); // B-REG3

    //---------------------------
    // Executorの準備
    
    // グラフ平坦化を行い、全ノード、エッジを取得
    std::vector<Node*>& nodes = gb.getNodes();
    std::vector<Edge*>& edges = gb.getEdges();
    std::pair<std::vector<Node*>, std::vector<Edge*>> p = graphFlatten(nodes, edges);
    nodes = p.first;
    edges = p.second;

    // Executorの準備
    auto exe = createExecutor();
    exe->setStartNode(static_cast<Node*>(nodeRC));
    exe->addNodes(nodes);
    exe->addEdges(edges);

    //---------------------------
    // CPU実行

    // ROMデータを設定
    nodeCPU->setROMData(rom1);

    // RESETは初期状態としてRESET状態にしておく。
    nodeRC->setReset(F);

    exe->step(); // リセット状態で一度実行

    nodeRC->setReset(T); // リセット解除

    for(int i=0;;i++){

        nodeRC->doClock();
        exe->step();

        // PROBE出力
        auto probes = nodeProbe->output();
        printVector(probes, "PROVE: ");

        // OUTPORT出力
        auto output_data = nodeOut->output();
        std::string name = "OUT[" + std::to_string(i) + "] ";
        printVector(output_data, name);

        auto halts = nodeDH->output();
        bool halt = halts[0];
        if(halt == F){
            std::cout << "--- PROGRAM HALT ---" << std::endl;;
            break;
        }

    }
}

#endif

