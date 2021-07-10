// test_16.hpp
// CPUパーツの単体評価
#ifndef __TEST_16_HPP__
#define __TEST_16_HPP__

#include <iostream>

#include "NodeTestBase.hpp"
#include "NodeROM.hpp"

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
void test16(void)
{
    test16_01();
}


#endif


