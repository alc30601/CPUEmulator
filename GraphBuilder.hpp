// GraphBuilder.hpp
#ifndef __GRAPHBUILDER_HPP__
#define __GRAPHBUILDER_HPP__

#include "Edge.hpp"
#include "Node.hpp"
#include "Executor.hpp"



//-----------------------------------------------------------
// 準ノード：GraphBuilderが取り扱う仮想的なノードクラス
class QuasiNode
{
    Node* _node;
public:
    QuasiNode(Node* node) : _node(node){}

    Node* getNode(void){ return _node; }
};

//-----------------------------------------------------------
// ノードとエッジの番号を組にしたタプル
// ノード間接続の際にこのPort型で指定する。
// 本来ノードにはIn,Outがあり、それぞれインデックスが1,2,3,...となり
// このタプルだけではInかOutか区別がつかない。
// 型としてIn/Out共用(同じPort型を使う)
// しかし、使用される文脈で区別可能である。
using Port = std::tuple<QuasiNode&, int>;

//-----------------------------------------------------------
// Portsは複数のPortを束ねたもの。
// エッジは１入力ポート、複数出力ポートの関係があるため、
// 出力ポートの組を保持するためにこの型を用いる。
using Ports = std::vector<Port>;



//-----------------------------------------------------------
// Helper class that construct node and edge that connected
class GraphBuilder
{
    std::vector<Edge*> _edges;
    std::vector<Node*> _nodes;

public:

    //-------------------------------------------------------
    std::vector<Edge*>& getEdges(void){ return _edges; }
    std::vector<Node*>& getNodes(void){ return _nodes; }

    //-------------------------------------------------------
    // 準ノードを生成する。本当のノードも生成し準ノードで包む。
    // 準ノードをユーザに返すとともに、GraphBuilder内でも管理しておく。　
    template <typename T>
    // QuasiNode& createNode(void)
    QuasiNode& createNode(const std::string& name = "")
    {
        T* node(new T);
        node->setName(name);
        _nodes.push_back(node);
        return quasiNodelize(node);
    }

    //-------------------------------------------------------
    // 生ノードを準ノード化する。
    QuasiNode& quasiNodelize(Node* node)
    {
        auto qn = new QuasiNode(node);
        return *qn;
    }

    //-------------------------------------------------------
    // ノードとノードをエッジで結ぶ。
    // エッジを１つ生成し出力先ノード(１個以上任意個)に繋げる。
    // 接続元は単一のポート、エッジの接続先ポートは複数有りうる。
    void outto(const Port& srcPort, const Ports& dstPorts)
    {
        int numDst = dstPorts.size();

        // エッジ生成
        Edge* edge(new Edge);
        _edges.push_back(edge);

        // エッジを出力元ノードに紐付ける
        Node* node = std::get<0>(srcPort).getNode();
        int index = std::get<1>(srcPort);
        node->setOutEdge(edge, index);

        // エッジを出力先ノードに紐付ける。
        for(int i=0;i<numDst;i++){
            Node* node = std::get<0>(dstPorts[i]).getNode();
            int index = std::get<1>(dstPorts[i]);
            node->setInEdge(edge, index);

            // エッジに出力先ノードを紐付ける。
            edge->addOutNode(node);
        }
    }

    //-------------------------------------------------------
    // エントリノードを指定してExecutorを生成する。
    Executor* createExecutor(QuasiNode&  entryQNode)
    {
        Node* entryNode = static_cast<Node*>(entryQNode.getNode());
        auto nodes = getNodes();
        auto edges = getEdges();
        Executor* exe(new Executor(entryNode, nodes, edges));
        return exe;
    }
};



#endif


