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

#include "exp_02.hpp"



//-----------------------------------------------------------
int main(void)
{
    std::cout << "-- Start - DataFlowGraph -- " << std::endl;

    // test1();
    // test2();
    // test3();
    // test4();

    exp_02();

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

    std::cout << "... Done." << std::endl;
    return 0;
}


