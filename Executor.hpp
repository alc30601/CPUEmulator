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
    Executor(Node* startNode, std::vector<Node*> nodes, std::vector<Edge*> edges)
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
    void addNodes(std::vector<Node*> nodes)
    {
        std::copy(nodes.begin(), nodes.end(), std::back_inserter(_nodes));
    }

    //-------------------------------------------------------
    // edgeを一括追加する。
    void addEdges(std::vector<Edge*> edges)
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
    // check if data that are necessary for execution of this node is ready or not
    // true : data are ready
    // false : data are not ready yet
    bool isInputDataCompleted(Node* node)
    {
        bool result = true;
        std::vector<Edge*> inEdges = node->getInEdges();

        // 入力元Edgeにデータが全て揃っているか確認する。
        for(auto edge : inEdges){
            if(edge->getStatus() == Edge::Status::DISABLE){
                result = false;
                break;
            }
        }
        return result;
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
        if(isInputDataCompleted(node) == false){
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



 