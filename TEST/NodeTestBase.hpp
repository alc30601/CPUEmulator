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
std::vector<std::vector<bool>> vectorBool1bits{
    {F},
    {T},
};

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
// std::vectorの値をコンソール出力する。
template <typename T>
void printVector(std::vector<T>& values, std::string title, bool cr)
{
    std::cout << title;
    for(int i=0; i<values.size();i++){
        T value = values.at(i);
        std::cout << value << " , ";
    }
    if(cr == true){
        std::cout << std::endl;
    }
}

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

        for(int i=0; i<outEdges.size();i++){
            T value = _values.at(i);
            outEdges.at(i)->setValue(value);
        }
    }

    //-------------------------------------------------------
    void printValues(void)
    {
        printVector<T>(_values, std::string("Input Values : "), false);
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

        if(inEdges.size() > _values.size()){
            _values.resize(inEdges.size());
        }

        for(int i=0;i<inEdges.size();i++){
            auto edge = inEdges[i];
            T value = std::any_cast<T>(edge->getValue());
            _values[i] = value;
        }
    }

    //-------------------------------------------------------
    void printValues(void)
    {
        printVector<T>(_values, std::string("Output Values : "), true);
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
// 
// <template引数>
// T1       : 入力データ型
// T2       : 出力データ型
//
// <関数引数>
// exe         : Executorのポインタ
// qnS         : NodeTestEntry準ノードのポインタ
// qnE         : NodeTestExit準ノードのポインタ
// test_vector : 入力値
// expected    : 出力期待値
// do_assert   : assertによるチェック実施要否(デフォルト：実施要)
template <typename T1, typename T2>
void evaluation(Executor* exe,
                QuasiNode& qnS, QuasiNode& qnE,
                std::vector<T1>& test_vector, std::vector<T2>& expected, 
                bool do_assert=true)
{
    auto nEntry = static_cast<NodeTestEntry<T1>*>(qnS.getNode());
    auto nExit = static_cast<NodeTestExit<T2>*>(qnE.getNode());

    // 実行
    nEntry->setValues(test_vector);
    exe->step();
    auto output = nExit->getValues();

    nEntry->printValues();
    nExit->printValues();

    // 答え合わせ
    if(do_assert){
        assert(output == expected);
    }
}


//-----------------------------------------------------------
// Node評価関数テンプレート
// 単一のNodeを評価する。入力値、出力値はそれぞれ単一の型である必要がある。
// 入力型、出力型は異なっていてもよい。
// 入力ポート数、出力ポート数は任意
// 
// <template引数>
// TYPENAME : 評価対象Nodeクラス
// T1       : 入力データ型
// T2       : 出力データ型
//
// <関数引数>
// test_vector : 入力値
// expected    : 出力期待値
// do_assert   : assertによるチェック実施要否(デフォルト：実施要)
template <typename TYPENODE, typename T1, typename T2>
void test_NtoM_template(std::vector<std::vector<T1>>& test_vector, std::vector<std::vector<T2>>& expected, bool do_assert=true)
{
    GraphBuilder gb;
    auto qnT = gb.createNode<TYPENODE>(typeid(TYPENODE).name());
    auto qnS = gb.createNode<NodeTestEntry<T1>>("NodeTestEntry");
    auto qnE = gb.createNode<NodeTestExit<T2>>("NodeTestExit");

    int num_input = test_vector[0].size();
    int num_output = expected[0].size();

    for(int i=1;i<= num_input; i++){
        gb.outto(Port(qnS, i), Ports{ Port(qnT,i) }, typeid(T1));
    }

    for(int i=1;i<= num_output;i++){
        gb.outto(Port(qnT, i), Ports{ Port(qnE,i) }, typeid(T2));
    }

     Executor* exe = gb.createExecutor(qnS);
    for(int i=0;i<test_vector.size();i++){
        evaluation<T1, T2>(exe, qnS, qnE, test_vector[i], expected[i], do_assert);
    }

}

#endif

