// ExecutorDepthFirstTraversal.hpp
#ifndef __EXECUTORDEPTHFIRSTTRAVERSAL_HPP__
#define __EXECUTORDEPTHFIRSTTRAVERSAL_HPP__

#include "Executor.hpp"


//-----------------------------------------------------------
// 再帰を用いた深さ優先探索によるノード巡回実行
class ExecutorDepthFirstTraversal : public Executor
{
    // Node* _startNode;

    //-------------------------------------------------------
    // 再帰を用いた深さ優先探索によるノード巡回実行
    // 効率は比較的いいが、深さ優先探索のため木構造のグラフにしか対応しない。
    void stepRecursive(Node* node)
    {
        // if this node already executed, do nothing
         if(node->isExecuted() == true){
            return;
        }
        
        // if datas are not ready yet, do nothing
        if(node->isExecutable() == false){
            return;
        }

        // do this node
        node->execute();

        // traverse out edges and nodes, then execute each node recursively
        for(auto edge : node->getOutEdges()){
            for(auto child_node : edge->getOutNodes()){
                stepRecursive(child_node);
            }
        }
    }

public:
    //-------------------------------------------------------
    void step(void)
    {
        // _startNodeから始めてグラフをたどる。
        // あるNodeからその先に行くにはNodeクラスのgetOutputEdge()によりつながるEdgeを取得する。
        // あるEdgeからその先のNodeを見つけるにはEdgeのメソッドgetoutputNode()によりNode一覧を取得する。

        // 個々のノードに到達したらそのノードの処理を実行する(execute())。
        // 但し、到達したNodeにまだデータが到達せずに処理が実行できない場合がある。
        // その判定のため、そのNodeに入力されるEdge一覧を取得し、全てのEdgeでデータが到達しているか
        // をチェックする。

        stepRecursive(_startNode);
        clearNodeStatus();
        clearEdgeStatus();
    }
};


#endif

