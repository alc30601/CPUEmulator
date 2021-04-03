// NodeTestBase.hpp
#ifndef __NODETESTBASE_HPP__
#define __NODETESTBASE_HPP__

#include <any>
#include <cassert>

#include "Edge.hpp"
#include "Node.hpp"
#include "Executor.hpp"
#include "GraphBuilder.hpp"


//-----------------------------------------------------------
// ０入力、複数出力ノード
template <typename T>
class NodeTestEntry : public Node
{
    std::vector<T> _values;

public:
    //-------------------------------------------------------
    void setValues(std::vector<T> values)
    {
        _values = values;
    }

    //-------------------------------------------------------
    void execute(void)
    {
        Node::execute();


        std::vector<Edge*> outEdges = getOutEdges();

        std::cout << "Input Values : ";
        for(int i=0; i<outEdges.size();i++){
            T value = _values.at(i);
            outEdges.at(i)->setValue(value);
            std::cout << value << " , ";
        }
        std::cout << std::endl;
    }

};


//-----------------------------------------------------------
// 複数入力、0出力ノード
template <typename T>
class NodeTestExit : public Node
{
    std::vector<T> _values;

public:
    //-------------------------------------------------------
    std::vector<T> getValues(void)
    {
        return _values;
    }

    //-------------------------------------------------------
    void execute(void)
    {
        Node::execute();

        std::vector<Edge*> inEdges = getInEdges();

        std::cout << "Output Values : ";

        if(inEdges.size() > _values.size()){
            _values.resize(inEdges.size());
        }

        for(int i=0;i<inEdges.size();i++){
            auto edge = inEdges[i];
            T value = std::any_cast<T>(edge->getValue());
            _values[i] = value;
            std::cout << value << " , ";
        }

        std::cout << std::endl;
    }
};

//-----------------------------------------------------------
// NodeTestEntry, NodeTestExitを用いたテストの実行、評価を行う。
// inputで与えられた値を入力としてNodeTestEntryに設定し、実行。
// 実行結果をNodeTestExitから取り出す。
// 与えられた正解と比較し間違いがあるとassertする。
template <typename T1, typename T2>
void evaluation(Executor* exe,
                QuasiNode& qnS, QuasiNode& qnE,
                std::vector<T1>& input, std::vector<T2>& expected)
{
    auto nEntry = static_cast<NodeTestEntry<T1>*>(qnS.getNode());
    auto nExit = static_cast<NodeTestExit<T2>*>(qnE.getNode());

    // 実行
    nEntry->setValues(input);
    exe->step();
    auto output = nExit->getValues();

    // 答え合わせ
    assert(output == expected);
}


//-----------------------------------------------------------
template <typename TYPENODE, typename T>
std::tuple<Executor*, QuasiNode, QuasiNode> test_1to1_template(void)
{
    GraphBuilder gb;
    auto nTarget = gb.createNode<TYPENODE>();
    auto nEntry = gb.createNode<NodeTestEntry<T>>();
    auto nExit = gb.createNode<NodeTestExit<T>>();

    gb.outto(Port(nEntry, 1), Ports{ Port(nTarget,1) });
    gb.outto(Port(nTarget,1), Ports{ Port(nExit,1) });

    Executor* exe = gb.createExecutor(nEntry);
    auto ret = std::tie(exe, nEntry, nExit);
    return ret;
}

//-----------------------------------------------------------
template <typename TYPENODE, typename T>
std::tuple<Executor*, QuasiNode, QuasiNode> test_2to1_template(void)
{
    GraphBuilder gb;
    auto nTarget = gb.createNode<TYPENODE>();
    auto nEntry = gb.createNode<NodeTestEntry<T>>();
    auto nExit = gb.createNode<NodeTestExit<T>>();

    gb.outto(Port(nEntry, 1), Ports{ Port(nTarget,1) });
    gb.outto(Port(nEntry, 2), Ports{ Port(nTarget,2) });
    gb.outto(Port(nTarget,1), Ports{ Port(nExit,1) });

    Executor* exe = gb.createExecutor(nEntry);
    auto ret = std::tie(exe, nEntry, nExit);
    return ret;
}


#endif

