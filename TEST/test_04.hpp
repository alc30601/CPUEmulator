// test_04.hpp
//
// Notノード評価
//  NodeS ---> Not ---> NodeE
// Orノード評価
//  NodeS ---> And ---> NodeE
// Andノード評価
//  NodeS ---> And ---> NodeE

#ifndef __TEST_04_HPP__
#define __TEST_04_HPP__

#include <string>
#include <memory>
#include <vector>
#include <cassert>
#include <tuple>

#include "Edge.hpp"
#include "Node.hpp"
#include "Executor.hpp"
#include "NodeLogics.hpp"

#include "NodeTestBase.hpp"




//-----------------------------------------------------------
template<typename T>
void testOp1to1(std::vector<bool>& input, std::vector<bool>& expected)
{
    auto ret = test_1to1_template<T, bool>();
    auto exe = std::get<0>(ret);
    auto nEntry = static_cast<NodeTestEntry<bool>*>(std::get<1>(ret).getNode());
    auto nExit = static_cast<NodeTestExit<bool>*>(std::get<2>(ret).getNode());

    for(int i=0;i<input.size();i++){
        std::vector<bool> inValue{input[i]};
        nEntry->setValues(inValue);
        exe->step();
        auto values = nExit->getValues();
        assert(expected[i] == values[0]);
    }
}

//-----------------------------------------------------------
template<typename T>
void testOp2to1(std::vector<std::vector<bool>>& input, std::vector<bool>& expected)
{
    auto ret = test_2to1_template<T, bool>();
    auto exe = std::get<0>(ret);
    auto nEntry = static_cast<NodeTestEntry<bool>*>(std::get<1>(ret).getNode());
    auto nExit = static_cast<NodeTestExit<bool>*>(std::get<2>(ret).getNode());

    for(int i=0;i<input.size();i++){
        nEntry->setValues(input[i]);
        exe->step();
        auto values = nExit->getValues();
        assert(expected[i] == values[0]);
    }
}


//-----------------------------------------------------------
void test04(void)
{
    std::vector<bool> testVector1{false,true};

    std::cout << "-- test NOT -- " << std::endl;
    std::vector<bool> expectedNot{true, false};
    testOp1to1<NodeNot>(testVector1, expectedNot);


    std::vector<std::vector<bool>> testVector{{false,false},{false,true}, {true,false},{true,true}};

    std::cout << "-- test AND -- " << std::endl;
    std::vector<bool> expectedAND{false, false, false, true};
    testOp2to1<NodeAnd>(testVector, expectedAND);

    std::cout << "-- test OR -- " << std::endl;
    std::vector<bool> expectedOR{false, true, true, true};
    testOp2to1<NodeOr>(testVector, expectedOR);

    std::cout << "-- test NOR -- " << std::endl;
    std::vector<bool> expectedNOR{true, false, false, false};
    testOp2to1<NodeNor>(testVector, expectedNOR);

    std::cout << "-- test NAND -- " << std::endl;
    std::vector<bool> expectedNAND{true, true, true, false};
    testOp2to1<NodeNand>(testVector, expectedNAND);

    std::cout << "-- test EXOR -- " << std::endl;
    std::vector<bool> expectedEXOR{false, true, true, false};
    testOp2to1<NodeExor>(testVector, expectedEXOR);

}


#endif
