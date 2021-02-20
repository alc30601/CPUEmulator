// Executor.hpp
#ifndef __EXECUTOR_HPP__
#define __EXECUTOR_HPP__

#include <iostream>
#include <memory>
#include <typeinfo>
#include <vector>
#include <iterator>

#include "Node.hpp"
#include "Edge.hpp"


//-----------------------------------------------------------
// Start node of the graph
class Executor
{
    Node* _startNode;
    std::vector<Node*> _nodes;
    std::vector<Edge*> _edges;

public:
    //-------------------------------------------------------
    Executor(Node* startNode, std::vector<Node*> nodes, std::vector<Edge*> edges)
    {
        _startNode = startNode;
        // std::copy(nodes.begin(), nodes.end(), std::back_inserter(_nodes));
        // std::copy(edges.begin(), edges.end(), std::back_inserter(_edges));
        _nodes = nodes;
        _edges = edges;

    }

    //-------------------------------------------------------
    void clearNodeStatus(void)
    {
        for(auto node : _nodes){
            node->clearStatus();
        }
    }

    //-------------------------------------------------------
    void clearEdgeStatus(void)
    {
        for(auto edge : _edges){
            edge->clearStatus();
        }
    }

    //-------------------------------------------------------
    // execute flow graph from start node
    void step(void)
    {
        std::cout << "--< Executor::step >-- " << std::endl;
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

    //-------------------------------------------------------
    // This method is called recursively with flow graph traversal.
    // First, execute the node
    // Second, find the edges from the node
    // Third, for each edge, find child nodes
    // Then, call this method for these child nodes
    void stepRecursive(Node* node)
    {
        // if this node already executed, do nothing
        if(node->isExecuted() == true){
            return;
        }
        
        // if datas are not ready yet, do nothing
        if(node->isInputDataCompleted() == false){
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

};

#endif



 