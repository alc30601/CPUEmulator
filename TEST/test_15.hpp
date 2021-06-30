// test_15.hpp
// Registerのテスト
#ifndef __TEST_15_HPP__
#define __TEST_15_HPP__

#include "NodeTestBase.hpp"
#include "NodeRegister.hpp"


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
void test15(void)
{
    test15_01();
}


#endif

