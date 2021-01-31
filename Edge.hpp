// Edge.hpp
#ifndef __EDGE_HPP__
#define __EDGE_HPP__

#include <memory>
#include <vector>

class Node;

//-----------------------------------------------------------
// Definition of Edge class.
// Edge is between Nodes.
// Edge itself has a current value.
// Edge has link to Ndestination nodes 
template <class T>
class Edge
{
    T _v;   // value that hold this edge

    // ★★★
    // 自身の出力先のNode一覧を保持する。
    std::vector<std::shared_ptr<Node> _outputNodes;

public:

    //-------------------------------------------------------
    T getValue(void)
    {
        return _v;
    }

    //-------------------------------------------------------
    void setValue(T value)
    {
        _v = value;
    }

    //-------------------------------------------------------
    // ★★★
    // 自身の先につながるNodeの一覧を取得する。
    // List<std::shared_ptr<Node> getoutputNodes();


};

#endif


