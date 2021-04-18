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
    auto qnT = gb.createNode<NodeSRFlipFlopNor>();
    auto qnS = gb.createNode<NodeTestEntry<bool>>();
    auto qnE = gb.createNode<NodeTestExit<bool>>();
   
    gb.outto(Port(qnS, 1), Ports{ Port(qnT, 1) });
    gb.outto(Port(qnS, 2), Ports{ Port(qnT, 2) });
    gb.outto(Port(qnT, 1), Ports{ Port(qnE, 1) });
    gb.outto(Port(qnT, 2), Ports{ Port(qnE, 2) });

    std::vector<std::vector<bool>>   testVector{
        {T, F}, // 不定状態を解消するためにリセット
        {T, F}, // 不定状態を解消するためにリセット
        {F, F}, // 前値保持
        {F, F}, // 前値保持
        {F, T}, // set
        {F, T}, // set
        {F, F}, // 前値保持
        {F, F}, // 前値保持
        {F, F}, // 前値保持
        {F, F}, // 前値保持
        {T, F}, // reset
        {T, F}, // reset
        {F, F}, // 前値保持
        {F, F}, // 前値保持
        {F, F}, // 前値保持
        {F, F}, // 前値保持
    };

    std::vector<std::vector<bool>> expected{
        {false, false},
        {false , true},
        {true, false}
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
}

#endif



