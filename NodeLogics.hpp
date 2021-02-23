// NodeLogics.hpp

#ifndef __NODELOGICS_HPP__
#define __NODELOGICS_HPP__

#include <iostream>
#include <memory>

#include "Node.hpp"
#include "NodeSubSystem.hpp"
#include "NodeBase.hpp"


//-----------------------------------------------------------
// NOT
// 1入力1出力ノード
// データ型はともにbool
class NodeNot : public Node1In1Out<bool>
{
    //-------------------------------------------------------
    bool calculate(bool p)
    {
        bool result = !p;
        return result;
    }
};


//-----------------------------------------------------------
// AND
// 2入力1出力ノード
// データ型はともにbool
class NodeAnd : public Node2In1Out<bool>
{
    //-------------------------------------------------------
    bool calculate(bool p1, bool p2)
    {
        bool result = p1 & p2;
        return result;
    }
};

//-----------------------------------------------------------
// OR
// 2入力1出力ノード
// データ型はともにbool
class NodeOr : public Node2In1Out<bool>
{
    //-------------------------------------------------------
    bool calculate(bool p1, bool p2)
    {
        bool result = p1 | p2;
        return result;
    }
};

//-----------------------------------------------------------
// NORをORとNOTの組合せで構成する。
// ノードの結合を行う。
// 複合ノードは外部から見れば１つのノードとして振る舞う。
// 内部的に複数のノードを組合せる。
class NodeNor : public NodeSubSystem
{
    NodeOr* _nodeOr;
    NodeNot* _nodeNot;
    Edge* _edge;

public:

    //-------------------------------------------------------
    NodeNor(void)
    {
        // ノード生成
        _nodeOr = new NodeOr();
        _nodeNot = new NodeNot();

        // エッジ生成
        _edge = new Edge(true);

        // ノードにエッジを紐付ける。
        std::vector<Edge*> edges = {_edge};

        _nodeOr->addOutEdges(edges);
        _nodeNot->addInEdges(edges);

        // エッジにノードを紐付ける
        _edge->addOutNode(_nodeNot);

        // 内部Executorにノードとエッジを登録する。
        // 入口/出口ノード及び双対エッジはベースクラスで追加されている
        std::vector<Node*> nodes = {_nodeOr, _nodeNot};
        getInnerExecutor()->addNodes(nodes);
        getInnerExecutor()->addEdges(edges);
    }

    //-------------------------------------------------------
    void addInEdges(std::vector<Edge*> edges)
    {
        NodeSubSystem::addInEdges(edges);

        std::vector<Edge*> dualEdges = getInDualEdges();
        _nodeOr->addInEdges(dualEdges);

        for(auto edge : dualEdges){
            edge->addOutNode(_nodeOr);
        }

    }

    //-------------------------------------------------------
    void addOutEdges(std::vector<Edge*> edges)
    {
        NodeSubSystem::addOutEdges(edges);

        std::vector<Edge*> dualEdges = getOutDualEdges();
        _nodeNot->addOutEdges(dualEdges);
    }


};

#endif


