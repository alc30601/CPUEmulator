// NodeLogics.hpp

#ifndef __NODELOGICS_HPP__
#define __NODELOGICS_HPP__

#include <iostream>
#include <memory>
#include <cassert>

// #include "NodeBase.hpp"
#include "GraphBuilder.hpp"
#include "NodeComplex.hpp"

// 論理演算の取りうる値boolのtrue,falseをT,Fで表現する。
static const bool T = true;
static const bool F = false;

//-----------------------------------------------------------
// 入力Edgeにまだ値が設定されていない場合はその型のデフォルト値を返す。
// 値が設定されている場合はその値を取得し返す。
template <typename T>
T getValueFromAny(std::any& anyValue)
{
    T value = T();
    if(anyValue.has_value() == true){
        value = std::any_cast<T>(anyValue);
    }
    else{
        std::cout << "Edge has no value" << std::endl;
        assert(false);
    }
    return value;
}


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

        T inValue = getValueFromAny<T>(_inEdges.at(0)->getValue());
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

        T inValue1 = getValueFromAny<T>(_inEdges.at(0)->getValue());
        T inValue2 = getValueFromAny<T>(_inEdges.at(1)->getValue());
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
template <typename T, class T21, class T11>
class Node21_11 : public NodeComplex
{
public:
    Node21_11(void)
    {
        // グラフの構築
        auto& gb = getGraphBuilder();
        auto enty = getEntryNode();
        auto exit = getExitNode();

        auto n21 = gb.createNode<T21>();
        auto n11 = gb.createNode<T11>();

        gb.outto(Port(enty, 1), Ports{ Port(n21, 1) }, typeid(T));
        gb.outto(Port(enty, 2), Ports{ Port(n21, 2) }, typeid(T));
        gb.outto(Port(n21, 1), Ports{ Port(n11, 1) }, typeid(T));
        gb.outto(Port(n11, 1), Ports{ Port(exit, 1) }, typeid(T));

        commit();
    }
};


//-----------------------------------------------------------
// NOT
// 1入力1出力ノード
// データ型はともにbool
class NodeNot : public Node1In1Out<bool>
{
    //-------------------------------------------------------
    bool calculate(bool p)
    {
        // std::cout << "NOT" << std::endl;
        bool result = !p;
        return result;
    }
};


//-----------------------------------------------------------
// AND
// 2入力1出力ノード
// データ型はともにbool
class NodeAnd : public Node2In1Out<bool>
{
    //-------------------------------------------------------
    bool calculate(bool p1, bool p2)
    {
        // std::cout << "AND" << std::endl;
        bool result = p1 & p2;
        return result;
    }
};

//-----------------------------------------------------------
// OR
// 2入力1出力ノード
// データ型はともにbool
class NodeOr : public Node2In1Out<bool>
{
    //-------------------------------------------------------
    bool calculate(bool p1, bool p2)
    {
        // std::cout << "OR" << std::endl;
        bool result = p1 | p2;
        return result;
    }
};


//-----------------------------------------------------------
// NOR
// 2入力1出力ノード
// データ型はともにbool
#define NodeNor Node21_11<bool, NodeOr, NodeNot>

//-----------------------------------------------------------
// NAND
// 2入力1出力ノード
// データ型はともにbool
#define NodeNand Node21_11<bool, NodeAnd, NodeNot>


//-----------------------------------------------------------
// Exclusive OR
class NodeExor : public NodeComplex
{
public:
    NodeExor(void)
    {
        auto& gb = getGraphBuilder();
        auto enty = getEntryNode();
        auto exit = getExitNode();

        auto not1 = gb.createNode<NodeNot>("NodeNot in NodeExor");
        auto not2 = gb.createNode<NodeNot>("NodeNot in NodeExor");
        auto and1 = gb.createNode<NodeAnd>("NodeAnd in NodeExor");
        auto and2 = gb.createNode<NodeAnd>("NodeAnd in NodeExor");
        auto or1  = gb.createNode<NodeOr>("NodeOr in NodeExor");

        gb.outto(Port(enty, 1), Ports{Port(not1, 1), Port(and2, 2)}, typeid(bool));
        gb.outto(Port(enty, 2), Ports{Port(and1, 2), Port(not2, 1)}, typeid(bool));
        gb.outto(Port(not1, 1), Ports{ Port(and1, 1) }, typeid(bool));
        gb.outto(Port(not2, 1), Ports{ Port(and2, 1) }, typeid(bool));
        gb.outto(Port(and1, 1), Ports{ Port(or1, 1) }, typeid(bool));
        gb.outto(Port(and2, 1), Ports{ Port(or1, 2) }, typeid(bool));
        gb.outto(Port(or1, 1), Ports{ Port(exit, 1)}, typeid(bool));

        commit();
    }
};


#endif


