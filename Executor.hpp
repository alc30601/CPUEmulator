// Executor.hpp
#ifndef __EXECUTOR_HPP__
#define __EXECUTOR_HPP__

#include <iostream>
#include <memory>
#include <typeinfo>

#include "Node.hpp"
#include "Edge.hpp"


//-----------------------------------------------------------
// Start node of the graph
class Executor
{
    std::shared_ptr<Node> _startNode;

public:
    //-------------------------------------------------------
    Executor(std::shared_ptr<Node> sNode)
    {
        _startNode = sNode;
    }

    //-------------------------------------------------------
    // execute flow graph from start node
    void step(void)
    {
        // ★★★
        // メンバ変数として持つ、_startNodeから始めてグラフをたどる。
        // あるNodeからその先に行くにはNodeクラスのgetOutputEdge()によりつながるEdgeを取得する。
        // あるEdgeからその先のNodeを見つけるにはEdgeのメソッドgetoutputNode()によりNode一覧を取得する。

        // 個々のノードに到達したらそのノードの処理を実行する(execute())。
        // 但し、到達したNodeにまだデータが到達せずに処理が実行できない場合がある。
        // その判定のため、そのNodeに入力されるEdge一覧を取得し、全てのEdgeでデータが到達しているか
        // をチェックする。

        step_recursive(_startNode);

    }

    //-------------------------------------------------------
    // This method is called recursively with flow graph traversal.
    // First, execute the node
    // Second, find the edges from the node
    // Third, for each edge, find child nodes
    // Then, call this method for these child nodes
    void step_recursive(std::shared_ptr<Node> node)
    {
        if(node->isExecuted() == true){
            return;
        }
        
        // if datas are not ready yet, do nothing
        if(node->isInputDataCompleted() == false){
            return;
        }

        // if datas are ready to execute, do below
        node->execute();
  
        for(auto edge : node->getOutEdges()){
            for(auto child_node : edge->getOutNodes()){
                step_recursive(child_node);
            }
        }
    }

};

#endif



 