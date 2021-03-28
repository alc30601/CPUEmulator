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
    Executor(){}

    //-------------------------------------------------------
    Executor(Node* startNode, std::vector<Node*>& nodes, std::vector<Edge*>& edges)
    {
        setStartNode(startNode);

        addNodes(nodes);
        addEdges(edges);
    }

    //-------------------------------------------------------
    // register start Node
    void setStartNode(Node* startNode)
    {
        _startNode = startNode;
    }

    //-------------------------------------------------------
    // Add a Node
    void addNodes(std::vector<Node*>& nodes)
    {
        std::copy(nodes.begin(), nodes.end(), std::back_inserter(_nodes));
    }

    //-------------------------------------------------------
    // edgeを一括追加する。
    void addEdges(std::vector<Edge*>& edges)
    {
        std::copy(edges.begin(), edges.end(), std::back_inserter(_edges));
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
        // std::cout << "--< Executor::step >-- " << std::endl;
        // _startNodeから始めてグラフをたどる。
        // あるNodeからその先に行くにはNodeクラスのgetOutputEdge()によりつながるEdgeを取得する。
        // あるEdgeからその先のNodeを見つけるにはEdgeのメソッドgetoutputNode()によりNode一覧を取得する。

        // 個々のノードに到達したらそのノードの処理を実行する(execute())。
        // 但し、到達したNodeにまだデータが到達せずに処理が実行できない場合がある。
        // その判定のため、そのNodeに入力されるEdge一覧を取得し、全てのEdgeでデータが到達しているか
        // をチェックする。

        // stepRecursive(_startNode);

        doStep();

        clearNodeStatus();
        clearEdgeStatus();
    }

    //-------------------------------------------------------
    // 再帰を用いた深さ優先探索によるノード巡回実行
    // 効率は比較的いいが、深さ優先探索のため木構造のグラフにしか対応しない。
    // This method is called recursively with flow graph traversal.
    // First, execute the node
    // Second, find the edges from the node
    // Third, for each edge, find child nodes
    // Then, call this method for these child nodes
    // void stepRecursive(Node* node)
    // {
    //     // if this node already executed, do nothing
    //     if(node->isExecuted() == true){
    //         return;
    //     }
        
    //     // if datas are not ready yet, do nothing
    //     if(isInputDataCompleted(node) == false){
    //         return;
    //     }

    //     // do this node
    //     node->execute();

    //     // traverse out edges and nodes, then execute each node recursively
    //     for(auto edge : node->getOutEdges()){
    //         for(auto child_node : edge->getOutNodes()){
    //             stepRecursive(child_node);
    //         }
    //     }
    // }


    //-------------------------------------------------------
    // ノード一覧をなめ、実行できるものから実行していく。
    // 実行していないノードが無くなるまで何度も繰り返す。
    // 探索効率は悪いが、木構造以外のグラフでも全ノード実行可能
    void doStep(void)
    {
        auto rest = _nodes.size();
        while(rest > 0){
            doOneRound();
            rest = countUnExecutedNodes();
        }
    }

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
            if(node->isInputDataCompleted() == false){
                continue;
            }

            // do this node
            node->execute();
        }
    }

    //-------------------------------------------------------
    // ノード一覧のうち、未実行のノード数を計算する。
    int countUnExecutedNodes(void)
    {
        int rest = 0;
        for(auto node : _nodes){

            if(node->isExecuted() == false){
                rest++;
            }
        }
        return rest;
    }

};

#endif



 