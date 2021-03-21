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
    // User Node that inherits this class must override this method.
    // And do the user specific action in the overrided execute() method
    // And also has to call this parent Node class execute() method
    // in order to change execution status.
    virtual void execute(void){ _status = Status::DONE; };

    //-------------------------------------------------------
    // Clear execution status
    virtual void clearStatus(void)
    {
        _status = Status::NOTYET;
    }

    //-------------------------------------------------------
    // if this node is already executed, return true
    // else return false
    virtual bool isExecuted(void)
    {
        bool result = _status == Status::DONE ? true : false;
        return result;
    }

    //-------------------------------------------------------
    // check if data that are necessary for execution of this node is ready or not
    // true : data are ready
    // false : data are not ready yet
    virtual bool isInputDataCompleted(void)
    {
        bool result = true;

        // 入力元Edgeにデータが全て揃っているか確認する。
        for(auto edge : _inEdges){
            if(edge->getStatus() == Edge::Status::DISABLE){
                result = false;
                break;
            }
        }
        return result;
    }


    //-------------------------------------------------------
    // 入力エッジを指定の場所に設定する。
    // indexは1オリジンとする。
    // 指定されたインデックスが現在の要素数外であれば要素数を拡張する。
    void setInEdge(Edge* edge, int index)
    {
        std::size_t size = _inEdges.size();
        if(size < index){
            _inEdges.resize(index);
        }
        _inEdges[index-1] = edge;
    }

    //-------------------------------------------------------
    // 出力エッジを指定の場所に設定する。
    // indexは1オリジンとする。
    // 指定されたインデックスが現在の要素数外であれば要素数を拡張する。
    void setOutEdge(Edge* edge, int index)
    {
        std::size_t size = _outEdges.size();
        if(size < index){
            _outEdges.resize(index);
        }
        _outEdges[index-1] = edge;
    }

    //-------------------------------------------------------
    // 入力元Edgeを一括追加する。
    virtual void addInEdges(std::vector<Edge*> edges)
    {
        std::copy(edges.begin(), edges.end(), std::back_inserter(_inEdges));
    }

    //-------------------------------------------------------
    // 出力先Edgeを一括追加する。
    virtual void addOutEdges(std::vector<Edge*> edges)
    {
        std::copy(edges.begin(), edges.end(), std::back_inserter(_outEdges));
    }


    //-------------------------------------------------------
    // 保持する出力Edge一覧を返す
    std::vector<Edge*> getOutEdges()
    {
        return _outEdges;
    }

    //-------------------------------------------------------
    // 保持する入力Edge一覧を返す
    std::vector<Edge*> getInEdges()
    {
        return _inEdges;
    }

};




#endif
