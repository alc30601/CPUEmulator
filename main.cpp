// main.cpp
//
// Remote git repository is below.
// https://github.com/alc30601/DataFlowGraph.git

#include <iostream>
#include <memory>

// #include "test_01.hpp"
// #include "test_02.hpp"
// #include "test_03.hpp"
#include "test_04.hpp"
// #include "test_05.hpp"
// #include "test_06.hpp"
// #include "test_07.hpp"
// #include "test_08.hpp"
// #include "test_09.hpp"
#include "test_10.hpp"
#include "test_11.hpp"
#include "test_12.hpp"
#include "test_13.hpp"
#include "test_14.hpp"
#include "test_15.hpp"
#include "test_16.hpp"
#include "test_17.hpp"

#include "CPU.hpp"


//-----------------------------------------------------------
void test(void)
{
    std::cout << "-- Start - DataFlowGraph -- " << std::endl;

    std::cout << "  -- test13" << std::endl;
    test13();

    std::cout << "  -- test04" << std::endl;
    test04();

    std::cout << "  -- test14" << std::endl;
    test14();

    std::cout << "  -- test12" << std::endl;
    test12();

    std::cout << "  -- test15" << std::endl;
    test15();

    std::cout << "  -- test16" << std::endl;
    test16();

    std::cout << "  -- test17" << std::endl;
    test17();

    std::cout << "... Test Done." << std::endl;
}

//-----------------------------------------------------------
void synTest(void)
{
    std::vector<std::string> v = {"aaa", "bbb", "ccc"};

    std::string s1 = "ddd";
    v.insert(v.begin(), s1);

    std::string s2 = "eee";
    v.insert(v.end(), s2);

}

//-----------------------------------------------------------
int main(void)
{
    synTest();

    test();

    CPU();
}
