// NodeSubSystem.hpp
#ifndef __NODESUBSYSTEM_HPP__
#define __NODESUBSYSTEM_HPP__

#include "Node.hpp"
#include "Edge.hpp"
#include "Executor.hpp"


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
//   | 入り口ノード            出口ノード|
//   +-----------------------------------+




//-----------------------------------------------------------
class NodeSubSystem : public Node
{

    //-----------------------------------------------------------
    // Internal Entry Node
    class NodeSubSystemEntry : public Node
    {
        NodeSubSystem& _parentNode;   // ref of parent instance

    public:
        //-------------------------------------------------------
        NodeSubSystemEntry(NodeSubSystem& parentNode) : _parentNode(parentNode){}

        //-------------------------------------------------------
        // 親ノードの入り口エッジの値を自身の出口エッジにコピーする。
        void execute(void)
        {
            Node::execute();

            std::vector<Edge*> inEdges = _parentNode.getInEdges();
            std::vector<Edge*> outEdges = getOutEdges();

            for(int i=0;i<inEdges.size();i++){
                auto value = inEdges.at(i)->getValue();
                outEdges.at(i)->setValue(value);
            }
        }
    };

    //-----------------------------------------------------------
    // Internal Exit Node
    class NodeSubSystemExit : public Node
    {
        NodeSubSystem& _parentNode;   // ref of parent instance

    public:
        //-------------------------------------------------------
        NodeSubSystemExit(NodeSubSystem& parentNode) : _parentNode(parentNode){}

        //-------------------------------------------------------
        // 自身の入り口エッジの値を親ノードの出口エッジにコピーする。
        void execute(void)
        {
            Node::execute();

            std::vector<Edge*> inEdges = getInEdges();
            std::vector<Edge*> outEdges = _parentNode.getOutEdges();

            for(int i=0;i<inEdges.size();i++){
                auto value = inEdges.at(i)->getValue();
                outEdges.at(i)->setValue(value);
            }
        }
    };


    // 本サブシステムの入力、出力は以下の入り口ノード、出口ノードで集約する。
    NodeSubSystemEntry* _nodeEntry;   // 入り口ノード
    NodeSubSystemExit* _nodeExit;    // 出口ノード

    // サブシステム内の実行は独自にExecutorを持ち、実行を行う。
    // 外部のシステムからは１つのノードにしか見えないため、
    // 外部のExecutorはサブシステム内部のノードを辿ってはくれない。
    // そのため、サブシステム内のノードを巡回するために独自の
    // Executorを持つ必要がある。
    Executor* _exe;

public:
    //-------------------------------------------------------
    NodeSubSystem(void)
    {
        _nodeEntry = new NodeSubSystemEntry(*this);
        _nodeExit  = new NodeSubSystemExit(*this);

        _exe = new Executor();
        _exe->setStartNode(_nodeEntry);
        std::vector<Node*> nodes = {_nodeEntry, _nodeExit};
        _exe->addNodes(nodes);
    }

    //-------------------------------------------------------
    // 入口ノードを返す。
    Node* getNodeEntry(void){ return _nodeEntry; }

    //-------------------------------------------------------
    // 出口ノードを返す。
    Node* getNodeExit(void){ return _nodeExit; }


    //-------------------------------------------------------
    // _nodeEntrtyノードの出力エッジ一覧を返す。
    std::vector<Edge*> getInDualEdges(void)
    {
        return _nodeEntry->getOutEdges();
    }

    //-------------------------------------------------------
    // _nodeExitノードの入力エッジ一覧を返す。
    std::vector<Edge*> getOutDualEdges(void)
    {
        return _nodeExit->getInEdges();
    }

    //-------------------------------------------------------
    Executor* getInnerExecutor(void)
    {
        return _exe;
    }

    //-------------------------------------------------------
    void execute(void)
    {
        Node::execute();

        _exe->step();
    }

};



#endif
