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
// 与えられたEdgeをチェックし、有効なEdgeでかつ値がbValueのものがあれば演算可能とする。
// 有効なEdgeでかつ値がbValueのものがなく、無効のEdgeが一つでもあれば演算不可とする。
// return true  : 演算可能
//        false : 演算不可
bool isEdgeEnough(std::vector<Edge*> edges, bool bValue)
{
    bool result = true;
    for(auto edge : edges){
        if(edge->getStatus() == Edge::Status::ENABLE){
            bool b = getValueFromAny<bool>(edge->getValue());
            if(b == bValue){
                result = true;
                goto _end;
            }
        }
        else{
            result = false;
        }
    }
_end:        
    return result;
}

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

        auto n21 = gb.createNode<T21>(typeid(T21).name());
        auto n11 = gb.createNode<T11>(typeid(T11).name());

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
class NodeNot : public Node
{
public:
    //-------------------------------------------------------
    void execute(void)
    {
        Node::execute();

        bool inValue = getValueFromAny<bool>(_inEdges.at(0)->getValue());
        bool outValue = !inValue;
        _outEdges.at(0)->setValue(outValue);
    }

};


//-----------------------------------------------------------
// AND
// 2入力1出力ノード
// データ型はともにbool
class NodeAnd : public Node
{
public:
    //-------------------------------------------------------
    void execute(void)
    {
        Node::execute();

        bool inValue1 = getValueFromAny<bool>(_inEdges.at(0)->getValue());
        bool inValue2 = getValueFromAny<bool>(_inEdges.at(1)->getValue());
        bool outValue = inValue1 & inValue2;
        _outEdges.at(0)->setValue(outValue);
    }

    //-------------------------------------------------------
    bool isInputDataCompleted(void)
    {
        return isEdgeEnough(_inEdges, F);

//         bool result = true;

//         // 入力元Edgeにデータが全て揃っているか確認する。
//         for(auto edge : _inEdges){
//             if(edge->getStatus() == Edge::Status::ENABLE){
//                 bool b = getValueFromAny<bool>(edge->getValue());
//                 if(b == F){
//                     result = true;
//                     goto _end;
//                 }
//             }
//             if(edge->getStatus() == Edge::Status::DISABLE){
//                 result = false;
//                 break;
//             }
//         }
// _end:        
//         return result;
    }

};


//-----------------------------------------------------------
// OR
// 2入力1出力ノード
// データ型はともにbool
class NodeOr : public Node
{
public:
    //-------------------------------------------------------
    void execute(void)
    {
        Node::execute();

        bool inValue1 = getValueFromAny<bool>(_inEdges.at(0)->getValue());
        bool inValue2 = getValueFromAny<bool>(_inEdges.at(1)->getValue());
        bool outValue = inValue1 | inValue2;
        _outEdges.at(0)->setValue(outValue);
    }

    //-------------------------------------------------------
    bool isInputDataCompleted(void)
    {
        return isEdgeEnough(_inEdges, T);

//         bool result = true;

//         // 入力元Edgeにデータが全て揃っているか確認する。
//         for(auto edge : _inEdges){
//             if(edge->getStatus() == Edge::Status::ENABLE){
//                 bool b = getValueFromAny<bool>(edge->getValue());
//                 if(b == T){
//                     result = true;
//                     goto _end;
//                 }
//             }
//             if(edge->getStatus() == Edge::Status::DISABLE){
//                 result = false;
//                 break;
//             }
//         }
// _end:        
//         return result;
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
// #define NodeNand Node21_11<bool, NodeAnd, NodeNot>
class NodeNand : public Node
{
public:
    //-------------------------------------------------------
    void execute(void)
    {
        Node::execute();

        bool result = F;
        bool inValue1 = getValueFromAny<bool>(_inEdges.at(0)->getValue());
        bool inValue2 = getValueFromAny<bool>(_inEdges.at(1)->getValue());

        if((inValue1 == F) || (inValue2 == F)){
            result = T;
        }
        else{
            result = F;
        }

        bool outValue = result;
        _outEdges.at(0)->setValue(outValue);
    }

    //-------------------------------------------------------
    // 有効で値がFの入力Edgeがあれば演算可能。
    // 有効で値がFの入力Edgeがなく、無効の入力Edgeが１つでもあれば演算不可。
    bool isInputDataCompleted(void)
    {
        return isEdgeEnough(_inEdges, F);
    }
};



//-----------------------------------------------------------
// Exclusive OR
class NodeExor : public NodeComplex
{
public:
    //-------------------------------------------------------
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

//-----------------------------------------------------------
// ３端子AND
class Node3And : public NodeComplex
{
public:
    //-------------------------------------------------------
    Node3And(void)
    {
        auto& gb = getGraphBuilder();
        auto enty = getEntryNode();
        auto exit = getExitNode();

        auto and1 = gb.createNode<NodeAnd>("NodeAnd in Node3And");
        auto and2 = gb.createNode<NodeAnd>("NodeAnd in Node3And");

        gb.outto(Port(enty, 1), Ports{Port(and1, 1) }, typeid(bool));
        gb.outto(Port(enty, 2), Ports{Port(and1, 2) }, typeid(bool));
        gb.outto(Port(enty, 3), Ports{Port(and2, 1) }, typeid(bool));
        gb.outto(Port(and1, 1), Ports{Port(and2, 2) }, typeid(bool));
        gb.outto(Port(and2, 1), Ports{ Port(exit, 1)}, typeid(bool));

        commit();
    }
};

//-----------------------------------------------------------
// ３端子NAND
// class Node3Nand : public NodeComplex
// {
// public:
//     //-------------------------------------------------------
//     Node3Nand(void)
//     {
//         auto& gb = getGraphBuilder();
//         auto enty = getEntryNode();
//         auto exit = getExitNode();

//         auto and1 = gb.createNode<Node3And>("Node3Nand in Node3Nand");
//         auto not1 = gb.createNode<NodeNot>("NodeNot in Node3Nand");

//         gb.outto(Port(enty, 1), Ports{Port(and1, 1) }, typeid(bool));
//         gb.outto(Port(enty, 2), Ports{Port(and1, 2) }, typeid(bool));
//         gb.outto(Port(enty, 3), Ports{Port(and1, 3) }, typeid(bool));
//         gb.outto(Port(and1, 1), Ports{Port(not1, 1) }, typeid(bool));
//         gb.outto(Port(not1, 1), Ports{ Port(exit, 1)}, typeid(bool));

//         commit();

//     }
// };

class Node3Nand : public Node
{
public:
    //-------------------------------------------------------
    void execute(void)
    {
        Node::execute();

        bool result = F;
        bool inValue1 = getValueFromAny<bool>(_inEdges.at(0)->getValue());
        bool inValue2 = getValueFromAny<bool>(_inEdges.at(1)->getValue());
        bool inValue3 = getValueFromAny<bool>(_inEdges.at(2)->getValue());

        if((inValue1 == F) || (inValue2 == F) || (inValue3 == F)){
            result = T;
        }
        else{
            result = F;
        }

        bool outValue = result;
        _outEdges.at(0)->setValue(outValue);
    }

    //-------------------------------------------------------
    // 有効で値がFの入力Edgeがあれば演算可能。
    // 有効で値がFの入力Edgeがなく、無効の入力Edgeが１つでもあれば演算不可。
    bool isInputDataCompleted(void)
    {
        return isEdgeEnough(_inEdges, F);
    }
};


#endif


