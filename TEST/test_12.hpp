// test_12.hpp
// Test for FlipFlop
#ifndef __TEST_12_HPP__
#define __TEST_12_HPP__

#include "NodeTestBase.hpp"
#include "NodeFlipFlop.hpp"


//-----------------------------------------------------------
// NOR型SRフリップフロップのテスト
void test12_01(void)
{
    std::cout << "-- TEST12-01 NOR type SR FlipFlop --" << std::endl;

    GraphBuilder gb;
    auto qnT = gb.createNode<NodeSRFlipFlopNor>("NodeSRFlipFlopNor");
    auto qnS = gb.createNode<NodeTestEntry<bool>>("NodeTestEntry");
    auto qnE = gb.createNode<NodeTestExit<bool>>("NodeTestExit");
   
    gb.outto(Port(qnS, 1), Ports{ Port(qnT, 1) }, typeid(bool));
    gb.outto(Port(qnS, 2), Ports{ Port(qnT, 2) }, typeid(bool));
    gb.outto(Port(qnT, 1), Ports{ Port(qnE, 1) }, typeid(bool));
    gb.outto(Port(qnT, 2), Ports{ Port(qnE, 2) }, typeid(bool));

    std::vector<std::vector<bool>>   testVector{ 
        {T, F}, // 不定状態を解消するためにリセット
        {T, F}, // 不定状態を解消するためにリセット
        {F, F}, // 前値保持
        {F, F}, // 前値保持
        {F, F}, // 前値保持
        {F, T}, // set
        {F, F}, // 前値保持
        {F, F}, // 前値保持
        {F, F}, // 前値保持
        {T, F}, // reset
        {F, F}, // 前値保持
        {F, F}, // 前値保持
        {F, F}, // 前値保持
    };

    std::vector<std::vector<bool>> expected{
        {F, T}, // 不定状態を解消するためにリセット
        {F, T}, // 不定状態を解消するためにリセット
        {F, T}, // 前値保持
        {F, T}, // 前値保持
        {F, T}, // 前値保持
        {T, F}, // set
        {T, F}, // 前値保持
        {T, F}, // 前値保持
        {T, F}, // 前値保持
        {F, T}, // reset
        {F, T}, // 前値保持
        {F, T}, // 前値保持
        {F, T}, // 前値保持

    };

    Executor* exe = gb.createExecutor(qnS);
    for(int i=0;i<testVector.size();i++){
        evaluation<bool, bool>(exe, qnS, qnE, testVector[i], expected[i], true);
    }

}

//-----------------------------------------------------------
// NAND型SRフリップフロップのテスト
void test12_02(void)
{
    std::cout << "-- TEST12-02 NAND type SR FlipFlop --" << std::endl;

    GraphBuilder gb;
    auto qnT = gb.createNode<NodeSRFlipFlopNand>("NodeSRFlipFlopNand");
    auto qnS = gb.createNode<NodeTestEntry<bool>>("NodeTestEntry");
    auto qnE = gb.createNode<NodeTestExit<bool>>("NodeTestExit");
   
    gb.outto(Port(qnS, 1), Ports{ Port(qnT, 1) }, typeid(bool));
    gb.outto(Port(qnS, 2), Ports{ Port(qnT, 2) }, typeid(bool));
    gb.outto(Port(qnT, 1), Ports{ Port(qnE, 1) }, typeid(bool));
    gb.outto(Port(qnT, 2), Ports{ Port(qnE, 2) }, typeid(bool));

    std::vector<std::vector<bool>>   testVector{ 
        {T, F}, // 不定状態を解消するためにリセット
        {T, F}, // 不定状態を解消するためにリセット
        {T, T}, // 前値保持
        {T, T}, // 前値保持
        {T, T}, // 前値保持
        {F, T}, // set
        {T, T}, // 前値保持
        {T, T}, // 前値保持
        {T, T}, // 前値保持
        {T, F}, // reset
        {T, T}, // 前値保持
        {T, T}, // 前値保持
        {T, T}, // 前値保持
    };

    std::vector<std::vector<bool>> expected{
        {F, T}, // 不定状態を解消するためにリセット
        {F, T}, // 不定状態を解消するためにリセット
        {F, T}, // 前値保持
        {F, T}, // 前値保持
        {F, T}, // 前値保持
        {T, F}, // set
        {T, F}, // 前値保持
        {T, F}, // 前値保持
        {T, F}, // 前値保持
        {F, T}, // reset
        {F, T}, // 前値保持
        {F, T}, // 前値保持
        {F, T}, // 前値保持
    };

     Executor* exe = gb.createExecutor(qnS);
    for(int i=0;i<testVector.size();i++){
        evaluation<bool, bool>(exe, qnS, qnE, testVector[i], expected[i], false);
    }

}

//-----------------------------------------------------------
void test12(void)
{
    test12_01();
    test12_02();
}

#endif



