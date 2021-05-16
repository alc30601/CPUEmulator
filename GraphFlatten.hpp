// GraphFlatten.hpp
// 階層構造のグラフを平坦化する。
// 階層構造はNodeSubSystemが存在するときに子グラフが存在することになる。
// これら小グラフを全て親階層に引き上げる。
//
// NodeSubSystemを分解しているが、NodeSubSystemEntry、NodeSubSystemExitは残している。
// 本来は平坦化された場合、この入口出口Nodeは不要であるが、
// 平坦化の処理簡易化のために残している。
// NodeSubSystemEntry、NodeSubSystemExitは平坦化されたグラフ上では何もせず
// データをバイパスするだけのNodeとなる。

#ifndef __GRAPHFLATTEN_HPP__
#define __GRAPHFLATTEN_HPP__

#include <tuple>
#include <vector>
#include <queue>
#include <typeinfo>

#include "Edge.hpp"
#include "Node.hpp"

#include "NodeSubSystem.hpp"


//-----------------------------------------------------------
// vectorの全要素をqueueにコピーする。
template <typename T>
void vec2queue(std::vector<T>& v, std::queue<T>& q)
{
    for(auto elem : v){
        q.push(elem);
    }
}

//-----------------------------------------------------------
// vectorの全要素を別のvectorに追可する。
template <typename T>
void vec2vec(std::vector<T>& v_src, std::vector<T>& v_dst)
{
    for(auto elem : v_src){
        v_dst.push_back(elem);
    }
}

//-----------------------------------------------------------
// 引数で渡されるT型クラスのポインタがBASE継承クラスか否か調べる。
// typeid()の比較で型を確認することはできるが、そのクラスそのものの型情報が得られるため
// typeid()ではあるインスタンス変数が特定のクラスのサブクラスかどうかを調べることができない。
// そのため、dynamic_castを用いて特定の型にダウンキャストしてみて、キャスト出来るか否かで
// その特定の型を継承しているか否かを確認する。
// 引数aで渡されるインスタンスも、THE_CLASSもBASE_CLASSのサブクラスとする。
// BASE_CLASSからの継承関係において、aのクラスがTHE_CLASSのサブクラスか否かを判定する。
// この関数自体は汎用
// return
//   THE_CLASS継承の場合:true
//   THE_CLASS継承でない:false
template <typename THE_CLASS, typename BASE_CLASS>
bool isTheClass(BASE_CLASS* a)
{
    bool result = false;
    THE_CLASS* bp = dynamic_cast<THE_CLASS*>(a);
    if(bp != nullptr){
        result = true;
    }
    return result;
}

//-----------------------------------------------------------
// Edgeの繋ぎ変えを行う。
void reconnectEdges(NodeSubSystem* node)
{
    // 入力Edgeの繋ぎ変え
    // nodeの入力Edgeをチェックし、node宛になっていたら
    // 子供のエントリノード宛に書き換える。
    Node* entryNode = node->getNodeEntry();
    std::vector<Edge*>& inEdges = node->getInEdges();
    for(auto edge : inEdges){
        std::vector<Node*>& nodes = edge->getOutNodes();
        for(int i=0; i<nodes.size();i++){
            if(nodes[i] == node){
                nodes[i] = entryNode;
            }
        }
    }
    // nodeのinEdgeをEntryノードに引き継ぐ。
    for(int i=0; i<inEdges.size();i++){
        entryNode->setInEdge(inEdges[i], i+1);
    }

    // 出力Edgeの繋ぎ変え
    // nodeのoutEdgeをExitノードに引き継ぐ。
    Node* exitNode = node->getNodeExit();
    std::vector<Edge*>& outEdges = node->getOutEdges();
    for(int i=0; i<outEdges.size();i++){
        exitNode->setOutEdge(outEdges[i], i+1);
    }

}

//-----------------------------------------------------------
// Nodeを平坦化する。
// 最上位層のNode一覧、Edge一覧を受取り、平坦化したNode一覧、Edge一覧を返す。
// 
// Nodeリストを辿る。
// 空の平坦化リストを生成しておく。
// 幅優先探索でNode一覧を辿る。(処理街キューを設ける)
//   SubNodeを見つけたら、
//     Edgeの繋ぎ変えを行い、SubNodeは破棄する。
//     (Edge繋ぎ変えによりそのSubNodeを参照するEdgeは無いはず)
//     SubNodeの内部Nodeをキューに追加する。
//   SubNodeでなければそのNodeを平坦化リストに追加する。
std::pair<std::vector<Node*>, std::vector<Edge*>> graphFlatten(std::vector<Node*>& nodes, std::vector<Edge*>& edges)
{
    // std::vector<Node*>* newNodeList = new std::vector<Node*>(); // 平坦化されたNodeリスト
    // std::vector<Edge*>* newEdgeList = new std::vector<Edge*>(); // 平坦化されたEdgeリスト
    std::vector<Node*> newNodeList; // 平坦化されたNodeリスト
    std::vector<Edge*> newEdgeList; // 平坦化されたEdgeリスト
    std::queue<Node*> waitingQ;     // 処理待ちキュー

    // 引数で与えられた最上位階層のNode一覧を処理待ちキューに入れる(初期値)。
    vec2queue<Node*>(nodes, waitingQ);

    // 引数で与えられた最上位階層のEdge一覧を平坦化Edgeリストに入れる。
    vec2vec<Edge*>(edges, newEdgeList);

    // 処理待ちキューが無くなるまで繰り返す。
    while(waitingQ.empty() == false){
        Node* node = waitingQ.front();
        waitingQ.pop();

        if(isTheClass<NodeSubSystem, Node>(node)){
            NodeSubSystem* nodesub = dynamic_cast<NodeSubSystem*>(node);
            // 入出力Edgeの繋ぎ変えを行う。
            reconnectEdges(nodesub);

            // 子Nodeたちを処理待ちキューに追加する。
            vec2queue<Node*>(nodesub->getInnerNodes(), waitingQ);

            vec2vec<Edge*>(nodesub->getInnerEdges(), newEdgeList);

            // nodeはこの時点で不要になったので破棄する。
            delete nodesub;
        }
        else{
            newNodeList.push_back(node); 
        }
    }

    std::pair<std::vector<Node*>, std::vector<Edge*>> p{newNodeList, newEdgeList};
    return p;
}

#endif

