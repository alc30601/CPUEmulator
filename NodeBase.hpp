// NodeBase.hpp
#ifndef __NODEBASE_HPP__
#define __NODEBASE_HPP__


#include "Node.hpp"
#include "Edge.hpp"
#include "NodeSubSystem.hpp"



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
// また、内部のノード、接続が出来ているので、処理の実行はNodeSubSystemのexecute()
// を実行するだけで良い。
template <class T21, class T11>
class Node212 : public NodeSubSystem
{
    T21* _node21;
    T11* _node11;
    Edge* _edge;

public:

    //-------------------------------------------------------
    Node212(void)
    {
        // ノード生成
        _node21 = new T21();
        _node11 = new T11();

        // エッジ生成
        _edge = new Edge(true);

        // ノードにエッジを紐付ける。
        std::vector<Edge*> edges = {_edge};

        _node21->addOutEdges(edges);
        _node11->addInEdges(edges);

        // エッジにノードを紐付ける
        _edge->addOutNode(_node11);

        // 内部Executorにノードとエッジを登録する。
        // 入口/出口ノード及び双対エッジはベースクラスで追加されている
        std::vector<Node*> nodes = {_node21, _node11};
        getInnerExecutor()->addNodes(nodes);
        getInnerExecutor()->addEdges(edges);
    }

    //-------------------------------------------------------
    void addInEdges(std::vector<Edge*> edges)
    {
        NodeSubSystem::addInEdges(edges);

        std::vector<Edge*> dualEdges = getInDualEdges();
        _node21->addInEdges(dualEdges);

        for(auto edge : dualEdges){
            edge->addOutNode(_node21);
        }
    }

    //-------------------------------------------------------
    void addOutEdges(std::vector<Edge*> edges)
    {
        NodeSubSystem::addOutEdges(edges);

        std::vector<Edge*> dualEdges = getOutDualEdges();
        _node11->addOutEdges(dualEdges);
    }


};


#endif


