// Node.hpp
#ifndef __NODE_HPP__
#define __NODE_HPP__

#include <memory>
#include <vector>

class Edge;

//-----------------------------------------------------------
class Node
{
public:
    enum class Status{
        DONE,       // already executed
        NOTYET      // hasn't been executed yet
    };

private:
    // 自身の出力先のEdge一覧を保持する。
    std::vector<std::shared_ptr<Edge>> _outEdges;

    // 自身の入力元のEdge一覧を保持する。
    std::vector<std::shared_ptr<Edge>> _inEdges;

    Status _status; // node execution status

public:

    //-------------------------------------------------------
    Node(void)
    {
        _status = Status::NOTYET;
    }

    // Execute the operation of this node
    virtual void execute(void) = 0;

    //-------------------------------------------------------
    // check if data that are necessary for execution of this node is ready or not
    // true : data are ready
    // false : data are not ready yet
    bool isInputDataCompleted(void)
    {
        bool result = true;

        // ★★★ 自身の入力元Edgeにデータが全て揃っているか確認する。
        for(auto edge : _inEdges){
            if(edge->getStatus() == Edge::Status::DISABLE){
                result = false;
                break;
            }
        }
        return result;
    }

    //-------------------------------------------------------
    // if this node is already executed, return true
    // else return false
    bool isExecuted(void)
    {
        bool result = _status == Status::DONE ? true : false;
    }

    //-------------------------------------------------------
    // 出力先Edgeを追加する。
    void addOutEdge(std::shared_ptr<Edge> edge)
    {
        _outEdges.push_back(edge);
    }

    //-------------------------------------------------------
    // 入力元Edgeを追加する。
    void addInEdge(std::shared_ptr<Edge> edge)
    {
        _inEdges.push_back(edge);
    }

    //-------------------------------------------------------
    // 保持するEdge一覧を返すメソッド
    std::vector<std::shared_ptr<Edge>> getOutEdges()
    {
        return _outEdges;
    }

};


#endif
