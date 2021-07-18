// NodeUtils.hpp
#ifndef __NODEUTILS_HPP__
#define __NODEUTILS_HPP__

#include <any>
#include "Edge.hpp"
#include "Node.hpp"


//-----------------------------------------------------------
// 数値　→　ビット列
template <int BITNUM>
class NodeDigit2Bit : public Node
{
public:
    //-------------------------------------------------------
    // 入力エッジの数値をビットに分解し、
    // 出力エッジのビット数分のboolエッジに出力する。
    void execute(void)
    {
        Node::execute();

        int inValue = std::any_cast<int>(_inEdges.at(0)->getValue());

        bool bits[BITNUM];
        for(int i=0;i<BITNUM; i++){
            if((inValue & 0x00000001) == 0x00000001){
                bits[i] = true;
            }
            else{
                bits[i] = false;
            }
            inValue = inValue >> 1;
        }

        for(int i=0;i<BITNUM; i++){
            _outEdges.at(i)->setValue(bits[i]);
        }

    }
};

//-----------------------------------------------------------
// ビット列　→　数値
template <int BITNUM>
class NodeBit2Digit : public Node
{
public:
    //-------------------------------------------------------
    // 入力エッジのビット数分のboolエッジを数値に変換し、
    // 出力エッジの数値エッジに出力する。
    void execute(void)
    {
        Node::execute();

        bool bits[BITNUM];
        std::vector<Edge*> inEdges = getInEdges();
        for(int i=0;i<BITNUM; i++){
            auto edge = inEdges[i];
            bool b = std::any_cast<bool>(edge->getValue());
            bits[i] = b;
        }

        int value = 0;
        for(int i=BITNUM-1;i>=0;i--){
            value = value << 1;
            value = bits[i] ? value + 1 : value;
        }

        _outEdges.at(0)->setValue(value);
    }

};


//-----------------------------------------------------------
// 透過
class NodeTransparent : public Node
{
public:
    //-------------------------------------------------------
    void execute(void)
    {
        Node::execute();

        std::vector<Edge*>& inEdges = getInEdges();
        std::vector<Edge*>& outEdges = getOutEdges();

        for(int i=0;i<inEdges.size();i++){
            std::any& value = inEdges[i]->getValue();
            outEdges[i]->setValue(value);
        }
    }
};

#endif


