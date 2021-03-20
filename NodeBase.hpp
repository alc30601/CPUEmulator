// NodeBase.hpp
#ifndef __NODEBASE_HPP__
#define __NODEBASE_HPP__

#include <functional>

#include "Node.hpp"
#include "Edge.hpp"
#include "NodeSubSystem.hpp"
#include "GraphBuilder.hpp"
#include "NodeComplex.hpp"


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

        gb.outto(Port(n21, 1), Ports{ Port(n11, 1) });

        setInPortss(Ports{Port(n21, 1)}, Ports{Port(n21, 2)});
        setOutPorts(Port(n11, 1));

        commit();
    }
};


//-----------------------------------------------------------
// template class for N input and one output
// 
// The way of caclulating
// v1 ----+
//       _|_
// v2 --|_f |--+
//             |
// v3 -------|_f |-- ...     --+
//  .....                      |
// vn -------------- .... ---|_f |----> output value
// That is ...
// output value = ((((v1 f v2) f v3) f v4) ... vn)
template <typename T>
class NodeN2One : public Node
{
    // function object that takes two input and put one output
    // this object must be set at constructor
    std::function<T(T,T)> _f;

public:
    NodeN2One(std::function<T(T,T)> f) : _f(f){}

    //-------------------------------------------------------
    void execute(void)
    {
        std::vector<Edge*> inEdges = getInEdges();
        std::vector<Edge*> outEdges = getOutEdges();

        // check number of input edge -- must be equal or largeer than 2
        std::size_t size = inEdges.size();
        if(size < 2){
            std::cout << "Input Edges of this Node is less than two" << std::endl;
        }

        // at least 2 input edges exist
        // first get value from first edge, then loop from second to last
        // for example, function is plus(+), the output value is
        // value1 + value2 + value3 + ...
        auto value = std::any_cast<T>(inEdges.at(0)->getValue());
        for(int i=1;i<size;i++){
            auto value2 = std::any_cast<T>(inEdges.at(i)->getValue());
            value = _f(value, value2);
        }

        // set output edge -- assume number of output edge must be one
        outEdges.at(0)->setValue(value);
    }

};

#endif


