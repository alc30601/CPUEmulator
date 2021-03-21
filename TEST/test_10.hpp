// test_10.hpp
#ifndef __TEST_10_HPP__
#define __TEST_10_HPP__

#include <vector>
#include <cassert>

#include "Executor.hpp"
#include "GraphBuilder.hpp"
#include "NodeAdder.hpp"
#include "NodeTestBase.hpp"


//-----------------------------------------------------------
void test10_halfaddr(void)
{
    std::cout << "-- TEST HalfAddr --" << std::endl;

    GraphBuilder gb;
    auto qnT = gb.createNode<NodeHalfAdder>();
    auto qnS = gb.createNode<NodeTestEntry<bool>>();
    auto qnE = gb.createNode<NodeTestExit<bool>>();

    gb.outto(Port(qnS, 1), Ports{ Port(qnT, 1) });
    gb.outto(Port(qnS, 2), Ports{ Port(qnT, 2) });
    gb.outto(Port(qnT, 1), Ports{ Port(qnE, 1) });
    gb.outto(Port(qnT, 2), Ports{ Port(qnE, 2) });

    Executor* exe = gb.createExecutor(qnS);

    std::vector<std::vector<bool>> testVector{{false,false},{false,true}, {true,false},{true,true}};
    std::vector<std::vector<bool>>   expected{{false,false},{true,false}, {true,false},{false,true}};

    for(int i=0;i<testVector.size();i++){
        evaluation<bool>(exe, qnS, qnE, testVector[i], expected[i]);
    }
}

//-----------------------------------------------------------
void test10_fulladdr(void)
{
    std::cout << "-- TEST FullAddr --" << std::endl;

    GraphBuilder gb;
    auto qnT = gb.createNode<NodeFullAdder>();
    auto qnS = gb.createNode<NodeTestEntry<bool>>();
    auto qnE = gb.createNode<NodeTestExit<bool>>();

    gb.outto(Port(qnS, 1), Ports{ Port(qnT, 1) });
    gb.outto(Port(qnS, 2), Ports{ Port(qnT, 2) });
    gb.outto(Port(qnS, 3), Ports{ Port(qnT, 3) });
    gb.outto(Port(qnT, 1), Ports{ Port(qnE, 1) });
    gb.outto(Port(qnT, 2), Ports{ Port(qnE, 2) });

    Executor* exe = gb.createExecutor(qnS);

    std::vector<std::vector<bool>> testVector{
        {false,false, false},
        {false,false, true},
        {false,true,  false},
        {false,true,  true},

        {true, false, false},
        {true, false, true},
        {true, true,  false},
        {true, true,  true}
    };

    std::vector<std::vector<bool>>   expected{
        {false,false},
        {true, false}, 
        {true, false},
        {false,true},

        {true, false},
        {false,true}, 
        {false,true},
        {true, true}
    };

    for(int i=0;i<testVector.size();i++){
        evaluation<bool>(exe, qnS, qnE, testVector[i], expected[i]);
    }

}

//-----------------------------------------------------------
void test10(void)
{
    test10_halfaddr();
    test10_fulladdr();
}


#endif


