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
    void execute(void)
    {
        Node::execute();

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


