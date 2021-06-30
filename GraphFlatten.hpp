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
#include <set>

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
// inNodeの入力元Edgeの出力先NodeをinNodeから本当の出力先に書き換える。
// 本当の出力先Nodeの入力EdgeをinEdgeに書き換える。
// inNodeの入力Edge数とoutNodeの出力Edge数が同じであるとは限らない。
// in/outで使用するEdgeが異なる場合がありうる。
// 特に該当する出力側Edgeが存在しない場合はありうる。
// これは、そのEdgeの値を出力側では使用しない場合。
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
// このように４つの階層があるため、4重ループとなる。→サブルーチン化して簡易化すべきか。。。
void mergeNode(Node* inNode, Node* outNode)
{
    std::vector<Edge*>& inEdges = inNode->getInEdges();
    std::vector<Edge*>& outEdges = outNode->getOutEdges();

    // InとOutで必ずしも全てが接続されているわけではない。
    // そのため、全てのEdgeを巡り、接続されているEdgeに対して処理を行う。
    int numOfEdge = std::max(inEdges.size(), outEdges.size());

    // [2] 全入力Edge(全出力Edge)をなめる。
    for(int i=0;i<numOfEdge;i++){

        Edge* inEdge = inEdges[i];
        Edge* outEdge = outEdges[i];

        // 恐らく、あり得ないケースだが、該当する入力Edgeがない場合は処理をスキップ。
        // 「あり得ない」の意味は、あり得た場合はグラフの接続が間違っている場合ということ。
        if(inEdge == NULL){
            std::cout << "!!! No Corresponding in edge" << std::endl;
            continue;
        }

        std::vector<Node*>& nodes1 = inEdge->getOutNodes();

        // [1] １つのEdgeの全分岐先Nodeをなめる。
        for(int j=0; j<nodes1.size(); j++){

            // ある分岐先NodeがinNodeだった場合、それはinNodeのi番目の入力Edgeであるはず。
            if(nodes1[j] == inNode){

                // 該当する出力Edgeがない場合は処理をスキップ。
                // 入力Edgeの該当の枝を削除するだけ。
                if(outEdge != NULL){

                    // [3] 対応する出力Edgeの全分岐先をなめる。
                    std::vector<Node*>& nodes2 = outEdge->getOutNodes();
                    for(int k=0;k<nodes2.size();k++){

                        // 出力先分岐Nodeを入力Edgeに追加する。
                        inEdge->addOutNode(nodes2[k]);

                        // [4] 分岐先Nodeに対する全入力Edgeをなめる。
                        std::vector<Edge*>& inEdges2 = nodes2[k]->getInEdges();
                        for(int m=0;m<inEdges2.size(); m++){
                            if(inEdges2[m] == outEdge){
                                // 出力先Nodeの入力Edgeを入れ替える。
                                inEdges2[m] = inEdge;

                                // １つの入力Edgeの入れ替えを行ったら、あるNodeに対する入力Edgeの検索は抜ける。
                                // あるNodeに対する複数の同一入力Edgeがあったとしても、入力Edgeの全分岐をなめる１つ上のループで
                                // 検出されるはずなので。
                                break;
                            }
                        }

                    }

                }
                else{
                    std::cout << "!!! No Corresponding out edge" << std::endl;
                }

                nodes1.erase(nodes1.begin()+j);

                // １つの入力Edgeに対して最初に見つけたinNode向け分岐を行い
                // そのEdgeのループは終わり。１つのEdgeに複数のinNode向け分岐があったとしても
                // それは1つ上の全入力Edgeのループで検出されるはずなので。
                break;
            }
        }
    }
}

//-----------------------------------------------------------
// NodeEntryからの出力Edgeの削除
// NodeEntry, NodeExitの削除
// Edge自体の削除はここでは行わない(不要となったEdgeは最後にまとめて一括削除)。
void deleteUnusedNode(NodeSubSystem* node)
{
    Node* entryNode = node->getNodeEntry();
    Node* exitNode = node->getNodeExit();

    // NodeEntry, NodeExitをnode内のリストから除去する
    std::vector<Node*>& nodes = node->getInnerNodes();
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
// 参照されていないEdgeを削除する。
// 参照されれているEdgeの一覧を返す。
std::vector<Edge*> removeUnusedEdges(std::vector<Node*>& nodes, std::vector<Edge*>& edges)
{
    // nodesのNodeからリンクされている全てのEdgeをstd::setに集める。
    std::set<Edge*> usedEdges;
    for(Node* node : nodes){
        std::vector<Edge*>& inEdges = node->getInEdges();
        std::vector<Edge*>& outEdges = node->getOutEdges();

        std::set<Edge*> edgesSet1(inEdges.begin(), inEdges.end());
        std::set<Edge*> edgesSet2(outEdges.begin(), outEdges.end());

        usedEdges.merge(edgesSet1);
        usedEdges.merge(edgesSet2);
    }

    // edgesの各Edgeが上で集めたリンクされているEdge一覧に含まれるかをチェックする。
    // 含まれるEdgeのみを新しいEdge一覧に加える。
    // 含まれないEdgeは破棄する。
    std::vector<Edge*> newEdges;
    for(auto edge : edges){
        auto result = std::find(usedEdges.begin(), usedEdges.end(), edge);
        if(result == usedEdges.end()){
            delete edge;
        }
        else{
            newEdges.push_back(edge);
        }
    }

    // 参照されているEdge一覧を返す。
    return newEdges;
}

//-----------------------------------------------------------
// Edgeの繋ぎ変えを行う。(Entry,Exitノード削除版)
void reconnectEdges(NodeSubSystem* node)
{
    // 入力Edgeの繋ぎ変え
    mergeNode(node, node->getNodeEntry());

    // 出力Edgeの繋ぎ変え
    mergeNode(node->getNodeExit(), node);

    // 不要になったNode(Entry,Exit)を削除する。
    deleteUnusedNode(node);
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

            // NodeSubSystem内のEdgeをEdge一覧に加える。(後で破棄されるEdgeも含む)
            vec2vec<Edge*>(nodesub->getInnerEdges(), newEdgeList);

            // nodeはこの時点で不要になったので破棄する。
            delete nodesub;
        }
        else{
            newNodeList.push_back(node); 
        }
    }

    // もう、参照されなくなったEdgeを削除する。
    newEdgeList = removeUnusedEdges(newNodeList, newEdgeList);
    std::pair<std::vector<Node*>, std::vector<Edge*>> p{newNodeList, newEdgeList};
    return p;
}

#endif

