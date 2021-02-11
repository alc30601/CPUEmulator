// Edge.hpp
#ifndef __EDGE_HPP__
#define __EDGE_HPP__

#include <iostream>
#include <memory>
#include <vector>
#include <any>
#include <typeinfo>
#include <cassert>

class Node;

//-----------------------------------------------------------
// Definition of Edge class.
// Edge is between Nodes.
// Edge itself has a current value.
// Edge has link to destination nodes 
class Edge
{
public:
    enum class Status{
        DISABLE,    // The value stored in this Edge that is _v cannot be used yet
        ENABLE      // The value stored in this Edge is able to be used.
    };

private:
    std::any  _v;   // value that hold this edge
    Status _status;    // status of this edge


    // hold output nodes from this Edge
    std::vector<std::shared_ptr<Node>> _outNodes;

public:

    //-------------------------------------------------------
    Edge(std::any initial_v)
    {
        _v = initial_v;
        _status = Status::DISABLE;
    }

    //-------------------------------------------------------
    std::any getValue(void)
    {
        return _v;
    }

    //-------------------------------------------------------
    void setValue(std::any value)
    {
        _v = value;
        _status = Status::ENABLE;
    }

    //-------------------------------------------------------
    Status getStatus(void)
    {
        return _status;
    }

    //-------------------------------------------------------
    void addOutNode(std::shared_ptr<Node> node)
    {
        _outNodes.push_back(node);
    }

    //-------------------------------------------------------
    // 自身の先につながるNodeの一覧を取得する。
    std::vector<std::shared_ptr<Node>> getOutNodes(void)
    {
        return _outNodes;
    }


};

#endif


