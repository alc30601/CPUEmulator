// test_15.hpp
// Registerのテスト
#ifndef __TEST_15_HPP__
#define __TEST_15_HPP__

#include "NodeTestBase.hpp"
#include "Node4bitRegister.hpp"


//-----------------------------------------------------------
// Node4bitRegisterのテスト
void test15_01(void)
{
    std::cout << "-- TEST 15-01 4bit Register --" << std::endl;

    std::vector<std::vector<bool>>   testVector{
        {F, F, F, F, F, T}, // [ 1] reset
        {T, F, F, F, F, T}, // [ 2] non-reset
        {T, T, F, F, F, T}, // [ 3] edge-up 0001
        {T, F, T, F, T, F}, // [ 4] edge-down
        {T, T, T, F, T, F}, // [ 5] edge-up 1010
        {T, F, F, T, F, T}, // [ 6] edge-down
        {T, T, F, T, F, T}, // [ 7] edge-up 0101
        {T, F, T, T, T, T}, // [ 8] edge-down
        {T, T, T, T, T, T}, // [ 9] edge-up 1111
        {F, T, T, T, T, T}, // [10] reset
    };

    std::vector<std::vector<bool>> expected{
        {F, F, F, F}, // [ 1] reset
        {F, F, F, F}, // [ 2] stay
        {F, F, F, T}, // [ 3] reflect the value
        {F, F, F, T}, // [ 4] stay
        {T, F, T, F}, // [ 5] reflect the value
        {T, F, T, F}, // [ 6] stay
        {F, T, F, T}, // [ 7] reflect the value
        {F, T, F, T}, // [ 8] stay
        {T, T, T, T}, // [ 9] reflect the value
        {F, F, F, F}, // [10] reset
    };

    std::vector<bool> do_asserts{
        true, true, true, true, true, true, true, true
    };

    test_NtoM_template<Node4bitRegister, bool, bool>(testVector, expected, do_asserts);
 
}


//-----------------------------------------------------------
// Node4bitAsyncCounterのテスト
void test15_02(void)
{
    std::cout << "-- TEST 15-02 4bit Async Counter --" << std::endl;

    std::vector<std::vector<bool>>   testVector{
        {F, F}, // [ 1] reset
        {T, F}, // [  ] reset <- 何故か同時にRSTとCKをF->Tとすると回路が安定しない。
        {T, T}, // [ 2] edge-up
        {T, F}, // [ 3] edge-down
        {T, T}, // [ 4] edge-up
        {T, F}, // [ 5] edge-down
        {T, T}, // [ 6] edge-up
        {T, F}, // [ 7] edge-down
        {T, T}, // [ 8] edge-up
        {T, F}, // [ 9] edge-down
        {T, T}, // [10] edge-up
        {T, F}, // [11] edge-down
        {T, T}, // [12] edge-up
        {T, F}, // [13] edge-down
        {T, T}, // [14] edge-up
        {T, F}, // [15] edge-down
        {T, T}, // [16] edge-up
        {T, F}, // [17] edge-down
        {T, T}, // [18] edge-up
        {T, F}, // [19] edge-down
        {T, T}, // [20] edge-up
        {T, F}, // [21] edge-down
        {T, T}, // [22] edge-up
        {T, F}, // [23] edge-down
        {T, T}, // [24] edge-up
        {T, F}, // [25] edge-down
        {T, T}, // [26] edge-up
        {T, F}, // [27] edge-down
        {T, T}, // [28] edge-up
        {T, F}, // [29] edge-down
        {T, T}, // [30] edge-up
        {T, F}, // [31] edge-down
        {T, T}, // [32] edge-up
    };

    std::vector<std::vector<bool>> expected{
        {F, F, F, F}, // [ 1] reset
        {F, F, F, F}, // [  ] 
        {T, F, F, F}, // [ 2] 1
        {T, F, F, F}, // [ 3] stay
        {F, T, F, F}, // [ 4] 2
        {F, T, F, F}, // [ 5] stay
        {T, T, F, F}, // [ 6] 3
        {T, T, F, F}, // [ 7] stay
        {F, F, T, F}, // [ 8] 4
        {F, F, T, F}, // [ 9] stay
        {T, F, T, F}, // [10] 5
        {T, F, T, F}, // [11] stay
        {F, T, T, F}, // [12] 6
        {F, T, T, F}, // [13] stay
        {T, T, T, F}, // [14] 7
        {T, T, T, F}, // [15] stay
        {F, F, F, T}, // [16] 8
        {F, F, F, T}, // [17] stay
        {T, F, F, T}, // [18] 9
        {T, F, F, T}, // [19] stay
        {F, T, F, T}, // [20] A
        {F, T, F, T}, // [21] stay
        {T, T, F, T}, // [22] B
        {T, T, F, T}, // [23] stay
        {F, F, T, T}, // [24] C
        {F, F, T, T}, // [25] stay
        {T, F, T, T}, // [26] D
        {T, F, T, T}, // [27] stay
        {F, T, T, T}, // [28] E
        {F, T, T, T}, // [29] stay
        {T, T, T, T}, // [30] F
        {T, T, T, T}, // [31] stay
        {F, F, F, F}, // [32] 10=0
    };

    std::vector<bool> do_asserts{
        true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true,
    };

    test_NtoM_template<Node4bitAsyncCounter, bool, bool>(testVector, expected, do_asserts);
 
}


//-----------------------------------------------------------
// Node4bitSyncCounterのテスト
void test15_03(void)
{
    std::cout << "-- TEST 15-03 4bit Sync Counter --" << std::endl;

    std::vector<std::vector<bool>>   testVector{
        {F, F}, // [ 1] reset
        {T, F}, // [  ] reset <- 何故か同時にRSTとCKをF->Tとすると回路が安定しない。
        {T, T}, // [ 2] edge-up
        {T, F}, // [ 3] edge-down
        {T, T}, // [ 4] edge-up
        {T, F}, // [ 5] edge-down
        {T, T}, // [ 6] edge-up
        {T, F}, // [ 7] edge-down
        {T, T}, // [ 8] edge-up
        {T, F}, // [ 9] edge-down
        {T, T}, // [10] edge-up
        {T, F}, // [11] edge-down
        {T, T}, // [12] edge-up
        {T, F}, // [13] edge-down
        {T, T}, // [14] edge-up
        {T, F}, // [15] edge-down
        {T, T}, // [16] edge-up
        {T, F}, // [17] edge-down
        {T, T}, // [18] edge-up
        {T, F}, // [19] edge-down
        {T, T}, // [20] edge-up
        {T, F}, // [21] edge-down
        {T, T}, // [22] edge-up
        {T, F}, // [23] edge-down
        {T, T}, // [24] edge-up
        {T, F}, // [25] edge-down
        {T, T}, // [26] edge-up
        {T, F}, // [27] edge-down
        {T, T}, // [28] edge-up
        {T, F}, // [29] edge-down
        {T, T}, // [30] edge-up
        {T, F}, // [31] edge-down
        {T, T}, // [32] edge-up
    };

    std::vector<std::vector<bool>> expected{
        {F, F, F, F}, // [ 1] reset
        {F, F, F, F}, // [  ] 
        {T, F, F, F}, // [ 2] 1
        {T, F, F, F}, // [ 3] stay
        {F, T, F, F}, // [ 4] 2
        {F, T, F, F}, // [ 5] stay
        {T, T, F, F}, // [ 6] 3
        {T, T, F, F}, // [ 7] stay
        {F, F, T, F}, // [ 8] 4
        {F, F, T, F}, // [ 9] stay
        {T, F, T, F}, // [10] 5
        {T, F, T, F}, // [11] stay
        {F, T, T, F}, // [12] 6
        {F, T, T, F}, // [13] stay
        {T, T, T, F}, // [14] 7
        {T, T, T, F}, // [15] stay
        {F, F, F, T}, // [16] 8
        {F, F, F, T}, // [17] stay
        {T, F, F, T}, // [18] 9
        {T, F, F, T}, // [19] stay
        {F, T, F, T}, // [20] A
        {F, T, F, T}, // [21] stay
        {T, T, F, T}, // [22] B
        {T, T, F, T}, // [23] stay
        {F, F, T, T}, // [24] C
        {F, F, T, T}, // [25] stay
        {T, F, T, T}, // [26] D
        {T, F, T, T}, // [27] stay
        {F, T, T, T}, // [28] E
        {F, T, T, T}, // [29] stay
        {T, T, T, T}, // [30] F
        {T, T, T, T}, // [31] stay
        {F, F, F, F}, // [32] 10=0
    };

    std::vector<bool> do_asserts{
        true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true
    };

    test_NtoM_template<Node4bitSyncCounter, bool, bool>(testVector, expected, do_asserts);
}


//-----------------------------------------------------------
// Node4bitShiftRegisterのテスト
void test15_04(void)
{
    std::cout << "-- TEST 15-04 4bit Shift Register --" << std::endl;

    std::vector<std::vector<bool>>   testVector{
        {F, F, T}, // [ 1] reset
        {T, F, T}, // [  ] reset <- 何故か同時にRSTとCKをF->Tとすると回路が安定しない。
        {T, T, T}, // [ 2] edge-up
        {T, F, F}, // [ 3] edge-down
        {T, T, F}, // [ 4] edge-up
        {T, F, F}, // [ 5] edge-down
        {T, T, F}, // [ 6] edge-up
        {T, F, F}, // [ 7] edge-down
        {T, T, F}, // [ 8] edge-up
        {T, F, F}, // [ 9] edge-down
        {T, T, F}, // [10] edge-up
    };

    std::vector<std::vector<bool>> expected{
        {F, F, F, F}, // [ 1] reset
        {F, F, F, F}, // [  ] 
        {T, F, F, F}, // [ 2] 1
        {T, F, F, F}, // [ 3] stay
        {F, T, F, F}, // [ 4] 2
        {F, T, F, F}, // [ 5] stay
        {F, F, T, F}, // [ 6] 3
        {F, F, T, F}, // [ 7] stay
        {F, F, F, T}, // [ 8] 4
        {F, F, F, T}, // [ 9] stay
        {F, F, F, F}, // [10] 5
    };

    std::vector<bool> do_asserts{
        true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true
    };

     test_NtoM_template<Node4bitShiftRegister, bool, bool>(testVector, expected, do_asserts);
}



//-----------------------------------------------------------
void test15(void)
{
    test15_01();
    test15_02();
    test15_03();
    test15_04();
}


#endif

