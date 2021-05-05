// NodeValue.hpp
#ifndef __NODEVALUE_HPP__
#define __NODEVALUE_HPP__

#include "Node.hpp"

//-----------------------------------------------------------
// 固定値ノード
// 入力エッジは０個または１個
// 出力エッジは１個の前提
template <typename T>
class NodeValue : public Node
{
    T _value;
    bool _outputed; // true: データ出力済 / false: データ未出力

public:
    //-------------------------------------------------------
    // データ出力済フラグをクリアしておく。
    NodeValue(void) : _outputed(false){}

    //-------------------------------------------------------
    // 実行開始前に初期値として保持データを設定する。
    void setValue(T value){ _value = value; }

    //-------------------------------------------------------
    // １回のステップでは必ず最初に保持しているデータの出力が実行される。
    // その後、入力データがあれば保持しているデータを入力データで更新する。
    // 
    // 1.1) データ出力未であれば保持しているデータを出力する。
    // 1.2) データを出力したら出力済フラグを立てる。
    // 2.1) 入力データがあれば、保持データを入力データで更新する。
    // 2.2) 保持データを入力データに更新したら、実行済とする。
    // ※入力データが本当に揃っているかはNodeクラスのisInputDataCompleted()を使う。
    // (isInputDataCompleted()は常にtrueを返すようにオーバライドしているので)
    void execute(void)
    {
        if(_outputed == false){
            std::vector<Edge*> outEdges = getOutEdges();
            outEdges[0]->setValue(_value);
            _outputed = true;
        }

        if(Node::isInputDataCompleted() == true){
            std::vector<Edge*> inEdges = getInEdges();
            if(inEdges.size() > 0){
                auto edge = inEdges[0];
                T value = std::any_cast<T>(edge->getValue());
                _value = value;
            }

            Node::execute();
        }
    }

    //-------------------------------------------------------
    // override from Node
    // Clear execution status
    // also initialize data output status
    void clearStatus(void)
    {
        Node::clearStatus();
        _outputed = false;
    }

    //-------------------------------------------------------
    // Nodeの実行が可能か確認する。
    // 常に実行可能としておく。
    virtual bool isExecutable(void)
    {
        return true; // return always true
    }

    //-------------------------------------------------------
    // override from Node
    // Executorから問い合わせされた際は常に実行可能としておく。
    bool isInputDataCompleted(void)
    {
        return true; // return always true - always input data completed
    }

    //-------------------------------------------------------
    // override from Node
    // Executorから問い合わせされた際は常に実行可能としておく。
    virtual bool isInputEdgeHaveValues(void)
    {
        return true; // return always true - always input data completed
    }

};

#endif

