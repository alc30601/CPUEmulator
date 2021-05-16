// NodeComplex.hpp
#ifndef __NODECOMPLEX_HPP__
#define __NODECOMPLEX_HPP__

#include "Edge.hpp"
#include "Node.hpp"
#include "NodeSubSystem.hpp"
#include "GraphBuilder.hpp"

//-----------------------------------------------------------
// GraphBuilderを用いてサブシステムのグラフを構成した結果を
// Nodeとして持つためのクラス。
// サブシステムのNodeはこのクラスを継承したNodeを生成する。
// コンストラクタにて個別の内部グラフを構築する。
class NodeComplex : public NodeSubSystem
{
    GraphBuilder _gb;

public:
    //-------------------------------------------------------
    NodeComplex(void){}

    //-------------------------------------------------------
    GraphBuilder& getGraphBuilder(void){ return _gb; }

    //-------------------------------------------------------
    QuasiNode& getEntryNode(void)
    {
        Node* nEntry = getNodeEntry();
        return quasiNodelize(nEntry);
    }

    //-------------------------------------------------------
    QuasiNode& getExitNode(void)
    {
        Node* nExit = getNodeExit();
        return quasiNodelize(nExit);
    }

    //-------------------------------------------------------
    // GraphBuilderを用いて本クラスをNodeSubSystemのノード化
    // (入口ノード、出口ノードとの接続)
    // 内部Executorに対してノード、エッジを登録する。
    // 本メソッドの実行により、本クラスが外部から利用できる複合ノードとなる。
    void commit(void)
    {
        auto nodes = _gb.getNodes();
        auto edges = _gb.getEdges();
        NodeSubSystem::addNodes(nodes);
        NodeSubSystem::addEdges(edges);
    }

};

#endif


