// test_17.hpp
#ifndef __TEST_17_HPP__
#define __TEST_17_HPP__

#include "NodeTestBase.hpp"
#include "NodeCPU.hpp"


//-----------------------------------------------------------
// 単に実行できるか確認するだけ。
void test17(void)
{
    std::cout << "-- TEST 17 CPU --" << std::endl;

    std::vector<std::vector<bool>>   testVector{
    //   1    2   3   4   5   6
    //   RST, CK, I0, I1, I2, I3
        {F,   F,  F,  F,  F,  F },
        {T,   F,  F,  F,  F,  F },
        {T,   T,  F,  F,  F,  F },
        {T,   F,  F,  F,  F,  F },
        {T,   T,  F,  F,  F,  F },
        {T,   F,  F,  F,  F,  F },
        {T,   T,  F,  F,  F,  F },
        {T,   F,  F,  F,  F,  F },
        {T,   T,  F,  F,  F,  F },
    };

    std::vector<std::vector<bool>> expected{
    //   1   2   3   4
    //   Y0, Y1, Y2, Y3
        {F,  F,  F,  F},
        {F,  F,  F,  F},
        {F,  F,  F,  F},
        {F,  F,  F,  F},
        {F,  F,  F,  F},
        {F,  F,  F,  F},
        {F,  F,  F,  F},
        {F,  F,  F,  F},
        {F,  F,  F,  F},
    };



    std::vector<bool> do_asserts{
        true, true, true, true, true,
        true, true, true, true, true,
        true, true, true, true, true,
        true, true, true, true, true,
    };

     test_NtoM_template<NodeCPU, bool, bool>(testVector, expected, do_asserts);

}

#endif



