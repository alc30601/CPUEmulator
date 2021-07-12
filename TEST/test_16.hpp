// test_16.hpp
// CPUパーツの単体評価
#ifndef __TEST_16_HPP__
#define __TEST_16_HPP__

#include <iostream>

#include "NodeTestBase.hpp"
#include "NodeROM.hpp"
#include "NodeRegister.hpp"

//-----------------------------------------------------------
void test16_01(void)
{
    std::cout << "-- TEST 16-01 ROM --" << std::endl;

    std::vector<std::vector<bool>>   testVector{
        {F, F, F, F, T},
        {F, F, F, T, T},
        {F, F, T, F, T},
        {F, F, T, T, T},
        {F, T, F, F, T},
        {F, T, F, T, T},
        {F, T, T, F, T},
        {F, T, T, T, T},
        {T, F, F, F, T},
    };

    std::vector<std::vector<bool>> expected{
        {F, F, F, F, F, F, F, F},
        {F, F, F, F, F, F, F, T},
        {F, F, F, F, F, F, T, F},
        {F, F, F, F, F, T, F, F},
        {F, F, F, F, T, F, F, F},
        {F, F, F, T, F, F, F, F},
        {F, F, T, F, F, F, F, F},
        {F, T, F, F, F, F, F, F},
        {T, F, F, F, F, F, F, F},
    };

    std::vector<bool> do_asserts{
        true
    };

     test_NtoM_template<NodeROM, bool, bool>(testVector, expected, do_asserts);

}

//-----------------------------------------------------------
void test16_02(void)
{
    std::cout << "-- TEST 16-02 Register --" << std::endl;

    std::vector<std::vector<bool>>   testVector{
        {F, F, T, F, F, F, F}, // [ 0] RESET
        {T, F, T, F, F, F, F}, // [ 1] non-RESET
        {T, T, T, F, F, F, F}, // [ 2] edge-up
        {T, F, T, F, T, F, T}, // [ 3] edge-down
        {T, T, T, F, T, F, T}, // [ 4] edge-up
        {T, F, T, T, F, T, F}, // [ 5] edge-down
        {T, T, T, T, F, T, F}, // [ 6] edge-up
        {T, F, F, T, T, T, T}, // [ 7] edge-down
        {T, T, F, T, T, T, T}, // [ 8] edge-up/LD=0
        {T, F, T, T, T, T, T}, // [ 9] edge-down
        {T, T, T, T, T, T, T}, // [10] edge-up
        {F, T, T, T, T, T, T}, // [11] RESET
    };

    std::vector<std::vector<bool>> expected{
        {F, F, F, F}, // [ 0] 
        {F, F, F, F}, // [ 1] 
        {F, F, F, F}, // [ 2] 0000
        {F, F, F, F}, // [ 3] 
        {F, T, F, T}, // [ 4] 0101
        {F, T, F, T}, // [ 5] 
        {T, F, T, F}, // [ 6] 1010
        {T, F, T, F}, // [ 7] 
        {T, F, T, F}, // [ 8] 1010(stay)
        {T, F, T, F}, // [ 9] 
        {T, T, T, T}, // [10] 1111
        {F, F, F, F}, // [11] 0000
    };

    std::vector<bool> do_asserts{
        true, true, true, true, true,
        true, true, true, true, true,
        true, true, true, true, true,
    };

     test_NtoM_template<NodeRegister, bool, bool>(testVector, expected, do_asserts);

}

//-----------------------------------------------------------
void test16_03_01(void)
{
    std::cout << "-- TEST 16-03-01 Node3Selector --" << std::endl;

    std::vector<std::vector<bool>>   testVector{
    //   S0, S1, S2, D0, D1, D2
        {T,  F,  F,  F,  F,  F}, // [ 0]
        {T,  F,  F,  T,  F,  F}, // [ 1]
        {F,  T,  F,  F,  F,  F}, // [ 2]
        {F,  T,  F,  F,  T,  F}, // [ 3]
        {F,  F,  T,  F,  F,  F}, // [ 4]
        {F,  F,  T,  F,  F,  T}, // [ 5]
    };

    std::vector<std::vector<bool>> expected{
    //   Q
        {F}, // [ 0] 
        {T}, // [ 1] 
        {F}, // [ 2] 
        {T}, // [ 3] 
        {F}, // [ 4] 
        {T}, // [ 5] 
    };

    std::vector<bool> do_asserts{
        true, true, true, true, true,
        true, true, true, true, true,
        true, true, true, true, true,
    };

     test_NtoM_template<Node3Selector, bool, bool>(testVector, expected, do_asserts);
}

//-----------------------------------------------------------
void test16_03_02(void)
{
    std::cout << "-- TEST 16-03-01 NodeUpLdSelector --" << std::endl;

    std::vector<std::vector<bool>>   testVector{
    //   UP, LD
        {F,  F}, // [ 0]
        {T,  F}, // [ 1]
        {F,  T}, // [ 2]
        {T,  T}, // [ 3]
    };

    std::vector<std::vector<bool>> expected{
    //   HOLD, UP_EN, LD
        {T,    F,     F}, // [ 0] 
        {F,    T,     F}, // [ 1] 
        {F,    F,     T}, // [ 2] 
        {F,    F,     T}, // [ 3] 
    };

    std::vector<bool> do_asserts{
        true, true, true, true, true,
        true, true, true, true, true,
        true, true, true, true, true,
    };

     test_NtoM_template<NodeUpLdSelector, bool, bool>(testVector, expected, do_asserts);
}

//-----------------------------------------------------------
void test16_03(void)
{
    std::cout << "-- TEST 16-03 PC --" << std::endl;

    std::vector<std::vector<bool>>   testVector{
    //   RST, CK, LD, UP, D0, D1, D2, D3
        {F,   F,  F,  F,  F,  F,  F,  F}, // [ 0] RESET
        {T,   F,  F,  F,  F,  F,  F,  F}, // [ 1] LD/UP = 0/0
        {T,   T,  F,  F,  F,  F,  F,  F}, // [ 2] clock-up
        {T,   F,  F,  T,  F,  F,  F,  F}, // [ 3] LD/UP = 0/1
        {T,   T,  F,  T,  F,  F,  F,  F}, // [ 4] clock-up
        {T,   F,  F,  T,  F,  F,  F,  F}, // [ 5] LD/UP = 0/1
        {T,   T,  F,  T,  F,  F,  F,  F}, // [ 6] clock-up
        {T,   F,  F,  T,  F,  F,  F,  F}, // [ 7] LD/UP = 0/1
        {T,   T,  F,  T,  F,  F,  F,  F}, // [ 8] clock-up
        {T,   F,  F,  T,  F,  F,  F,  F}, // [ 9] LD/UP = 0/1
        {T,   T,  F,  T,  F,  F,  F,  F}, // [10] clock-up
        {T,   F,  T,  F,  F,  F,  T,  T}, // [11] LD/UP = 1/0
        {T,   T,  T,  F,  F,  F,  T,  T}, // [12] clock-up
        {T,   F,  F,  F,  F,  F,  T,  T}, // [13] LD/UP = 0/0
        {T,   T,  F,  F,  F,  F,  T,  T}, // [14] clock-up
    };

    std::vector<std::vector<bool>> expected{
    //   Q0, Q1, Q2, Q3
        {F,  F,  F,  F}, // [ 0] 
        {F,  F,  F,  F}, // [ 1]
        {F,  F,  F,  F}, // [ 2] Q(0)
        {F,  F,  F,  F}, // [ 3] 
        {T,  F,  F,  F}, // [ 4] Q+1(1)
        {T,  F,  F,  F}, // [ 5] 
        {F,  T,  F,  F}, // [ 6] Q+1(2)
        {F,  T,  F,  F}, // [ 7] 
        {T,  T,  F,  F}, // [ 8] Q+1(3)
        {T,  T,  F,  F}, // [ 9]
        {F,  F,  T,  F}, // [10] Q+1(4)
        {F,  F,  T,  F}, // [11] 
        {F,  F,  T,  T}, // [12] Q <- D(12) 
        {F,  F,  T,  T}, // [13]
        {F,  F,  T,  T}, // [14] stay
    };

    std::vector<bool> do_asserts{
        true, true, true, true, true,
        true, true, true, true, true,
        true, true, true, true, true,
    };

     test_NtoM_template<NodePC, bool, bool>(testVector, expected, do_asserts);
}

//-----------------------------------------------------------
void test16(void)
{
    test16_01();
    test16_02();
    test16_03_01();
    test16_03_02();
    test16_03();
}


#endif


