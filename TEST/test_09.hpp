// test_09.hpp
// 入力エッジが分岐するモジュール化ノード
// GraphBuilderによるNodeComplexにおいて入力エッジが内部で分岐するケースの確認
// GraphBuilderのinPortsの持ち方を変更したことの動作確認
//
//   +-------+      +--------------+      +-------+
//   |       |      |              |      |       |
//   |       |      |   +------+   |      |       |
// --|-------|--+-->|-->|      |   |      |       |
//   | Entry |  |   |   | AND  |-->|----->|-------|-->
//   |       |  +-->|-->|      |   |      |       |
//   |       |      |   +------+   |      |       |
//   |       |      |              |      |       |
//   +-------+      +--------------+      +-------+

#ifndef __TEST_09_HPP__
#define __TEST_09_HPP__

#include "Node.hpp"
#include "Edge.hpp"
#include "Executor.hpp"
#include "GraphBuilder.hpp"
#include "NodeComplex.hpp"

#include "NodeLogics.hpp"
#include "NodeTestBase.hpp"


//-----------------------------------------------------------
// 内部ノードはANDのみ
class NodeComplexTest09 : public NodeComplex
{
public:
    NodeComplexTest09(void)
    {
        // グラフの構築
        auto& gb = getGraphBuilder();
        auto enty = getEntryNode();
        auto exit = getExitNode();
        auto n1 = gb.createNode<NodeAnd>();

        gb.outto(Port(enty, 1), Ports{ Port(n1, 1), Port(n1, 2) }, typeid(bool));
        gb.outto(Port(n1, 1), Ports{ Port(exit, 1) }, typeid(bool));

       commit();
    }
};

//-----------------------------------------------------------
void test09(void)
{
    // 全体の生成
    GraphBuilder gb0;
    auto qnT = gb0.createNode<NodeComplexTest09>();
    auto qnS = gb0.createNode<NodeTestEntry<bool>>();
    auto qnE = gb0.createNode<NodeTestExit<bool>>();

    gb0.outto(Port(qnS, 1), Ports{ Port(qnT, 1) }, typeid(bool));
    gb0.outto(Port(qnT, 1), Ports{ Port(qnE, 1) }, typeid(bool));

    // 実行
    // Executor* exe = gb0.createExecutor(qnS);
    auto exe = createExecutor();
    auto nEntry = static_cast<NodeTestEntry<bool>*>(qnS.getNode());
    auto nExit = static_cast<NodeTestExit<bool>*>(qnE.getNode());

    nEntry->setValues(std::vector<bool>{true});
    std::cout << "test09 (1)" << std::endl;
    exe->step();
    nEntry->printValues();
    nExit->printValues();

    nEntry->setValues(std::vector<bool>{false});
    std::cout << "test09 (2)" << std::endl;
    exe->step();
    nEntry->printValues();
    nExit->printValues();
}


#endif

