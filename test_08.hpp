// test_08.hpp
// GraphBuilderの評価
// グラフはa, b, cを入力、Gをパラメータとして
// (a+G*b)*(c+Gb)

#ifndef __TEST_08_HPP__
#define __TEST_08_HPP__

#include "Node.hpp"
#include "Edge.hpp"
#include "Executor.hpp"
#include "GraphBuilder.hpp"
#include "NodeComplex.hpp"

#include "NodeLogics.hpp"
#include "NodeArithmetic.hpp"
#include "NodeTestBase.hpp"


//-----------------------------------------------------------
// フラットなグラフ
void test08_01(void)
{
    // グラフの構築
    GraphBuilder gb;

    auto n0 = gb.createNode<NodeTestEntry<double>>();
    auto n1 = gb.createNode<NodeGain>();
    auto n2 = gb.createNode<NodeAdd>();
    auto n3 = gb.createNode<NodeAdd>();
    auto n4 = gb.createNode<NodeMul>();
    auto n5 = gb.createNode<NodeTestExit<double>>();

    gb.outto(Port(n0, 1), Port(n2, 1));
    gb.outto(Port(n0, 2), Port(n1, 1));
    gb.outto(Port(n0, 3), Port(n3, 2));
    gb.outto(Port(n1, 1), Port(n2, 2), Port(n3, 1));
    gb.outto(Port(n2, 1), Port(n4, 1));
    gb.outto(Port(n3, 1), Port(n4, 2));
    gb.outto(Port(n4, 1), Port(n5, 1));

    // 実行
    Executor* exe = gb.createExecutor(n0);

    auto nGain = static_cast<NodeGain*>(n1.getNode());
    nGain->setParam(1.3);
    auto nEntry = static_cast<NodeTestEntry<double>*>(n0.getNode());
    nEntry->setValues(std::vector<double>{1,2,3});
    exe->step();
    exe->step();


}

//-----------------------------------------------------------
// NodeSubSystemを設けるケース
// 実行する処理ははtest08_01と同じ。
void test08_02(void)
{
    // サブシステムの生成
    GraphBuilder gb;
    auto n1 = gb.createNode<NodeGain>();
    auto n2 = gb.createNode<NodeAdd>();
    auto n3 = gb.createNode<NodeAdd>();
    auto n4 = gb.createNode<NodeMul>();

    gb.outto(Port(n1, 1), Port(n2, 2), Port(n3, 1));
    gb.outto(Port(n2, 1), Port(n4, 1));
    gb.outto(Port(n3, 1), Port(n4, 2));

    gb.setInPorts(Port(n2, 1), Port(n1, 1), Port(n3, 2));
    gb.setOutPorts(Port(n4, 1));

    QuasiNode qnSub = gb.nodelize();


    // 全体の生成
    GraphBuilder gb0;
    auto n0 = gb0.createNode<NodeTestEntry<double>>();
    auto n5 = gb0.createNode<NodeTestExit<double>>();

    gb0.outto(Port(n0, 1), Port(qnSub, 1));
    gb0.outto(Port(n0, 2), Port(qnSub, 2));
    gb0.outto(Port(n0, 3), Port(qnSub, 3));
    gb0.outto(Port(qnSub, 1), Port(n5, 1));

    // 実行
    Executor* exe = gb0.createExecutor(n0);

    auto nGain = static_cast<NodeGain*>(n1.getNode());
    nGain->setParam(1.3);
    auto nEntry = static_cast<NodeTestEntry<double>*>(n0.getNode());
    nEntry->setValues(std::vector<double>{1,2,3});
    exe->step();

}

//-----------------------------------------------------------
// NodeComplexを使用するケース
// 実行する処理ははtest08_01, test08_02と同じ。
class NodeComplexTest08 : public NodeComplex
{
public:
    NodeComplexTest08(void)
    {
        // グラフの構築
        auto& gb = getGraphBuilder();
        auto n1 = gb.createNode<NodeGain>();
        auto n2 = gb.createNode<NodeAdd>();
        auto n3 = gb.createNode<NodeAdd>();
        auto n4 = gb.createNode<NodeMul>();

        gb.outto(Port(n1, 1), Port(n2, 2), Port(n3, 1));
        gb.outto(Port(n2, 1), Port(n4, 1));
        gb.outto(Port(n3, 1), Port(n4, 2));

        gb.setInPorts(Port(n2, 1), Port(n1, 1), Port(n3, 2));
        gb.setOutPorts(Port(n4, 1));

        commit();

        // パラメータの設定
        auto nGain = static_cast<NodeGain*>(n1.getNode());
        nGain->setParam(1.3);
    }
};

void test08_03(void)
{
    // 全体の生成
    GraphBuilder gb0;
    auto qnSub = gb0.createNode<NodeComplexTest08>();
    auto n0 = gb0.createNode<NodeTestEntry<double>>();
    auto n5 = gb0.createNode<NodeTestExit<double>>();

    gb0.outto(Port(n0, 1), Port(qnSub, 1));
    gb0.outto(Port(n0, 2), Port(qnSub, 2));
    gb0.outto(Port(n0, 3), Port(qnSub, 3));
    gb0.outto(Port(qnSub, 1), Port(n5, 1));

    // 実行
    Executor* exe = gb0.createExecutor(n0);

    auto nEntry = static_cast<NodeTestEntry<double>*>(n0.getNode());
    nEntry->setValues(std::vector<double>{1,2,3});
    std::cout << "test08_03 (1)" << std::endl;
    exe->step();
    std::cout << "test08_03 (2)" << std::endl;
    exe->step();
    std::cout << "test08_03 (3)" << std::endl;
    exe->step();

}

//-----------------------------------------------------------
void test08(void)
{
    test08_01();
    test08_02();
    test08_03();
}


#endif


