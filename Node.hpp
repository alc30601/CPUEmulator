// Node.hpp
#ifndef __NODE_HPP__
#define __NODE_HPP__

#include <memory>
#include <vector>
#include <any>


class Edge;

//-----------------------------------------------------------
// ノードは0〜複数個の入力エッジ、0〜複数個の出力エッジを持つ。
// 実行(execute)がコールされたら、入力エッジから必要なデータを取り出し
// 演算を行い、出力端子に結果を書き出す。
class Node
{
public:
    enum class Status{
        DONE,       // already executed
        NOTYET      // hasn't been executed yet
    };

protected:
    // 自身の出力先のEdge一覧を保持する。
    std::vector<Edge*> _outEdges;

    // 自身の入力元のEdge一覧を保持する。
    std::vector<Edge*> _inEdges;

    Status _status; // node execution status

public:

    //-------------------------------------------------------
    Node(void)
    {
        clearStatus();
    }

    //-------------------------------------------------------
    // Execute the operation of this node
    virtual void execute(void){ _status = Status::DONE; };

    //-------------------------------------------------------
    // 実行履歴をクリアする。
    void clearStatus(void)
    {
        _status = Status::NOTYET;
    }

    //-------------------------------------------------------
    // check if data that are necessary for execution of this node is ready or not
    // true : data are ready
    // false : data are not ready yet
    bool isInputDataCompleted(void)
    {
        bool result = true;

        // 自身の入力元Edgeにデータが全て揃っているか確認する。
        for(auto edge : _inEdges){
            // if(edge->getStatus() == Edge::Status::DISABLE){
            //     result = false;
            //     break;
            // }
        }
        return result;
    }

    //-------------------------------------------------------
    // if this node is already executed, return true
    // else return false
    bool isExecuted(void)
    {
        bool result = _status == Status::DONE ? true : false;
        return result;
    }

    //-------------------------------------------------------
    // 出力先Edgeを追加する。
    void addOutEdge(Edge* edge)
    {
        _outEdges.push_back(edge);
    }

    //-------------------------------------------------------
    // 入力元Edgeを追加する。
    void addInEdge(Edge* edge)
    {
        _inEdges.push_back(edge);
    }

    //-------------------------------------------------------
    // 出力先Edgeを一括追加する。
    void addOutEdges(std::vector<Edge*> edges)
    {
        std::copy(edges.begin(), edges.end(), std::back_inserter(_outEdges));
    }

    //-------------------------------------------------------
    // 入力元Edgeを一括追加する。
    void addInEdges(std::vector<Edge*> edges)
    {
        std::copy(edges.begin(), edges.end(), std::back_inserter(_inEdges));
    }

    //-------------------------------------------------------
    // 保持する出力Edge一覧を返す
    std::vector<Edge*> getOutEdges()
    {
        return _outEdges;
    }

};

//-----------------------------------------------------------
// モジュール化されたノードのベースクラス
// 本クラスを継承したサブモジュールを作成する。
// 
//   +-----------------------------------+
//   | +---+                       +---+ |
//  -|-|   |-Edge ...              |   | |
//   | |   |              ... Edge-|   |-|-
//  -|-|   |-Edge ...              |   | |
//   | +---+                       +---+ |
//   | 入り口ノード            出口ノード|
//   +-----------------------------------+


class NodeSub : public Node
{
    // 本サブモジュールの入力、出力は以下の入り口ノード、出口ノードで集約する。
    Node* _nodeEntry;   // 入り口ノード
    Node* _nodeExit;    // 出口ノード

public:
    //-------------------------------------------------------
    NodeSub(void)
    {
        _nodeEntry = new Node();
        _nodeExit  = new Node();
    }

    //-------------------------------------------------------
    // 入力エッジは入り口ノードに、出力エッジは出口ノードに紐付ける。
    void setEdge(std::vector<Edge*> inEdges, std::vector<Edge*> outEdges)
    {
        // 渡された外部エッジを入り口ノード、出口ノードに紐付ける。
        _nodeEntry->addInEdges(inEdges);
        _nodeExit->addOutEdges(outEdges);

        // 入力エッジ数、出力エッジ数に応じた内部エッジを生成し
        // 入り口ノード、出口ノードに紐つけておく。
        // Edgeを生成するためにはEdgeの型がわかっていないといけない。
        // 引数で渡されたinEdges,outEdgesをコピーして使用する。

        // for(auto edge : inEdges){
        //     Edge internalEdge = *edge;
        //     std::shared_ptr<Edge> temp = std::make_shared<Edge>(internalEdge);
        //     _nodeEntry->addOutEdge(temp);
        // }

        // for(auto edge : outEdges){
        //     Edge internalEdge = *edge;
        //     std::shared_ptr<Edge> temp = std::make_shared<Edge>(internalEdge);
        //     _nodeEntry->addInEdge(temp);
        // }

    }



};




#endif
