// ExecutorForCircuit.hpp
// 論理回路用Executor
#ifndef __EXECUTORFORCIRCUIT_HPP__
#define __EXECUTORFORCIRCUIT_HPP__

#include "Executor.hpp"
#include "ExecutorDepthFirstTraversal.hpp"
#include "ExecutorStableEdge.hpp"

//-----------------------------------------------------------
// 論理回路用Executor
// 論理回路は循環する場合がある。この場合、頭からデータ到達を確認しながら
// 実行したのでは、いつまでも実行できないNodeが出てくる。
// これを避けるためにデータ到達に関係なく、現在の入力Edgeの値で実行してみて、
// 全Edgeの値が安定するまで実行を繰り返す。
// しかし、この場合も循環を含む回路ではEdgeの履歴が問題となり、
// 一度古い値で演算してしまうと、おかしな状態に陥る可能性がある。
// これを避けるために、まず、伝搬可能なデータを全て伝搬させてから、
// Edgeの状態が落ち着くまで実行を繰り返すようにする。
// このため、２種類のExecutorを順番にじっこうする。
// 最初に伝搬可能なデータを全て伝搬させるExecutor
// 次に、Edgeの状態が落ち着くまで実行を繰り返すExecutor
class ExecutorForCircuit : public Executor
{
    ExecutorDepthFirstTraversal* _exe1;
    ExecutorStableEdge* _exe2;

public:
    //-------------------------------------------------------
    ExecutorForCircuit(void)
    {
        _exe1 = new ExecutorDepthFirstTraversal();
        _exe2 = new ExecutorStableEdge();
    }

    //-------------------------------------------------------
    void step(void)
    {
        _exe1->step();
        _exe2->step();
        clearNodeStatus();
        clearEdgeStatus();
    }

};


#endif
