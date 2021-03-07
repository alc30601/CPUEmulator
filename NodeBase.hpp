// NodeBase.hpp
#ifndef __NODEBASE_HPP__
#define __NODEBASE_HPP__


#include "Node.hpp"
#include "Edge.hpp"
#include "NodeSubSystem.hpp"
#include "GraphBuilder.hpp"


//-----------------------------------------------------------
// 1入力1出力ノードの基本クラス
// 仮想クラス使用する際、サブクラス化してcalculate()をオーバライドする必要がある。
// 入出力のデータは全て同じ型とする(テンプレートでT型)
template <typename T>
class Node1In1Out : public Node
{
public:
    //-------------------------------------------------------
    void execute(void)
    {
        Node::execute();

        T inValue = std::any_cast<T>(_inEdges.at(0)->getValue());
        T outValue = calculate(inValue);
        _outEdges.at(0)->setValue(outValue);
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
template <typename T>
class Node2In1Out : public Node
{
public:
    //-------------------------------------------------------
    void execute(void)
    {
        Node::execute();

        T inValue1 = std::any_cast<T>(_inEdges.at(0)->getValue());
        T inValue2 = std::any_cast<T>(_inEdges.at(1)->getValue());
        T outValue = calculate(inValue1, inValue2);
        _outEdges.at(0)->setValue(outValue);
    }

    //-------------------------------------------------------
    virtual T calculate(T in1, T in2) = 0;

};


//-----------------------------------------------------------
// 2入力1出力ノードと1入力1出力ノードの組合せで構成する複合ノード。
// 前段：2入力1出力ノード T21
// 後段：1入力1出力ノード T11
// テンプレートで上記ノードを指定すれば接続は本テンプレートクラスで実現される。
template <class T21, class T11>
class Node21_11 : public NodeComplex
{
public:
    Node21_11(void)
    {
        // グラフの構築
        auto& gb = getGraphBuilder();

        auto n21 = gb.createNode<T21>();
        auto n11 = gb.createNode<T11>();

        gb.outto(Port(n21, 1), Port(n11, 1));

        gb.setInPorts(Port(n21, 1), Port(n21, 2));
        gb.setOutPorts(Port(n11, 1));

        commit();
    }
};



#endif


