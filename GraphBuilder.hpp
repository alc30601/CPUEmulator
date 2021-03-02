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
        auro a = new QuasiNode(node);
        return a;
    }

    //-------------------------------------------------------
    // ノードとノードをエッジで結ぶ。
    // エッジを１つ生成し出力先ノード(１個以上任意個)に繋げる。
    // エッジの接続先は複数有りうるので可変引数テンプレートを用いる。
    template <typename... Args>
    void outto(QuasiNode& fromNode, Args... args)
    {
        Edge* edge(new Edge);


    }
};




#endif


