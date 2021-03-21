// NodeValue.hpp
#ifndef __NODEVALUE_HPP__
#define __NODEVALUE_HPP__

#include "Node.hpp"

//-----------------------------------------------------------
// ★★★再検討要
// ★★★
// 
// 固定値ノード
// 入力エッジは０個または１個
// 出力エッジは１個の前提
template <typename T>
class NodeValue : public Node
{
    T _value;

public:
    //-------------------------------------------------------
    void setValue(T value){ _value = value; }

    //-------------------------------------------------------
    // 入力エッジが存在し、かつそのエッジにデータが届いている場合は
    // 入力エッジから値を読み出し自身に格納し出力エッジに転送する。
    // 入力エッジが存在しない、またはデータが届いていない場合は
    // 自身の保持する値を出力する。
    void execute(void)
    {
        Node::execute();

        std::vector<Edge*> inEdges = getInEdges();
        if(inEdges.size() > 0){
            auto edge = inEdges[0];
            if(edge->getStatus() == Edge::Status::ENABLE){
                T value = std::any_cast<T>(edge->getValue());
                _value = value;
            }
        }

        std::vector<Edge*> outEdges = getOutEdges();
        outEdges[0] = value;
    }

    //-------------------------------------------------------
    // This Node is always executable.
    bool isExecuted(void)
    {
        return true;
    }

    //-------------------------------------------------------
    // This Node is not required input data available.
    bool isInputDataCompleted(void)
    {
        return true;
    }

};

#endif

