// test_10.hpp
// 半加算器、全加算器
// 4bit加算器、4bit加減算器
// マルチプレクサ
// デコーダー
#ifndef __TEST_10_HPP__
#define __TEST_10_HPP__

#include <vector>
#include <cassert>

#include "Executor.hpp"
#include "GraphBuilder.hpp"
#include "NodeAdder.hpp"
#include "NodeMultiplexer.hpp"
#include "NodeTestBase.hpp"
#include "NodeUtils.hpp"
#include "NodeDecoder.hpp"


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
        evaluation<bool, bool>(exe, qnS, qnE, testVector[i], expected[i]);
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

    std::vector<std::vector<bool>>   testVector = vectorBool3bits;

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
        evaluation<bool, bool>(exe, qnS, qnE, testVector[i], expected[i]);
    }

}

//-----------------------------------------------------------
// int -> ビット変換ノードのテスト
void test10_int2bit(void)
{
    std::cout << "-- TEST int to bit --" << std::endl;

    GraphBuilder gb;
    auto qnT = gb.createNode<NodeDigit2Bit<4>>();
    auto qnS = gb.createNode<NodeTestEntry<int>>();
    auto qnE = gb.createNode<NodeTestExit<bool>>();

    gb.outto(Port(qnS, 1), Ports{ Port(qnT, 1) });
    gb.outto(Port(qnT, 1), Ports{ Port(qnE, 1) });
    gb.outto(Port(qnT, 2), Ports{ Port(qnE, 2) });
    gb.outto(Port(qnT, 3), Ports{ Port(qnE, 3) });
    gb.outto(Port(qnT, 4), Ports{ Port(qnE, 4) });

    Executor* exe = gb.createExecutor(qnS);

    std::vector<std::vector<int>> testVector{
        {0},{1},{2},{3},{4},{5},{6},{7},{8},{9},{10},{11},{12},{13},{14},{15}
    };

    std::vector<std::vector<bool>>   expected = vectorBool4bits;

    for(int i=0;i<testVector.size();i++){
        evaluation<int, bool>(exe, qnS, qnE, testVector[i], expected[i]);
    }

}

//-----------------------------------------------------------
// ビット -> int 変換ノードのテスト
void test10_bit2int(void)
{
    std::cout << "-- TEST int to bit --" << std::endl;

    GraphBuilder gb;
    auto qnT = gb.createNode<NodeBit2Digit<4>>();
    auto qnS = gb.createNode<NodeTestEntry<bool>>();
    auto qnE = gb.createNode<NodeTestExit<int>>();

    gb.outto(Port(qnS, 1), Ports{ Port(qnT, 1) });
    gb.outto(Port(qnS, 2), Ports{ Port(qnT, 2) });
    gb.outto(Port(qnS, 3), Ports{ Port(qnT, 3) });
    gb.outto(Port(qnS, 4), Ports{ Port(qnT, 4) });
    gb.outto(Port(qnT, 1), Ports{ Port(qnE, 1) });

    Executor* exe = gb.createExecutor(qnS);

    std::vector<std::vector<bool>>   testVector = vectorBool4bits;

    std::vector<std::vector<int>> expected{
        {0},{1},{2},{3},{4},{5},{6},{7},{8},{9},{10},{11},{12},{13},{14},{15}
    };

    for(int i=0;i<testVector.size();i++){
        evaluation<bool, int>(exe, qnS, qnE, testVector[i], expected[i]);
    }

}

//-----------------------------------------------------------
// int -> ビット変換ノード
// ビット -> int 変換ノード
// の複合系。結局int -> intの確認
void test10_int2int(void)
{
    std::cout << "-- TEST int to bit --" << std::endl;

    GraphBuilder gb;
    auto qnA = gb.createNode<NodeDigit2Bit<4>>();
    auto qnB = gb.createNode<NodeBit2Digit<4>>();
    auto qnS = gb.createNode<NodeTestEntry<int>>();
    auto qnE = gb.createNode<NodeTestExit<int>>();

    gb.outto(Port(qnS, 1), Ports{ Port(qnA, 1) });

    gb.outto(Port(qnA, 1), Ports{ Port(qnB, 1) });
    gb.outto(Port(qnA, 2), Ports{ Port(qnB, 2) });
    gb.outto(Port(qnA, 3), Ports{ Port(qnB, 3) });
    gb.outto(Port(qnA, 4), Ports{ Port(qnB, 4) });

    gb.outto(Port(qnB, 1), Ports{ Port(qnE, 1) });

    Executor* exe = gb.createExecutor(qnS);


    std::vector<std::vector<int>>   testVector{
        {0},{1},{2},{3},{4},{5},{6},{7},{8},{9},{10},{11},{12},{13},{14},{15}
    };

    std::vector<std::vector<int>> expected{
        {0},{1},{2},{3},{4},{5},{6},{7},{8},{9},{10},{11},{12},{13},{14},{15}
    };

    for(int i=0;i<testVector.size();i++){
        evaluation<int, int>(exe, qnS, qnE, testVector[i], expected[i]);
    }

}

//-----------------------------------------------------------
// |S|----|A|--|T|--|C|--|E|
// | |    | |--| |--| |
// | |    | |--| |--| |
// | |    | |--| |--| |
// | |         | |
// | |----|B|--| |-------|D|
// | |    | |--| |
// | |    | |--| |
// | |    | |--| |

void test10_4bitadder(void)
{
    std::cout << "-- TEST 4 bit Addr --" << std::endl;

    GraphBuilder gb;
    auto qnS = gb.createNode<NodeTestEntry<int>>();
    auto qnE = gb.createNode<NodeTestExit<int>>();
    auto qnD = gb.createNode<NodeTestExit<bool>>();
    auto qnT = gb.createNode<NodeAddr4bit>();
    auto qnA = gb.createNode<NodeDigit2Bit<4>>();
    auto qnB = gb.createNode<NodeDigit2Bit<4>>();
    auto qnC = gb.createNode<NodeBit2Digit<4>>();

    gb.outto(Port(qnS, 1), Ports{ Port(qnA, 1) });
    gb.outto(Port(qnS, 2), Ports{ Port(qnB, 1) });
    gb.outto(Port(qnC, 1), Ports{ Port(qnE, 1) });

    gb.outto(Port(qnA, 1), Ports{ Port(qnT, 1) });
    gb.outto(Port(qnA, 2), Ports{ Port(qnT, 2) });
    gb.outto(Port(qnA, 3), Ports{ Port(qnT, 3) });
    gb.outto(Port(qnA, 4), Ports{ Port(qnT, 4) });

    gb.outto(Port(qnB, 1), Ports{ Port(qnT, 5) });
    gb.outto(Port(qnB, 2), Ports{ Port(qnT, 6) });
    gb.outto(Port(qnB, 3), Ports{ Port(qnT, 7) });
    gb.outto(Port(qnB, 4), Ports{ Port(qnT, 8) });

    gb.outto(Port(qnT, 1), Ports{ Port(qnC, 1) });
    gb.outto(Port(qnT, 2), Ports{ Port(qnC, 2) });
    gb.outto(Port(qnT, 3), Ports{ Port(qnC, 3) });
    gb.outto(Port(qnT, 4), Ports{ Port(qnC, 4) });

    gb.outto(Port(qnT, 5), Ports{ Port(qnD, 1) });

    Executor* exe = gb.createExecutor(qnS);

    std::vector<std::vector<int>>   testVector{
        {2, 3}, {0, 1}, {7, 8}, {8, 8}, {10, 10}
    };

    std::vector<std::vector<int>> expectedV{
        {5}, {1}, {15}, {0}, {4}
    };

    std::vector<std::vector<bool>> expectedC{
        {false}, {false}, {false}, {true}, {true}
    };

    auto nEntry = static_cast<NodeTestEntry<int>*>(qnS.getNode());
    auto nExitV = static_cast<NodeTestExit<int>*>(qnE.getNode());
    auto nExitC = static_cast<NodeTestExit<bool>*>(qnD.getNode());

    for(int i=0;i<testVector.size();i++){

        nEntry->setValues(testVector[i]);
        exe->step();
        auto outputV = nExitV->getValues();
        auto outputC = nExitC->getValues();

        // 答え合わせ
        assert(outputV == expectedV[i]);
        assert(outputC == expectedC[i]);

    }

}

//-----------------------------------------------------------
// |S|----|A|--|T|--|C|--|E|
// | |    | |--| |--| |
// | |    | |--| |--| |
// | |    | |--| |--| |
// | |         | |
// | |----|B|--| |-------|D|
// | |    | |--| |
// | |    | |--| |
// | |    | |--| |
//             | |
// |Z|---------| |
// 
void test10_4bitaddsub(void)
{
    std::cout << "-- TEST 4 bit Add/Sub --" << std::endl;

    GraphBuilder gb;
    auto qnS = gb.createNode<NodeTestEntry<int>>();
    auto qnZ = gb.createNode<NodeTestEntry<bool>>();
    auto qnE = gb.createNode<NodeTestExit<int>>();
    auto qnD = gb.createNode<NodeTestExit<bool>>();
    auto qnT = gb.createNode<NodeAddSub4bit>();
    auto qnA = gb.createNode<NodeDigit2Bit<4>>();
    auto qnB = gb.createNode<NodeDigit2Bit<4>>();
    auto qnC = gb.createNode<NodeBit2Digit<4>>();

    gb.outto(Port(qnS, 1), Ports{ Port(qnA, 1) });
    gb.outto(Port(qnS, 2), Ports{ Port(qnB, 1) });

    gb.outto(Port(qnA, 1), Ports{ Port(qnT, 1) });
    gb.outto(Port(qnA, 2), Ports{ Port(qnT, 2) });
    gb.outto(Port(qnA, 3), Ports{ Port(qnT, 3) });
    gb.outto(Port(qnA, 4), Ports{ Port(qnT, 4) });

    gb.outto(Port(qnB, 1), Ports{ Port(qnT, 5) });
    gb.outto(Port(qnB, 2), Ports{ Port(qnT, 6) });
    gb.outto(Port(qnB, 3), Ports{ Port(qnT, 7) });
    gb.outto(Port(qnB, 4), Ports{ Port(qnT, 8) });

    gb.outto(Port(qnZ, 1), Ports{ Port(qnT, 9) });

    gb.outto(Port(qnT, 1), Ports{ Port(qnC, 1) });
    gb.outto(Port(qnT, 2), Ports{ Port(qnC, 2) });
    gb.outto(Port(qnT, 3), Ports{ Port(qnC, 3) });
    gb.outto(Port(qnT, 4), Ports{ Port(qnC, 4) });

    gb.outto(Port(qnT, 5), Ports{ Port(qnD, 1) });

    gb.outto(Port(qnC, 1), Ports{ Port(qnE, 1) });


    Executor* exe = gb.createExecutor(qnS);

    std::vector<std::vector<int>>   testVector{
        {2, 3},  {0, 1}, {7, 8},   {8, 8}, {10, 10},
        {12, 7}, {1, 0}, {15, 15}, {3, 5}, {0, 1}
    };

    std::vector<std::vector<bool>>   testSign{
        {false}, {false}, {false}, {false}, {false}, 
        {true},  {true},  {true},  {true},  {true}, 
    };

    std::vector<std::vector<int>> expectedV{
        {5}, {1}, {15}, {0}, {4},
        {5}, {1}, {0},  {14}, {15}
    };

    std::vector<std::vector<bool>> expectedC{
        {false}, {false}, {false}, {true},  {true},
        {true},  {true},  {true},  {false}, {false}
    };

    auto nEntryV = static_cast<NodeTestEntry<int>*>(qnS.getNode());
    auto nEntryS = static_cast<NodeTestEntry<bool>*>(qnZ.getNode());
    auto nExitV = static_cast<NodeTestExit<int>*>(qnE.getNode());
    auto nExitC = static_cast<NodeTestExit<bool>*>(qnD.getNode());

    for(int i=0;i<testVector.size();i++){

        nEntryV->setValues(testVector[i]);
        nEntryS->setValues(testSign[i]);
        exe->step();
        auto outputV = nExitV->getValues();
        auto outputC = nExitC->getValues();

        // 答え合わせ
        assert(outputV == expectedV[i]);
        assert(outputC == expectedC[i]);

    }

}

//-----------------------------------------------------------
void test10_multiplexer(void)
{
    std::cout << "-- TEST Multiplexer --" << std::endl;

    GraphBuilder gb;
    auto qnS = gb.createNode<NodeTestEntry<bool>>();
    auto qnE = gb.createNode<NodeTestExit<bool>>();
    auto qnT = gb.createNode<NodeMultiplexer>();

    gb.outto(Port(qnS, 1), Ports{ Port(qnT, 1) });
    gb.outto(Port(qnS, 2), Ports{ Port(qnT, 2) });
    gb.outto(Port(qnS, 3), Ports{ Port(qnT, 3) });
    gb.outto(Port(qnT, 1), Ports{ Port(qnE, 1) });

    Executor* exe = gb.createExecutor(qnS);

    std::vector<std::vector<bool>>   testVector = vectorBool3bits;

    std::vector<std::vector<bool>> expected{
        {false},
        {false},
        {true},
        {true},
        {false},
        {true},
        {false},
        {true}
    };

    for(int i=0;i<testVector.size();i++){
        evaluation<bool, bool>(exe, qnS, qnE, testVector[i], expected[i]);
    }

}

//-----------------------------------------------------------
void test10_demultiplexer(void)
{
    std::cout << "-- TEST DeMultiplexer --" << std::endl;

    GraphBuilder gb;
    auto qnS = gb.createNode<NodeTestEntry<bool>>();
    auto qnE = gb.createNode<NodeTestExit<bool>>();
    auto qnT = gb.createNode<NodeDeMultiplexer>();

    gb.outto(Port(qnS, 1), Ports{ Port(qnT, 1) });
    gb.outto(Port(qnS, 2), Ports{ Port(qnT, 2) });
    gb.outto(Port(qnT, 1), Ports{ Port(qnE, 1) });
    gb.outto(Port(qnT, 2), Ports{ Port(qnE, 2) });

    Executor* exe = gb.createExecutor(qnS);

    std::vector<std::vector<bool>>   testVector = vectorBool2bits;

    std::vector<std::vector<bool>> expected{
        {false, false},
        {true , false},
        {false, false},
        {false, true},
    };

    for(int i=0;i<testVector.size();i++){
        evaluation<bool, bool>(exe, qnS, qnE, testVector[i], expected[i]);
    }

}

//-----------------------------------------------------------
void test10_decoder(void)
{
    std::cout << "-- TEST Decoder --" << std::endl;

    GraphBuilder gb;
    auto qnS = gb.createNode<NodeTestEntry<bool>>();
    auto qnE = gb.createNode<NodeTestExit<bool>>();
    auto qnT = gb.createNode<NodeDecoder4to16>();

    gb.outto(Port(qnS, 1), Ports{ Port(qnT, 1) });
    gb.outto(Port(qnS, 2), Ports{ Port(qnT, 2) });
    gb.outto(Port(qnS, 3), Ports{ Port(qnT, 3) });
    gb.outto(Port(qnS, 4), Ports{ Port(qnT, 4) });

    gb.outto(Port(qnT, 1), Ports{ Port(qnE, 1) });
    gb.outto(Port(qnT, 2), Ports{ Port(qnE, 2) });
    gb.outto(Port(qnT, 3), Ports{ Port(qnE, 3) });
    gb.outto(Port(qnT, 4), Ports{ Port(qnE, 4) });
    gb.outto(Port(qnT, 5), Ports{ Port(qnE, 5) });
    gb.outto(Port(qnT, 6), Ports{ Port(qnE, 6) });
    gb.outto(Port(qnT, 7), Ports{ Port(qnE, 7) });
    gb.outto(Port(qnT, 8), Ports{ Port(qnE, 8) });
    gb.outto(Port(qnT, 9), Ports{ Port(qnE, 9) });
    gb.outto(Port(qnT, 10), Ports{ Port(qnE, 10) });
    gb.outto(Port(qnT, 11), Ports{ Port(qnE, 11) });
    gb.outto(Port(qnT, 12), Ports{ Port(qnE, 12) });
    gb.outto(Port(qnT, 13), Ports{ Port(qnE, 13) });
    gb.outto(Port(qnT, 14), Ports{ Port(qnE, 14) });
    gb.outto(Port(qnT, 15), Ports{ Port(qnE, 15) });
    gb.outto(Port(qnT, 16), Ports{ Port(qnE, 16) });

    Executor* exe = gb.createExecutor(qnS);

    std::vector<std::vector<bool>>   testVector = vectorBool4bits;

    std::vector<std::vector<bool>> expected{
        {true,  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},
        {false, true,  false, false, false, false, false, false, false, false, false, false, false, false, false, false},
        {false, false, true,  false, false, false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, true,  false, false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, true,  false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, true,  false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, true,  false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, true,  false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, true,  false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, true,  false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, true,  false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false, true,  false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false, false, true,  false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false, false, false, true,  false, false},
        {false, false, false, false, false, false, false, false, false, false, false, false, false, false, true,  false},
        {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true}
    };

    for(int i=0;i<testVector.size();i++){
        evaluation<bool, bool>(exe, qnS, qnE, testVector[i], expected[i]);
    }


}

//-----------------------------------------------------------
void test10(void)
{
    test10_halfaddr();
    test10_fulladdr();

    test10_int2bit();
    test10_bit2int();
    test10_int2int();

    test10_4bitadder();
    test10_4bitaddsub();

    test10_multiplexer();
    test10_demultiplexer();

    test10_decoder();
}


#endif


