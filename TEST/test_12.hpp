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

    std::vector<std::vector<bool>>   testVector = vectorBool2bits;

    std::vector<std::vector<bool>> expected{
        {false, false},
        {true , false},
        {false, false},
        {false, true},
    };

    Executor* exe = gb.createExecutor(qnS);
    for(int i=0;i<testVector.size();i++){
        evaluation<bool, bool>(exe, qnS, qnE, testVector[i], expected[i]);
    }

}

//-----------------------------------------------------------
void test12(void)
{
    test12_01();
}

#endif



