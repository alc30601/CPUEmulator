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

    test_NtoM_template<NodeSRFlipFlopNor, bool, bool>(testVector, expected);

}

//-----------------------------------------------------------
// NAND型SRフリップフロップのテスト
void test12_02(void)
{
    std::cout << "-- TEST12-02 NAND type SR FlipFlop --" << std::endl;

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

    test_NtoM_template<NodeSRFlipFlopNand, bool, bool>(testVector, expected);
}

//-----------------------------------------------------------
// Gated SR Latchのテスト
void test12_03(void)
{
    std::cout << "-- TEST12-03 Gated SR Latch --" << std::endl;

    std::vector<std::vector<bool>>   testVector{ 
        {T, F, T},  // 不定状態を解消するためにリセット
        {T, F, T},  // 不定状態を解消するためにリセット
        {T, F, F}, // 前値保持
        {T, F, F}, // 前値保持
        {T, F, F}, // 前値保持
        {T, T, F}, // set
        {T, F, F}, // 前値保持
        {T, F, F}, // 前値保持
        {T, F, F}, // 前値保持
        {T, F, T}, // reset
        {T, F, F}, // 前値保持
        {T, F, F}, // 前値保持
        {T, F, F}, // 前値保持

        {F, T, F}, // set(Gate OFF)
        {T, T, F}, // set(Gate ON)
        {F, F, T}, // reset(Gate OFF)
        {T, F, T}, // reset(Gate ON)
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

        {F, T}, // set(Gate OFF)
        {T, F}, // set(Gate ON)
        {T, F}, // reset(Gate OFF)
        {F, T}, // reset(Gate ON)
    };

    test_NtoM_template<NodeGatedSRLatch, bool, bool>(testVector, expected);
}

//-----------------------------------------------------------
// Gated D Latchのテスト
void test12_04(void)
{
    std::cout << "-- TEST12-04 Gated D Latch --" << std::endl;

    std::vector<std::vector<bool>>   testVector{
        {T, F}, // reset
        {T, T}, // set
        {F, F}, // reset(Gate OFF)
        {T, F}, // reset(Gate ON)
        {F, T}, // set(Gate OFF)
        {T, T}, // set(Gate ON)
    };

    std::vector<std::vector<bool>> expected{
        {F, T}, // reset
        {T, F}, // set
        {T, F}, // reset(Gate OFF)
        {F, T}, // reset(Gate ON)
        {F, T}, // set(Gate OFF) 
        {T, F}, // set(Gate ON)
    };

    test_NtoM_template<NodeGatedDLatch, bool, bool>(testVector, expected);
}

//-----------------------------------------------------------
// JK Flip Flopのテスト
// Toggle(CK, J, K = T, T, T)では本システムでもEdgeの値が安定しない。
// Edgeが安定するまで処理を繰り返すExecutorでは処理が終わらなくなる。
// JK FlipFlopのテスト
void test12_05(void)
{
    std::cout << "-- TEST12-05 JK FlipFlop --" << std::endl;

    std::vector<std::vector<bool>>   testVector{
        {T, F, T},  // reset
        {T, F, F},  // 前値保持
        {T, F, F},  // 前値保持
        {T, F, F},  // 前値保持
        {T, T, F},  // set
        {F, F, F},  // 前値保持
        {F, T, T},  // 前値保持
        // {T, T, T},  // Toggle
        // {T, T, T},  // Toggle
        // {T, T, T},  // Toggle
        // {T, T, T},  // Toggle
    };

    std::vector<std::vector<bool>> expected{
        {F, T},     // reset
        {F, T},     // 前値保持
        {F, T},     // 前値保持
        {F, T},     // 前値保持
        {T, F},     // set
        {T, F},     // 前値保持
        {T, F},     // 前値保持
        // {F, T},     // Toggle
        // {T, F},     // Toggle
        // {F, T},     // Toggle
        // {T, F},     // Toggle
    };

    test_NtoM_template<NodeJKFlipFlop, bool, bool>(testVector, expected);
}


//-----------------------------------------------------------
// D Flip Flopのテスト
void test12_06(void)
{
    std::cout << "-- TEST12-06 D FlipFlop --" << std::endl;

    std::vector<std::vector<bool>>   testVector{
        {T, F},     // reset
        {F, F},     // 前値保持
        {F, T},     // 前値保持
        {F, T},     // 前値保持
        {T, T},     // set
        {F, T},     // 前値保持
        {F, T},     // 前値保持
        {F, F},     // 前値保持
    };

    std::vector<std::vector<bool>> expected{
        {F, T},     // reset
        {F, T},     // 前値保持
        {F, T},     // 前値保持
        {F, T},     // 前値保持
        {T, F},     // set
        {T, F},     // 前値保持
        {T, F},     // 前値保持
        {T, F},     // 前値保持
    };

    test_NtoM_template<NodeDFlipFlop, bool, bool>(testVector, expected);
}


//-----------------------------------------------------------
// JK Flip Flop(Master-Slave)のテスト
void test12_07(void)
{
    std::cout << "-- TEST12-07 JK FlipFlop (MasterSlave) --" << std::endl;

    std::vector<std::vector<bool>>   testVector{
        {T, F, T},  // reset
        {F, F, T},  // reset
        {T, F, F},  // 前値保持
        {F, F, F},  // 前値保持
        {T, F, F},  // 前値保持
        {F, F, F},  // 前値保持
        {T, T, F},  // set
        {F, T, F},  // set
        {T, F, F},  // 前値保持
        {F, F, F},  // 前値保持
        {T, F, F},  // 前値保持
        {F, F, F},  // 前値保持
        {T, F, T},  // reset
        {F, F, T},  // reset
        {T, T, T},  // Toggle
        {F, T, T},  // Toggle
        {T, T, T},  // Toggle
        {F, T, T},  // Toggle
    };

    std::vector<std::vector<bool>> expected{
        {F, T},     // reset
        {F, T},     // reset
        {F, T},     // 前値保持
        {F, T},     // 前値保持
        {F, T},     // 前値保持
        {F, T},     // 前値保持
        {F, T},     // set
        {T, F},     // set
        {T, F},     // 前値保持
        {T, F},     // 前値保持
        {T, F},     // 前値保持
        {T, F},     // 前値保持
        {T, F},     // reset
        {F, T},     // reset
        {F, T},     // Toggle
        {T, F},     // Toggle
        {T, F},     // Toggle
        {F, T},     // Toggle
    };

    std::vector<bool> do_asserts{
        false, true,
        true,  true,
        true,  true,
        true,  true,
        true,  true,
        true,  true,
        true,  true,
        true,  true,
    };

    test_NtoM_template<NodeJKFlipFlopMasterSlave, bool, bool>(testVector, expected, do_asserts);
}


//-----------------------------------------------------------
void test12(void)
{
    test12_01();
    test12_02();
    test12_03();
    test12_04();
    test12_05(); 
    test12_06(); 
    test12_07(); 
}

#endif



