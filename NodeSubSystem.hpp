// NodeSubSystem.hpp
#ifndef __NODESUBSYSTEM_HPP__
#define __NODESUBSYSTEM_HPP__

#include "Node.hpp"
#include "Edge.hpp"
#include "Executor.hpp"

//-----------------------------------------------------------
// ByPass Node
class NodeByPass : public Node
{
public:
    //-------------------------------------------------------
    // 入口エッジの値を出口エッジにコピーする。
    // 入力Edgeの状態は出力Edgeにも維持する。
    // (入力EdgeがENABLEのもののみ出力EdgeもENABLEとする)
    void execute(void)
    {
        bool allEdgeEnabled = true;

        std::vector<Edge*> inEdges = getInEdges();
        std::vector<Edge*> outEdges = getOutEdges();

        for(int i=0;i<inEdges.size();i++){
            Edge* edge = inEdges.at(i);
            auto value = edge->getValue();
            if(edge->getStatus() == Edge::Status::DISABLE){
                allEdgeEnabled = false;
                outEdges.at(i)->setJustValue(value);
            }
            else{
                outEdges.at(i)->setValue(value);
            }
        }

        // 全てのEdgeがデータ可用ならば、実行済とする。
        if(allEdgeEnabled == true){
            Node::execute();
        }
    }

    //-------------------------------------------------------
    // データが揃っていようがいまいが実行可能にする。
    bool isExecutable(void)
    {
        return true;
    }
};


//-----------------------------------------------------------
// 入口ノード
// class NodeSubSystemEntry : public Node
class NodeSubSystemEntry : public NodeByPass
{
    Node& _parentNode;   // ref of parent instance

public:
    //-------------------------------------------------------
    NodeSubSystemEntry(Node& parentNode) : _parentNode(parentNode)
    {
        _name = "NodeSubSystemEntry";
    }

    //-------------------------------------------------------
    // 親ノードの入り口エッジの値を自身の出口エッジにコピーする。
    // void execute(void)
    // {
    //     Node::execute();

    //     std::vector<Edge*> inEdges = _parentNode.getInEdges();
    //     std::vector<Edge*> outEdges = getOutEdges();

    //     for(int i=0;i<inEdges.size();i++){
    //         Edge* edge = inEdges.at(i);
    //         if(edge->getStatus() == Edge::Status::ENABLE){
    //             auto value = edge->getValue();
    //             outEdges.at(i)->setValue(value);
    //         }
    //     }
    // }

};

//-----------------------------------------------------------
// 出口ノード
// class NodeSubSystemExit : public Node
class NodeSubSystemExit : public NodeByPass
{
    Node& _parentNode;   // ref of parent instance

public:
    //-------------------------------------------------------
    NodeSubSystemExit(Node& parentNode) : _parentNode(parentNode)
    {
        _name = "NodeSubSystemExit";
    }

    //-------------------------------------------------------
    // 自身の入り口エッジの値を親ノードの出口エッジにコピーする。
    // void execute(void)
    // {
    //     Node::execute();

    //     std::vector<Edge*> inEdges = getInEdges();
    //     std::vector<Edge*> outEdges = _parentNode.getOutEdges();

    //     for(int i=0;i<inEdges.size();i++){
    //         auto value = inEdges.at(i)->getValue();
    //         outEdges.at(i)->setValue(value);
    //     }
    // }

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
    NodeSubSystemEntry* _nodeEntry;   // 入り口ノード
    NodeSubSystemExit* _nodeExit;    // 出口ノード

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

        // Node::execute();

        // Executor* _exe;
        // _exe = createExecutor();
        // _exe->setStartNode(_nodeEntry);
        // _exe->addNodes(_nodes);
        // _exe->step();
    }


};



#endif
