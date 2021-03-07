// test_06.hpp
// Test for Arithmetic Node
// 算術ノードの評価
// add, mul gain. const

#ifndef __TEST_06_HPP__
#define __TEST_06_HPP__

#include "Edge.hpp"
#include "Node.hpp"
#include "Executor.hpp"
#include "NodeTestBase.hpp"
#include "NodeArithmetic.hpp"



//-----------------------------------------------------------
void test06_add(void)
{
    std::tuple<Executor*, NodeTestEntry<double>*> ret = test_2to1<NodeAdd, double>();
    auto exe = std::get<0>(ret);
    auto nEntry = std::get<1>(ret);
    nEntry->setValues(std::vector<double>{1.1, 2.2});
    exe->step();
}

//-----------------------------------------------------------
void test06_mul(void)
{
    std::tuple<Executor*, NodeTestEntry<double>*> ret = test_2to1<NodeMul, double>();
    auto exe = std::get<0>(ret);
    auto nEntry = std::get<1>(ret);
    nEntry->setValues(std::vector<double>{1.1, 2.2});
    exe->step();
}

//-----------------------------------------------------------
void test06_gain(void)
{
    std::tuple<Executor*, NodeTestEntry<double>*> ret = test_1to1<NodeGain, double>(2.0);
    auto exe = std::get<0>(ret);
    auto nEntry = std::get<1>(ret);
    nEntry->setValues(std::vector<double>{3.1});
    exe->step();
}

//-----------------------------------------------------------
void test06_const(void)
{
    std::tuple<Executor*, NodeTestEntry<double>*> ret = test_1to1<NodeConst, double>(2.0);
    auto exe = std::get<0>(ret);
    auto nEntry = std::get<1>(ret);
    nEntry->setValues(std::vector<double>{3.1});
    exe->step();
}

//-----------------------------------------------------------
void test06(void)
{
    test06_add();
    test06_mul();
    test06_gain();
    test06_const();
}

#endif


