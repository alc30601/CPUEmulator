// ExecutorNodeListTraversal.hpp
#ifndef __EXECUTORNODELISTTRAVERSAL_HPP__
#define __EXECUTORNODELISTTRAVERSAL_HPP__

#include "Executor.hpp"

//-----------------------------------------------------------
// ノードリストを全ノード実行済まで何度もなめることによる実行
class ExecutorNodeListTraversal : public Executor
{
    //-------------------------------------------------------
    // ノード一覧を一巡して、実行出来るノードは実行する。
    void doOneRound(void)
    {
        for(auto node : _nodes){

            // if this node already executed, do nothing
            if(node->isExecuted() == true){
                continue;
            }
            
            // if datas are not ready yet, do nothing
            if(node->isExecutable() == false){
                continue;
            }

            // do this node
            node->execute();
        }
    }


public:
    //-------------------------------------------------------
    // ノード一覧をなめ、実行できるものから実行していく。
    // 実行していないノードが無くなるまで何度も繰り返す。
    // 探索効率は悪いが、木構造以外のグラフでも全ノード実行可能
    void step(void)
    {
        auto rest = _nodes.size();
        while(rest > 0){
            doOneRound();
            rest = countUnExecutedNodes();
        }

        clearNodeStatus();
        clearEdgeStatus();
    }
};



#endif

