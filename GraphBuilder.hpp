// GraphBuilder.hpp
#ifndef __GRAPHBUILDER_HPP__
#define __GRAPHBUILDER_HPP__

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



class GraphBuilder
{
public:




};




#endif


