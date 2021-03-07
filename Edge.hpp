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
// Edge source is just one Node.
// Edge has link to multiple destination nodes.
// 
//              +----->
//    +-----+   +----->
// ---|value|---+----->
//    +-----+   +----->
//              +----->
// 
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
    std::vector<Node*> _outNodes;

public:

    //-------------------------------------------------------
    Edge(void)
    {
        clearStatus();
    }

    //-------------------------------------------------------
    Edge(std::any initial_v)
    {
        _v = initial_v;
        clearStatus();
    }

    //-------------------------------------------------------
    void clearStatus(void)
    {
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
    void addOutNode(Node* node)
    {
        _outNodes.push_back(node);
    }

    //-------------------------------------------------------
    void addOutNodes(std::vector<Node*> nodes)
    {
        std::copy(nodes.begin(), nodes.end(), std::back_inserter(_outNodes));
    }

    //-------------------------------------------------------
    // get Nodes connected from out direction of this Edge
    std::vector<Node*> getOutNodes(void)
    {
        return _outNodes;
    }


};

#endif


