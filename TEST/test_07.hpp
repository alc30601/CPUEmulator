// test_07.hpp
// 少し複雑なグラフ
// ２次方程式

#ifndef __TEST_07_HPP__
#define __TEST_07_HPP__

#include "Edge.hpp"
#include "Node.hpp"
#include "Executor.hpp"
#include "NodeTestBase.hpp"
#include "NodeArithmetic.hpp"


//-----------------------------------------------------------
std::tuple<Executor*, NodeTestEntry<double>*> test07_createGraph(double a, double b, double c)
{
    NodeTestEntry<double>* nEntry(new NodeTestEntry<double>);
    NodeTestExit<double>* nExit(new NodeTestExit<double>);
    NodeMul* nMul(new  NodeMul());
    NodeGain* nGainA(new NodeGain());       nGainA->setParam(a);
    NodeGain* nGainB(new NodeGain());       nGainB->setParam(b);
    NodeConst* nConstC(new NodeConst());    nConstC->setParam(c);
    NodeAdd* nAdd(new NodeAdd());

    Edge* e1(new Edge);
    Edge* e2(new Edge);
    Edge* e3(new Edge);
    Edge* e4(new Edge);
    Edge* e5(new Edge);
    Edge* e6(new Edge);
    Edge* e7(new Edge);

    nEntry->addOutEdges(std::vector<Edge*>{e1, e2});
    nMul->addInEdges(std::vector<Edge*>{e1, e1});
    nMul->addOutEdges(std::vector<Edge*>{e3});
    nGainA->addInEdges(std::vector<Edge*>{e3});
    nGainA->addOutEdges(std::vector<Edge*>{e4});
    nGainB->addInEdges(std::vector<Edge*>{e1});
    nGainB->addOutEdges(std::vector<Edge*>{e5});
    nConstC->addInEdges(std::vector<Edge*>{e2});
    nConstC->addOutEdges(std::vector<Edge*>{e6});
    nAdd->addInEdges(std::vector<Edge*>{e4, e5, e6});
    nAdd->addOutEdges(std::vector<Edge*>{e7});
    nExit->addInEdges(std::vector<Edge*>{e7});

    e1->addOutNodes(std::vector<Node*>{nMul, nMul, nGainB});
    e2->addOutNode(nConstC);
    e3->addOutNode(nGainA);
    e4->addOutNode(nAdd);
    e5->addOutNode(nAdd);
    e6->addOutNode(nAdd);
    e7->addOutNode(nExit);

    std::vector<Node*> nodes = {nEntry, nExit, nMul, nGainA, nGainB, nConstC, nAdd};
    std::vector<Edge*> edges = {e1, e2, e3, e4, e5, e6, e7};
    Executor* exe(new Executor(nEntry, nodes, edges));
    std::tuple<Executor*, NodeTestEntry<double>*> ret = std::make_tuple(exe, nEntry);
    return ret;
}

//-----------------------------------------------------------
void testQuadraticEquation(void)
{
    double a = 2.0;
    double b = 3.0;
    double c = 4.0;
    std::tuple<Executor*, NodeTestEntry<double>*> ret = test07_createGraph(a, b, c);

    auto exe = std::get<0>(ret);
    auto nEntry = std::get<1>(ret);

    for(double x = 2.0;x<3.0;x += 0.1){
        nEntry->setValues(std::vector<double>{x, 0.0});
        exe->step();
    }
}

//-----------------------------------------------------------
void test07(void)
{
    testQuadraticEquation();
}

#endif



