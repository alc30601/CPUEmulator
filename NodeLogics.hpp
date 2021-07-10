// NodeLogics.hpp

#ifndef __NODELOGICS_HPP__
#define __NODELOGICS_HPP__

#include <iostream>
#include <memory>
#include <cassert>

// #include "NodeBase.hpp"
#include "GraphBuilder.hpp"
#include "NodeComplex.hpp"
#include "LogicDefines.hpp"



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

        bool inValue = _inEdges.at(0)->value<bool>();
        bool outValue = !inValue;
        _outEdges.at(0)->setValue(outValue);
    }

};


//-----------------------------------------------------------
// 論理素子の基本形
// N入力1出力とする。入力数は不定。接続された入力Edge数とする。
class NodeLogic : public Node
{
public:
    //-------------------------------------------------------
    void execute(void)
    {
        Node::execute();

        // 入力Edgeのデータを収集する。
        std::vector<Edge*>& inEdges = getInEdges();
        int numInEdges = inEdges.size();
        std::vector<bool> inValues;
        for(int i=0;i<numInEdges;i++){
            bool value = inEdges[i]->value<bool>();
            inValues.push_back(value);
        }

        // 入力データを元に出力データを計算する。
        bool result = calculate(inValues);

        // 出力は1本。
        std::vector<Edge*>& outEdges = getOutEdges();
        outEdges[0]->setValue(result);
    }

    //-------------------------------------------------------
    virtual bool calculate(std::vector<bool>& input) = 0;


    //-------------------------------------------------------
    // 与えられたEdgeをチェックし、有効なEdgeでかつ値がbValueのものがあれば演算可能とする。
    // 有効なEdgeでかつ値がbValueのものがなく、無効のEdgeが一つでもあれば演算不可とする。
    // return true  : 演算可能
    //        false : 演算不可
    bool isEdgeEnough(bool bValue)
    {
        std::vector<Edge*>& edges = getInEdges();
        bool result = true;

        for(auto edge : edges){
            if(edge->getStatus() == Edge::Status::ENABLE){
                bool b = edge->value<bool>();
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

};



//-----------------------------------------------------------
class NodeAnd : public NodeLogic
{
public:
    //-------------------------------------------------------
    bool calculate(std::vector<bool>& input)
    {
        bool answer = input[0];
        for(int i=1;i<input.size();i++){
            answer = answer & input[i];
        }
        return answer;
    }

    //-------------------------------------------------------
    bool isInputDataCompleted(void)
    {
        return isEdgeEnough(F);
    }
};

//-----------------------------------------------------------
class NodeOr : public NodeLogic
{
public:
    //-------------------------------------------------------
    bool calculate(std::vector<bool>& input)
    {
        bool answer = input[0];
        for(int i=1;i<input.size();i++){
            answer = answer | input[i];
        }
        return answer;
    }

    //-------------------------------------------------------
    bool isInputDataCompleted(void)
    {
        return isEdgeEnough(T);
    }
};

//-----------------------------------------------------------
class NodeNand : public NodeLogic
{
public:
    //-------------------------------------------------------
    bool calculate(std::vector<bool>& input)
    {
        bool answer = input[0];
        for(int i=1;i<input.size();i++){
            answer = answer & input[i];
        }
        return !answer;
    }

    //-------------------------------------------------------
    bool isInputDataCompleted(void)
    {
        return isEdgeEnough(F);
    }
};

//-----------------------------------------------------------
class NodeNor : public NodeLogic
{
public:
    //-------------------------------------------------------
    bool calculate(std::vector<bool>& input)
    {
        bool answer = input[0];
        for(int i=1;i<input.size();i++){
            answer = answer | input[i];
        }
        return !answer;
    }

    //-------------------------------------------------------
    bool isInputDataCompleted(void)
    {
        return isEdgeEnough(T);
    }
};

// 3入力素子も上記で対応可能であるが明示的に3入力であることを表すために別名をつける。
// この名前を使っても、上記の3のつかない名前を使ってもどちらでもよい。
using Node3And = NodeAnd;
using Node3Nand = NodeNand;




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


#endif


