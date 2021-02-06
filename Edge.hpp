// Edge.hpp
#ifndef __EDGE_HPP__
#define __EDGE_HPP__

#include <iostream>
#include <memory>
#include <vector>
#include <any>
#include <typeinfo>


class Node;

//-----------------------------------------------------------
// Definition of Edge class.
// Edge is between Nodes.
// Edge itself has a current value.
// Edge has link to destination nodes 
class Edge
{
    std::any  _v;   // value that hold this edge

    // ★★★
    // 自身の出力先のNode一覧を保持する。
    std::vector<std::shared_ptr<Node>> _outputNodes;

public:

    //-------------------------------------------------------
    Edge(std::any initial_v)
    {
        std::cout << "Type of Edge : " << typeid(_v).name() << std::endl;
        _v = initial_v;
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
    }

    //-------------------------------------------------------
    // ★★★
    // 自身の先につながるNodeの一覧を取得する。
    // List<std::shared_ptr<Node> getoutputNodes();


};

#endif


