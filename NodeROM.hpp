// NodeROM.hpp
#ifndef __NODEROM_HPP__
#define __NODEROM_HPP__

#include "Node.hpp"


//-----------------------------------------------------------
// 4 bit register
// In-Ports  : A0, A1, A2, A3, OE
// Out-Ports : D0, D1, D2, D3, D4, D5, D6, D7
// 指定されたアドレス値(A0-A3)のROM値を出力(D0-D7)する。
// アドレスは4bit指定で8bitのデータを出力する。
// 実装として、ファイルからデータを読み込み、保持する。
// ノード実行時に、入力アドレスに対応するデータを出力する。
// 本ノードは論理回路から構成されるのではなく、簡易のため疑似実装とする。
class NodeROM : public Node
{
public:
    //-------------------------------------------------------
    NodeROM(void)
    {
    }
};

#endif

