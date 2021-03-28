// test_11.hpp
// NodeValueのテスト
#ifndef __TEST_11_HPP__
#define __TEST_11_HPP__

#include "NodeValue.hpp"
#include "NodeArithmetic.hpp"
#include "NodeTestBase.hpp"

//-----------------------------------------------------------
// 入力ありの場合。グラフ逆流無し
void test11_01(void)
{
    std::cout << "-- TEST11-01 a input no loop --" << std::endl;

    GraphBuilder gb;
    auto qnT = gb.createNode<NodeValue<int>>();
    auto qnS = gb.createNode<NodeTestEntry<int>>();
    auto qnE = gb.createNode<NodeTestExit<int>>();

    gb.outto(Port(qnS, 1), Ports{ Port(qnT, 1) });
    gb.outto(Port(qnT, 1), Ports{ Port(qnE, 1) });

    Executor* exe = gb.createExecutor(qnS);

    auto nVal = static_cast<NodeValue<int>*>(qnT.getNode());
    nVal->setValue(0);

    std::vector<std::vector<int>> testVector{{1},{2},{3},{4}};
    std::vector<std::vector<int>>   expected{{0},{1},{2},{3}};
    for(int i=0;i<testVector.size();i++){
        evaluation<int>(exe, qnS, qnE, testVector[i], expected[i]);
    }
}

//-----------------------------------------------------------
// 入力無しの場合。グラフ逆流無し
void test11_02(void)
{
    std::cout << "-- TEST11-02 no input no loop --" << std::endl;

    GraphBuilder gb;
    auto qnT = gb.createNode<NodeValue<int>>();
    auto qnS = gb.createNode<NodeTestEntry<int>>();
    auto qnE = gb.createNode<NodeTestExit<int>>();

    gb.outto(Port(qnT, 1), Ports{ Port(qnE, 1) });

    Executor* exe = gb.createExecutor(qnS);

    auto nVal = static_cast<NodeValue<int>*>(qnT.getNode());
    nVal->setValue(7);

    std::vector<std::vector<int>> testVector{{1},{2},{3},{4}};
    std::vector<std::vector<int>>   expected{{7},{7},{7},{7}};
    for(int i=0;i<testVector.size();i++){
        evaluation<int>(exe, qnS, qnE, testVector[i], expected[i]);
    }
}

//-----------------------------------------------------------
// 巡回するグラフ
// 自分の出力ポートが自分の入力ポートにつながる。
void test11_03(void)
{
    std::cout << "-- TEST11-03 a input a loop --" << std::endl;

    GraphBuilder gb;
    auto qnT = gb.createNode<NodeValue<int>>();
    auto qnS = gb.createNode<NodeTestEntry<int>>();
    auto qnE = gb.createNode<NodeTestExit<int>>();

    gb.outto(Port(qnT, 1), Ports{ Port(qnT, 1), Port(qnE, 1) });

    Executor* exe = gb.createExecutor(qnS);

    auto nVal = static_cast<NodeValue<int>*>(qnT.getNode());
    nVal->setValue(9);

    std::vector<std::vector<int>> testVector{{1},{2},{3},{4}};
    std::vector<std::vector<int>>   expected{{9},{9},{9},{9}};    
    for(int i=0;i<testVector.size();i++){
        evaluation<int>(exe, qnS, qnE, testVector[i], expected[i]);
    }

}

//-----------------------------------------------------------
// カウンタ
void test11_04(void)
{
    std::cout << "-- TEST11-04 counter --" << std::endl;

    GraphBuilder gb;
    auto qnCounter = gb.createNode<NodeValue<int>>();
    auto qnStep = gb.createNode<NodeValue<int>>();
    auto qnAdd = gb.createNode<NodeAdd<int>>();

    auto qnS = gb.createNode<NodeTestEntry<int>>();
    auto qnE = gb.createNode<NodeTestExit<int>>();

    gb.outto(Port(qnStep, 1),    Ports{ Port(qnAdd, 1) });
    gb.outto(Port(qnCounter, 1), Ports{ Port(qnAdd, 2), Port(qnE, 1) });
    gb.outto(Port(qnAdd, 1), Ports{ Port(qnCounter, 1) });

    Executor* exe = gb.createExecutor(qnS);

    auto nVal = static_cast<NodeValue<int>*>(qnCounter.getNode());
    nVal->setValue(0);

    auto nStep = static_cast<NodeValue<int>*>(qnStep.getNode());
    nStep->setValue(1);

    std::vector<std::vector<int>> testVector{{},{},{},{}};
    std::vector<std::vector<int>>   expected{{0},{1},{2},{3}};    
    for(int i=0;i<expected.size();i++){
        evaluation<int>(exe, qnS, qnE, testVector[i], expected[i]);
    }

    // 100回カウンタ
    nVal->setValue(0);
    for(int i=0;i<100;i++){
        exe->step();
    }

}

//-----------------------------------------------------------
void test11(void)
{
    test11_01();
    test11_02();
    test11_03();
    test11_04();
}

#endif

