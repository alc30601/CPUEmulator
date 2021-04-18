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
// bit-0がMSB
std::vector<std::vector<bool>> vectorBool2bits{
    {false,false},
    {false,true},
    {true, false},
    {true, true},
};

//-----------------------------------------------------------
// bit-0がMSB
std::vector<std::vector<bool>> vectorBool3bits{
    {false,false, false},
    {false,false, true},
    {false,true,  false},
    {false,true,  true},

    {true, false, false},
    {true, false, true},
    {true, true,  false},
    {true, true,  true}
};

//-----------------------------------------------------------
// bit-0がLSB
std::vector<std::vector<bool>>   vectorBool4bits{

    {false,false, false, false},    // 0
    {true, false, false, false},    // 1
    {false,true,  false, false},    // 2
    {true, true,  false, false},    // 3
    {false,false, true,  false},    // 4
    {true, false, true,  false},    // 5
    {false,true,  true,  false},    // 6
    {true, true,  true,  false},    // 7

    {false,false, false, true },    // 8
    {true, false, false, true },    // 9
    {false,true,  false, true },    // 10
    {true, true,  false, true },    // 11
    {false,false, true,  true },    // 12
    {true, false, true,  true },    // 13
    {false,true,  true,  true },    // 14
    {true, true,  true,  true }     // 15

};


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

        std::cout << "Input  Values : ";
        for(int i=0; i<outEdges.size();i++){
            T value = _values.at(i);
            outEdges.at(i)->setValue(value);
            std::cout << value << " , ";
        }
        // std::cout << std::endl;
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
// assertによる判定を実行するか否かは引数do_assertによって判断する。
// do_assertはデフォルト値がtrue(=assert判定有り)となっている。
// assert判定したくない場合はdo_assertをfalseで呼べば良い。
template <typename T1, typename T2>
void evaluation(Executor* exe,
                QuasiNode& qnS, QuasiNode& qnE,
                std::vector<T1>& input, std::vector<T2>& expected, bool do_assert=true)
{
    auto nEntry = static_cast<NodeTestEntry<T1>*>(qnS.getNode());
    auto nExit = static_cast<NodeTestExit<T2>*>(qnE.getNode());

    // 実行
    nEntry->setValues(input);
    exe->step();
    auto output = nExit->getValues();

    // 答え合わせ
    if(do_assert){
        assert(output == expected);
    }
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

