// NodeBase.hpp
#ifndef __NODEBASE_HPP__
#define __NODEBASE_HPP__


#include "Node.hpp"
#include "Edge.hpp"



//-----------------------------------------------------------
// 1入力1出力ノードの基本クラス
// 仮想クラス使用する際、サブクラス化してcalculate()をオーバライドする必要がある。
// 入出力のデータは全て同じ型とする(テンプレートでT型)
template <class T>
class Node1In1Out : public Node
{
    //-------------------------------------------------------
    static const int INDEX_EDGE_IN = 0;     // index of input edge
    static const int INDEX_EDGE_OUT = 0;    // index of output edge

public:
    //-------------------------------------------------------
    void setEdge(Edge* inEdge, Edge* outEdge)
    {
        _inEdges.resize(1);
        _inEdges.at(0) = inEdge;

        _outEdges.resize(1);
        _outEdges.at(0) = outEdge;
    }

    //-------------------------------------------------------
    void execute(void)
    {
        Node::execute();

        // Edge* aaa = _inEdges.at(INDEX_EDGE_IN);
        // aaa->getValue();
        // auto bbb = aaa->getValue();
        // auto bbb = _inEdges.at(0)->getValue();
        // T ccc = std::any_cast<T>(bbb);
        // auto vvv = std::any_cast<T>(aaa->getValue());
        // auto temp1 = _inEdges.at(INDEX_EDGE_IN)->getValue();
        // T inValue = std::any_cast<T>(temp1);

        T inValue = std::any_cast<T>(_inEdges.at(INDEX_EDGE_IN)->getValue());
        T outValue = calculate(inValue);
        _outEdges.at(INDEX_EDGE_OUT)->setValue(outValue);
    }

    //-------------------------------------------------------
    virtual T calculate(T in1) = 0;
};


//-----------------------------------------------------------
// ２入力１出力型ノードの基本クラス
// 仮想クラス使用する際、サブクラス化してcalculate()をオーバライドする必要がある。
// 入出力のデータは全て同じ型とする(テンプレートでT型)
// 本来はbool型のandやor演算ノード用に設けたがTをintやdoubleなどにして
// 加減乗除などの演算ノードのベースとしても使用できる。
template <class T>
class Node2In1Out : public Node
{
    //-------------------------------------------------------
    static const int INDEX_EDGE_IN1 = 0;     // index of input edge 1
    static const int INDEX_EDGE_IN2 = 1;     // index of input edge 2
    static const int INDEX_EDGE_OUT = 0;     // index of output edge

public:
    //-------------------------------------------------------
    void setEdge(Edge* inEdge1, Edge* inEdge2, Edge* outEdge)
    {
        _inEdges.resize(2);
        _inEdges.at(0) = inEdge1;
        _inEdges.at(1) = inEdge2;

        _outEdges.resize(1);
        _outEdges.at(0) = outEdge;
    }

    //-------------------------------------------------------
    void execute(void)
    {
        Node::execute();

        T inValue1 = std::any_cast<T>(_inEdges.at(INDEX_EDGE_IN1)->getValue());
        T inValue2 = std::any_cast<T>(_inEdges.at(INDEX_EDGE_IN2)->getValue());
        T outValue = calculate(inValue1, inValue2);
        _outEdges.at(INDEX_EDGE_OUT)->setValue(outValue);
    }

    //-------------------------------------------------------
    virtual T calculate(T in1, T in2) = 0;

};





#endif


