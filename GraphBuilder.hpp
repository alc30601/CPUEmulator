// GraphBuilder.hpp
#ifndef __GRAPHBUILDER_HPP__
#define __GRAPHBUILDER_HPP__

#include "Edge.hpp"
#include "Node.hpp"

//-----------------------------------------------------------
// Helper class that construct node and edge that connected
// 
// なにか、↓こんな感じで書けるような。
// 
// ノードの生成(文字列で型名指定)
// a = createNode("NodeMul");
// b = createNode("NodeAdd");
// c = createNode("NodeGain", 3.0);
// または(テンプレートで型名指定、可変引数)
// a = createNode<NodeMul>();
// b = createNode<NodeAdd>();
// c = createNode<NodeGain>(3.0);
// 
// エッジの生成及びノード間の紐付け
// a.outto(1, b);
// a.outto(2, {b,c});
// または(テンプレートで出力ポート番号を指定、可変引数)
// a.outto<1>(b)
// a.outto<2>(b,c)

// test_07の場合↓ぐらいの設定で済むように。
//     nEntry = createNode<NodeMul, double>();
//     nExit = createNode<NodeTestExit, double>();
//     nMul = createNode<NodeMul>();
//     nGainA = createNode<NodeGain>(2.0);
//     nGainB = createNode<NodeGain>(3.0);
//     nConstC = createNode<NodeConst>(4.0);
//     nAdd = createNode<NodeAdd>();
// 
//     nEntry.outto<1>(nMul, nMul, nGainB);
//     nEntry.outto<2>(nConstC);
//     nMul.outto<1>(nGainA);
//     nGainA.outto<1>(nAdd);
//     nGainB.outto<1>(nAdd);
//     nConstC.outto<1>(nAdd);
//     nAdd.outto<1>(nExit);


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
class GraphBuilder
{
    std::vector<Edge*> _edges;
    
public:

    //-------------------------------------------------------
    // 準ノードを生成する。本当のノードも生成し準ノードで包む。
    // 準ノードをユーザにリターンするとともに、GraphBuilder内でも管理しておく。　
    template <typename T>
    QuasiNode& createNode(void)
    {
        T* node(new T);
        auto a = new QuasiNode(node);
        return *a;
    }

    //-------------------------------------------------------
    // ノードとノードをエッジで結ぶ。
    // エッジを１つ生成し出力先ノード(１個以上任意個)に繋げる。
    // エッジの接続先は複数有りうるので可変引数テンプレートを用いる。
    template <typename... Args>
    void outto(Port outPort, Args... inPorts)
    {
        size_t sizeToPort = sizeof...(Args);
        std::cout << "The number of args: " << sizeToPort << std::endl;

        // 可変引数で渡された出力先ポートを配列として展開
        Port array[] = { static_cast<Port>(inPorts)... };

        // エッジ生成
        Edge* edge(new Edge);

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
};




#endif


