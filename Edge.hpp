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
    std::any  _v;       // value that hold this edge
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
    void clearStatus(void)
    {
        _status = Status::DISABLE;
    }

    //-------------------------------------------------------
    Status getStatus(void)
    {
        return _status;
    }

    //-------------------------------------------------------
    std::any& getValue(void)
    {
        return _v;
    }


    //-------------------------------------------------------
    // Edgeで保持する値をテンプレートで指定された型で取り出す。
    // Edgeで保持するstd::anyがまだ値を持たなければassert
    template <typename T>
    T value(void)
    {
        std::any anyValue = getValue();
        T v = T();
        if(anyValue.has_value() == true){
            v = std::any_cast<T>(anyValue);
        }
        else{
            std::cout << "Edge has no value" << std::endl;
            assert(false);
        }
        return v;
    }


    //-------------------------------------------------------
    void setJustValue(std::any value)
    {
        _v = value;
    }

    //-------------------------------------------------------
    void setValue(std::any value)
    {
        setJustValue(value);
        _status = Status::ENABLE;
    }

    //-------------------------------------------------------
    void addOutNode(Node* node)
    {
        _outNodes.push_back(node);
    }

    //-------------------------------------------------------
    // get Nodes connected from out direction of this Edge
    std::vector<Node*>& getOutNodes(void)
    {
        return _outNodes;
    }


};

#endif


