// NodeSubSystem.hpp
#ifndef __NODESUBSYSTEM_HPP__
#define __NODESUBSYSTEM_HPP__

#include "Node.hpp"
#include "Edge.hpp"


//-----------------------------------------------------------
// 入口ノード
class NodeSubSystemEntry : public Node
{
    Node& _parentNode;   // ref of parent instance

public:
    //-------------------------------------------------------
    NodeSubSystemEntry(Node& parentNode) : _parentNode(parentNode)
    {
        _name = "NodeSubSystemEntry";
    }

};

//-----------------------------------------------------------
// 出口ノード
class NodeSubSystemExit : public Node
{
    Node& _parentNode;   // ref of parent instance

public:
    //-------------------------------------------------------
    NodeSubSystemExit(Node& parentNode) : _parentNode(parentNode)
    {
        _name = "NodeSubSystemExit";
    }

};


//-----------------------------------------------------------
// モジュール化されたノードのベースクラス
// 本クラスを継承したサブシステムを作成する。
// 
//   +-----------------------------------+
//   | +---+                       +---+ |
//  -| |   |-Edge ...              |   | |
//   | |   |              ... Edge-|   | |-
//  -| |   |-Edge ...              |   | |  
//   | +---+                       +---+ |
//   | 入口ノード　            出口ノード|
//   +-----------------------------------+
// 
//-----------------------------------------------------------
class NodeSubSystem : public Node
{
    // 本サブシステムの入力、出力は以下の入り口ノード、出口ノードで集約する。
    NodeSubSystemEntry* _nodeEntry; // 入り口ノード
    NodeSubSystemExit* _nodeExit;   // 出口ノード

    // 自身が内部に持つNode,Edgeを保持する。
    std::vector<Node*> _nodes;
    std::vector<Edge*> _edges;

public:
    //-------------------------------------------------------
    NodeSubSystem(void)
    {
        _nodeEntry = new NodeSubSystemEntry(*this);
        _nodeExit  = new NodeSubSystemExit(*this);
        std::vector<Node*> nodes = {_nodeEntry, _nodeExit};
        addNodes(nodes);
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

    //-------------------------------------------------------
    // 入口ノードを返す。
    Node* getNodeEntry(void){ return _nodeEntry; }

    //-------------------------------------------------------
    // 出口ノードを返す。
    Node* getNodeExit(void){ return _nodeExit; }

    //-------------------------------------------------------
    // 内部ノード一覧を返す。
    std::vector<Node*>& getInnerNodes(void){ return _nodes; }

    //-------------------------------------------------------
    // 内部エッジ一覧を返す。
    std::vector<Edge*>& getInnerEdges(void){ return _edges; }

    //-------------------------------------------------------
    void execute(void)
    {
        std::cout << "This Node never be executed" << std::endl;
        assert(false);
    }


};


#endif
