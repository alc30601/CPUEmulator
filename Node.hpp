// Node.hpp
#ifndef __NODE_HPP__
#define __NODE_HPP__

#include <memory>
#include <vector>
#include <any>
#include <cassert>

#include "Edge.hpp"

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

    std::string _name;

public:

    //-------------------------------------------------------
    Node()
    {
        clearStatus();
    }

    //-------------------------------------------------------
    void setName(const std::string& name){ _name = name; }
    std::string& getName(void){ return _name; }

    //-------------------------------------------------------
    // Execute the operation of this node
    // User Node that inherits this class must override this method.
    // And do the user specific action in the overrided execute() method
    // And also has to call this parent Node class execute() method
    // in order to change execution status.
    virtual void execute(void){ _status = Status::DONE; };

    //-------------------------------------------------------
    // 実行状態フラグを初期化(→未実行)する
    virtual void clearStatus(void)
    {
        _status = Status::NOTYET;
    }

    //-------------------------------------------------------
    // Nodeが既に実行済か確認する。
    virtual bool isExecuted(void)
    {
        bool result = _status == Status::DONE ? true : false;
        return result;
    }

    //-------------------------------------------------------
    // Nodeの実行が可能か確認する。
    virtual bool isExecutable(void)
    {
        return isInputDataCompleted();
    }

    //-------------------------------------------------------
    // 各ステップで新規入力データが全て届いているか確認する。
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
    // 入力Edgeに値が設定されているか確認する。
    // Edgeのデータはstd::any型なので、そもそもデータがまだ一度も
    // 設定されていなければ値の取り出しが出来ない。
    // true : data exists
    // false : data doesn't exist
    virtual bool isInputEdgeHaveValues(void)
    {
        bool result = true;

        // 入力元Edgeにデータが全て揃っているか確認する。
        for(auto edge : _inEdges){
            if(edge->getValue().has_value() == false){
                std::cout << "Input Edge has no value" << std::endl;
                assert(false);
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
    // 保持する出力Edge一覧を返す
    std::vector<Edge*>& getOutEdges()
    {
        return _outEdges;
    }

    //-------------------------------------------------------
    // 保持する入力Edge一覧を返す
    std::vector<Edge*>& getInEdges()
    {
        return _inEdges;
    }

};


#endif
