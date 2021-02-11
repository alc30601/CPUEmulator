// NodeLogics.hpp

#ifndef __NODELOGICS_HPP__
#define __NODELOGICS_HPP__

#include <iostream>
#include <memory>

#include "Node.hpp"
#include "Edge.hpp"


//-----------------------------------------------------------
// NOT
// 1入力1出力ノード
// データ型はともにbool
class NodeNot : public Node
{
    //-------------------------------------------------------
    static const int INDEX_EDGE_IN = 0;     // index of input edge
    static const int INDEX_EDGE_OUT = 0;    // index of output edge

public:
    //-------------------------------------------------------
    void setEdge(std::shared_ptr<Edge> inEdge, std::shared_ptr<Edge> outEdge)
    {
        _inEdges.resize(1);
        _inEdges.at(0) = inEdge;

        _outEdges.resize(1);
        _outEdges.at(0) = outEdge;
    }

    //-------------------------------------------------------
    // 入力値を反転し出力する。
    void execute(void)
    {
        Node::execute();

        bool inValue = std::any_cast<bool>(_inEdges.at(INDEX_EDGE_IN)->getValue());
        bool outValue = !inValue;
        _outEdges.at(INDEX_EDGE_OUT)->setValue(outValue);
    }
};


//-----------------------------------------------------------
// AND
// 2入力1出力ノード
// データ型はともにbool
class NodeAnd : public Node
{

};



#endif


