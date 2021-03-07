// GraphBuilder.hpp
#ifndef __GRAPHBUILDER_HPP__
#define __GRAPHBUILDER_HPP__

#include "Edge.hpp"
#include "Node.hpp"
#include "NodeSubSystem.hpp"
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
// しかし、使用される文脈で区別可能である。
using Port = std::tuple<QuasiNode&, int>;

//-----------------------------------------------------------
// Helper class that construct node and edge that connected
class GraphBuilder
{
    std::vector<Edge*> _edges;
    std::vector<Node*> _nodes;

    std::vector<Port> _inPorts;
    std::vector<Port> _outPorts;

public:

    //-------------------------------------------------------
    std::vector<Edge*> getEdges(void){ return _edges; }
    std::vector<Node*> getNodes(void){ return _nodes; }

    //-------------------------------------------------------
    // 準ノードを生成する。本当のノードも生成し準ノードで包む。
    template <typename T>
    QuasiNode& createNode(void)
    {
        T* node(new T);
        return quasiNodelize(node);
    }

    //-------------------------------------------------------
    // 生ノードを準ノード化する。
    // 準ノードをユーザにリターンするとともに、GraphBuilder内でも管理しておく。　
    QuasiNode& quasiNodelize(Node* node)
    {
        _nodes.push_back(node);
        auto qn = new QuasiNode(node);
        return *qn;
    }

    //-------------------------------------------------------
    // ノードとノードをエッジで結ぶ。
    // エッジを１つ生成し出力先ノード(１個以上任意個)に繋げる。
    // エッジの接続先は複数有りうるので可変引数テンプレートを用いる。
    template <typename... Args>
    void outto(Port outPort, Args... inPorts)
    {
        size_t sizeToPort = sizeof...(Args);

        // 可変引数で渡された出力先ポートを配列として展開
        Port array[] = { static_cast<Port>(inPorts)... };

        // エッジ生成
        Edge* edge(new Edge);
        _edges.push_back(edge);

        // エッジを出力元ノードに紐付ける
        Node* node = std::get<0>(outPort).getNode();
        int index = std::get<1>(outPort);
        node->setOutEdge(edge, index);

        // エッジを出力先ノードに紐付ける。
        for(int i=0;i<sizeToPort;i++){
            Node* node = std::get<0>(array[i]).getNode();
            int index = std::get<1>(array[i]);
            node->setInEdge(edge, index);

            // エッジに出力先ノードを紐付ける。
            edge->addOutNode(node);
        }

    }

    //-------------------------------------------------------
    // 本グラフの入力ポートを定義する。
    template <typename... Args>
    void setInPorts(Args... ports)
    {
        size_t sizePorts = sizeof...(Args);
        Port array[] = { static_cast<Port>(ports)... };
        std::vector<Port> inPorts(std::begin(array), std::end(array));
        _inPorts = inPorts;
    }

    //-------------------------------------------------------
    // 本グラフの出力ポートを定義する。
    template <typename... Args>
    void setOutPorts(Args... ports)
    {
        size_t sizePorts = sizeof...(Args);
        Port array[] = { static_cast<Port>(ports)... };
        std::vector<Port> outPorts(std::begin(array), std::end(array));
        _outPorts = outPorts;
    }

    //-------------------------------------------------------
    // 構築されたグラフを含むノード(SubSystem)を生成する。
    QuasiNode& nodelize(void)
    {
        NodeSubSystem* nSS(new NodeSubSystem());
        return nodelize(nSS);
    }

    //-------------------------------------------------------
    // NodeSubSystemに対して、
    // 入口ノード、出口ノードを取得し、準ノード化する。
    // 事前に設定されている入力ポート(_inPorts)を入口ノードの出力ポートに繋げる。
    // 事前に設定されている出力ポート(_outPorts)を出口ノードの入口ポートに繋げる。
    // NodeSubSystemは生成せず、与えられたNodeSubSystem内にグラフを設定する。
    QuasiNode& nodelize(NodeSubSystem* nSS)
    {
        Node* nEntry = nSS->getNodeEntry();
        Node* nExit = nSS->getNodeExit();

        auto qnEntry = quasiNodelize(nEntry);
        auto qnExit = quasiNodelize(nExit);

        for(int i=0;i<_inPorts.size();i++){
            outto(Port(qnEntry, i+1), _inPorts[i]);
        }

        for(int i=0;i<_outPorts.size();i++){
            outto(_outPorts[i], Port(qnExit, i+1));
        }

        return quasiNodelize(nSS);
    }

};


//-----------------------------------------------------------
// GraphBuilderを用いてサブシステムのグラフを構成した結果を
// Nodeとして持つためのクラス。
// サブシステムのNodeはこのクラスを継承したNodeを生成する。
class NodeComplex : public NodeSubSystem
{
    GraphBuilder _gb;

public:
    //-------------------------------------------------------
    NodeComplex(void){}

    //-------------------------------------------------------
    GraphBuilder& getGraphBuilder(void){ return _gb; }

    //-------------------------------------------------------
    // GraphBuilderを用いて本クラスをNodeSubSystemのノード化
    // (入口ノード、出口ノードとの接続)
    // 内部Executorに対してノード、エッジを登録する。
    // 本メソッドの実行により、本クラスが外部から利用できる複合ノードとなる。
    void commit(void)
    {
        _gb.nodelize(this);

        auto exe = NodeSubSystem::getInnerExecutor();
        auto nodes = _gb.getNodes();
        auto edges = _gb.getEdges();
        exe->addNodes(nodes);
        exe->addEdges(edges);
    }
};

#endif


