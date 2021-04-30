// test_05.hpp
// モジュール化ノードの評価(NodeSubSystem)
// 
// +-------+      +--------------+      +-------+
// |       |      | NodeComplex  |      |       |
// |       |      |   +------+   |      |       |
// |       |----->|-->|      |-->|----->|       |
// | NodeS |----->|-->|      |-->|----->| NodeE |
// |       |      |   +------+   |      |       |
// |       |      | NodeInternal |      |       |
// +-------+      +--------------+      +-------+


#ifndef __TEST_05_HPP__
#define __TEST_05_HPP__

#include <iostream>
#include <any>

#include "Node.hpp"
#include "Edge.hpp"
#include "Executor.hpp"
#include "NodeSubSystem.hpp"
#include "GraphBuilder.hpp"

//-----------------------------------------------------------
class Test05NodeS : public Node
{
    bool _value1;
    bool _value2;

public:

    //-------------------------------------------------------
    void setValues(bool v1, bool v2)
    {
        _value1 = v1;
        _value2 = v2;
    }

    //-------------------------------------------------------
    void execute(void)
    {
        Node::execute();

        std::cout << "Test05NodeS::execute() : " << _value1 << " , " << _value2 << std::endl;

        _outEdges.at(0)->setValue(_value1);
        _outEdges.at(1)->setValue(_value2);
    }
};


//-----------------------------------------------------------
class Test05NodeE : public Node
{
    bool _value1;
    bool _value2;

public:


    //-------------------------------------------------------
    void execute(void)
    {
        Node::execute();

        _value1 = std::any_cast<bool>(_inEdges.at(0)->getValue());
        _value2 = std::any_cast<bool>(_inEdges.at(1)->getValue());
        std::cout << "Test05NodeE::execute() : " << _value1 << " , " << _value2 << std::endl;
    }
};


//-----------------------------------------------------------
// 複合ノード内の内部ノード
class Test05NodeInternal : public Node
{
public:
    //-------------------------------------------------------
    void execute(void)
    {
        Node::execute();

        // 入力エッジの値をそのまま出力エッジに出力する。
        bool value1 = std::any_cast<bool>(_inEdges.at(0)->getValue());
        bool value2 = std::any_cast<bool>(_inEdges.at(1)->getValue());
        _outEdges.at(0)->setValue(value1);
        _outEdges.at(1)->setValue(value2);

        std::cout << "Test05NodeInternal::execute() : " << value1 << " , " << value2 << std::endl;
    }    
};




//-----------------------------------------------------------
void test05(void)
{
    // サブシステムの生成
    GraphBuilder gb1;
    auto n1 = gb1.createNode<Test05NodeInternal>();
    gb1.setInPorts(Port(n1, 1), Port(n1, 2));
    gb1.setOutPorts(Port(n1, 1), Port(n1, 2));
    QuasiNode qnSub = gb1.nodelize();

    // 全体の生成
    GraphBuilder gb0;
    auto n0 = gb0.createNode<NodeTestEntry<bool>>();
    auto n2 = gb0.createNode<NodeTestExit<bool>>();

    gb0.outto(Port(n0, 1), Port(qnSub, 1));
    gb0.outto(Port(n0, 2), Port(qnSub, 2));
    gb0.outto(Port(qnSub, 1), Port(n2, 1));
    gb0.outto(Port(qnSub, 2), Port(n2, 2));

    // 実行
    auto nodes = gb0.getNodes();
    auto edges = gb0.getEdges();
    auto nEntry = static_cast<NodeTestEntry<bool>*>(n0.getNode());
    // Executor* exe(new Executor(nEntry, nodes, edges));
    Executor* exe = getExecutor(nEntry, nodes, edges);

    nEntry->setValues(std::vector<bool>{true, true});
    exe->step();

    nEntry->setValues(std::vector<bool>{false, true});
    exe->step();

    nEntry->setValues(std::vector<bool>{true, false});
    exe->step();

    nEntry->setValues(std::vector<bool>{false, false});
    exe->step();

}


#endif


