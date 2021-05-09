// Executor.hpp
#ifndef __EXECUTOR_HPP__
#define __EXECUTOR_HPP__

#include <iostream>
#include <memory>
#include <typeinfo>
#include <vector>
#include <iterator>
#include <any>
#include <algorithm>

#include "Node.hpp"
#include "Edge.hpp"


//-----------------------------------------------------------
// Executor実行クラスのベースクラス
// 純仮想クラス
class Executor
{
protected:
    Node* _startNode;
    std::vector<Node*> _nodes;
    std::vector<Edge*> _edges;

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

public:
    //-------------------------------------------------------
    virtual void step(void) = 0;

    //-------------------------------------------------------
    // 開始Nodeを登録する。
    virtual void setStartNode(Node* startNode)
    {
        _startNode = startNode;
    }

    //-------------------------------------------------------
    // Nodeを一括追加する。
    virtual void addNodes(std::vector<Node*>& nodes)
    {
        std::copy(nodes.begin(), nodes.end(), std::back_inserter(_nodes));
    }

    //-------------------------------------------------------
    // edgeを一括追加する。
    virtual void addEdges(std::vector<Edge*>& edges)
    {
        std::copy(edges.begin(), edges.end(), std::back_inserter(_edges));
    }

};


#endif



 