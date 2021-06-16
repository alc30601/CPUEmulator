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
// inNodeの入力EdgeとoutNodeの出力Edgeを結合する。
// 前提としてinNodeの入力Edge数とoutNodeの出力Edge数が同じであることが必要。
// inNodeの入力元Edgeの出力先NodeをinNodeから本当の出力先に書き換える。
// 本当の出力先Nodeの入力EdgeをinEdgeに書き換える。
//             
//            +--   +------+  +-------+               +--   +--+
//   ______   |   --|inNode|  |outNode|--  _______    |   --|  |
// -|inEdge|--+-----|      |--|       |---|outEdge|---+-----|  |
//   ------   +-- --|      |  |       |--  -------    +-- --|  |
//                  +------+  +-------+                     +--+
//            [1] [2]                                 [3] [4]
// 
// この関数のとりあえ使う範囲は上図のようになっている。即ち、
// [1] 入力Edgeからの複数分岐Node   : nodes1
// [2] あるNodeへの複数入力Edge     : inEdges/outEdges -> inEdge / outEdge
// [3] 出力Edgeからの複数分岐Node   : nodes2
// [4] あるNodeへの複数入力Edge     : inEdges2
// このように４つの階層があるため、4重ループとなる。→サブルーチン化して簡易化すべきか。
void mergeNode(Node* inNode, Node* outNode)
{
    std::vector<Edge*>& inEdges = inNode->getInEdges();
    std::vector<Edge*>& outEdges = outNode->getOutEdges();

    // [2] 全入力Edge(全出力Edge)をなめる。
    for(int i=0;i<inEdges.size();i++){

        Edge* inEdge = inEdges[i];
        Edge* outEdge = outEdges[i];

        std::vector<Node*>& nodes1 = inEdge->getOutNodes();

        // [1] １つのEdgeの全分岐先Nodeをなめる。
        for(auto it = nodes1.begin(); it != nodes1.end();){

            // ある分岐先NodeがinNodeだった場合、それはinNodeのi番目の入力Edgeであるはず。
            if(*it == inNode){

                // [3] 対応する出力Edgeの全分岐先をなめる。
                std::vector<Node*>& nodes2 = outEdge->getOutNodes();
                for(int k=0;k<nodes2.size();k++){

                    // 出力先分岐Nodeを入力Edgeに追加する。
                    inEdge->addOutNode(nodes2[k]);

                    // [4] 分岐先Nodeに対する全入力Edgeをなめる。
                    std::vector<Edge*>& inEdges2 = nodes2[k]->getInEdges();
                    for(int j=0;j<inEdges2.size(); j++){
                        if(inEdges2[j] == outEdge){
                            // 出力先Nodeの入力Edgeを入れ替える。
                            inEdges2[j] = inEdge;

                            // １つの入力Edgeの入れ替えを行ったら、あるNodeに対する入力Edgeの検索は抜ける。
                            // あるNodeに対する複数の同一入力Edgeがあったとしても、入力Edgeの全分岐をなめる１つ上のループで
                            // 検出されるはずなので。
                            break;
                        }
                    }

                }
                it = nodes1.erase(it);

                // １つの入力Edgeに対して最初に見つけたinNode向け分岐を行い
                // そのEdgeのループは終わり。１つのEdgeに複数のinNode向け分岐があったとしても
                // それは1つ上の全入力Edgeのループで検出されるはずなので。
                break;
            }
            else{
                ++it;
            }
        }
    }
}

//-----------------------------------------------------------
// nodeからの出力Edgeの削除
// NodeEntryからの出力Edgeの削除
// NodeEntry, NodeExitの削除
void deleteUnusedNodeEdge(NodeSubSystem* node)
{
    //-----------------------------------
    // NodeSubSystemからの出力Edgeの削除
    // Edgeの削除およびNodeのOutEdgesのクリア
    std::vector<Edge*>& edges1 = node->getOutEdges();
    for(auto edge : edges1){
        delete edge;
    }
    edges1.clear();

    Node* entryNode = node->getNodeEntry();
    Node* exitNode = node->getNodeExit();

    //-----------------------------------
    // NodeEntryからの出力Edgeの削除
    std::vector<Edge*> edges = node->getInnerEdges();
    std::vector<Edge*>& edges2 = entryNode->getOutEdges();

    // node内の全Edgeのうち、NodeEntryから出力されているEdgeを見つけ
    // node内のリストから除去する。
    for(auto it = edges.begin(); it != edges.end();){
        auto three = find(edges2.cbegin(), edges2.cend(), *it);
        if(three != edges2.cend()){
            it = edges.erase(it);
        }
        else{
            ++it;
        }
    }

    // NodeEntryから出力されるEdgeを破棄する。
    for(auto edge : edges2){
        delete edge;
    }
    edges2.clear();


    //-----------------------------------
    // NodeEntry, NodeExitの除去

    // NodeEntry, NodeExitをnode内のリストから除去する
    std::vector<Node*> nodes = node->getInnerNodes();
    for(auto it = nodes.begin(); it != nodes.end();){
        if((*it == entryNode) || (*it == exitNode)){
            it = nodes.erase(it);
        }
        else{
            ++it;
        }

    }

    // NodeEntry, NodeExitの破棄。
    delete entryNode;
    delete exitNode;
}

//-----------------------------------------------------------
// Edgeの繋ぎ変えを行う。(Entry,Exitノード削除版)
void reconnectEdges(NodeSubSystem* node)
{
    // 入力Edgeの繋ぎ変え
    mergeNode(node, node->getNodeEntry());

    // 出力Edgeの繋ぎ変え
    mergeNode(node->getNodeExit(), node);


    // 不要になったNode，Edgeを削除する。
    deleteUnusedNodeEdge(node);
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

